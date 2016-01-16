/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/tests/TestDatabaseLoader.hpp"

#include <string>
#include <utility>
#include <vector>

#include "catalog/Catalog.hpp"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "types/CharType.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

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

  BlockPoolInsertDestination destination(&storage_manager_,
                                         test_relation_,
                                         nullptr,
                                         0 /* dummy op index */,
                                         tmb::kClientIdNone /* foreman_client_id */,
                                         nullptr /* TMB */);
  destination.addAllBlocksFromRelation();
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
