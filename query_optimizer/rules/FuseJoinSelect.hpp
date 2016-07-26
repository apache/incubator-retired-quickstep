#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_JOIN_SELECT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_JOIN_SELECT_HPP_

#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"

namespace quickstep {
namespace optimizer {

namespace P = ::quickstep::optimizer::physical;

class FuseJoinSelect : public Rule<physical::Physical> {
 public:
  FuseJoinSelect() {
  }

  P::PhysicalPtr apply(const P::PhysicalPtr &input) override;

 private:
  P::PhysicalPtr applyInternal(const P::PhysicalPtr &input);
};

}
}

#endif
