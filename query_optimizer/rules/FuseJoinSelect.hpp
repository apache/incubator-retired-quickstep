#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_JOIN_SELECT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_FUSE_JOIN_SELECT_HPP_

#include <string>

#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

namespace P = ::quickstep::optimizer::physical;

class FuseJoinSelect : public BottomUpRule<P::Physical> {
 public:
  FuseJoinSelect() {
  }

  std::string getName() const override { return "FuseJoinSelect"; }

 protected:
  P::PhysicalPtr applyToNode(const P::PhysicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(FuseJoinSelect);
};

}
}

#endif
