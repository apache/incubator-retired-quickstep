/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/OptimizerContext.hpp"

#include <string>

namespace quickstep {
namespace optimizer {

const char *OptimizerContext::kInternalTemporaryRelationNamePrefix = "_qstemp_result_";

}  // namespace optimizer
}  // namespace quickstep
