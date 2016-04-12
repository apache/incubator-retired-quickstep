/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "query_optimizer/strategy/Join.hpp"

#include <vector>

#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/NestedLoopsJoin.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/strategy/tests/StrategyTest.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

class JoinTest : public StrategyTest {
 protected:
  JoinTest() : StrategyTest() {}

  ~JoinTest() override {}

  void SetUp() override {
    StrategyTest::SetUp();

    logical_nested_loops_join_ =
        L::NestedLoopsJoin::Create(logical_table_reference_0_,
                                   logical_table_reference_1_,
                                   non_hash_join_predicate_0_1_);
    logical_hash_join_ = L::HashJoin::Create(
        logical_table_reference_0_,
        logical_table_reference_1_,
        {relation_attribute_reference_0_0_},
        {relation_attribute_reference_1_0_},
        nullptr /* residual_predicate */,
        L::HashJoin::JoinType::kInnerJoin);


    std::vector<E::NamedExpressionPtr> project_expressions(
        E::ToNamedExpressions(logical_table_reference_0_->getOutputAttributes()));
    const std::vector<E::NamedExpressionPtr> project_expressions_from_table_reference_1 =
        E::ToNamedExpressions(logical_table_reference_1_->getOutputAttributes());
    project_expressions.insert(project_expressions.end(),
                               project_expressions_from_table_reference_1.begin(),
                               project_expressions_from_table_reference_1.end());

    physical_nested_loops_join_ = P::NestedLoopsJoin::Create(
        physical_table_reference_0_,
        physical_table_reference_1_,
        non_hash_join_predicate_0_1_,
        project_expressions);
    physical_hash_join_ =
        P::HashJoin::Create(physical_table_reference_0_,
                            physical_table_reference_1_,
                            {relation_attribute_reference_0_0_},
                            {relation_attribute_reference_1_0_},
                            E::PredicatePtr(),
                            project_expressions,
                            P::HashJoin::JoinType::kInnerJoin);
  }

  void setupStrategy(std::unique_ptr<Strategy> *strategy) override {
    strategy->reset(new Join(physical_generator()));
  }

  L::NestedLoopsJoinPtr logical_nested_loops_join_;
  L::HashJoinPtr logical_hash_join_;

  P::NestedLoopsJoinPtr physical_nested_loops_join_;
  P::HashJoinPtr physical_hash_join_;

 private:
  DISALLOW_COPY_AND_ASSIGN(JoinTest);
};

