/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#include "query_optimizer/tests/TestDatabaseLoader.hpp"

#include <cmath>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "types/CharType.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

CatalogRelation *TestDatabaseLoader::createTestRelation(bool allow_vchar) {
  std::unique_ptr<CatalogRelation> catalog_relation;
  catalog_relation.reset(new CatalogRelation(&catalog_database_,
                                             "Test" /* name */,
                                             0 /* id */,
                                             true /* temporary */));
  int attr_id = -1;
  catalog_relation->addAttribute(new CatalogAttribute(catalog_relation.get(),
                                                      "int_col" /* name */,
                                                      TypeFactory::GetType(kInt, true /* nullable */),
                                                      ++attr_id));
  catalog_relation->addAttribute(new CatalogAttribute(catalog_relation.get(),
                                                      "long_col" /* name */,
                                                      TypeFactory::GetType(kLong),
                                                      ++attr_id));
  catalog_relation->addAttribute(new CatalogAttribute(catalog_relation.get(),
                                                      "float_col" /* name */,
                                                      TypeFactory::GetType(kFloat),
                                                      ++attr_id));
  catalog_relation->addAttribute(new CatalogAttribute(catalog_relation.get(),
                                                      "double_col" /* name */,
                                                      TypeFactory::GetType(kDouble, true /* nullable */),
                                                      ++attr_id));
  catalog_relation->addAttribute(new CatalogAttribute(
      catalog_relation.get(),
      "char_col" /* name */,
      TypeFactory::GetType(kChar, 20 /* length */, false /* nullable */),
      ++attr_id));

  if (allow_vchar) {
    catalog_relation->addAttribute(new CatalogAttribute(
        catalog_relation.get(),
        "vchar_col",
        TypeFactory::GetType(kVarChar, 20 /* length */, true /* nullable */),
        ++attr_id));
  }
  test_relation_ = catalog_relation.get();
  catalog_database_.addRelation(catalog_relation.release());
  return test_relation_;
}

void TestDatabaseLoader::loadTestRelation() {
  CHECK(test_relation_ != nullptr);
  CHECK(!test_relation_->hasAttributeWithName("vchar_col"));

  BlockPoolInsertDestination destination(*test_relation_,
                                         nullptr,
                                         &storage_manager_,
                                         0 /* dummy op index */,
                                         foreman_client_id_,
                                         agent_client_id_,
                                         &bus_);
  int sign = 1;
  for (int x = 0; x < 25; ++x) {
    // Column values: ((-1)^x*x, x^2, sqrt(x), (-1)^x*x*sqrt(x),
    // concat(string(int_col), string(float_col)).
    std::vector<TypedValue> attr_values;

    if (x % 10 == 0) {
      attr_values.emplace_back(kInt);
    } else {
      attr_values.emplace_back(sign * x);
    }

    attr_values.emplace_back(static_cast<std::int64_t>(x * x));
    attr_values.emplace_back(TypedValue(static_cast<float>(std::sqrt(x))));

    if (x % 10 == 0) {
      attr_values.emplace_back(kDouble);
    } else {
      attr_values.emplace_back(sign * std::sqrt(x) * x);
    }

    std::string char_col_value;
    char_col_value.append(std::to_string(sign * x)).append(" ").append(
        std::to_string(std::sqrt(x)));
    if (char_col_value.size() >= 20) {
      char_col_value = char_col_value.substr(0, 19);
    }

    attr_values.emplace_back(
        CharType::InstanceNonNullable(20)
            .makeValue(char_col_value.c_str(), char_col_value.size() + 1));

    Tuple tuple(std::move(attr_values));
    destination.insertTuple(tuple);

    sign = -sign;
  }

  processCatalogRelationNewBlockMessages();
}

void TestDatabaseLoader::processCatalogRelationNewBlockMessages() {
  AnnotatedMessage msg;
  while (bus_.ReceiveIfAvailable(foreman_client_id_, &msg)) {
    const TaggedMessage &tagged_message = msg.tagged_message;
    if (tagged_message.message_type() == kCatalogRelationNewBlockMessage) {
      serialization::CatalogRelationNewBlockMessage proto;
      CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

      test_relation_->addBlock(proto.block_id());
    }
  }
}

void TestDatabaseLoader::clear() {
  for (const CatalogRelation &relation : catalog_database_) {
    const std::vector<block_id> relation_block_ids = relation.getBlocksSnapshot();
    for (const block_id relation_block_id : relation_block_ids) {
      storage_manager_.deleteBlockOrBlobFile(relation_block_id);
    }
    catalog_database_.dropRelationById(relation.getID());
  }
  test_relation_ = nullptr;
}

}  // namespace optimizer
}  // namespace quickstep
