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

#include "relational_operators/HashJoinOperator.hpp"

#include <algorithm>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/HashTable.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/SubBlocksReference.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleReference.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/EventProfiler.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"
#include "utility/lip_filter/LIPFilterUtil.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::unique_ptr;
using std::vector;

namespace quickstep {

namespace {

typedef std::vector<std::pair<tuple_id, tuple_id>> VectorOfTupleIdPair;
typedef std::pair<std::vector<tuple_id>, std::vector<tuple_id>> PairOfTupleIdVector;

// Functor passed to HashTable::getAllFromValueAccessor() to collect matching
// tuples from the inner relation. It stores matching tuple ID pairs
// in an unordered_map keyed by inner block ID and a vector of
// pairs of (build-tupleID, probe-tuple-ID).
class VectorsOfPairsJoinedTuplesCollector {
 public:
  VectorsOfPairsJoinedTuplesCollector() {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor,
                         const TupleReference &tref) {
    joined_tuples_[tref.block].emplace_back(tref.tuple, accessor.getCurrentPosition());
  }

  // Get a mutable pointer to the collected map of joined tuple ID pairs. The
  // key is inner block_id, values are vectors of joined tuple ID pairs with
  // tuple ID from the inner block on the left and the outer block on the
  // right.
  inline std::unordered_map<block_id, VectorOfTupleIdPair>* getJoinedTuples() {
    return &joined_tuples_;
  }

 private:
  // NOTE(chasseur): It would also be possible to represent joined tuples for a
  // particular pair of blocks as a TupleIdSequence/BitVector over the
  // cross-product of all tuples from both blocks, but simply using pairs of
  // tuple-IDs is expected to be more space efficient if the result set is less
  // than 1/64 the cardinality of the cross-product.
  std::unordered_map<block_id, VectorOfTupleIdPair> joined_tuples_;
};

// Another collector using an unordered_map keyed on inner block just like above,
// except that it uses of a pair of (build-tupleIDs-vector, probe-tuple-IDs-vector).
class PairsOfVectorsJoinedTuplesCollector {
 public:
  PairsOfVectorsJoinedTuplesCollector() {
  }

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor,
                         const TupleReference &tref) {
    auto &entry = joined_tuples_[tref.block];
    entry.first.emplace_back(tref.tuple);
    entry.second.emplace_back(accessor.getCurrentPosition());
  }

  // Get a mutable pointer to the collected map of joined tuple ID pairs. The
  // key is inner block_id, value is a pair consisting of
  // inner block tuple IDs (first) and outer block tuple IDs (second).
  inline std::unordered_map<block_id, PairOfTupleIdVector>* getJoinedTuples() {
    return &joined_tuples_;
  }

 private:
  std::unordered_map<
    block_id,
    std::pair<std::vector<tuple_id>, std::vector<tuple_id>>> joined_tuples_;
};

class SemiAntiJoinTupleCollector {
 public:
  explicit SemiAntiJoinTupleCollector(TupleIdSequence *filter)
      : filter_(filter) {}

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor) {
    filter_->set(accessor.getCurrentPosition(), false);
  }

 private:
  TupleIdSequence *filter_;
};

class OuterJoinTupleCollector {
 public:
  explicit OuterJoinTupleCollector(TupleIdSequence *filter)
      : filter_(filter) {}

  template <typename ValueAccessorT>
  inline void operator()(const ValueAccessorT &accessor,
                         const TupleReference &tref) {
    joined_tuples_[tref.block].emplace_back(tref.tuple, accessor.getCurrentPosition());
  }

  template <typename ValueAccessorT>
  inline void recordMatch(const ValueAccessorT &accessor) {
    filter_->set(accessor.getCurrentPosition(), false);
  }

  inline std::unordered_map<block_id, std::vector<std::pair<tuple_id, tuple_id>>>*
      getJoinedTupleMap() {
    return &joined_tuples_;
  }

 private:
  std::unordered_map<block_id, std::vector<std::pair<tuple_id, tuple_id>>> joined_tuples_;
  // BitVector on the probe relation. 1 if the corresponding tuple has no match.
  TupleIdSequence *filter_;
};

}  // namespace

