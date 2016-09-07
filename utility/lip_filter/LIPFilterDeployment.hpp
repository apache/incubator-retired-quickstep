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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_DEPLOYMENT_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_DEPLOYMENT_HPP_

#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

namespace serialization { class LIPFilterDeployment; }

class LIPFilter;
class LIPFilterBuilder;
class LIPFilterAdaptiveProber;
class Type;

/** \addtogroup Utility
 *  @{
 */

enum class LIPFilterActionType {
  kBuild = 0,
  kProbe
};

/**
 * @brief Helper class for organizing a group of LIPFilters in the backend.
 *        Each LIPFilterDeployment object is attached to a RelationalOperator.
 */
class LIPFilterDeployment {
 public:
  /**
   * @brief Constructor.
   *
   * @param proto The Protocol Buffer representation of a LIPFilterDeployment.
   * @param lip_filters The LIPFilter objects to be deployed.
   */
  LIPFilterDeployment(const serialization::LIPFilterDeployment &proto,
                      const std::vector<std::unique_ptr<LIPFilter>> &lip_filters);
  /**
   * @brief Determine if a serialized protobuf representation of a
   *        LIPFilterDeployment is fully-formed and valid.
   *
   * @param proto A serialized protobuf representation of a LIPFilterDeployment
   *        to check for validity.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::LIPFilterDeployment &proto);

  /**
   * @brief Get the action type for this group of LIPFilters (i.e. whether
   *        to build or probe the filters).
   *
   * @return The action type.
   */
  LIPFilterActionType getActionType() const {
    return action_type_;
  }

  /**
   * @brief Create a LIPFilterBuilder for this group of LIPFilters.
   *
   * @return A new LIPFilterBuilder object for this group of LIPFilters.
   *         Caller should take ownership of the returned object.
   */
  LIPFilterBuilder* createLIPFilterBuilder() const;

  /**
   * @brief Create a LIPFilterAdaptiveProber for this group of LIPFilters.
   *
   * @return A new LIPFilterAdaptiveProber object for this group of LIPFilters.
   *         Caller should take ownership of the returned object.
   */
  LIPFilterAdaptiveProber* createLIPFilterAdaptiveProber() const;

 private:
  LIPFilterActionType action_type_;

  std::vector<LIPFilter *> lip_filters_;
  std::vector<attribute_id> attr_ids_;
  std::vector<const Type *> attr_types_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterDeployment);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_DEPLOYMENT_HPP_
