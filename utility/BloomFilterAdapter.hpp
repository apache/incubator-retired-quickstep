/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_UTILITY_BLOOM_FILTER_ADAPTER_HPP
#define QUICKSTEP_UTILITY_BLOOM_FILTER_ADAPTER_HPP

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "utility/BloomFilter.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

class BloomFilterAdapter {
 public:
  BloomFilterAdapter(const std::vector<const BloomFilter*> &bloom_filters,
                     const std::vector<std::vector<attribute_id>> &attribute_ids)
      : num_bloom_filters_(bloom_filters.size()) {
    DCHECK_EQ(bloom_filters.size(), attribute_ids.size());

    bloom_filter_entries_.reserve(num_bloom_filters_);
    bloom_filter_entry_indices_.reserve(num_bloom_filters_);

    for (std::size_t i = 0; i < num_bloom_filters_; ++i) {
      bloom_filter_entries_.emplace_back(bloom_filters[i], attribute_ids[i]);
      bloom_filter_entry_indices_.emplace_back(i);
    }
  }

  template <typename ValueAccessorT>
  inline bool miss(const ValueAccessorT *accessor) {
    return missImpl<ValueAccessorT, true>(accessor);
  }

  template <typename ValueAccessorT, bool adapt_filters>
  inline bool missImpl(const ValueAccessorT *accessor) {
    for (std::size_t i = 0; i < num_bloom_filters_; ++i) {
      const std::size_t entry_idx = bloom_filter_entry_indices_[i];
      BloomFilterEntry &entry = bloom_filter_entries_[entry_idx];
      if (adapt_filters) {
        ++entry.cnt;
      }

      const BloomFilter *bloom_filter = entry.bloom_filter;
      for (const attribute_id &attr_id : entry.attribute_ids) {
        const std::pair<const void*, std::size_t> value_and_byte_length =
            accessor->getUntypedValueAndByteLength(attr_id);
        if (!bloom_filter->contains(static_cast<const std::uint8_t*>(value_and_byte_length.first),
                                    value_and_byte_length.second)) {
          if (adapt_filters) {
            // Record miss
            ++entry.miss;

            // Update entry order
            if (i > 0) {
              const std::size_t prev_entry_idx = bloom_filter_entry_indices_[i-1];
              if (entry.isBetterThan(bloom_filter_entries_[prev_entry_idx])) {
                bloom_filter_entry_indices_[i-1] = entry_idx;
                bloom_filter_entry_indices_[i] = prev_entry_idx;
              }
            }
          }
          return true;
        }
      }
    }
    return false;
  }

 private:
  struct BloomFilterEntry {
    BloomFilterEntry(const BloomFilter *in_bloom_filter,
                     const std::vector<attribute_id> &in_attribute_ids)
        : bloom_filter(in_bloom_filter),
          attribute_ids(in_attribute_ids),
          miss(0),
          cnt(0) {
    }

    inline bool isBetterThan(const BloomFilterEntry& other) {
      return static_cast<std::uint64_t>(miss) * other.cnt
                 > static_cast<std::uint64_t>(cnt + 5) * (other.miss + 5);
    }

    const BloomFilter *bloom_filter;
    const std::vector<attribute_id> &attribute_ids;
    std::uint32_t miss;
    std::uint32_t cnt;
  };

  const std::size_t num_bloom_filters_;
  std::vector<BloomFilterEntry> bloom_filter_entries_;
  std::vector<std::size_t> bloom_filter_entry_indices_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilterAdapter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BLOOM_FILTER_ADAPTER_HPP
