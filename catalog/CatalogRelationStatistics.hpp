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

#ifndef QUICKSTEP_CATALOG_CATALOG_RELATION_STATISTICS_HPP_
#define QUICKSTEP_CATALOG_CATALOG_RELATION_STATISTICS_HPP_

#include <cstddef>
#include <unordered_map>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief Statistics of a catalog relation. E.g. total number of tuples,
 *        number of distinct values for each column.
 **/
class CatalogRelationStatistics {
 public:
  /**
   * @brief Constructor.
   **/
  CatalogRelationStatistics()
      : num_tuples_(0) {}

  /**
   * @brief Reconstruct a CatalogRelationStatistics object from its serialized
   *        Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a CatalogRelationStatistics
   *        object, previously produced by getProto().
   **/
  explicit CatalogRelationStatistics(const serialization::CatalogRelationStatistics &proto);

  /**
   * @brief Serialize the CatalogRelationStatistics object as Protocol Buffer.
   *
   * @return The Protocol Buffer representation of the CatalogRelationStatistics
   *         object.
   **/
  serialization::CatalogRelationStatistics getProto() const;

  /**
   * @brief Set the number of tuples statistic.
   *
   * @param num_tuples The number of tuples statistic.
   */
  void setNumTuples(std::size_t num_tuples) {
    num_tuples_ = num_tuples;
  }

  /**
   * @brief Get the number of tuples statistic.
   *
   * @return The number of tuples. Returns 0 if the statistic is not set.
   */
  std::size_t getNumTuples() const {
    return num_tuples_;
  }

  /**
   * @brief Set the number of distinct values statistic for a column (catalog attribute).
   *
   * @param attr_id The id of the column.
   * @param num_distinct_values The number of distinct values statistic.
   */
  void setNumDistinctValues(attribute_id attr_id, std::size_t num_distinct_values) {
    num_distinct_values_map_[attr_id] = num_distinct_values;
  }

  /**
   * @brief Get the number of distinct values statistic for a column (catalog attribute).
   *
   * @param The id of the column.
   * @return The number of distinct values statistic for the column. Returns 0
   *         if the statistic is not set.
   */
  std::size_t getNumDistinctValues(attribute_id attr_id) const {
    const auto it = num_distinct_values_map_.find(attr_id);
    if (it == num_distinct_values_map_.end()) {
      return static_cast<std::size_t>(0);
    } else {
      return it->second;
    }
  }

 private:
  // Total number of tuples in the relation.
  std::size_t num_tuples_;

  // Number of distinct values for each column.
  std::unordered_map<attribute_id, std::size_t> num_distinct_values_map_;

  DISALLOW_COPY_AND_ASSIGN(CatalogRelationStatistics);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_RELATION_STATISTICS_HPP_
