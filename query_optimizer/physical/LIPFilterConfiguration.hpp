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
#include <cstdint>
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

class LIPFilterBuildInfo;
typedef std::shared_ptr<const LIPFilterBuildInfo> LIPFilterBuildInfoPtr;

class LIPFilterProbeInfo;
typedef std::shared_ptr<const LIPFilterProbeInfo> LIPFilterProbeInfoPtr;

/**
 * @brief Optimizer information for a LIP filter builder.
 */
class LIPFilterBuildInfo {
 public:
  /**
   * @return The LIPFilter's type.
   */
  LIPFilterType filter_type() const {
    return filter_type_;
  }

  /**
   * @return The LIPFilter's build attribute.
   */
  const expressions::AttributeReferencePtr& build_attribute() const {
    return build_attribute_;
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param filter_type_in The LIP filter's type.
   * @param build_attribute_in The attribute to build the LIP filter with.
   */
  LIPFilterBuildInfo(const LIPFilterType &filter_type,
                     const expressions::AttributeReferencePtr &build_attribute)
      : filter_type_(filter_type),
        build_attribute_(build_attribute) {}

 private:
  const LIPFilterType filter_type_;
  const expressions::AttributeReferencePtr build_attribute_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterBuildInfo);
};

/**
 * @brief Subclass that contains extra information for SingleIdentityHashFilter
 *        builder.
 */
class SingleIdentityHashFilterBuildInfo : public LIPFilterBuildInfo {
 public:
  /**
   * @return The cardinality of this SingleIdentityHashFilter.
   */
  std::size_t filter_cardinality() const {
    return filter_cardinality_;
  }

  /**
   * @brief Creates a shared SingleIdentityHashFilterBuildInfo.
   *
   * @param build_attribute The attribute to build the filter with.
   * @param filter_cardinality The cardinality of this SingleIdentityHashFilter.
   */
  static LIPFilterBuildInfoPtr Create(
      const expressions::AttributeReferencePtr &build_attribute,
      const std::size_t filter_cardinality) {
    return LIPFilterBuildInfoPtr(
        new SingleIdentityHashFilterBuildInfo(build_attribute,
                                              filter_cardinality));
  }

 private:
  SingleIdentityHashFilterBuildInfo(const expressions::AttributeReferencePtr &build_attribute,
                                    const std::size_t filter_cardinality)
      : LIPFilterBuildInfo(LIPFilterType::kSingleIdentityHashFilter,
                           build_attribute),
        filter_cardinality_(filter_cardinality) {}

  const std::size_t filter_cardinality_;

  DISALLOW_COPY_AND_ASSIGN(SingleIdentityHashFilterBuildInfo);
};

/**
 * @brief Subclass that contains extra information for BitVectorExactFilter
 *        builder.
 */
class BitVectorExactFilterBuildInfo : public LIPFilterBuildInfo {
 public:
  /**
   * @return The minimum possible value for this BitVectorExactFilter.
   */
  std::int64_t min_value() const {
    return min_value_;
  }

  /**
   * @return The maximum possible value for this BitVectorExactFilter.
   */
  std::int64_t max_value() const {
    return max_value_;
  }

  /**
   * @return Whether this is an anti-filter.
   */
  bool is_anti_filter() const {
    return is_anti_filter_;
  }

  /**
   * @brief Creates a shared BitVectorExactFilterBuildInfo.
   *
   * @param build_attribute The attribute to build the filter with.
   * @param min_value The minimum possible value for this BitVectorExactFilter
   *        to set.
   * @param max_value The maximum possible value for this BitVectorExactFilter
   *        to set.
   * @param is_anti_filter Whether this is an anti-filter.
   */
  static LIPFilterBuildInfoPtr Create(
      const expressions::AttributeReferencePtr &build_attribute,
      const std::int64_t min_value,
      const std::int64_t max_value,
      const bool is_anti_filter) {
    return LIPFilterBuildInfoPtr(
        new BitVectorExactFilterBuildInfo(build_attribute,
                                          min_value,
                                          max_value,
                                          is_anti_filter));
  }

 private:
  BitVectorExactFilterBuildInfo(const expressions::AttributeReferencePtr &build_attribute,
                                const std::int64_t min_value,
                                const std::int64_t max_value,
                                const bool is_anti_filter)
      : LIPFilterBuildInfo(LIPFilterType::kBitVectorExactFilter,
                           build_attribute),
        min_value_(min_value),
        max_value_(max_value),
        is_anti_filter_(is_anti_filter) {}

