/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_EXPRESSIONS_PREDICATE_BLOOM_FILTERS_PREDICATE_HPP_
#define QUICKSTEP_EXPRESSIONS_PREDICATE_BLOOM_FILTERS_PREDICATE_HPP_

#include <memory>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/BloomFilter.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class TupleIdSequence;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

class BloomFiltersPredicate : public Predicate {
 public:
  BloomFiltersPredicate() {
  }

  ~BloomFiltersPredicate() override {
  }

  serialization::Predicate getProto() const override;

  Predicate* clone() const override;

  PredicateType getPredicateType() const override {
    return kBloomFilters;
  }

  bool matchesForSingleTuple(const ValueAccessor &accessor,
                             const tuple_id tuple) const override;

  bool matchesForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override;

  TupleIdSequence* getAllMatches(ValueAccessor *accessor,
                                 const SubBlocksReference *sub_blocks_ref,
                                 const TupleIdSequence *filter,
                                 const TupleIdSequence *existence_map) const override;

  void addBloomFilter(const BloomFilter *bloom_filter) {
    bloom_filters_.emplace_back(bloom_filter);
  }

  void addAttributeId(const attribute_id probe_attribute_id) {
    bloom_filter_attribute_ids_.push_back(probe_attribute_id);
  }

 private:
  std::vector<const BloomFilter *> bloom_filters_;
  std::vector<attribute_id> bloom_filter_attribute_ids_;

  friend class PredicateTest;

  DISALLOW_COPY_AND_ASSIGN(BloomFiltersPredicate);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_PREDICATE_BLOOM_FILTERS_PREDICATE_HPP_
