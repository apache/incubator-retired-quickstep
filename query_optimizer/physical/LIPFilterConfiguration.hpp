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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_LIP_FILTER_CONFIGURATION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_LIP_FILTER_CONFIGURATION_HPP_

#include <cstddef>
#include <map>
#include <memory>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilter.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Physical;
typedef std::shared_ptr<const Physical> PhysicalPtr;

/**
 * @brief Optimizer information for a LIP filter builder.
 */
struct LIPFilterBuildInfo {
  /**
   * @brief Constructor.
   *
   * @param build_attribute_in The attribute to build the LIP filter with.
   * @param filter_cardinality_in The LIP filter's cardinality.
   * @param filter_type_in The LIP filter's type.
   */
  LIPFilterBuildInfo(const expressions::AttributeReferencePtr &build_attribute_in,
                     const std::size_t filter_cardinality_in,
                     const LIPFilterType &filter_type_in)
      : build_attribute(build_attribute_in),
        filter_cardinality(filter_cardinality_in),
        filter_type(filter_type_in) {
  }
  const expressions::AttributeReferencePtr build_attribute;
  const std::size_t filter_cardinality;
  const LIPFilterType filter_type;
};

/**
 * @brief Optimizer information for a LIP filter prober.
 */
struct LIPFilterProbeInfo {
  /**
   * @brief Constructor.
   *
   * @param probe_attribute_in The attribute to probe the LIP filter with.
   * @param build_attribute_in The attribute that the LIP filter is built with.
   * @param builder_in The physical node that the LIP filter's builder is attached to.
   */
  LIPFilterProbeInfo(const expressions::AttributeReferencePtr &probe_attribute_in,
                     const expressions::AttributeReferencePtr &build_attribute_in,
                     const PhysicalPtr &builder_in)
      : probe_attribute(probe_attribute_in),
        build_attribute(build_attribute_in),
        builder(builder_in) {
  }
  const expressions::AttributeReferencePtr probe_attribute;
  const expressions::AttributeReferencePtr build_attribute;
  const PhysicalPtr builder;
};


class LIPFilterConfiguration;
typedef std::shared_ptr<const LIPFilterConfiguration> LIPFilterConfigurationPtr;

/**
 * @brief Configuration information of all the LIP filters in a query plan.
 */
class LIPFilterConfiguration {
 public:
  /**
   * @brief Constructor.
   */
  LIPFilterConfiguration() {
  }

  /**
   * @brief Add information for a LIP filter builder.
   *
   * @param build_attribute The attribute to build the LIP filter with.
   * @param builder The physical node to attach the LIP filter builder to.
   * @param filter_size The LIP filter's cardinality.
   * @param filter_type The LIP filter's type.
   */
  void addBuildInfo(const expressions::AttributeReferencePtr &build_attribute,
                    const PhysicalPtr &builder,
                    const std::size_t filter_size,
                    const LIPFilterType &filter_type) {
    build_info_map_[builder].emplace_back(
        build_attribute, filter_size, filter_type);
  }

  /**
   * @brief Add information for a LIP filter prober.
   *
   * @param probe_attribute The attribute to probe the LIP filter with.
   * @param prober The physical node to attach the LIP filter prober to.
   * @param build_attribute The attribute that the LIP filter is built with.
   * @param builder The physical node that the LIP filter's builder is attached to.
   */
  void addProbeInfo(const expressions::AttributeReferencePtr &probe_attribute,
                    const PhysicalPtr &prober,
                    const expressions::AttributeReferencePtr &build_attribute,
                    const PhysicalPtr &builder) {
    probe_info_map_[prober].emplace_back(
        probe_attribute, build_attribute, builder);
  }

  /**
   * @brief Get all the LIP filter builders.
   *
   * @return A map where each key is a physical node and each mapped value is
   *         a vector of all the LIP filter builders that are attached to the
   *         physical node.
   */
  const std::map<PhysicalPtr, std::vector<LIPFilterBuildInfo>>& getBuildInfoMap() const {
    return build_info_map_;
  }

  /**
   * @brief Get all the LIP filter probers.
   *
   * @return A map where each key is a physical node and each mapped value is
   *         a vector of all the LIP filter probers that are attached to the
   *         physical node.
   */
  const std::map<PhysicalPtr, std::vector<LIPFilterProbeInfo>>& getProbeInfoMap() const {
    return probe_info_map_;
  }

 private:
  std::map<PhysicalPtr, std::vector<LIPFilterBuildInfo>> build_info_map_;
  std::map<PhysicalPtr, std::vector<LIPFilterProbeInfo>> probe_info_map_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterConfiguration);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_LIP_FILTER_CONFIGURATION_HPP_ */
