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
                     const std::vector<std::vector<attribute_id>> attribute_ids,
                     const std::vector<std::vector<std::size_t>> attr_sizes) {
    DCHECK_EQ(bloom_filters.size(), attribute_ids.size());
    DCHECK_EQ(bloom_filters.size(), attr_sizes.size());

    bloom_filter_entries_.reserve(bloom_filters.size());
    for (std::size_t i = 0; i < bloom_filters.size(); ++i) {
      auto entry = new BloomFilterEntry(
          bloom_filters[i], attribute_ids[i], attr_sizes[i]);
      bloom_filter_entries_.push_back(entry);
    }
  }

  ~BloomFilterAdapter() {
    for (auto &entry : bloom_filter_entries_)
      delete entry;
  }

  template <typename ValueAccessorT, bool adapt_filters = true>
  std::size_t bulkProbe(
      const ValueAccessorT *accessor,
      std::vector<tuple_id> &batch) {
    std::size_t out_size = batch.size();
    for (auto &entry : bloom_filter_entries_)
      out_size = bulkProbeBloomFilterEntry(*entry, accessor, batch, out_size);
    adaptEntryOrder();
    return out_size;
  }

 private:
  struct BloomFilterEntry {
    BloomFilterEntry(const BloomFilter *in_bloom_filter,
                     const std::vector<attribute_id> &in_attribute_ids,
                     const std::vector<std::size_t> &in_attribute_sizes)
        : bloom_filter(in_bloom_filter),
          attribute_ids(in_attribute_ids),
          attribute_sizes(in_attribute_sizes),
          miss(0),
          cnt(0) {
    }

    static bool isBetterThan(const BloomFilterEntry *a,
                             const BloomFilterEntry *b) {
      return a->miss_rate > b->miss_rate;
      // return static_cast<std::uint64_t>(a.miss) * b.cnt
      //            > static_cast<std::uint64_t>(a.cnt + 5) * (b.miss + 5);
    }

    const BloomFilter *bloom_filter;
    std::vector<attribute_id> attribute_ids;
    std::vector<std::size_t> attribute_sizes;
    std::uint32_t miss;
    std::uint32_t cnt;
    float miss_rate;
  };

  template <typename ValueAccessorT, bool adapt_filters = true>
  std::size_t bulkProbeBloomFilterEntry(
      BloomFilterEntry &entry,
      const ValueAccessorT *accessor,
      std::vector<tuple_id> &batch,
      const std::size_t in_size) {
    std::size_t out_size = 0;
    const BloomFilter *bloom_filter = entry.bloom_filter;

    for (std::size_t t = 0; t < in_size; ++t) {
      tuple_id tid = batch[t];
      for (std::size_t a = 0; a < entry.attribute_ids.size(); ++a) {
        const attribute_id &attr_id = entry.attribute_ids[a];
        const std::size_t size = entry.attribute_sizes[a];
        auto value = static_cast<const std::uint8_t*>(
            accessor->getUntypedValueAtAbsolutePosition(attr_id, tid));
        if (bloom_filter->contains(value, size)) {
          batch[out_size] = tid;
          ++out_size;
        }
      }
    }
    if (adapt_filters) {
      entry.cnt += in_size;
      entry.miss += (in_size - out_size);
    }
    return out_size;
  }

  void adaptEntryOrder() {
    for (auto &entry : bloom_filter_entries_)
      entry->miss_rate = static_cast<float>(entry->miss) / entry->cnt;
    std::sort(bloom_filter_entries_.begin(),
              bloom_filter_entries_.end(),
              BloomFilterEntry::isBetterThan);
  }

  std::vector<BloomFilterEntry *> bloom_filter_entries_;

  DISALLOW_COPY_AND_ASSIGN(BloomFilterAdapter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BLOOM_FILTER_ADAPTER_HPP
