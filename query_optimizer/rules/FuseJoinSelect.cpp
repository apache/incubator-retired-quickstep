#include "query_optimizer/rules/FuseJoinSelect.hpp"

#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Selection.hpp"

namespace quickstep {
namespace optimizer {

namespace P = ::quickstep::optimizer::physical;

P::PhysicalPtr FuseJoinSelect::apply(const P::PhysicalPtr &input) {
  DCHECK(input->getPhysicalType() == P::PhysicalType::kTopLevelPlan);

  return applyInternal(input);
}

P::PhysicalPtr FuseJoinSelect::applyInternal(const P::PhysicalPtr &input) {
  P::HashJoinPtr hash_join;
  const bool is_hash_inner_join =
      P::SomeHashJoin::MatchesWithConditionalCast(input, &hash_join)
          && hash_join->join_type() == P::HashJoin::JoinType::kInnerJoin;

  bool fuse_right = false;

  if (is_hash_inner_join) {
    //auto &left = hash_join->left();
    auto &right = hash_join->right();
    P::SelectionPtr selection;
    const bool is_right_child_selection =
        P::SomeSelection::MatchesWithConditionalCast(right, &selection);
    if (is_right_child_selection) {
      if (selection->input() != nullptr) {

      }
    }
  }
}

}
}
