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

#include "expressions/predicate/BloomFiltersPredicate.hpp"

#include "expressions/Expressions.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "storage/TupleIdSequence.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ValueAccessor;

serialization::Predicate BloomFiltersPredicate::getProto() const {
  serialization::Predicate proto;
  proto.set_predicate_type(serialization::Predicate::BLOOM_FILTERS);
  return proto;
}

Predicate* BloomFiltersPredicate::clone() const {
  LOG(FATAL) << "Not implemented\n";
  return nullptr;
}

bool BloomFiltersPredicate::matchesForSingleTuple(const ValueAccessor &accessor,
                                                  const tuple_id tuple) const {
  LOG(FATAL) << "Not implemented\n";
  return false;
}

bool BloomFiltersPredicate::matchesForJoinedTuples(
    const ValueAccessor &left_accessor,
    const relation_id left_relation_id,
    const tuple_id left_tuple_id,
    const ValueAccessor &right_accessor,
    const relation_id right_relation_id,
    const tuple_id right_tuple_id) const {
  LOG(FATAL) << "Not implemented\n";
  return false;
}

TupleIdSequence* BloomFiltersPredicate::getAllMatches(
    ValueAccessor *accessor,
    const SubBlocksReference *sub_blocks_ref,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map) const {
  LOG(FATAL) << "Not implemented\n";
  return nullptr;
}

}  // namespace quickstep
