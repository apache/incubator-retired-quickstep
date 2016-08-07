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

#include "query_optimizer/strategy/Selection.hpp"

#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/strategy/tests/StrategyTest.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

class SelectionTest : public StrategyTest {
 protected:
  SelectionTest() : StrategyTest() {}

  ~SelectionTest() override {}

  void setupStrategy(std::unique_ptr<Strategy> *strategy) override {
    strategy->reset(new Selection(physical_generator()));
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(SelectionTest);
};

TEST_F(SelectionTest, BasicTests) {
  input_logical_ = logical_project_0_;
  expected_physical_ = physical_project_0_;
  EXPECT_CORRECT_PHYSICAL();

  input_logical_ = logical_filter_0_;
  expected_physical_ = physical_filter_0_;
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(SelectionTest, CollapseProjectOnFilter) {
  const L::ProjectPtr project = L::Project::Create(
      logical_filter_0_,
      {alias_add_literal_0_,
       relation_attribute_reference_0_1_} /* project_expressions */);
  input_logical_ = project;
  expected_physical_ = P::Selection::Create(
      physical_table_reference_0_, project->project_expressions(),
      logical_filter_0_->filter_predicate());
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(SelectionTest, PullupProjectExpressionsToPredicates) {
  // Filter on top of Project.
  const E::PredicatePtr filter_predicate = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      E::ToRef(alias_add_literal_0_),
      literal_1_);
  const L::FilterPtr filter =
      L::Filter::Create(logical_project_0_, filter_predicate);
  input_logical_ = filter;

  // Since filter predicate references a new attribute created by the project,
  // the reference will be replaced by the project expression after
  // the Selection for the project is pulled up.
  const E::PredicatePtr filter_predicate_after_pullup =
      std::static_pointer_cast<const E::Predicate>(filter_predicate->copyWithNewChildren(
          {alias_add_literal_0_->expression(), literal_1_} /* new_children */));
  expected_physical_ = P::Selection::Create(
      physical_table_reference_0_,
      logical_project_0_->project_expressions(),
      filter_predicate_after_pullup);
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(SelectionTest, PullupProjectExpressionsToProjectExpressions) {
  // Project on top of Project.
  const E::AliasPtr alias_on_alias_reference = E::Alias::Create(
      optimizer_context()->nextExprId(),
      E::ToRef(alias_add_literal_0_),
      "alias_on_alias_reference" /* attribute_name */,
      "alias_on_alias_reference" /* attribute_alias */,
      "select_test_relation" /* relation_name */);
  const E::ExpressionPtr alias_0_add_literal_0 = E::BinaryExpression::Create(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      E::ToRef(alias_0_),
      literal_0_);
  const E::AliasPtr alias_on_alias_0_add_literal_0 = createAlias(
      alias_0_add_literal_0,
      "alias_alias_0_add_literal_0" /* attribute_name */,
      "select_test_relation" /* relation_name */);

  input_logical_ = L::Project::Create(
      logical_project_0_,
      {E::ToRef(alias_add_literal_0_),
       alias_on_alias_reference,
       alias_on_alias_0_add_literal_0} /* project_expressions */);

  // The references to the Alias in the lower Project are replaced by the
  // expressions of the Alias. alias_on_alias_reference is replaced by
  // its child (i.e. an attribute reference).
  const E::AliasPtr alias_on_alias_reference_after_pullup =
      std::static_pointer_cast<const E::Alias>(alias_on_alias_reference->copyWithNewChildren(
          {alias_add_literal_0_->expression()} /* new_children */));
  // The first child of alias_0_add_literal_0 is replaced by the literal_0_.
  const E::ExpressionPtr alias_0_add_literal_0_after_pullup =
      alias_0_add_literal_0->copyWithNewChildren(
          {alias_0_->expression(), literal_0_} /* new_children */);
  const E::AliasPtr alias_on_alias_0_add_literal_0_after_pullup =
      std::static_pointer_cast<const E::Alias>(alias_on_alias_0_add_literal_0->copyWithNewChildren(
          {alias_0_add_literal_0_after_pullup} /* new_children */));

  expected_physical_ = P::Selection::Create(
      physical_table_reference_0_,
      {alias_add_literal_0_,
       alias_on_alias_reference_after_pullup,
       alias_on_alias_0_add_literal_0_after_pullup} /* project_expressions */,
      E::PredicatePtr());

  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(SelectionTest, PullupFilters) {
  // Filter on top of Filter.
  // This is only for test. Note that it is an invalid plan which as dangling
  // attribute references in filter_predicate_1_.
  input_logical_ = L::Filter::Create(logical_filter_0_, filter_predicate_1_);
  expected_physical_ = P::Selection::CreateWithConjuction(
      physical_table_reference_0_,
      E::ToNamedExpressions(physical_table_reference_0_->getOutputAttributes()),
      {filter_predicate_1_, filter_predicate_0_} /* filter_predicates */);
  EXPECT_CORRECT_PHYSICAL();
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
