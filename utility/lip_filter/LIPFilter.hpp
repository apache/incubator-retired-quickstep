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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_HPP_

#include <cstddef>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class Type;
class ValueAccessor;

/** \addtogroup Utility
 *  @{
 */

enum class LIPFilterType {
  kBloomFilter,
  kExactFilter,
  kSingleIdentityHashFilter
};

/**
 * @brief Base class for LIP (Lookahead Information Passing) filters.
 */
class LIPFilter {
 public:
  /**
   * @breif Get the type of this LIPFilter.
   *
   * @return The type of this LIPFilter.
   */
  LIPFilterType getType() const {
    return type_;
  }

  /**
   * @brief Insert the values drawn from a ValueAccessor into this LIPFilter.
   *
   * @param accessor A ValueAccessor which will be used to access the values.
   * @param attr_id The attribute id of the values to be read from accessor.
   * @param attr_type The type of the values.
   */
  virtual void insertValueAccessor(ValueAccessor *accessor,
                                   const attribute_id attr_id,
                                   const Type *attr_type) = 0;

  /**
   * @brief Filter the given batch of tuples from a ValueAccessor. Remove any
   *        tuple in the batch that does not have a hit in this filter.
   *
   * @param accessor A ValueAccessor which will be used to access the tuples.
   * @param attr_id The attribute id of the values to be filtered.
   * @param is_attr_nullable Whether the values can be NULL.
   * @param batch The batch of tuple ids to be filtered. This vector will also
   *        be updated in place in this method to hold the output tuple ids.
   * @param batch_size The input batch size.
   *
   * @return The output batch size.
   */
  virtual std::size_t filterBatch(ValueAccessor *accessor,
                                  const attribute_id attr_id,
                                  const bool is_attr_nullable,
                                  std::vector<tuple_id> *batch,
                                  const std::size_t batch_size) const = 0;

 protected:
  explicit LIPFilter(const LIPFilterType &type)
      : type_(type) {}

 private:
  LIPFilterType type_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_HPP_
