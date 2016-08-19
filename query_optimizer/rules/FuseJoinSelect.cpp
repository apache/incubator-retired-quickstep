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

  if (P::SomeHashJoin::MatchesWithConditionalCast(input, &hash_join)
      && P::SomeSelection::MatchesWithConditionalCast(hash_join->left(), &selection)
      && selection->isSimpleSelection()) {
    P::PhysicalPtr output = P::HashJoin::Create(selection->input(),
                                                hash_join->right(),
                                                hash_join->left_join_attributes(),
                                                hash_join->right_join_attributes(),
                                                hash_join->residual_predicate(),
                                                hash_join->project_expressions(),
                                                hash_join->join_type(),
                                                selection->filter_predicate());
    LOG_APPLYING_RULE(input, output);
    return output;
  }

  LOG_IGNORING_RULE(input);
  return input;
}

}
}
