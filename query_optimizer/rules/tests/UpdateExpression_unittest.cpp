/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/rules/UpdateExpression.hpp"

#include <map>

#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/rules/tests/ExpressionRuleTest.hpp"
#include "query_optimizer/rules/tests/RuleTest.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;

class UpdateExpressionTest : public ExpressionRuleTest {
 protected:
  UpdateExpressionTest()
      : add_operation_(BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd)) {}

  ~UpdateExpressionTest() {}

  void setupRule(std::unique_ptr<Rule<E::Expression>> *rule) override {
    substitution_map_[relation_attribute_reference_0_0_->id()] =
        relation_attribute_reference_1_0_;
    substitution_map_[relation_attribute_reference_0_1_->id()] =
        relation_attribute_reference_1_1_;
    substitution_map_[relation_attribute_reference_0_2_->id()] =
        attribute_reference_0_;
    substitution_map_[alias_0_->id()] = alias_1_;
    rule->reset(new UpdateExpression(substitution_map_));
  }

  void SetUp() override { ExpressionRuleTest::SetUp(); }

  const BinaryOperation &add_operation_;
  std::unordered_map<expressions::ExprId, expressions::ExpressionPtr> substitution_map_;

 private:
  DISALLOW_COPY_AND_ASSIGN(UpdateExpressionTest);
};

TEST_F(UpdateExpressionTest, Test) {
  // One level.
  input_ = alias_0_;
  expect_output_ = alias_1_;
  APPLY_RULE_AND_CHECK_OUTPUT();

  input_ = relation_attribute_reference_0_0_;
  expect_output_ = relation_attribute_reference_1_0_;
  APPLY_RULE_AND_CHECK_OUTPUT();

  input_ = relation_attribute_reference_1_0_;
  expect_output_ = input_;
  APPLY_RULE_AND_CHECK_OUTPUT();

  // Two levels.
  input_ = E::BinaryExpression::Create(add_operation_,
                                       relation_attribute_reference_0_0_,
                                       attribute_reference_1_);
  expect_output_ = E::BinaryExpression::Create(
      add_operation_,
      relation_attribute_reference_1_0_,
      attribute_reference_1_);

  // Three levels.
  const E::AliasPtr alias_on_substituted_attribute = createAlias(
      relation_attribute_reference_0_0_, "test_attribute", "test_relation");
  substitution_map_[alias_on_substituted_attribute->id()] =
      attribute_reference_0_;
  input_ = E::BinaryExpression::Create(add_operation_,
                                       alias_on_substituted_attribute,
                                       relation_attribute_reference_0_2_);
  expect_output_ = input_->copyWithNewChildren(
      {attribute_reference_0_, attribute_reference_0_} /* new_children */);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

}  // namespace optimizer
}  // namespace quickstep
