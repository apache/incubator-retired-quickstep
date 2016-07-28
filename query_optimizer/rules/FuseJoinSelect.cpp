#include <iostream>

#include "query_optimizer/rules/FuseJoinSelect.hpp"

#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/rules/Rule.hpp"

namespace quickstep {
namespace optimizer {

namespace P = ::quickstep::optimizer::physical;
namespace E = ::quickstep::optimizer::expressions;

P::PhysicalPtr FuseJoinSelect::applyToNode(const P::PhysicalPtr &input) {
  P::HashJoinPtr hash_join;
  P::SelectionPtr selection;
  P::TableReferencePtr table_reference;

  if (P::SomeHashJoin::MatchesWithConditionalCast(input, &hash_join)
      && hash_join->join_type() == P::HashJoin::JoinType::kInnerJoin
      && P::SomeSelection::MatchesWithConditionalCast(hash_join->left(), &selection)
      && P::SomeTableReference::MatchesWithConditionalCast(selection->input(), &table_reference)) {
    const E::PredicatePtr filter_predicate = selection->filter_predicate();
    P::PhysicalPtr output = P::HashJoin::CreateWithFusedSelect(hash_join->left(),
                                                               table_reference,
                                                               hash_join->left_join_attributes(),
                                                               hash_join->right_join_attributes(),
                                                               hash_join->residual_predicate(),
                                                               hash_join->project_expressions(),
                                                               hash_join->join_type(),
                                                               filter_predicate);
    LOG_APPLYING_RULE(input, output);
    return output;
  }

  LOG_IGNORING_RULE(input);
  return input;
}

}
}