bool HashJoinOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  switch (join_type_) {
    case JoinType::kInnerJoin:
      return getAllNonOuterJoinWorkOrders<HashInnerJoinWorkOrder>(
          container, query_context, storage_manager);
    case JoinType::kLeftSemiJoin:
      return getAllNonOuterJoinWorkOrders<HashSemiJoinWorkOrder>(
          container, query_context, storage_manager);
    case JoinType::kLeftAntiJoin:
      return getAllNonOuterJoinWorkOrders<HashAntiJoinWorkOrder>(
          container, query_context, storage_manager);
    case JoinType::kLeftOuterJoin:
      return getAllOuterJoinWorkOrders(
          container, query_context, storage_manager);
    default:
      LOG(FATAL) << "Unknown join type in HashJoinOperator::getAllWorkOrders()";
  }
  return false;
}

template <class JoinWorkOrderClass>
bool HashJoinOperator::getAllNonOuterJoinWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager) {
  // We wait until the building of global hash table is complete.
  if (blocking_dependencies_met_) {
    DCHECK(query_context != nullptr);

    const Predicate *residual_predicate =
        query_context->getPredicate(residual_predicate_index_);
    const vector<unique_ptr<const Scalar>> &selection =
        query_context->getScalarGroup(selection_index_);
    InsertDestination *output_destination =
        query_context->getInsertDestination(output_destination_index_);

    if (probe_relation_is_stored_) {
      if (started_) {
        return true;
      }

      for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
        const JoinHashTable &hash_table =
            *(query_context->getJoinHashTable(hash_table_index_, part_id));

        for (const block_id probe_block_id : probe_relation_block_ids_[part_id]) {
          container->addNormalWorkOrder(
              new JoinWorkOrderClass(query_id_, build_relation_, probe_relation_, join_key_attributes_,
                                     any_join_key_attributes_nullable_, part_id, probe_block_id, residual_predicate,
                                     selection, hash_table, output_destination, storage_manager,
                                     CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context)),
              op_index_);
        }
      }
      started_ = true;
      return true;
    } else {
      for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
        const JoinHashTable &hash_table =
            *(query_context->getJoinHashTable(hash_table_index_, part_id));

        while (num_workorders_generated_[part_id] < probe_relation_block_ids_[part_id].size()) {
          container->addNormalWorkOrder(
              new JoinWorkOrderClass(query_id_, build_relation_, probe_relation_, join_key_attributes_,
                                     any_join_key_attributes_nullable_, part_id,
                                     probe_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                     residual_predicate, selection, hash_table, output_destination, storage_manager,
                                     CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context)),
              op_index_);
          ++num_workorders_generated_[part_id];
        }  // end while
      }  // end for
      return done_feeding_input_relation_;
    }  // end else (probe_relation_is_stored_)
  }  // end if (blocking_dependencies_met_)
  return false;
}

bool HashJoinOperator::getAllOuterJoinWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager) {
  // We wait until the building of global hash table is complete.
  if (blocking_dependencies_met_) {
    DCHECK(query_context != nullptr);

    const vector<unique_ptr<const Scalar>> &selection =
        query_context->getScalarGroup(selection_index_);

    InsertDestination *output_destination =
        query_context->getInsertDestination(output_destination_index_);

    if (probe_relation_is_stored_) {
      if (started_) {
        return true;
      }

      for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
        const JoinHashTable &hash_table =
            *(query_context->getJoinHashTable(hash_table_index_, part_id));

        for (const block_id probe_block_id : probe_relation_block_ids_[part_id]) {
          container->addNormalWorkOrder(
              new HashOuterJoinWorkOrder(query_id_, build_relation_, probe_relation_, join_key_attributes_,
                                         any_join_key_attributes_nullable_, part_id, probe_block_id, selection,
                                         is_selection_on_build_, hash_table, output_destination, storage_manager,
                                         CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context)),
              op_index_);
        }
      }
      started_ = true;
      return true;
    } else {
      for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
        const JoinHashTable &hash_table =
            *(query_context->getJoinHashTable(hash_table_index_, part_id));

        while (num_workorders_generated_[part_id] < probe_relation_block_ids_[part_id].size()) {
          container->addNormalWorkOrder(
              new HashOuterJoinWorkOrder(query_id_, build_relation_, probe_relation_, join_key_attributes_,
                                         any_join_key_attributes_nullable_, part_id,
                                         probe_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                         selection, is_selection_on_build_, hash_table, output_destination,
                                         storage_manager,
                                         CreateLIPFilterAdaptiveProberHelper(lip_deployment_index_, query_context)),
              op_index_);
          ++num_workorders_generated_[part_id];
        }
      }
      return done_feeding_input_relation_;
    }  // end else (probe_relation_is_stored_)
  }  // end if (blocking_dependencies_met_)
  return false;
}

