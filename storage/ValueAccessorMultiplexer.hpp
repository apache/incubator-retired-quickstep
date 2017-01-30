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

#ifndef QUICKSTEP_STORAGE_VALUE_ACCESSOR_MULTIPLEXER_HPP_
#define QUICKSTEP_STORAGE_VALUE_ACCESSOR_MULTIPLEXER_HPP_

#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class ValueAccessor;

/** \addtogroup Utility
 *  @{
 */

enum class ValueAccessorSource {
  kBase = 0,
  kDerived,
  kInvalid
};

/**
 * @brief A data structure for representing attribute ids referring multiple
 *        ValueAccessors.
 */
struct MultiSourceAttributeId {
  MultiSourceAttributeId(const ValueAccessorSource in_source,
                         const attribute_id in_attr_id)
      : source(in_source),
        attr_id(in_attr_id) {}

  MultiSourceAttributeId(const MultiSourceAttributeId &other)
      : source(other.source),
        attr_id(other.attr_id) {}

  const ValueAccessorSource source;
  const attribute_id attr_id;
};

/**
 * @brief A class that encapsulates multiple ValueAccessors and provides helper
 *        methods for accessing the ValueAccessors with MultiSourceAttributeId.
 *
 * This class is in its very initial form that serves a small set of essential
 * functionalities for the purpose of aggregation copy elision. That is, given a
 * storage block to be aggregated on, we may have aggregations on a storage
 * attribute (e.g. SUM(x)) or on a non-trivial expression (e.g. SUM(x * y)).
 * For the former case, copy elision is applicable that the attribute gets accessed
 * directly from the storage block. In the later case, we have to create a
 * temporary data structure (i.e. ColumnVectorsValueAccessor) that stores the
 * intermediate results. Thus, we refer to the ValueAccessor created directly
 * from the storage block as the BASE accessor and the intermediate result
 * ColumnVectorsValueAccessor as the DERIVED accessor. And we utilize this class
 * (ValueAccessorMultiplexer) to pass both accessors around to enable copy elision.
 *
 * This class (together with ValueAccessorSource and MultiSourceAttributeId)
 * may be rewritten or exteneded later to more generally support copy elisions
 * in various scenarios.
 */
class ValueAccessorMultiplexer {
 public:
  /**
   * @brief Constructor for base accessor only.
   *
   * @param base_accessor The base accessor.
   */
  explicit ValueAccessorMultiplexer(ValueAccessor *base_accessor)
      : base_accessor_(base_accessor),
        derived_accessor_(nullptr) {}

  /**
   * @brief Constructor.
   *
   * @param base_accessor The base accessor.
   * @param derived_accessor The derived accessor.
   */
  ValueAccessorMultiplexer(ValueAccessor *base_accessor,
                           ValueAccessor *derived_accessor)
      : base_accessor_(base_accessor),
        derived_accessor_(derived_accessor) {}

  /**
   * @return The base accessor.
   */
  inline ValueAccessor* getBaseAccessor() const {
    return base_accessor_;
  }

  /**
   * @return The derived accessor.
   */
  inline ValueAccessor* getDerivedAccessor() const {
    return derived_accessor_;
  }

  /**
   * @brief Get the value accessor that corresponds to the specified source.
   *
   * @param source The value accessor source.
   * @return The value accessor that corresponds to \p source.
   */
  inline ValueAccessor* getValueAccessorBySource(
      const ValueAccessorSource &source) const {
    switch (source) {
      case ValueAccessorSource::kBase:
        return base_accessor_;
      case ValueAccessorSource::kDerived:
        return derived_accessor_;
      default:
        return nullptr;
    }
  }

 private:
  ValueAccessor *base_accessor_;
  ValueAccessor *derived_accessor_;

  DISALLOW_COPY_AND_ASSIGN(ValueAccessorMultiplexer);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_VALUE_ACCESSOR_MULTIPLEXER_HPP_
