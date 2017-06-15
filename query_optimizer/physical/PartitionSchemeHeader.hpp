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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PARTITION_SCHEME_HEADER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PARTITION_SCHEME_HEADER_HPP_

#include <cstddef>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "query_optimizer/expressions/ExprId.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

/**
 * @brief Store the partitioning info for a physical plan node.
 */
struct PartitionSchemeHeader {
  // Each id produces the same partition effect.
  typedef std::set<expressions::ExprId> EquivalentPartitionExprIds;
  typedef std::vector<EquivalentPartitionExprIds> PartitionExprIds;

  enum class PartitionType {
    kHash = 0,
    kRandom,
    kRange
  };

  /**
   * @brief Constructor.
   *
   * @param type The type of partitioning.
   * @param num_partitions_in The number of partitions to be created.
   * @param expr_ids The attributes on which the partitioning happens.
   **/
  PartitionSchemeHeader(const PartitionType type,
                        const std::size_t num_partitions_in,
                        PartitionExprIds &&expr_ids)  // NOLINT(whitespace/operators)
      : partition_type(type),
        num_partitions(num_partitions_in),
        partition_expr_ids(std::move(expr_ids)) {
  }

  /**
   * @brief Copy constructor.
   *
   * @param other The copy-from instance.
   **/
  PartitionSchemeHeader(const PartitionSchemeHeader &other)
      : partition_type(other.partition_type),
        num_partitions(other.num_partitions),
        partition_expr_ids(other.partition_expr_ids) {
  }

  /**
   * @brief Check whether the partition scheme header is hash-based.
   *
   * @return True if this is a hash partition scheme header. Otherwise, false.
   **/
  bool isHashPartition() const {
    return partition_type == PartitionType::kHash;
  }

  /**
   * @brief Check whether the partition scheme header is euqal.
   *
   * @param other The instance to compare.
   *
   * @return True if both are the same partition scheme header. Otherwise, false.
   **/
  bool equal(const PartitionSchemeHeader &other) const {
    return partition_type == other.partition_type &&
           num_partitions == other.num_partitions &&
           partition_expr_ids == other.partition_expr_ids;
  }

  /**
   * @brief Check whether the partition scheme header is reusable given 'output_expr_ids'.
   *
   * @param output_expr_ids The expressions used to check against PartitionExprIds.
   *
   * @return True if this is reusable. Otherwise, false.
   **/
  bool reusablePartitionScheme(const std::unordered_set<expressions::ExprId> &output_expr_ids) const;

  /**
   * @brief Display the partition scheme header as a string.
   *
   * @return the string serialized partition scheme header.
   **/
  std::string toString() const;

  const PartitionType partition_type;
  const std::size_t num_partitions;
  const PartitionExprIds partition_expr_ids;

  // Copyable.
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PARTITION_SCHEME_HEADER_HPP_