bool HashJoinOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  switch (join_type_) {
    case JoinType::kInnerJoin:
      return getAllNonOuterJoinWorkOrderProtos(container, serialization::HashJoinWorkOrder::HASH_INNER_JOIN);
    case JoinType::kLeftSemiJoin:
      return getAllNonOuterJoinWorkOrderProtos(container, serialization::HashJoinWorkOrder::HASH_SEMI_JOIN);
    case JoinType::kLeftAntiJoin:
      return getAllNonOuterJoinWorkOrderProtos(container, serialization::HashJoinWorkOrder::HASH_ANTI_JOIN);
    case JoinType::kLeftOuterJoin:
      return getAllOuterJoinWorkOrderProtos(container);
    default:
      LOG(FATAL) << "Unknown join type in HashJoinOperator::getAllWorkOrderProtos()";
  }
}

bool HashJoinOperator::getAllNonOuterJoinWorkOrderProtos(
    WorkOrderProtosContainer *container,
    const serialization::HashJoinWorkOrder::HashJoinWorkOrderType hash_join_type) {
  // We wait until the building of global hash table is complete.
  if (!blocking_dependencies_met_) {
    return false;
  }

  if (probe_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
      for (const block_id probe_block_id : probe_relation_block_ids_[part_id]) {
        container->addWorkOrderProto(
            createNonOuterJoinWorkOrderProto(hash_join_type, probe_block_id, part_id),
            op_index_);
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
      while (num_workorders_generated_[part_id] < probe_relation_block_ids_[part_id].size()) {
        container->addWorkOrderProto(
            createNonOuterJoinWorkOrderProto(hash_join_type,
                                             probe_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                             part_id),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }

    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* HashJoinOperator::createNonOuterJoinWorkOrderProto(
    const serialization::HashJoinWorkOrder::HashJoinWorkOrderType hash_join_type,
    const block_id block, const partition_id part_id) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::HASH_JOIN);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::HashJoinWorkOrder::hash_join_work_order_type, hash_join_type);
  proto->SetExtension(serialization::HashJoinWorkOrder::build_relation_id, build_relation_.getID());
  proto->SetExtension(serialization::HashJoinWorkOrder::probe_relation_id, probe_relation_.getID());
  for (const attribute_id attr_id : join_key_attributes_) {
    proto->AddExtension(serialization::HashJoinWorkOrder::join_key_attributes, attr_id);
  }
  proto->SetExtension(serialization::HashJoinWorkOrder::any_join_key_attributes_nullable,
                      any_join_key_attributes_nullable_);
  proto->SetExtension(serialization::HashJoinWorkOrder::insert_destination_index, output_destination_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::join_hash_table_index, hash_table_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::partition_id, part_id);
  proto->SetExtension(serialization::HashJoinWorkOrder::selection_index, selection_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::block_id, block);
  proto->SetExtension(serialization::HashJoinWorkOrder::residual_predicate_index, residual_predicate_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::lip_deployment_index, lip_deployment_index_);

  return proto;
}

bool HashJoinOperator::getAllOuterJoinWorkOrderProtos(WorkOrderProtosContainer *container) {
  // We wait until the building of global hash table is complete.
  if (!blocking_dependencies_met_) {
    return false;
  }

  if (probe_relation_is_stored_) {
    if (started_) {
      return true;
    }

    for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
      for (const block_id probe_block_id : probe_relation_block_ids_[part_id]) {
        container->addWorkOrderProto(createOuterJoinWorkOrderProto(probe_block_id, part_id), op_index_);
      }
    }
    started_ = true;
    return true;
  } else {
    for (std::size_t part_id = 0; part_id < build_num_partitions_; ++part_id) {
      while (num_workorders_generated_[part_id] < probe_relation_block_ids_[part_id].size()) {
        container->addWorkOrderProto(
            createOuterJoinWorkOrderProto(probe_relation_block_ids_[part_id][num_workorders_generated_[part_id]],
                                          part_id),
            op_index_);
        ++num_workorders_generated_[part_id];
      }
    }

    return done_feeding_input_relation_;
  }
}

