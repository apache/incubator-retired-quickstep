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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_BUILDER_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_BUILDER_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilter.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;
class ValueAccessor;

/** \addtogroup Utility
 *  @{
 */

class LIPFilterBuilder;
typedef std::shared_ptr<LIPFilterBuilder> LIPFilterBuilderPtr;

/**
 * @brief Helper class for building LIPFilters from a relation (i.e. ValueAccessor).
 */
class LIPFilterBuilder {
 public:
  /**
   * @brief Constructor.
   *
   * @param lip_filters The LIPFilters that will be built.
   * @param attr_ids The target attribute ids for the LIPFilters.
   * @param attr_types The target attribute types for the LIPFilters.
   */
  LIPFilterBuilder(const std::vector<LIPFilter *> &lip_filters,
                   const std::vector<attribute_id> &attr_ids,
                   const std::vector<const Type *> &attr_types) {
    DCHECK_EQ(lip_filters.size(), attr_ids.size());
    DCHECK_EQ(lip_filters.size(), attr_types.size());

    build_entries_.reserve(lip_filters.size());
    for (std::size_t i = 0; i < lip_filters.size(); ++i) {
      build_entries_.emplace_back(lip_filters[i], attr_ids[i], attr_types[i]);
    }
  }

  /**
   * @brief Insert all the values from the given ValueAccessor into the attached
   *        LIPFilters with regard to each target attribute id in \p attr_ids_.
   *
   * @param accessor The ValueAccessor which will be used to access the values.
   */
  void insertValueAccessor(ValueAccessor *accessor) {
    for (auto &entry : build_entries_) {
      entry.lip_filter->insertValueAccessor(accessor,
                                            entry.attr_id,
                                            entry.attr_type);
    }
  }

 private:
  /**
   * @brief Internal data structure for representing each LIPFilter building entry.
   */
  struct BuildEntry {
    BuildEntry(LIPFilter *lip_filter_in,
               const attribute_id attr_id_in,
               const Type *attr_type_in)
        : lip_filter(lip_filter_in),
          attr_id(attr_id_in),
          attr_type(attr_type_in) {
    }
    LIPFilter *lip_filter;
    const attribute_id attr_id;
    const Type *attr_type;
  };

  std::vector<BuildEntry> build_entries_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterBuilder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_BUILDER_HPP_
