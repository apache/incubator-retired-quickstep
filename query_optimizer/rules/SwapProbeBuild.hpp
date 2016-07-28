#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_SWAP_PROBE_BUILD_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_SWAP_PROBE_BUILD_HPP_

#include <string>

#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

namespace P = ::quickstep::optimizer::physical;
namespace E = ::quickstep::optimizer::expressions;
namespace C = ::quickstep::optimizer::cost;

class SwapProbeBuild : public BottomUpRule<P::Physical> {
 public:
  SwapProbeBuild() {
  }

  std::string getName() const override { return "SwapProbeBuild"; }

 protected:
  P::PhysicalPtr applyToNode(const P::PhysicalPtr &input) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(SwapProbeBuild);

  std::unique_ptr<C::StarSchemaSimpleCostModel> cost_model_;
};

}
}

#endif