serialization::WorkOrder* HashJoinOperator::createOuterJoinWorkOrderProto(const block_id block,
                                                                          const partition_id part_id) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::HASH_JOIN);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::HashJoinWorkOrder::hash_join_work_order_type,
                      serialization::HashJoinWorkOrder::HASH_OUTER_JOIN);
  proto->SetExtension(serialization::HashJoinWorkOrder::build_relation_id, build_relation_.getID());
  proto->SetExtension(serialization::HashJoinWorkOrder::probe_relation_id, probe_relation_.getID());
  for (const attribute_id attr_id : join_key_attributes_) {
    proto->AddExtension(serialization::HashJoinWorkOrder::join_key_attributes, attr_id);
  }
  proto->SetExtension(serialization::HashJoinWorkOrder::any_join_key_attributes_nullable,
                      any_join_key_attributes_nullable_);
  proto->SetExtension(serialization::HashJoinWorkOrder::insert_destination_index, output_destination_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::join_hash_table_index, hash_table_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::selection_index, selection_index_);
  proto->SetExtension(serialization::HashJoinWorkOrder::block_id, block);
  proto->SetExtension(serialization::HashJoinWorkOrder::partition_id, part_id);
  proto->SetExtension(serialization::HashJoinWorkOrder::lip_deployment_index, lip_deployment_index_);

  for (const bool is_attribute_on_build : is_selection_on_build_) {
    proto->AddExtension(serialization::HashJoinWorkOrder::is_selection_on_build, is_attribute_on_build);
  }

  return proto;
}


void HashInnerJoinWorkOrder::execute() {
  BlockReference probe_block(
      storage_manager_->getBlock(block_id_, probe_relation_));
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // Probe the LIPFilters to generate an existence bitmap for probe_accessor, if enabled.
  std::unique_ptr<TupleIdSequence> existence_map;
  std::unique_ptr<ValueAccessor> base_accessor;
  if (lip_filter_adaptive_prober_ != nullptr) {
    base_accessor.reset(probe_accessor.release());
    existence_map.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(base_accessor.get()));
    probe_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  }


//  if (probe_accessor->getImplementationType() == ValueAccessor::Implementation::kSplitRowStore) {
//    executeWithCopyElision(probe_accessor.get());
//  } else {
    executeWithoutCopyElision(probe_accessor.get());
//  }

}

void HashInnerJoinWorkOrder::executeWithoutCopyElision(ValueAccessor *probe_accessor) {
  auto *container = simple_profiler.getContainer();
  auto *event_hash = container->getEventLine("ProbeHash");
  event_hash->emplace_back();
  VectorsOfPairsJoinedTuplesCollector collector;
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessor(
        probe_accessor,
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor,
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }
  event_hash->back().endEvent();

  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  auto *materialize_line = container->getEventLine("materialize");
  materialize_line->emplace_back();
  MutableBlockReference output_block;
  for (std::pair<const block_id, VectorOfTupleIdPair>
           &build_block_entry : *collector.getJoinedTuples()) {
    BlockReference build_block =
        storage_manager_->getBlock(build_block_entry.first, build_relation_);
    const TupleStorageSubBlock &build_store = build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(build_store.createValueAccessor());

    // Evaluate '*residual_predicate_', if any.
    //
    // TODO(chasseur): We might consider implementing true vectorized
    // evaluation for join predicates that are not equijoins (although in
    // general that would require evaluating and materializing some expressions
    // over the cross-product of all tuples in a pair of blocks in order to
    // evaluate the predicate). We could use a heuristic where we only do the
    // vectorized materialization and evaluation if the set of matches from the
    // hash join is below a reasonable threshold so that we don't blow up
    // temporary memory requirements to an unreasonable degree.
    if (residual_predicate_ != nullptr) {
      VectorOfTupleIdPair filtered_matches;

      for (const std::pair<tuple_id, tuple_id> &hash_match
           : build_block_entry.second) {
        if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                        build_relation_id,
                                                        hash_match.first,
                                                        *probe_accessor,
                                                        probe_relation_id,
                                                        hash_match.second)) {
          filtered_matches.emplace_back(hash_match);
        }
      }

      build_block_entry.second = std::move(filtered_matches);
    }

    ColumnVectorsValueAccessor temp_result;
    for (auto selection_cit = selection_.begin();
         selection_cit != selection_.end();
         ++selection_cit) {
      temp_result.addColumn((*selection_cit)->getAllValuesForJoin(build_relation_id,
                                                                  build_accessor.get(),
                                                                  probe_relation_id,
                                                                  probe_accessor,
                                                                  build_block_entry.second));
    }

