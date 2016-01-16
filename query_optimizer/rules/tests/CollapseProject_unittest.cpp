/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/rules/CollapseProject.hpp"

#include <memory>

#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/rules/tests/LogicalRuleTest.hpp"
#include "query_optimizer/rules/tests/RuleTest.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

class CollapseProjectTest : public LogicalRuleTest {
 protected:
  CollapseProjectTest() {}

  ~CollapseProjectTest() override {}

  void setupRule(std::unique_ptr<Rule<L::Logical>> *rule) override {
    rule->reset(new CollapseProject());
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CollapseProjectTest);
};

TEST_F(CollapseProjectTest, Tests) {
  // Merge three-level Projects into one.
  const E::ExpressionPtr alias_0_add_literal_0 = E::BinaryExpression::Create(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      E::ToRef(alias_0_),
      literal_0_);
  const E::AliasPtr alias_on_alias_0_add_literal_0 = createAlias(
      alias_0_add_literal_0,
      "alias_alias_0_add_literal_0" /* attribute_name */,
      "select_test_relation" /* relation_name */);
  const L::ProjectPtr second_level_project = L::Project::Create(
      logical_project_0_ /* first level project */,
      {alias_on_alias_0_add_literal_0} /* project_expressions */);
  const E::AliasPtr alias_on_alias_on_alias_0_add_literal_0 = createAlias(
      E::ToRef(alias_on_alias_0_add_literal_0),
      "alias_on_alias_on_alias_0_add_literal_0" /* attribute_name */,
      "select_test_relation" /* relation_name */);
  // The top level project.
  input_ = L::Project::Create(second_level_project,
                              {alias_on_alias_on_alias_0_add_literal_0});

  const E::ExpressionPtr alias_0_add_literal_0_after_pullup =
      alias_0_add_literal_0->copyWithNewChildren(
          {alias_0_->expression(), literal_0_} /* new_children */);
  const E::AliasPtr alias_on_alias_on_alias_0_add_literal_0_after_pullup =
      std::static_pointer_cast<const E::Alias>(
          alias_on_alias_on_alias_0_add_literal_0->copyWithNewChildren(
              {alias_0_add_literal_0_after_pullup} /* new_children */));
  expect_output_ = L::Project::Create(
      logical_table_reference_0_,
      {alias_on_alias_on_alias_0_add_literal_0_after_pullup} /* project_expressions */);
  APPLY_RULE_AND_CHECK_OUTPUT();
}

}  // namespace optimizer
}  // namespace quickstep
