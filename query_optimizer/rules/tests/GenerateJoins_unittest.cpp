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

#include "query_optimizer/rules/GenerateJoins.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/HashJoin.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/MultiwayCartesianJoin.hpp"
#include "query_optimizer/logical/NestedLoopsJoin.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/tests/LogicalRuleTest.hpp"
#include "query_optimizer/rules/tests/RuleTest.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

class JoinGeneratorTest : public LogicalRuleTest {
 protected:
  JoinGeneratorTest() {}

  ~JoinGeneratorTest() {}

  void setupRule(std::unique_ptr<Rule<L::Logical>> *rule) override {
    rule->reset(new GenerateJoins());
  }

  void SetUp() override {
    LogicalRuleTest::SetUp();
    logical_table_reference_2_ = createTableReference("table_2");
    logical_table_reference_3_ = createTableReference("table_3");
  }

  // Creates an equality predicate between <left_attribute> and
  // <right_attribute>.
  E::PredicatePtr createEqualityPredicate(
      const E::AttributeReferencePtr &left_attribute,
      const E::AttributeReferencePtr &right_attribute) const {
    return E::ComparisonExpression::Create(
        ComparisonFactory::GetComparison(ComparisonID::kEqual),
        left_attribute,
        right_attribute);
  }

  // Creates a cartesian join among <join_operands>.
  L::LogicalPtr createCartesianJoin(
      const std::vector<L::LogicalPtr> &join_operands) const {
    return L::MultiwayCartesianJoin::Create(join_operands);
  }

  // Creates a Filter with <filter_predicates> on top of a MultiwayCartesianJoin
  // on <join_operands>.
  L::LogicalPtr createTestInput(
      const std::vector<L::LogicalPtr> &join_operands,
      const std::vector<E::PredicatePtr> &filter_predicates) const {
    return L::Filter::Create(createCartesianJoin(join_operands),
                             filter_predicates);
  }

  // Creates a left-deep tree of hash join operators.
  L::LogicalPtr createChainedHashJoins(
      const std::vector<L::LogicalPtr> &join_operands,
      const std::vector<std::pair<E::AttributeReferencePtr,
                                  E::AttributeReferencePtr>> &join_attribute_pairs) const {
    CHECK_GE(join_operands.size(), 2u);
    CHECK_EQ(join_operands.size(), join_attribute_pairs.size() + 1);

    L::LogicalPtr hash_join = L::HashJoin::Create(
        join_operands[0],
        join_operands[1],
        {join_attribute_pairs[0].first},
        {join_attribute_pairs[0].second},
        nullptr /* residual_predicate */,
        L::HashJoin::JoinType::kInnerJoin);
    for (size_t i = 2; i < join_operands.size(); ++i) {
      hash_join = L::HashJoin::Create(hash_join,
                                      join_operands[i],
                                      {join_attribute_pairs[i - 1].first},
                                      {join_attribute_pairs[i - 1].second},
                                      nullptr /* residual_predicate */,
                                      L::HashJoin::JoinType::kInnerJoin);
    }
    return hash_join;
  }

  // Creates a left-deep tree of nested loops join operators.
  L::LogicalPtr createChainedNestedLoopsJoins(
      const std::vector<L::LogicalPtr> &join_operands,
      const std::vector<E::PredicatePtr> &join_predicates) const {
    CHECK_GE(join_operands.size(), 2u);
    CHECK_EQ(join_operands.size(), join_predicates.size() + 1);

    L::LogicalPtr nested_loops_join = L::NestedLoopsJoin::Create(
        join_operands[0], join_operands[1], join_predicates[0]);
    for (size_t i = 2; i < join_operands.size(); ++i) {
      nested_loops_join = L::NestedLoopsJoin::Create(
          nested_loops_join, join_operands[i], join_predicates[i - 1]);
    }
    return nested_loops_join;
  }

  L::TableReferencePtr logical_table_reference_2_;
  L::TableReferencePtr logical_table_reference_3_;

 private:
  DISALLOW_COPY_AND_ASSIGN(JoinGeneratorTest);
};

