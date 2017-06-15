/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "query_optimizer/physical/PartitionSchemeHeader.hpp"

#include <set>
#include <string>
#include <unordered_set>

#include "query_optimizer/expressions/ExprId.hpp"

#include "glog/logging.h"

using std::string;

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = expressions;

bool PartitionSchemeHeader::reusablePartitionScheme(const std::unordered_set<E::ExprId> &output_expr_ids) const {
  DCHECK(!output_expr_ids.empty());

  if (partition_expr_ids.empty()) {
    return false;
  }

  for (const EquivalentPartitionExprIds &expr_ids : partition_expr_ids) {
    bool has_matched_expr = false;
    for (const E::ExprId expr_id : expr_ids) {
      if (output_expr_ids.find(expr_id) != output_expr_ids.end()) {
        has_matched_expr = true;
        break;
      }
    }

    if (!has_matched_expr) {
      return false;
    }
  }

  return true;
}

namespace {

void PrintEquivalentPartitionExprIds(const PartitionSchemeHeader::EquivalentPartitionExprIds &equivalent_ids,
                                     string *serialized_header) {
  *serialized_header += "{ ";

  auto cit = equivalent_ids.cbegin();
  *serialized_header += std::to_string(*cit);
  for (++cit; cit != equivalent_ids.cend(); ++cit) {
    *serialized_header += ", ";
    *serialized_header += std::to_string(*cit);
  }

  *serialized_header += " }";
}

}  // namespace

string PartitionSchemeHeader::toString() const {
  string serialized_header("PARTITION BY ");
  switch (partition_type) {
    case PartitionType::kHash:
      serialized_header += "HASH";
      break;
    case PartitionType::kRandom:
      serialized_header += "RANDOM";
      break;
    case PartitionType::kRange:
      serialized_header += "RANGE";
      break;
    default:
      break;
  }

  serialized_header += " ( ";
  if (!partition_expr_ids.empty()) {
    auto cit = partition_expr_ids.cbegin();
    PrintEquivalentPartitionExprIds(*cit, &serialized_header);
    for (++cit; cit != partition_expr_ids.cend(); ++cit) {
      serialized_header += ", ";
      PrintEquivalentPartitionExprIds(*cit, &serialized_header);
    }
  }
  serialized_header += " ) PARTITIONS ";
  serialized_header += std::to_string(num_partitions);

  return serialized_header;
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
