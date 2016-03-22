/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/WindowAggregationOperator.hpp"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregationHandle.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "storage/HashTableBase.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "types/TypeFactory.hpp"
#include "types/IntType.hpp"
#include "types/NumericSuperType.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/HashPair.hpp"

#include "glog/logging.h"

namespace quickstep {

WindowAggregationOperator::WindowAggregationOperator(
    const CatalogRelation &input_relation,
    bool input_relation_is_stored,
    const std::vector<const AggregateFunction *> &aggregate_functions,
    std::vector<std::vector<std::unique_ptr<const Scalar>>>
        &&aggregate_arguments,
    std::vector<std::unique_ptr<const Scalar>> &&group_by,
    const CatalogAttribute &window_attribute,
    const TypedValue &window_duration,
    std::int32_t age_duration,
    const QueryContext::insert_destination_id output_dest_id,
    const relation_id output_rel_id,
    const serialization::HashTableImplType hash_table_impl_type,
    StorageManager *storage_manager)
    : input_relation_(input_relation),
      input_relation_is_stored_(input_relation_is_stored),
      input_relation_block_ids_(input_relation.getBlocksSnapshot()),
      input_write_threshold_(input_relation_block_ids_.size() > 0
                                 ? input_relation_block_ids_.size() - 1
                                 : 0),
      output_dest_id_(output_dest_id),
      output_rel_id_(output_rel_id),
      state_(window_attribute, std::move(group_by), window_duration, age_duration) {
  DCHECK(!aggregate_functions.empty());
  DCHECK_EQ(aggregate_functions.size(), aggregate_arguments.size());

  state_.arguments = std::move(aggregate_arguments);

  // Window attribute scalar expression to find the tumbling point.
  state_.bucket_key.emplace_back(new ScalarBinaryExpression(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kDivide),
      new ScalarAttribute(state_.window_attribute),
      new ScalarLiteral(
          state_.window_duration,
          TypeFactory::GetType(state_.window_duration.getTypeID()))));

  // Hashtable key types.
  std::vector<const Type *> key_types;
  for (const auto &key : state_.bucket_key) {
    key_types.push_back(&key->getType());
  }

  // Operation to recreate the window attribute from the value in the hash
  // table.
  state_.rev_bucket_key.reset(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kMultiply)
          .makeUncheckedBinaryOperatorForTypes(
              *key_types.front(), IntType::InstanceNonNullable()));

  // Initialize aggregation handles and hash tables.
  for (std::size_t i = 0; i < aggregate_functions.size(); ++i) {
    std::vector<const Type *> agg_args;
    for (const auto &arg : state_.arguments[i]) {
      agg_args.push_back(&arg->getType());
    }
    HashTableImplType hash_table_type;
    if(hash_table_impl_type==serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING)
    {
    	hash_table_type=HashTableImplType::kLinearOpenAddressing;
    }
    else if(hash_table_impl_type==serialization::HashTableImplType::SEPARATE_CHAINING)
    {
    	hash_table_type=HashTableImplType::kSeparateChaining;
    }
    else if(hash_table_impl_type==serialization::HashTableImplType::SIMPLE_SCALAR_SEPARATE_CHAINING )
    {
    	hash_table_type=HashTableImplType::kSimpleScalarSeparateChaining;
    }
    DCHECK(aggregate_functions[i]->canApplyToTypes(agg_args));
    state_.handles.emplace_back(aggregate_functions[i]->createHandle(agg_args));
    state_.hashtables.emplace_back(state_.handles.back()->createGroupByHashTable(
    		hash_table_type, key_types, 100, storage_manager));
  }
}