TEST_F(JoinGeneratorTest, CreateHashJoinForBinaryEqualityPredicate) {
  const E::PredicatePtr join_0_1 = createEqualityPredicate(
      relation_attribute_reference_0_0_, relation_attribute_reference_1_0_);
  const E::PredicatePtr join_1_2 =
      createEqualityPredicate(logical_table_reference_1_->getOutputAttributes()[1],
                              logical_table_reference_2_->getOutputAttributes()[1]);
  const E::PredicatePtr join_2_3 =
      createEqualityPredicate(logical_table_reference_2_->getOutputAttributes()[0],
                              logical_table_reference_3_->getOutputAttributes()[0]);

  input_ = createTestInput(
      {logical_table_reference_0_,
       logical_table_reference_1_,
       logical_table_reference_2_,
       logical_table_reference_3_} /* join_operands */,
      {join_0_1, join_1_2, join_2_3} /* filter_predicates */);
  expect_output_ = createChainedHashJoins(
      {logical_table_reference_0_,
       logical_table_reference_1_,
       logical_table_reference_2_,
       logical_table_reference_3_} /* join_operands */,
      {{relation_attribute_reference_0_0_, relation_attribute_reference_1_0_},
       {logical_table_reference_1_->getOutputAttributes()[1],
        logical_table_reference_2_->getOutputAttributes()[1]},
       {logical_table_reference_2_->getOutputAttributes()[0],
        logical_table_reference_3_->getOutputAttributes()[0]}} /* join_attribute_pairs */);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(JoinGeneratorTest, CreateNestedLoopsJoinForNonBinaryEqualityPredicate) {
  const E::PredicatePtr join_0_1 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
      relation_attribute_reference_0_0_,
      relation_attribute_reference_1_0_);
  const E::PredicatePtr join_0_1_2 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      E::BinaryExpression::Create(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          relation_attribute_reference_0_0_,
          relation_attribute_reference_1_1_),
      logical_table_reference_2_->getOutputAttributes()[1]);
  const E::PredicatePtr join_2_3 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual),
      logical_table_reference_2_->getOutputAttributes()[0],
      logical_table_reference_3_->getOutputAttributes()[0]);

  input_ = createTestInput(
      {logical_table_reference_0_,
       logical_table_reference_1_,
       logical_table_reference_2_,
       logical_table_reference_3_} /* join_operands */,
      {join_0_1, join_0_1_2, join_2_3} /* filter_predicates */);
  expect_output_ = createChainedNestedLoopsJoins(
      {logical_table_reference_0_,
       logical_table_reference_1_,
       logical_table_reference_2_,
       logical_table_reference_3_} /* join_operands */,
      {join_0_1, join_0_1_2, join_2_3} /* join_predicates */);

  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(JoinGeneratorTest, MultiAttributesHashJoin) {
  // Two join predicates between logical_table_reference_0_ and logical_table_reference_1_
  // and a single table predicate.
  const E::PredicatePtr join_0_1 = createEqualityPredicate(
      relation_attribute_reference_0_0_, relation_attribute_reference_1_0_);
  const E::PredicatePtr join_1_0 = createEqualityPredicate(
      relation_attribute_reference_1_1_, relation_attribute_reference_0_1_);
  const E::PredicatePtr single_table_predicate = createEqualityPredicate(
      relation_attribute_reference_0_0_, relation_attribute_reference_0_1_);

  input_ = createTestInput(
      {logical_table_reference_0_, logical_table_reference_1_} /* join_operands */,
      {join_0_1, join_1_0, single_table_predicate} /* filter_predicates */);
  expect_output_ =
      L::Filter::Create(L::HashJoin::Create(logical_table_reference_0_,
                                            logical_table_reference_1_,
                                            {relation_attribute_reference_0_0_,
                                             relation_attribute_reference_0_1_},
                                            {relation_attribute_reference_1_0_,
                                             relation_attribute_reference_1_1_},
                                            nullptr /* residual_predicate */,
                                            L::HashJoin::JoinType::kInnerJoin),
                        single_table_predicate);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(JoinGeneratorTest, DoNotNestedLoopsJoinTwoLogicalMoreThanOnce) {
  const E::PredicatePtr join_0_1 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
      relation_attribute_reference_0_0_,
      relation_attribute_reference_1_0_);
  const E::PredicatePtr join_0_1_2 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      E::BinaryExpression::Create(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          relation_attribute_reference_0_0_,
          relation_attribute_reference_1_1_),
      logical_table_reference_2_->getOutputAttributes()[1]);
  E::PredicatePtr join_1_2 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
      relation_attribute_reference_1_1_,
      logical_table_reference_2_->getOutputAttributes()[0]);

  input_ =
      createTestInput({logical_table_reference_0_,
                       logical_table_reference_1_,
                       logical_table_reference_2_} /* join_operands */,
                      {join_0_1, join_0_1_2, join_1_2} /* filter_predicate */);
  // join_1_2 is not used as a join predicate.
  expect_output_ =
      L::Filter::Create(createChainedNestedLoopsJoins(
                            {logical_table_reference_0_,
                             logical_table_reference_1_,
                             logical_table_reference_2_} /* join_operands */,
                            {join_0_1, join_0_1_2} /* join_predicates*/),
                        join_1_2);
  APPLY_RULE_AND_CHECK_OUTPUT();

  // Change the non-equality predicate to equality predicate.
  join_1_2 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      relation_attribute_reference_1_1_,
      logical_table_reference_2_->getOutputAttributes()[0]);
  input_ =
      createTestInput({logical_table_reference_0_,
                       logical_table_reference_1_,
                       logical_table_reference_2_} /* join_operands */,
                      {join_0_1, join_0_1_2, join_1_2} /* filter_predicate */);
  // join_0_1_2 is not used as a join predicate.
  expect_output_ = L::Filter::Create(
      createChainedNestedLoopsJoins(
          {logical_table_reference_0_,
           createChainedHashJoins(
               {logical_table_reference_1_,
                logical_table_reference_2_} /* join_operands */,
               {{relation_attribute_reference_1_1_,
                 logical_table_reference_2_->getOutputAttributes()[0]}} /* join_attribute_pairs */)
          } /* join_operands */,
          {join_0_1} /* join_predicates*/),
      join_0_1_2);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

