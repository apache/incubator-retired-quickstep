/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/rules/PruneColumns.hpp"

#include <memory>
#include <vector>

#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/PredicateLiteral.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/tests/PhysicalRuleTest.hpp"
#include "query_optimizer/rules/tests/RuleTest.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

class PruneColumnTest : public PhysicalRuleTest {
 protected:
  PruneColumnTest() : PhysicalRuleTest() {}

  ~PruneColumnTest() override {}

  void SetUp() override {
    PhysicalRuleTest::SetUp();
    trivial_selection_0_ =
        P::Selection::Create(physical_table_reference_0_,
                             CastSharedPtrVector<E::NamedExpression>(
                                 physical_table_reference_0_->getOutputAttributes()),
                             E::PredicatePtr());
    trivial_selection_1_ =
        P::Selection::Create(physical_table_reference_1_,
                             CastSharedPtrVector<E::NamedExpression>(
                                 physical_table_reference_1_->getOutputAttributes()),
                             E::PredicatePtr());
  }

  void setupRule(std::unique_ptr<Rule<P::Physical>> *rule) override {
    rule->reset(new PruneColumns());
  }

  // Two trivial Selections that simply project all attributes with no filtering
  // predicates.
  P::SelectionPtr trivial_selection_0_;
  P::SelectionPtr trivial_selection_1_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PruneColumnTest);
};

TEST_F(PruneColumnTest, PrimaryTest) {
  std::vector<E::NamedExpressionPtr> project_expressions_with_redundancy =
      trivial_selection_0_->project_expressions();
  const std::vector<E::NamedExpressionPtr> project_expressions_from_selection_1 =
      trivial_selection_1_->project_expressions();
  project_expressions_with_redundancy.insert(project_expressions_with_redundancy.end(),
                                             project_expressions_from_selection_1.begin(),
                                             project_expressions_from_selection_1.end());

  // Hash join on the two trivial Selections with all attributes being output.
  const P::HashJoinPtr hash_join = P::HashJoin::Create(trivial_selection_0_,
                                                       trivial_selection_1_,
                                                       {relation_attribute_reference_0_0_},
                                                       {relation_attribute_reference_1_0_},
                                                       E::PredicatePtr(),
                                                       project_expressions_with_redundancy);

  // alias_add_literal_0_: relation_attribute_reference_0_0_ + literal_0_.
  // filter_predicate_1_: relation_attribute_reference_1_0_ > literal_0_.
  const P::SelectionPtr selection = P::Selection::Create(hash_join,
                                                         {alias_add_literal_0_} /* project_expressions */,
                                                         filter_predicate_1_);
  input_ = P::TopLevelPlan::Create(selection);

  const P::SelectionPtr pruned_selection_0 = P::Selection::Create(
      physical_table_reference_0_,
      {relation_attribute_reference_0_0_} /* project_expressions */,
      E::PredicatePtr());
  const P::SelectionPtr pruned_selection_1 = P::Selection::Create(
      physical_table_reference_1_,
      {relation_attribute_reference_1_0_} /* project_expressions */,
      E::PredicatePtr());

  const std::vector<E::NamedExpressionPtr> pruned_project_expressions_for_join{
      relation_attribute_reference_0_0_, relation_attribute_reference_1_0_};

  const P::HashJoinPtr new_hash_join = P::HashJoin::Create(
      pruned_selection_0,
      pruned_selection_1,
      {relation_attribute_reference_0_0_},
      {relation_attribute_reference_1_0_},
      E::PredicatePtr(),
      pruned_project_expressions_for_join);
  const P::SelectionPtr new_selection = std::static_pointer_cast<const P::Selection>(
      selection->copyWithNewChildren({new_hash_join} /* new_children */));
  expect_output_ = P::TopLevelPlan::Create(new_selection);

  APPLY_RULE_AND_CHECK_OUTPUT();
}

TEST_F(PruneColumnTest, EmptyProjection) {
  // When we apply the PruneColumns rule, it is possible that all the project
  // expressions are removed from one physical node.

  std::vector<E::NamedExpressionPtr> project_expressions_with_redundancy =
      trivial_selection_0_->project_expressions();
  const std::vector<E::NamedExpressionPtr> project_expressions_from_selection_1 =
      trivial_selection_1_->project_expressions();
  project_expressions_with_redundancy.insert(project_expressions_with_redundancy.end(),
                                             project_expressions_from_selection_1.begin(),
                                             project_expressions_from_selection_1.end());

  // Cross product trivial_selection_0_ and trivial_selection_1_.
  const P::NestedLoopsJoinPtr cartesian_join = P::NestedLoopsJoin::Create(
      trivial_selection_0_,
      trivial_selection_1_,
      E::PredicateLiteral::Create(true),
      project_expressions_with_redundancy);

  // Only relation_attribute_reference_0_0_ for table_0 is used in the
  // Selection.
  const P::SelectionPtr selection = P::Selection::Create(
      cartesian_join,
      {alias_add_literal_0_} /* project_expressions */,
      E::PredicatePtr());
  input_ = P::TopLevelPlan::Create(selection);

  const P::SelectionPtr pruned_selection_0 = P::Selection::Create(
      physical_table_reference_0_,
      {relation_attribute_reference_0_0_} /* project_expressions */,
      E::PredicatePtr());
  // Empty project expression list.
  const P::SelectionPtr pruned_selection_1 = P::Selection::Create(
      physical_table_reference_1_,
      {} /* project_expressions */,
      E::PredicatePtr());
  const P::NestedLoopsJoinPtr new_cartesian_join = P::NestedLoopsJoin::Create(
      pruned_selection_0,
      pruned_selection_1,
      E::PredicateLiteral::Create(true),
      {relation_attribute_reference_0_0_} /* project_expressions */);
  const P::SelectionPtr new_selection = std::static_pointer_cast<const P::Selection>(
      selection->copyWithNewChildren({new_cartesian_join} /* new_children */));
  expect_output_ = P::TopLevelPlan::Create(new_selection);

  APPLY_RULE_AND_CHECK_OUTPUT();
}

}  // namespace optimizer
}  // namespace quickstep