TEST_F(JoinTest, BasicTests) {
  input_logical_ = logical_nested_loops_join_;
  expected_physical_ = physical_nested_loops_join_;
  EXPECT_CORRECT_PHYSICAL();

  input_logical_ = logical_hash_join_;
  expected_physical_ = physical_hash_join_;
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, ProjectOnJoin) {
  input_logical_ = L::Project::Create(
      logical_nested_loops_join_, logical_project_0_->project_expressions());
  expected_physical_ = P::NestedLoopsJoin::Create(
      physical_table_reference_0_,
      physical_table_reference_1_,
      non_hash_join_predicate_0_1_,
      logical_project_0_->project_expressions());
  EXPECT_CORRECT_PHYSICAL();

  input_logical_ = L::Project::Create(
      logical_hash_join_, logical_project_0_->project_expressions());
  expected_physical_ = P::HashJoin::Create(
      physical_table_reference_0_,
      physical_table_reference_1_,
      {relation_attribute_reference_0_0_},
      {relation_attribute_reference_1_0_},
      E::PredicatePtr(),
      logical_project_0_->project_expressions(),
      P::HashJoin::JoinType::kInnerJoin);
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, ProjectOnFilterOnNestedLoopsJoin) {
  const L::FilterPtr filter =
      L::Filter::Create(logical_nested_loops_join_, filter_predicate_0_);
  input_logical_ =
      L::Project::Create(filter, logical_project_1_->project_expressions());
  // Project and Filter are collapsed into the physical NestedLooopsJoin.
  expected_physical_ = P::NestedLoopsJoin::Create(
      physical_table_reference_0_,
      physical_table_reference_1_,
      E::LogicalAnd::Create(
          {filter_predicate_0_,
           logical_nested_loops_join_->join_predicate()} /* operands */),
      logical_project_1_->project_expressions());
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, ProjectOnFilterOnHashJoin) {
  const E::PredicatePtr extra_hash_join_predicate =
      E::ComparisonExpression::Create(
            ComparisonFactory::GetComparison(ComparisonID::kEqual),
            relation_attribute_reference_0_1_,
            relation_attribute_reference_1_1_);
  const L::FilterPtr filter =
      L::Filter::Create(logical_hash_join_,
                        {filter_predicate_0_, extra_hash_join_predicate});
  input_logical_ =
      L::Project::Create(filter, logical_project_1_->project_expressions());
  // Project and Filter are collapsed into the physical HashJoin.
  expected_physical_ = P::HashJoin::Create(
      physical_table_reference_0_,
      physical_table_reference_1_,
      {relation_attribute_reference_0_0_,
       relation_attribute_reference_0_1_},
      {relation_attribute_reference_1_0_,
       relation_attribute_reference_1_1_},
      filter_predicate_0_,
      logical_project_1_->project_expressions(),
      P::HashJoin::JoinType::kInnerJoin);
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, FilterOnNestedLoopsJoin) {
  input_logical_ =
      L::Filter::Create(logical_nested_loops_join_, filter_predicate_0_);
  // Filter predicates are merged into the join predicates.
  expected_physical_ = P::NestedLoopsJoin::Create(
      physical_table_reference_0_,
      physical_table_reference_1_,
      E::LogicalAnd::Create(
          {filter_predicate_0_,
           logical_nested_loops_join_->join_predicate()} /* operands */),
      physical_nested_loops_join_->project_expressions());
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, FilterOnHashJoin) {
  input_logical_ =
      L::Filter::Create(logical_hash_join_, filter_predicate_0_);
  // Filter predicates are merged into the join predicates.
  expected_physical_ = P::HashJoin::Create(
      physical_table_reference_0_,
      physical_table_reference_1_,
      {relation_attribute_reference_0_0_},
      {relation_attribute_reference_1_0_},
      filter_predicate_0_,
      physical_nested_loops_join_->project_expressions(),
      P::HashJoin::JoinType::kInnerJoin);
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, HashJoinOnSelection) {
  // Project -- HashJoin -- Project
  //                     -- Project -- Filter
  const L::HashJoinPtr hash_join =
      L::HashJoin::Create(logical_project_0_,
                          logical_project_on_filter_1_,
                          {relation_attribute_reference_0_0_},
                          {relation_attribute_reference_1_0_},
                          nullptr /* residual_predicate */,
                          L::HashJoin::JoinType::kInnerJoin);
  // References an attribute created by the left underlying project of the hash
  // join.
  const E::AliasPtr alias_on_alias_reference = E::Alias::Create(
      optimizer_context()->nextExprId(),
      E::ToRef(alias_add_literal_0_),
      "alias_on_alias_reference", /* attribute_name */
      "alias_on_alias_reference", /* attribute_alias */
      "join_test_relation" /* relation_name */);
  const E::AliasPtr alias_on_alias_reference_after_pullup =
      std::static_pointer_cast<const E::Alias>(alias_on_alias_reference->copyWithNewChildren(
          {alias_add_literal_0_->expression()} /* new_children */));
  input_logical_ = L::Project::Create(
      hash_join, {alias_on_alias_reference} /* project_expressions */);
  // Since the right Selection for the join have a filter, it will not be pulled
  // up.
  expected_physical_ = P::HashJoin::Create(
      physical_table_reference_0_,
      physical_project_on_filter_1_,
      {relation_attribute_reference_0_0_},
      {relation_attribute_reference_1_0_},
      E::PredicatePtr(),
      {alias_on_alias_reference_after_pullup} /* project_expressions */,
      P::HashJoin::JoinType::kInnerJoin);
  EXPECT_CORRECT_PHYSICAL();

  // HashJoin -- Project
  //          -- Project
  const E::AliasPtr alias_on_attribute_reference =
      createAlias(relation_attribute_reference_1_0_, "alias_on_reference",
                  "join_test_relation");
  const L::ProjectPtr logical_project_on_attribute_reference =
      L::Project::Create(
          logical_table_reference_1_,
          {alias_on_attribute_reference} /* project_expressions */);
  const P::PhysicalPtr physical_project_on_attribute_reference =
      P::Selection::Create(
          physical_table_reference_1_,
          logical_project_on_attribute_reference->project_expressions(),
          E::PredicatePtr());
  physical_generator()->setBestPhysicalForLogical(
      logical_project_on_attribute_reference,
      physical_project_on_attribute_reference);

  // The left Project cannot be pulled up, while the right can be.
  input_logical_ = L::HashJoin::Create(logical_project_0_,
                                       logical_project_on_attribute_reference,
                                       {E::ToRef(alias_add_literal_0_)},
                                       {relation_attribute_reference_1_0_},
                                       nullptr /* residual_predicate */,
                                       L::HashJoin::JoinType::kInnerJoin);
  std::vector<E::NamedExpressionPtr> project_expressions(
      E::ToNamedExpressions(physical_project_0_->getOutputAttributes()));
  project_expressions.push_back(alias_on_attribute_reference);
  expected_physical_ =
      P::HashJoin::Create(physical_project_0_,
                          physical_table_reference_1_,
                          {E::ToRef(alias_add_literal_0_)},
                          {relation_attribute_reference_1_0_},
                          E::PredicatePtr(),
                          project_expressions,
                          P::HashJoin::JoinType::kInnerJoin);
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(JoinTest, NestedLoopsJoinOnSelection) {
  // We pull a any Selection to a NestedLoopsJoin only if it does not have a
  // filter predicate.
  // Project -- NestedLoopsJoin -- Project
  //                            -- Project -- Filter
  const E::PredicatePtr join_predicate = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
      E::ToRef(alias_add_literal_0_),
      E::ToRef(alias_1_));
  const L::NestedLoopsJoinPtr nested_loops_join = L::NestedLoopsJoin::Create(
      logical_project_0_, logical_project_on_filter_1_, join_predicate);

  const E::AliasPtr alias_on_alias_reference = E::Alias::Create(
      optimizer_context()->nextExprId(),
      E::ToRef(alias_add_literal_0_),
      "alias_on_alias_reference" /* attribute_name */,
      "alias_on_alias_reference" /* attribute_alias */,
      "join_test_relation" /* relation_name */);
  input_logical_ = L::Project::Create(
      nested_loops_join, {alias_on_alias_reference} /* project_expressions */);

  // The top-level project references an attribute created by the left
  // underlying project of the nested loops join.
  // The project expression will be rewritten when the left project is merged to
  // the join.
  const E::AliasPtr alias_on_alias_reference_after_pullup =
      std::static_pointer_cast<const E::Alias>(alias_on_alias_reference->copyWithNewChildren(
          {alias_add_literal_0_->expression()} /* new_children */));
  const E::PredicatePtr join_predicate_after_pullup =
      E::ComparisonExpression::Create(
          ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
          std::static_pointer_cast<const E::NamedExpression>(alias_add_literal_0_->expression()),
          E::ToRef(alias_1_));
  expected_physical_ = P::NestedLoopsJoin::Create(
      physical_table_reference_0_,
      physical_project_on_filter_1_,
      join_predicate_after_pullup,
      {alias_on_alias_reference_after_pullup} /* project_expressions */);
  EXPECT_CORRECT_PHYSICAL();
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