  const std::int64_t min_value_;
  const std::int64_t max_value_;
  const bool is_anti_filter_;

  DISALLOW_COPY_AND_ASSIGN(BitVectorExactFilterBuildInfo);
};


/**
 * @brief Optimizer information for a LIP filter prober.
 */
class LIPFilterProbeInfo {
 public:
  /**
   * @return The attribute to probe the LIP Filter with.
   */
  const expressions::AttributeReferencePtr& probe_attribute() const {
    return probe_attribute_;
  }

  /**
   * @return The attribute that the LIP filter is built with.
   */
  const expressions::AttributeReferencePtr& build_attribute() const {
    return build_attribute_;
  }

  /**
   * @return The physical node that the LIP filter's builder is attached to.
   */
  const PhysicalPtr& builder() const {
    return builder_;
  }

  /**
   * @brief Creates a shared LIPFilterProbeInfo.
   *
   * @param probe_attribute The attribute to probe the LIP filter with.
   * @param build_attribute The attribute that the LIP filter is built with.
   * @param builder The physical node that the LIP filter's builder is attached to.
   */
  static LIPFilterProbeInfoPtr Create(
      const expressions::AttributeReferencePtr &probe_attribute,
      const expressions::AttributeReferencePtr &build_attribute,
      const PhysicalPtr &builder) {
    return LIPFilterProbeInfoPtr(
        new LIPFilterProbeInfo(probe_attribute, build_attribute, builder));
  }

 private:
  LIPFilterProbeInfo(const expressions::AttributeReferencePtr &probe_attribute,
                     const expressions::AttributeReferencePtr &build_attribute,
                     const PhysicalPtr &builder)
      : probe_attribute_(probe_attribute),
        build_attribute_(build_attribute),
        builder_(builder) {}

  const expressions::AttributeReferencePtr probe_attribute_;
  const expressions::AttributeReferencePtr build_attribute_;
  const PhysicalPtr builder_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterProbeInfo);
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
   * @param build_info A shared_ptr to LIPFilterBuildInfo.
   * @param builder The physical node to attach the LIP filter builder to.
   */
  void addBuildInfo(const LIPFilterBuildInfoPtr &build_info,
                    const PhysicalPtr &builder) {
    build_info_map_[builder].emplace_back(build_info);
  }

  /**
   * @brief Add information for a LIP filter prober.
   *
   * @param probe_info A shared_ptr to LIPFilterProbeInfo.
   * @param prober The physical node to attach the LIP filter prober to.
   */
  void addProbeInfo(const LIPFilterProbeInfoPtr &probe_info,
                    const PhysicalPtr &prober) {
    probe_info_map_[prober].emplace_back(probe_info);
  }

  /**
   * @brief Get all the LIP filter builders.
   *
   * @return A map where each key is a physical node and each mapped value is
   *         a vector of all the LIP filter builders that are attached to the
   *         physical node.
   */
  const std::map<PhysicalPtr, std::vector<LIPFilterBuildInfoPtr>>& getBuildInfoMap() const {
    return build_info_map_;
  }

  /**
   * @brief Get all the LIP filter probers.
   *
   * @return A map where each key is a physical node and each mapped value is
   *         a vector of all the LIP filter probers that are attached to the
   *         physical node.
   */
  const std::map<PhysicalPtr, std::vector<LIPFilterProbeInfoPtr>>& getProbeInfoMap() const {
    return probe_info_map_;
  }

  /**
   * @brief Clone a copy of this configuration.
   *
   * @return A copy of this confiugration. Caller should take ownership of the
   *         returned object.
   */
  LIPFilterConfiguration* clone() const {
    LIPFilterConfiguration *new_conf = new LIPFilterConfiguration();
    new_conf->build_info_map_ = build_info_map_;
    new_conf->probe_info_map_ = probe_info_map_;
    return new_conf;
  }

 private:
  std::map<PhysicalPtr, std::vector<LIPFilterBuildInfoPtr>> build_info_map_;
  std::map<PhysicalPtr, std::vector<LIPFilterProbeInfoPtr>> probe_info_map_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterConfiguration);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_LIP_FILTER_CONFIGURATION_HPP_ */
