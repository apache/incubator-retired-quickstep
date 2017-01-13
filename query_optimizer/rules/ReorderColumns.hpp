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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_REORDER_COLUMNS_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_REORDER_COLUMNS_HPP_

#include <string>

#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to adjust the orderings of some
 *        intermediate nodes' output attributes to improve copy performance.
 *
 * @note This optimization is based on the fact that the intermediate relations
 *       all have SPLIT_ROW_STORE layouts. If this fact gets changed, the rule's
 *       algorithm may need to be updated and the performance impact should be
 *       re-evaluated.
 */
class ReorderColumns : public Rule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   */
  ReorderColumns() {}

  ~ReorderColumns() override {}

  std::string getName() const override {
    return "ReorderColumns";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  physical::PhysicalPtr applyInternal(const physical::PhysicalPtr &input,
                                      const bool lock_ordering);

  // Whether the physical node can
  inline static bool IsTransformable(const physical::PhysicalPtr &input);

  DISALLOW_COPY_AND_ASSIGN(ReorderColumns);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_REORDER_COLUMNS_HPP_
