/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_UTIL_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_UTIL_HPP_

#include "query_execution/QueryContext.hpp"
#include "utility/lip_filter/LIPFilterDeployment.hpp"

namespace quickstep {

class LIPFilterBuilder;
class LIPFilterAdaptiveProber;

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Create a LIPFilterBuilder for the given LIPFilterDeployment in QueryContext.
 *
 * @param lip_deployment_index The id of the LIPFilterDeployment in QueryContext.
 * @param query_context The QueryContext.
 * @return A LIPFilterBuilder object, or nullptr if \p lip_deployment_index is invalid.
 */
inline LIPFilterBuilder* CreateLIPFilterBuilderHelper(
    const QueryContext::lip_deployment_id lip_deployment_index,
    const QueryContext *query_context) {
  if (lip_deployment_index == QueryContext::kInvalidLIPDeploymentId) {
    return nullptr;
  } else {
    const LIPFilterDeployment *lip_filter_deployment =
        query_context->getLIPDeployment(lip_deployment_index);
    return lip_filter_deployment->createLIPFilterBuilder();
  }
}

/**
 * @brief Create a LIPFilterAdaptiveProber for the given LIPFilterDeployment
 *        in QueryContext.
 *
 * @param lip_deployment_index The id of the LIPFilterDeployment in QueryContext.
 * @param query_context The QueryContext.
 * @return A LIPFilterAdaptiveProber object, or nullptr if \p lip_deployment_index
 *         is invalid.
 */
inline LIPFilterAdaptiveProber* CreateLIPFilterAdaptiveProberHelper(
    const QueryContext::lip_deployment_id lip_deployment_index,
    const QueryContext *query_context) {
  if (lip_deployment_index == QueryContext::kInvalidLIPDeploymentId) {
    return nullptr;
  } else {
    const LIPFilterDeployment *lip_filter_deployment =
        query_context->getLIPDeployment(lip_deployment_index);
    return lip_filter_deployment->createLIPFilterAdaptiveProber();
  }
}

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_UTIL_HPP_