//    output_destination_->bulkInsertTuples(&temp_result);
    output_destination_->bulkInsertTuples(&temp_result, &output_block);
  }

  output_destination_->returnBlock(&output_block);
  materialize_line->back().endEvent();
}

void HashInnerJoinWorkOrder::executeWithCopyElision(ValueAccessor *probe_accessor) {
  PairsOfVectorsJoinedTuplesCollector collector;
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessor(
        probe_accessor,
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor,
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  constexpr std::size_t kNumIndexes = 3u;
  constexpr std::size_t kBuildIndex = 0, kProbeIndex = 1u, kTempIndex = 2u;

  // Create a map of ValueAccessors and what attributes we want to pick from them.
  std::vector<std::pair<ValueAccessor *, std::vector<attribute_id>>> accessor_attribute_map(
      kNumIndexes, std::make_pair(nullptr /* late binding ValueAccessor */,
                                  vector<attribute_id>(selection_.size(), kInvalidCatalogId)));

  std::vector<const Scalar *> non_trivial_expressions;
  attribute_id dest_attr = 0;

  for (const auto &scalar : selection_) {
    // If the Scalar (column) is not an attribute in build/probe blocks, we will
    // insert it into a ColumnVectorsValueAccessor.
    if (scalar->getDataSource() != Scalar::ScalarDataSource::kAttribute) {
      // Current destination attribute maps to the column we'll create now.
      accessor_attribute_map[kTempIndex].second[dest_attr] = non_trivial_expressions.size();
      non_trivial_expressions.emplace_back(scalar.get());
    } else {
      const CatalogAttribute &attr = static_cast<const ScalarAttribute *>(scalar.get())->getAttribute();
      const attribute_id attr_id = attr.getID();
      if (attr.getParent().getID() == build_relation_id) {
        accessor_attribute_map[kBuildIndex].second[dest_attr] = attr_id;
      } else {
        accessor_attribute_map[kProbeIndex].second[dest_attr] = attr_id;
      }
    }
    ++dest_attr;
  }

  for (std::pair<const block_id, PairOfTupleIdVector>
           &build_block_entry : *collector.getJoinedTuples()) {
    BlockReference build_block =
        storage_manager_->getBlock(build_block_entry.first, build_relation_);
    const TupleStorageSubBlock &build_store = build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(build_store.createValueAccessor());
    const std::vector<tuple_id> &build_tids = build_block_entry.second.first;
    const std::vector<tuple_id> &probe_tids = build_block_entry.second.second;

    // Evaluate '*residual_predicate_', if any.
    //
    // TODO(chasseur): We might consider implementing true vectorized
    // evaluation for join predicates that are not equijoins (although in
    // general that would require evaluating and materializing some expressions
    // over the cross-product of all tuples in a pair of blocks in order to
    // evaluate the predicate). We could use a heuristic where we only do the
    // vectorized materialization and evaluation if the set of matches from the
    // hash join is below a reasonable threshold so that we don't blow up
    // temporary memory requirements to an unreasonable degree.
    if (residual_predicate_ != nullptr) {
      PairOfTupleIdVector filtered_matches;

      for (std::size_t i = 0; i < build_tids.size(); ++i) {
        if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                        build_relation_id,
                                                        build_tids[i],
                                                        *probe_accessor,
                                                        probe_relation_id,
                                                        probe_tids[i])) {
          filtered_matches.first.emplace_back(build_tids[i]);
          filtered_matches.second.emplace_back(probe_tids[i]);
        }
      }

      build_block_entry.second = std::move(filtered_matches);
    }

    // TODO(chasseur): See TODO in NestedLoopsJoinOperator.cpp about limiting
    // the size of materialized temporary results. In common usage, this
    // probably won't be an issue for hash-joins, but in the worst case a hash
    // join can still devolve into a cross-product.

    // We also need a temp value accessor to store results of any scalar expressions.
    ColumnVectorsValueAccessor temp_result;
    if (!non_trivial_expressions.empty()) {
      // The getAllValuesForJoin function below needs joined tuple IDs as a
      // vector of pair of (build-tuple-ID, probe-tuple-ID), and we have a pair
      // of (build-tuple-IDs-vector, probe-tuple-IDs-vector). So we'll have to
      // zip our two vectors together.
      VectorOfTupleIdPair zipped_joined_tuple_ids;
      for (std::size_t i = 0; i < build_tids.size(); ++i) {
        zipped_joined_tuple_ids.emplace_back(build_tids[i], probe_tids[i]);
      }

      for (const Scalar *scalar : non_trivial_expressions) {
        temp_result.addColumn(scalar->getAllValuesForJoin(build_relation_id,
                                                          build_accessor.get(),
                                                          probe_relation_id,
                                                          probe_accessor,
                                                          zipped_joined_tuple_ids));
      }
    }

    // We now create ordered value accessors for both build and probe side,
    // using the joined tuple IDs.
    std::unique_ptr<ValueAccessor> ordered_build_accessor(
        build_accessor->createSharedOrderedTupleIdSequenceAdapterVirtual(build_tids));
    std::unique_ptr<ValueAccessor> ordered_probe_accessor(
        probe_accessor->createSharedOrderedTupleIdSequenceAdapterVirtual(probe_tids));

    accessor_attribute_map[kBuildIndex].first = ordered_build_accessor.get();
    accessor_attribute_map[kProbeIndex].first = ordered_probe_accessor.get();
    accessor_attribute_map[kTempIndex].first = &temp_result;

    // NOTE(chasseur): calling the bulk-insert method of InsertDestination once
    // for each pair of joined blocks incurs some extra overhead that could be
    // avoided by keeping checked-out MutableBlockReferences across iterations
    // of this loop, but that would get messy when combined with partitioning.
    output_destination_->bulkInsertTuplesFromValueAccessors(accessor_attribute_map);
  }
}