bool WindowAggregationOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  if (input_relation_is_stored_) {
    if (!started_) {
      for (std::size_t i = 0; i < input_relation_block_ids_.size(); ++i) {
        container->addNormalWorkOrder(
            new WindowAggregationWorkOrder(input_relation_block_ids_[i],
                                           input_relation_.getID(),
                                           output_dest_id_,
					    &state_,	
					   storage_manager,
					   query_context
					   ),
            op_index_);
      }
      started_ = true;
    }
    return started_;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      container->addNormalWorkOrder(
          new WindowAggregationWorkOrder(
              input_relation_block_ids_[num_workorders_generated_],
              input_relation_.getID(),
              output_dest_id_,
	      &state_,
              storage_manager,
              query_context
              ),
          op_index_);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

namespace window_vector{

struct VecTypedValueHash {
  inline std::size_t operator() (const std::vector<TypedValue> &value) const {
    std::size_t hash = 0;
    for (const auto &v : value) {
      hash = CombineHashes(hash, v.getHash());
    }
    return hash;
  }
};

struct VecTypedValueEqualTo {
  // NOTE: TypedValues here are always the same type and never null.

  inline bool operator() (const std::vector<TypedValue> &left, const std::vector<TypedValue> &right) const{
    bool equal = true;
    for (std::size_t i = 0; i < left.size(); ++i) {
      equal = equal && left[i].fastEqualCheck(right[i]);
    }
    return equal;
  }
};

}  // namespace

void WindowAggregationWorkOrder::execute() {
  VLOG(3) << "WindowAggregationWorkOrder::execute() called.";
  DCHECK(query_context != nullptr);
  //DCHECK(database != nullptr);
  DCHECK(storage_manager != nullptr);

  InsertDestination *output_dest = query_context->getInsertDestination(output_dest_id_);
  DCHECK(output_dest != nullptr);
//TODO	
  BlockReference block(storage_manager->getBlock(
      input_block_id_, output_dest->getRelation()));

  std::unique_ptr<TupleIdSequence> reuse_matches;
  std::vector<std::unique_ptr<ColumnVector>> reuse_group_by_vectors;

  // Aggregate the block.
  for (std::size_t i = 0; i < state_->handles.size(); i++) {
    block->aggregateGroupBy(*state_->handles[i],
                            state_->arguments[i],
                            state_->bucket_key,
                            nullptr,
                            state_->hashtables[i].get(),
                            &reuse_matches,
                            &reuse_group_by_vectors);
  }

  // Bye-pass writing changes, if not required.
  if (write_mode_ == kNoBuckets) {
    return;
  }

  // Find changed buckets, and create a set of buckets.
  DCHECK(reuse_group_by_vectors.back()->isNative());
  std::unordered_set<std::vector<TypedValue>,
  window_vector::VecTypedValueHash,
  window_vector::VecTypedValueEqualTo> changed_buckets;
  std::size_t num_tuples = static_cast<const NativeColumnVector *>(
                               reuse_group_by_vectors.back().get())
                               ->size();
  std::size_t num_cols = reuse_group_by_vectors.size();
  for (std::size_t row = 0; row < num_tuples; ++row) {
    std::vector<TypedValue> tuple_key;
    tuple_key.reserve(num_cols);
    for (std::size_t col = 0; col < num_cols; ++col) {
      if (reuse_group_by_vectors[col]->isNative()) {
        const auto &cv = static_cast<const NativeColumnVector &>(
            *reuse_group_by_vectors[col]);
        tuple_key.emplace_back(cv.getTypedValue(row));
      } else {
        const auto &cv = static_cast<const IndirectColumnVector &>(
            *reuse_group_by_vectors[col]);
        tuple_key.emplace_back(cv.getTypedValue(row));
      }
    }
    changed_buckets.emplace(std::move(tuple_key));
  }

  // Construct vector of changed buckets for aggregate values.
  std::vector<std::vector<TypedValue>> changed_buckets_vec;
  changed_buckets_vec.reserve(changed_buckets.size());
  for (const auto &v : changed_buckets) {
    changed_buckets_vec.emplace_back(v);
  }

  // Construct the aggregate values.
  std::vector<std::unique_ptr<ColumnVector>> agg_columns;
  for (std::size_t agg = 0; agg < state_->handles.size(); ++agg) {
    agg_columns.emplace_back(state_->handles[agg]->finalizeHashTable(
        *state_->hashtables[agg], &changed_buckets_vec));
  }

  // Construct the window values.
  std::unique_ptr<ColumnVector> window_column;
  std::unique_ptr<NativeColumnVector> pre_window_column(new NativeColumnVector(
      state_->bucket_key.back()->getType(), changed_buckets_vec.size()));
  for (auto &key : changed_buckets_vec) {
    pre_window_column->appendTypedValue(std::move(key.back()));
  }
  window_column.reset(
      state_->rev_bucket_key->applyToColumnVectorAndStaticValue(
          *pre_window_column, state_->window_duration));

  // Construct the partition by values.
  std::vector<std::unique_ptr<ColumnVector>> grp_columns;
  for (std::size_t col = 0; col < state_->bucket_key.size() - 1; ++col) {
    if (state_->bucket_key[col]->getType().getTypeID() == TypeID::kVarChar) {
      std::unique_ptr<IndirectColumnVector> group_by_col(new IndirectColumnVector(
            state_->bucket_key[col]->getType(), changed_buckets_vec.size()));
      for (auto &key : changed_buckets_vec) {
        group_by_col->appendTypedValue(std::move(key[col]));
      }
      grp_columns.emplace_back(group_by_col.release());
    } else {
      std::unique_ptr<NativeColumnVector> group_by_col(new NativeColumnVector(
            state_->bucket_key[col]->getType(), changed_buckets_vec.size()));
      for (auto &key : changed_buckets_vec) {
        group_by_col->appendTypedValue(std::move(key[col]));
      }
      grp_columns.emplace_back(group_by_col.release());
    }
  }

  // Write output.
  ColumnVectorsValueAccessor result;
  result.addColumn(window_column.release());
  for (auto &col : grp_columns) {
    result.addColumn(col.release());
  }
  for (auto &col : agg_columns) {
    result.addColumn(col.release());
  }
  output_dest->bulkInsertTuples(&result, true);
}

}  // namespace quickstep
