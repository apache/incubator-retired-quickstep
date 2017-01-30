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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_LOW_COST_DISJUNCTIVE_PREDICATE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_LOW_COST_DISJUNCTIVE_PREDICATE_HPP_

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

/**
 * @brief Rule that applies to a physical plan to push down low-cost disjunctive
 *        predicate when proper conditions are met.
 *
 * Here we elaborate the conditions.
 *
 * Let
 *   P = p_{1,1} AND ... AND p_{1, m_1} OR ... OR p_{n,1} AND ... AND p_{n, m_n}
 * be a predicate in disjunctive normal form.
 *
 * Now consider each small-cardinality relation R, if for each i in 1..n, there
 * exists at least one predicate p_{i, k_i} that is applicable to R. Then we can
 * construct a new predicate
 *   P' = p_{1, k_1} OR ... OR p_{n, k_n}
 * and push down P' to be applied to R.
 *
 * Also, if any conjunctive component in P contains more than one predicate that
 * is applicable to R, then we can combine all these applicable predicates as a
 * conjunctive component in P'.
 *
 * Finally, note that if there exists a conjunctive component that contains no
 * predicate applicable to R. Then the condition fails and we cannot do a push
 * down for R.
 */
class PushDownLowCostDisjunctivePredicate : public Rule<physical::Physical> {
 public:
  /**
   * @brief Constructor.
   */
  PushDownLowCostDisjunctivePredicate() {}

  ~PushDownLowCostDisjunctivePredicate() override {}

  std::string getName() const override {
    return "PushDownLowCostDisjunctivePredicate";
  }

  physical::PhysicalPtr apply(const physical::PhysicalPtr &input) override;

 private:
  struct PredicateInfo {
    PredicateInfo() {}
    inline void add(expressions::PredicatePtr predicate) {
      predicates.emplace_back(predicate);
    }
    std::vector<expressions::PredicatePtr> predicates;
  };

  void collectApplicablePredicates(const physical::PhysicalPtr &input);

  physical::PhysicalPtr attachPredicates(const physical::PhysicalPtr &input) const;

  static expressions::PredicatePtr CreateConjunctive(
      const std::vector<expressions::PredicatePtr> predicates);

  static expressions::PredicatePtr CreateDisjunctive(
      const std::vector<expressions::PredicatePtr> predicates);

  std::unique_ptr<cost::StarSchemaSimpleCostModel> cost_model_;

  std::vector<std::pair<physical::PhysicalPtr,
                        const std::vector<expressions::AttributeReferencePtr> *>> applicable_nodes_;
  std::map<physical::PhysicalPtr, PredicateInfo> applicable_predicates_;

  DISALLOW_COPY_AND_ASSIGN(PushDownLowCostDisjunctivePredicate);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_RULES_PUSH_DOWN_LOW_COST_DISJUNCTIVE_PREDICATE_HPP_
