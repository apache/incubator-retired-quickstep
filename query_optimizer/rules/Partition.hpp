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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_PARTITION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_PARTITION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest_prod.h"

namespace quickstep {
namespace optimizer {

class OptimizerContext;

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Added partition support.
 */
class Partition final : public BottomUpRule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   *
   * @param optimizer_context The optimizer context.
   */
  explicit Partition(OptimizerContext *optimizer_context)
      : optimizer_context_(optimizer_context) {
  }

  ~Partition() override {}

  std::string getName() const override { return "Partition"; }

 protected:
  void init(const physical::PhysicalPtr &input) override;

  physical::PhysicalPtr applyToNode(const physical::PhysicalPtr &input) override;

 private:
  /*
   * Whether left or right side needs to repartition.
   *
   * --------------------------------------------------------------------------
   * | Right \ Left     | No Partition  | Hash Partition h' | Other Partition |
   * --------------------------------------------------------------------------
   * | No Partition     | false \ false |  false \ false    |  true \ true    |
   * --------------------------------------------------------------------------
   * | Hash Partition h | false \ true  | false# \ false    | false \ true    |
   * --------------------------------------------------------------------------
   * | Other Partition  |  true \ true  |   true \ false    |  true \ true    |
   * --------------------------------------------------------------------------
   *
   * Hash Partition h / h': the partition attributes are as the same as the join attributes.
   * #: If h and h' has different number of partitions, the right side needs to repartition.
   */
  static void needsRepartitionForHashJoin(const physical::PartitionSchemeHeader *left_partition_scheme_header,
                                          const std::vector<expressions::AttributeReferencePtr> &left_join_attributes,
                                          const physical::PartitionSchemeHeader *right_partition_scheme_header,
                                          const std::vector<expressions::AttributeReferencePtr> &right_join_attributes,
                                          bool *left_needs_repartition,
                                          bool *right_needs_repartition,
                                          std::size_t *num_partitions);

  OptimizerContext *optimizer_context_;

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;

  friend class PartitionTest;

  DISALLOW_COPY_AND_ASSIGN(Partition);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_PARTITION_HPP_
