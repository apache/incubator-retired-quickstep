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
#include "types/LongType.hpp"
#include "types/NullType.hpp"
#include "types/TypedValue.hpp"
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
      : is_exact_(false),
        num_tuples_(kNullValue) {}

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
   * @brief Check whether the statistics are exact for the relation.
   *
   * return True if the statistics are exact for the relation, false otherwise.
   */
  bool isExact() const {
    return is_exact_;
  }

  /**
   * @brief Set the boolean flag that indicates whether the statistics are exact
   *        for the relation.
   *
   * @param Exactness of the statistics.
   */
  void setExactness(const bool is_exact) {
    is_exact_ = is_exact;
  }

  /**
   * @brief Check whether the number of tuples statistic is available.
   *
   * @return True if the number of tuples statistic is available, false otherwise.
   */
  bool hasNumTuples() const {
    return !num_tuples_.isNull();
  }

  /**
   * @brief Get the number of tuples statistic.
   *
   * @return The number of tuples. Returns 0 if the statistic is not set.
   */
  std::size_t getNumTuples() const {
    DCHECK(hasNumTuples());
    return num_tuples_.getLiteral<std::int64_t>();
  }

  /**
   * @brief Set the number of tuples statistic.
   *
   * @param num_tuples The number of tuples statistic.
   */
  void setNumTuples(const std::size_t num_tuples) {
    num_tuples_ = LongType::InstanceNonNullable().makeValue(&num_tuples);
  }

  /**
   * @brief Check whether the number of distinct values statistic is available
   *        for a column.
   *
   * @param The id of the column.
   * @return True if the number of distinct values statistic is available,
   *         false otherwise.
   */
  bool hasNumDistinctValues(const attribute_id attr_id) const {
    const ColumnStats *stats = getColumnStats(attr_id);
    return (stats != nullptr && !stats->num_distinct_values.isNull());
  }

  /**
   * @brief Get the number of distinct values statistic for a column.
   *
   * @param The id of the column.
   * @return The number of distinct values statistic for the column.
   */
  std::size_t getNumDistinctValues(const attribute_id attr_id) const {
    DCHECK(hasNumDistinctValues(attr_id));
    return column_stats_.at(attr_id).num_distinct_values.getLiteral<std::int64_t>();
  }

  /**
   * @brief Set the number of distinct values statistic for a column.
   *
   * @param attr_id The id of the column.
   * @param num_distinct_values The number of distinct values statistic.
   */
  void setNumDistinctValues(const attribute_id attr_id,
                            const std::size_t num_distinct_values) {
    column_stats_[attr_id].num_distinct_values =
        LongType::InstanceNonNullable().makeValue(&num_distinct_values);
  }

  /**
   * @brief Check whether the minimum value statistic is available for a column.
   *
   * @param The id of the column.
   * @return True if the minimum value statistic is available, false otherwise.
   */
  bool hasMinValue(const attribute_id attr_id) const {
    const ColumnStats *stats = getColumnStats(attr_id);
    return (stats != nullptr && !stats->min_value.isNull());
  }

  /**
   * @brief Get the minimum value statistic for a column.
   *
   * @param The id of the column.
   * @return The minimum value statistic for the column.
   */
  const TypedValue& getMinValue(const attribute_id attr_id) const {
    DCHECK(hasMinValue(attr_id));
    return column_stats_.at(attr_id).min_value;
  }

  /**
   * @brief Set the minimum value statistic for a column.
   *
   * @param The id of the column.
   * @return The minimum value statistic for the column.
   */
  void setMinValue(const attribute_id attr_id,
                   const TypedValue &min_value) {
    column_stats_[attr_id].min_value = min_value;
  }

  /**
   * @brief Check whether the maximum value statistic is available for a column.
   *
   * @param The id of the column.
   * @return True if the maximum value statistic is available, false otherwise.
   */
  bool hasMaxValue(const attribute_id attr_id) const {
    const ColumnStats *stats = getColumnStats(attr_id);
    return (stats != nullptr && !stats->max_value.isNull());
  }

  /**
   * @brief Get the maximum value statistic for a column.
   *
   * @param The id of the column.
   * @return The maximum value statistic for the column.
   */
  const TypedValue& getMaxValue(const attribute_id attr_id) const {
    DCHECK(hasMaxValue(attr_id));
    return column_stats_.at(attr_id).max_value;
  }

  /**
   * @brief Set the maximum value statistic for a column.
   *
   * @param The id of the column.
   * @return The maximum value statistic for the column.
   */
  void setMaxValue(const attribute_id attr_id,
                   const TypedValue &max_value) {
    column_stats_[attr_id].max_value = max_value;
  }

 private:
  struct ColumnStats {
    ColumnStats()
        : num_distinct_values(kNullValue),
          min_value(kNullValue),
          max_value(kNullValue) {
    }
    TypedValue num_distinct_values;
    TypedValue min_value;
    TypedValue max_value;
  };

  inline const ColumnStats* getColumnStats(const attribute_id attr_id) const {
    const auto it = column_stats_.find(attr_id);
    return (it == column_stats_.end() ? nullptr : &it->second);
  }

  static const TypedValue kNullValue;

  // Whether the statistics are exact (i.e. up-to-date) for the relation.
  bool is_exact_;

  // Total number of tuples in the relation.
  TypedValue num_tuples_;

  // Statistics for each column.
  std::unordered_map<attribute_id, ColumnStats> column_stats_;

  DISALLOW_COPY_AND_ASSIGN(CatalogRelationStatistics);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_RELATION_STATISTICS_HPP_