// If there is no available join predicate for a MultiwayCartesianJoin, we
// create nested loops joins with empty
// predicates.
TEST_F(JoinGeneratorTest, CreateCartesianJoinWhenNoJoinPredicatesAvailable) {
  // No filter predicate.
  input_ = createCartesianJoin({logical_table_reference_0_,
                                logical_table_reference_1_,
                                logical_table_reference_2_} /* join_operands */);
  expect_output_ = createChainedNestedLoopsJoins(
      {logical_table_reference_0_,
       logical_table_reference_1_,
       logical_table_reference_2_} /* join_operands */,
      {E::PredicatePtr(), E::PredicatePtr()} /* join_predicates */);
  APPLY_RULE_AND_CHECK_OUTPUT();

  // Unary join predicate.
  const E::PredicatePtr filter_predicate = createEqualityPredicate(
      relation_attribute_reference_0_0_, relation_attribute_reference_0_1_);
  input_ = createTestInput({logical_table_reference_0_,
                            logical_table_reference_1_,
                            logical_table_reference_2_} /* join_operands */,
                           {filter_predicate});
  expect_output_ =
      L::Filter::Create(createChainedNestedLoopsJoins(
                            {logical_table_reference_2_,
                             createChainedNestedLoopsJoins(
                                 {logical_table_reference_0_,
                                  logical_table_reference_1_} /* join_operands */,
                                 {E::PredicatePtr()})} /* join_operands */,
                            {E::PredicatePtr()} /* join_predicates */),
                        filter_predicate);
  APPLY_RULE_AND_CHECK_OUTPUT();

  // Ternary join predicate.
  const E::PredicatePtr join_0_2_1 = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      E::BinaryExpression::Create(
          BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
          relation_attribute_reference_0_0_,
          logical_table_reference_2_->getOutputAttributes()[1]),
      relation_attribute_reference_1_1_);
  input_ = createTestInput({logical_table_reference_0_,
                            logical_table_reference_1_,
                            logical_table_reference_2_} /* join_operands */,
                           {join_0_2_1});
  expect_output_ = createChainedNestedLoopsJoins(
      {createChainedNestedLoopsJoins(
           {logical_table_reference_0_, logical_table_reference_1_} /* join_operands */,
           {E::PredicatePtr()} /* join_predicate */),
       logical_table_reference_2_} /* join_operands */,
      {join_0_2_1} /* join_predicate */);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

}  // namespace optimizer
}  // namespace quickstep