void HashSemiJoinWorkOrder::execute() {
  if (residual_predicate_ == nullptr) {
    executeWithoutResidualPredicate();
  } else {
    executeWithResidualPredicate();
  }
}

void HashSemiJoinWorkOrder::executeWithResidualPredicate() {
  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // Probe the LIPFilters to generate an existence bitmap for probe_accessor, if enabled.
  std::unique_ptr<TupleIdSequence> existence_map;
  std::unique_ptr<ValueAccessor> base_accessor;
  if (lip_filter_adaptive_prober_ != nullptr) {
    base_accessor.reset(probe_accessor.release());
    existence_map.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(base_accessor.get()));
    probe_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  }

  // We collect all the matching probe relation tuples, as there's a residual
  // preidcate that needs to be applied after collecting these matches.
  VectorsOfPairsJoinedTuplesCollector collector;
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessor(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  // Get a filter for tuples in the given probe block.
  TupleIdSequence filter(probe_store.getMaxTupleID() + 1);
  for (const std::pair<const block_id,
                       std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTuples()) {
    // First element of the pair build_block_entry is the build block ID
    // 2nd element of the pair is a vector of pairs, in each of which -
    // 1st element is a matching tuple ID from the inner (build) relation.
    // 2nd element is a matching tuple ID from the outer (probe) relation.

    // Get the block from the build relation for this pair of matched tuples.
    BlockReference build_block =
        storage_manager_->getBlock(build_block_entry.first, build_relation_);
    const TupleStorageSubBlock &build_store =
        build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(
        build_store.createValueAccessor());
    for (const std::pair<tuple_id, tuple_id> &hash_match
         : build_block_entry.second) {
      // For each pair, 1st element is a tuple ID from the build relation in the
      // given build block, 2nd element is a tuple ID from the probe relation.
      if (filter.get(hash_match.second)) {
        // We have already found matches for this tuple that belongs to the
        // probe side, skip it.
        continue;
      }
      if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                      build_relation_id,
                                                      hash_match.first,
                                                      *probe_accessor,
                                                      probe_relation_id,
                                                      hash_match.second)) {
        filter.set(hash_match.second, true);
      }
    }
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_store.createValueAccessor(&filter));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end();
       ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(
        probe_accessor_with_filter.get(), &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashSemiJoinWorkOrder::executeWithoutResidualPredicate() {
  DCHECK(residual_predicate_ == nullptr);

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // Probe the LIPFilters to generate an existence bitmap for probe_accessor, if enabled.
  std::unique_ptr<TupleIdSequence> existence_map;
  std::unique_ptr<ValueAccessor> base_accessor;
  if (lip_filter_adaptive_prober_ != nullptr) {
    base_accessor.reset(probe_accessor.release());
    existence_map.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(base_accessor.get()));
    probe_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  }

  if (existence_map == nullptr) {
    existence_map.reset(probe_store.getExistenceMap());
  }

  SemiAntiJoinTupleCollector collector(existence_map.get());
  // We collect all the probe relation tuples which have at least one matching
  // tuple in the build relation. As a performance optimization, the hash table
  // just looks for the existence of the probing key in the hash table and sets
  // the bit for the probing key in the collector. The optimization works
  // because there is no residual predicate in this case, unlike
  // executeWithResidualPredicate().
  if (join_key_attributes_.size() == 1u) {
    // Call the collector to set the bit to 0 for every key without a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchNotFound(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    // Call the collector to set the bit to 0 for every key without a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchNotFoundCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end(); ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(
        probe_accessor_with_filter.get(), &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashAntiJoinWorkOrder::executeWithoutResidualPredicate() {
  DCHECK(residual_predicate_ == nullptr);

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // Probe the LIPFilters to generate an existence bitmap for probe_accessor, if enabled.
  std::unique_ptr<TupleIdSequence> existence_map;
  std::unique_ptr<ValueAccessor> base_accessor;
  if (lip_filter_adaptive_prober_ != nullptr) {
    base_accessor.reset(probe_accessor.release());
    existence_map.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(base_accessor.get()));
    probe_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  }

  if (existence_map == nullptr) {
    existence_map.reset(probe_store.getExistenceMap());
  }

  SemiAntiJoinTupleCollector collector(existence_map.get());
  // We probe the hash table to find the keys which have an entry in the
  // hash table.
  if (join_key_attributes_.size() == 1) {
    // Call the collector to set the bit to 0 for every key with a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchFound(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    // Call the collector to set the bit to 0 for every key with a match.
    hash_table_.runOverKeysFromValueAccessorIfMatchFoundCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end(); ++selection_it) {
    temp_result.addColumn((*selection_it)->getAllValues(
        probe_accessor_with_filter.get(), &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashAntiJoinWorkOrder::executeWithResidualPredicate() {
  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                          probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // Probe the LIPFilters to generate an existence bitmap for probe_accessor, if enabled.
  std::unique_ptr<TupleIdSequence> existence_map;
  std::unique_ptr<ValueAccessor> base_accessor;
  if (lip_filter_adaptive_prober_ != nullptr) {
    base_accessor.reset(probe_accessor.release());
    existence_map.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(base_accessor.get()));
    probe_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  }

  VectorsOfPairsJoinedTuplesCollector collector;
  // We probe the hash table and get all the matches. Unlike
  // executeWithoutResidualPredicate(), we have to collect all the matching
  // tuples, because after this step we still have to evalute the residual
  // predicate.
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessor(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKey(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  // If the existence map has not been initialized by the pre-filtering LIPFilters.
  // Then create it for all the tuples from the given probe block.
  if (existence_map == nullptr) {
    existence_map.reset(probe_store.getExistenceMap());
  }

  for (const std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTuples()) {
    // First element of the pair build_block_entry is the build block ID
    // 2nd element of the pair is a vector of pairs, in each of which -
    // 1st element is a matching tuple ID from the inner (build) relation.
    // 2nd element is a matching tuple ID from the outer (probe) relation.
    BlockReference build_block = storage_manager_->getBlock(build_block_entry.first,
                                                            build_relation_);
    const TupleStorageSubBlock &build_store = build_block->getTupleStorageSubBlock();
    std::unique_ptr<ValueAccessor> build_accessor(build_store.createValueAccessor());
    for (const std::pair<tuple_id, tuple_id> &hash_match
         : build_block_entry.second) {
      if (!existence_map->get(hash_match.second)) {
        // We have already seen this tuple, skip it.
        continue;
      }
      if (residual_predicate_->matchesForJoinedTuples(*build_accessor,
                                                      build_relation_id,
                                                      hash_match.first,
                                                      *probe_accessor,
                                                      probe_relation_id,
                                                      hash_match.second)) {
        // Note that the existence map marks a match as false, as needed by the
        // anti join definition.
        existence_map->set(hash_match.second, false);
      }
    }
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
      probe_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  ColumnVectorsValueAccessor temp_result;
  for (vector<unique_ptr<const Scalar>>::const_iterator selection_it = selection_.begin();
       selection_it != selection_.end();
       ++selection_it) {
    temp_result.addColumn(
        (*selection_it)->getAllValues(probe_accessor_with_filter.get(),
                                      &sub_blocks_ref));
  }

  output_destination_->bulkInsertTuples(&temp_result);
}

void HashOuterJoinWorkOrder::execute() {
  const relation_id build_relation_id = build_relation_.getID();
  const relation_id probe_relation_id = probe_relation_.getID();

  const BlockReference probe_block = storage_manager_->getBlock(block_id_,
                                                                probe_relation_);
  const TupleStorageSubBlock &probe_store = probe_block->getTupleStorageSubBlock();
  std::unique_ptr<ValueAccessor> probe_accessor(probe_store.createValueAccessor());

  // Probe the LIPFilters to generate an existence bitmap for probe_accessor, if enabled.
  std::unique_ptr<TupleIdSequence> existence_map;
  std::unique_ptr<ValueAccessor> base_accessor;
  if (lip_filter_adaptive_prober_ != nullptr) {
    base_accessor.reset(probe_accessor.release());
    existence_map.reset(
        lip_filter_adaptive_prober_->filterValueAccessor(base_accessor.get()));
    probe_accessor.reset(
        base_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
  }

  if (existence_map == nullptr) {
    existence_map.reset(probe_store.getExistenceMap());
  }

  OuterJoinTupleCollector collector(existence_map.get());
  if (join_key_attributes_.size() == 1) {
    hash_table_.getAllFromValueAccessorWithExtraWorkForFirstMatch(
        probe_accessor.get(),
        join_key_attributes_.front(),
        any_join_key_attributes_nullable_,
        &collector);
  } else {
    hash_table_.getAllFromValueAccessorCompositeKeyWithExtraWorkForFirstMatch(
        probe_accessor.get(),
        join_key_attributes_,
        any_join_key_attributes_nullable_,
        &collector);
  }

  // Populate the output tuples for matches.
  for (const std::pair<const block_id, std::vector<std::pair<tuple_id, tuple_id>>>
           &build_block_entry : *collector.getJoinedTupleMap()) {
    const BlockReference build_block =
        storage_manager_->getBlock(build_block_entry.first, build_relation_);
    const TupleStorageSubBlock &build_store =
        build_block->getTupleStorageSubBlock();

    std::unique_ptr<ValueAccessor> build_accessor(
        build_store.createValueAccessor());
    ColumnVectorsValueAccessor temp_result;
    for (auto selection_it = selection_.begin();
         selection_it != selection_.end();
         ++selection_it) {
      temp_result.addColumn(
          (*selection_it)->getAllValuesForJoin(
              build_relation_id,
              build_accessor.get(),
              probe_relation_id,
              probe_accessor.get(),
              build_block_entry.second));
    }
    output_destination_->bulkInsertTuples(&temp_result);
  }

  SubBlocksReference sub_blocks_ref(probe_store,
                                    probe_block->getIndices(),
                                    probe_block->getIndicesConsistent());

  // Populate the output tuples for non-matches.
  const TupleIdSequence::size_type num_tuples_without_matches = existence_map->size();
  if (num_tuples_without_matches > 0) {
    std::unique_ptr<ValueAccessor> probe_accessor_with_filter(
        probe_accessor->createSharedTupleIdSequenceAdapterVirtual(*existence_map));
    ColumnVectorsValueAccessor temp_result;

    for (std::size_t i = 0; i < selection_.size(); ++i) {
      if (is_selection_on_build_[i]) {
        // NOTE(harshad, jianqiao): The assumption here is that any operation
        // involving NULL as operands will return NULL result. This assumption
        // will become invalid if later we add support for functions that can
        // produce non-NULL result with NULL operands, e.g.
        //   CASE WHEN x IS NOT NULL THEN x ELSE 0
        // or equivalently
        //   COALESCE(x, 0)
        // where x is an attribute of the build relation.
        // In that case, this HashOuterJoinWorkOrder needs to be updated to
        // correctly handle the selections.
        const Type &column_type = selection_[i]->getType().getNullableVersion();
        if (NativeColumnVector::UsableForType(column_type)) {
          NativeColumnVector *result = new NativeColumnVector(
              column_type, num_tuples_without_matches);
          result->fillWithNulls();
          temp_result.addColumn(result);
        } else {
          IndirectColumnVector *result = new IndirectColumnVector(
              column_type, num_tuples_without_matches);
          result->fillWithValue(TypedValue(column_type.getTypeID()));
          temp_result.addColumn(result);
        }
      } else {
        temp_result.addColumn(
            selection_[i]->getAllValues(probe_accessor_with_filter.get(),
                                        &sub_blocks_ref));
      }
    }
    output_destination_->bulkInsertTuples(&temp_result);
  }
}

}  // namespace quickstep
