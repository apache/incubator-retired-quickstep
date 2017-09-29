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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_ADAPTIVE_PROBER_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_ADAPTIVE_PROBER_HPP_

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "utility/Macros.hpp"
#include "utility/lip_filter/LIPFilter.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Helper class for adaptively applying a group of LIPFilters to a
 *        ValueAccessor. Here "adaptive" means that the application ordering
 *        of the filters will be adjusted on the fly based on the filters' miss
 *        rates.
 */
class LIPFilterAdaptiveProber {
 public:
  /**
   * @brief Constructor.
   *
   * @param lip_filters The LIPFilters that will be probed.
   * @param The target attribute ids for the LIPFilters.
   * @param The target attribute types for the LIPFilters.
   */
  LIPFilterAdaptiveProber(const std::vector<LIPFilter *> &lip_filters,
                          const std::vector<attribute_id> &attr_ids,
                          const std::vector<const Type *> &attr_types) {
    DCHECK_EQ(lip_filters.size(), attr_ids.size());
    DCHECK_EQ(lip_filters.size(), attr_types.size());

    probe_entries_.reserve(lip_filters.size());
    for (std::size_t i = 0; i < lip_filters.size(); ++i) {
      DCHECK(lip_filters[i] != nullptr);
      probe_entries_.emplace_back(
          new ProbeEntry(lip_filters[i], attr_ids[i], attr_types[i]));
    }
  }

  /**
   * @brief Destructor.
   */
  ~LIPFilterAdaptiveProber() {
    for (ProbeEntry *entry : probe_entries_) {
      delete entry;
    }
  }

  /**
   * @brief Apply this group of LIPFilters to the given ValueAccessor.
   *
   * @param accessor A ValueAccessor to be filtered.
   * @return A TupleIdSequence for the hit tuples in the ValueAccessor.
   */
  TupleIdSequence* filterValueAccessor(ValueAccessor *accessor) {
    const TupleIdSequence *existence_map = accessor->getTupleIdSequenceVirtual();
    if (existence_map == nullptr) {
      return filterValueAccessorNoExistenceMap(accessor);
    } else {
      return filterValueAccessorWithExistenceMap(accessor, existence_map);
    }
  }

 private:
  /**
   * @brief Internal data structure for representing each LIPFilter probing entry.
   */
  struct ProbeEntry {
    ProbeEntry(const LIPFilter *lip_filter_in,
               const attribute_id attr_id_in,
               const Type *attr_type_in)
        : lip_filter(lip_filter_in),
          attr_id(attr_id_in),
          attr_type(attr_type_in),
          miss(0),
          cnt(0) {
    }

    /**
     * @brief Whether a LIPFilter is more selective than the other.
     */
    static bool isBetterThan(const ProbeEntry *a,
                             const ProbeEntry *b) {
      return a->miss_rate > b->miss_rate;
    }

    const LIPFilter *lip_filter;
    const attribute_id attr_id;
    const Type *attr_type;
    std::uint32_t miss;
    std::uint32_t cnt;
    float miss_rate;
  };

  /**
   * @brief Sepecialized filterValueAccessor implementation where the given
   *        ValueAccessor has no existence map.
   */
  inline TupleIdSequence* filterValueAccessorNoExistenceMap(ValueAccessor *accessor) {
    const std::uint32_t num_tuples = accessor->getNumTuplesVirtual();
    std::unique_ptr<TupleIdSequence> matches(new TupleIdSequence(num_tuples));
    std::uint32_t next_batch_size = 64u;
    std::vector<tuple_id> batch(num_tuples);

    // Apply the filters in a batched manner.
    std::uint32_t batch_start = 0;
    do {
      const std::uint32_t batch_size =
          std::min(next_batch_size, num_tuples - batch_start);
      for (std::uint32_t i = 0; i < batch_size; ++i) {
        batch[i] = batch_start + i;
      }

      const std::uint32_t num_hits = filterBatch(accessor, &batch, batch_size);
      for (std::uint32_t i = 0; i < num_hits; ++i) {
        matches->set(batch[i]);
      }

      batch_start += batch_size;
      next_batch_size *= 2;
    } while (batch_start < num_tuples);

    return matches.release();
  }

  /**
   * @brief Sepecialized filterValueAccessor implementation where the given
   *        ValueAccessor has an existence map.
   */
  inline TupleIdSequence* filterValueAccessorWithExistenceMap(ValueAccessor *accessor,
                                                              const TupleIdSequence *existence_map) {
    std::unique_ptr<TupleIdSequence> matches(
        new TupleIdSequence(existence_map->length()));
    std::uint32_t next_batch_size = 64u;
    std::uint32_t num_tuples_left = existence_map->numTuples();
    std::vector<tuple_id> batch(num_tuples_left);

    // Apply the filters in a batched manner.
    TupleIdSequence::const_iterator tuple_it = existence_map->before_begin();
    do {
      const std::uint32_t batch_size =
          next_batch_size < num_tuples_left ? next_batch_size : num_tuples_left;
      for (std::uint32_t i = 0; i < batch_size; ++i) {
        ++tuple_it;
        batch[i] = *tuple_it;
      }

      const std::uint32_t num_hits = filterBatch(accessor, &batch, batch_size);
      for (std::uint32_t i = 0; i < num_hits; ++i) {
        matches->set(batch[i]);
      }

      num_tuples_left -= batch_size;
      next_batch_size *= 2;
    } while (num_tuples_left > 0);

    return matches.release();
  }

  /**
   * @brief Filter the given batch of tuples from the ValueAccessor. Remove any
   *        tuple in the batch that misses any filter.
   */
  inline std::size_t filterBatch(ValueAccessor *accessor,
                                 std::vector<tuple_id> *batch,
                                 std::uint32_t batch_size) {
    // Apply the LIPFilters one by one to the batch and update corresponding
    // cnt/miss statistics.
    for (auto *entry : probe_entries_) {
      const std::uint32_t out_size =
          entry->lip_filter->filterBatch(accessor,
                                         entry->attr_id,
                                         entry->attr_type->isNullable(),
                                         batch,
                                         batch_size);
      entry->cnt += batch_size;
      entry->miss += batch_size - out_size;
      batch_size = out_size;
    }

    // Adaptively adjust the application ordering after each batch.
    adaptEntryOrder();

    return batch_size;
  }

  /**
   * @brief Adjust LIPFilter application ordering with regard to their miss
   *        rates (i.e. selectivites).
   */
  inline void adaptEntryOrder() {
    for (auto &entry : probe_entries_) {
      entry->miss_rate = static_cast<float>(entry->miss) / entry->cnt;
    }
    std::sort(probe_entries_.begin(),
              probe_entries_.end(),
              ProbeEntry::isBetterThan);
  }

  std::vector<ProbeEntry *> probe_entries_;

  DISALLOW_COPY_AND_ASSIGN(LIPFilterAdaptiveProber);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_ADAPTIVE_PROBER_HPP_
