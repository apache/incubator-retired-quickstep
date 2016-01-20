/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "query_optimizer/rules/PushDownFilter.hpp"

#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/LogicalNot.hpp"
#include "query_optimizer/expressions/LogicalOr.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/MultiwayCartesianJoin.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/rules/tests/LogicalRuleTest.hpp"
#include "query_optimizer/rules/tests/RuleTest.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

class PushDownFilterTest : public LogicalRuleTest {
 protected:
  PushDownFilterTest() {}

  ~PushDownFilterTest() override {}

  void setupRule(std::unique_ptr<Rule<L::Logical>> *rule) override {
    rule->reset(new PushDownFilter());
  }

  void SetUp() override {
    LogicalRuleTest::SetUp();

    join_ = L::MultiwayCartesianJoin::Create(
        {logical_table_reference_0_, logical_table_reference_1_} /* operands */);
    join_predicate_ = E::ComparisonExpression::Create(
        ComparisonFactory::GetComparison(ComparisonID::kEqual),
        relation_attribute_reference_0_0_,
        relation_attribute_reference_1_1_);
    single_table_predicate_0_ = E::ComparisonExpression::Create(
        ComparisonFactory::GetComparison(ComparisonID::kEqual),
        relation_attribute_reference_0_0_,
        literal_0_);
    single_table_predicate_1_ = E::ComparisonExpression::Create(
        ComparisonFactory::GetComparison(ComparisonID::kEqual),
        relation_attribute_reference_1_0_,
        literal_1_);
  }

  L::JoinPtr join_;
  E::PredicatePtr join_predicate_;
  E::PredicatePtr single_table_predicate_0_;
  E::PredicatePtr single_table_predicate_1_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PushDownFilterTest);
};

TEST_F(PushDownFilterTest, NoFilterWithoutChange) {
  input_ = L::Project::Create(
      logical_table_reference_0_,
      {relation_attribute_reference_0_0_,
       relation_attribute_reference_0_1_} /* project_expressions */);
  expect_output_ = input_;
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(PushDownFilterTest, FilterWithJoinPredicates) {
  const E::PredicatePtr predicate =
      E::LogicalNot::Create(E::ComparisonExpression::Create(
          ComparisonFactory::GetComparison(ComparisonID::kEqual),
          relation_attribute_reference_0_0_,
          relation_attribute_reference_1_1_));
  input_ = L::Filter::Create(join_, predicate);
  expect_output_ = input_;
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(PushDownFilterTest, FilterWithConjunctiveSingleTablePredicates) {
  input_ = L::Filter::Create(
      join_,
      E::LogicalAnd::Create({join_predicate_, single_table_predicate_0_, single_table_predicate_1_} /* operands */));
  const L::JoinPtr join_after_push_down = L::MultiwayCartesianJoin::Create(
      {L::Filter::Create(logical_table_reference_0_, single_table_predicate_0_),
       L::Filter::Create(logical_table_reference_1_,
                         single_table_predicate_1_)} /* operands */);
  expect_output_ = L::Filter::Create(join_after_push_down, join_predicate_);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(PushDownFilterTest, FilterWithDisjunctiveSingleTablePredicates) {
  const E::PredicatePtr disjunctive_single_table_predicate =
      E::LogicalOr::Create({single_table_predicate_0_,
                            single_table_predicate_1_} /* operands */);

  input_ = L::Filter::Create(
      join_, E::LogicalAnd::Create(
                 {join_predicate_,
                  disjunctive_single_table_predicate} /* operands */));
  expect_output_ = input_;
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(PushDownFilterTest, FullFilterPushedDown) {
  // All the predicates in the Filter is pushed down and the filter is removed.
  input_ = L::Filter::Create(
      join_,
      E::LogicalAnd::Create({single_table_predicate_0_,
                             single_table_predicate_1_} /* operands */));
  expect_output_ = L::MultiwayCartesianJoin::Create(
      {L::Filter::Create(logical_table_reference_0_, single_table_predicate_0_),
       L::Filter::Create(logical_table_reference_1_,
                         single_table_predicate_1_)} /* operands */);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(PushDownFilterTest, FilterChainPushDown) {
  // A chain of a Filter is collapsed.
  const L::FilterPtr filter =
      L::Filter::Create(join_, single_table_predicate_0_);
  const L::FilterPtr filter_on_filter =
      L::Filter::Create(filter, single_table_predicate_1_);
  input_ = L::Filter::Create(filter_on_filter, single_table_predicate_0_);
  expect_output_ = L::MultiwayCartesianJoin::Create(
      {L::Filter::Create(
           logical_table_reference_0_,
           E::LogicalAnd::Create({single_table_predicate_0_,
                                  single_table_predicate_0_} /* operands */)),
       L::Filter::Create(logical_table_reference_1_, single_table_predicate_1_)});
  APPLY_RULE_AND_CHECK_OUTPUT();
}

}  // namespace optimizer
}  // namespace quickstep
