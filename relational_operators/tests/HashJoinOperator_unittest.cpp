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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
#include "catalog/PartitionSchemeHeader.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/DestroyHashOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/HashTable.pb.h"
#include "storage/HashTableBase.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/CharType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypedValue.hpp"
#include "types/VarCharType.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"

// NetBSD's libc has snprintf, but it doesn't show up in the std namespace for
// C++.
#ifndef __NetBSD__
using std::snprintf;
#endif

using std::make_unique;
using std::size_t;
using std::unique_ptr;

namespace quickstep {

namespace {

constexpr std::size_t kCharLength = 16;
constexpr tuple_id kNumDimTuples = 200;
constexpr tuple_id kNumFactTuples = 300;
constexpr tuple_id kBlockSize = 10;

constexpr std::size_t kQueryId = 0;
constexpr int kOpIndex = 0;

constexpr std::size_t kSinglePartition = 1;
constexpr std::size_t kMultiplePartitions = 4;

const bool kHasRepartition = true;
const bool kNoRepartition = false;

}  // namespace

class HashJoinOperatorTest : public ::testing::TestWithParam<HashTableImplType> {
 protected:
  virtual void SetUp() {
    thread_id_map_ = ClientIDMap::Instance();

    bus_.Initialize();

    const tmb::client_id worker_thread_client_id = bus_.Connect();
    bus_.RegisterClientAsSender(worker_thread_client_id, kCatalogRelationNewBlockMessage);

    // Usually the worker thread makes the following call. In this test setup,
    // we don't have a worker thread hence we have to explicitly make the call.
    thread_id_map_->addValue(worker_thread_client_id);

    foreman_client_id_ = bus_.Connect();
    bus_.RegisterClientAsSender(foreman_client_id_, kCatalogRelationNewBlockMessage);
    bus_.RegisterClientAsReceiver(foreman_client_id_, kCatalogRelationNewBlockMessage);

    storage_manager_.reset(new StorageManager("./hash_join_operator_test_data/"));

    // Create a database.
    db_.reset(new CatalogDatabase(nullptr, "database"));

    // Create tables, owned by db_.
    dim_table_ = new CatalogRelation(NULL, "dim_table", 100);
    db_->addRelation(dim_table_);

    fact_table_ = new CatalogRelation(NULL, "fact_table", 101);
    db_->addRelation(fact_table_);

    // Add attributes.
    const Type &long_type = LongType::InstanceNonNullable();
    const Type &int_type = IntType::InstanceNonNullable();
    const Type &char_type = CharType::InstanceNonNullable(kCharLength);
    const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);
    dim_table_->addAttribute(new CatalogAttribute(dim_table_, "long", long_type));
    dim_table_->addAttribute(new CatalogAttribute(dim_table_, "int", int_type));
    dim_table_->addAttribute(new CatalogAttribute(dim_table_, "char", char_type));
    dim_table_->addAttribute(new CatalogAttribute(dim_table_, "varchar", varchar_type));
    fact_table_->addAttribute(new CatalogAttribute(fact_table_, "long", long_type));
    fact_table_->addAttribute(new CatalogAttribute(fact_table_, "int", int_type));
    fact_table_->addAttribute(new CatalogAttribute(fact_table_, "char", char_type));
    fact_table_->addAttribute(new CatalogAttribute(fact_table_, "varchar", varchar_type));
  }

  virtual void TearDown() {
    thread_id_map_->removeValue();

    // Drop blocks from relations.
    const std::vector<block_id> dim_blocks = dim_table_->getBlocksSnapshot();
    for (const block_id block : dim_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }

    const std::vector<block_id> fact_blocks = fact_table_->getBlocksSnapshot();
    for (const block_id block : fact_blocks) {
      storage_manager_->deleteBlockOrBlobFile(block);
    }
  }

  StorageBlockLayout* createStorageLayout(const CatalogRelation &relation) {
    StorageBlockLayout *layout = new StorageBlockLayout(relation);
    StorageBlockLayoutDescription *layout_desc = layout->getDescriptionMutable();

    layout_desc->set_num_slots(1);

    layout_desc->mutable_tuple_store_description()->set_sub_block_type(
        TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);

    // Attempt to compress variable-length columns.
    for (CatalogRelation::const_iterator attr_it = relation.begin();
         attr_it != relation.end();
         ++attr_it) {
      if (attr_it->getType().isVariableLength()) {
        layout_desc->mutable_tuple_store_description()->AddExtension(
            CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id,
            attr_it->getID());
      }
    }

    layout->finalize();
    return layout;
  }

  Tuple* createTuple(const CatalogRelation &relation,
                     const std::int64_t long_val,
                     const std::int64_t int_val,
                     const std::int64_t char_val,
                     const std::int64_t varchar_val) {
    static const Type &char_type = CharType::InstanceNonNullable(kCharLength);
    static const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);
    char buffer[kCharLength];

    std::vector<TypedValue> attr_values;
    attr_values.emplace_back(long_val);
    attr_values.emplace_back(static_cast<int>(int_val));

    snprintf(buffer, kCharLength, "%" PRId64, char_val);
    attr_values.emplace_back(char_type.getTypeID(), buffer, kCharLength);
    attr_values.back().ensureNotReference();

    snprintf(buffer, kCharLength, "%" PRId64, varchar_val);
    attr_values.emplace_back(varchar_type.getTypeID(), buffer, std::strlen(buffer) + 1);
    attr_values.back().ensureNotReference();

    return new Tuple(std::move(attr_values));
  }

  void insertTuplesWithoutPartitions() {
    // Create StorageLayout
    std::unique_ptr<StorageBlockLayout> dim_layout(createStorageLayout(*dim_table_));
    std::unique_ptr<StorageBlockLayout> fact_layout(createStorageLayout(*fact_table_));

    // Insert tuples to dim table.
    std::unique_ptr<Tuple> tuple;
    MutableBlockReference storage_block;
    for (tuple_id i = 0; i < kNumDimTuples; i += kBlockSize) {
      // Create block.
      block_id block_id = storage_manager_->createBlock(*dim_table_, *dim_layout);
      storage_block = storage_manager_->getBlockMutable(block_id, *dim_table_);
      dim_table_->addBlock(block_id);

      // Insert tuples.
      tuple_id block_bound = i + kBlockSize < kNumDimTuples ? i + kBlockSize : kNumDimTuples;
      for (tuple_id tid = i; tid < block_bound; ++tid) {
        // First attribute (long): a sequence id.
        // Second attribute (int): a looped value to test duplicate keys.
        // Third attribute (char): an identical value to test Cartesian product.
        // Forth attribute (varchar): a value to test duplicate variable-length keys.
        tuple.reset(createTuple(*dim_table_, tid, tid % kBlockSize, 100, tid / 2 * 2));
        EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
      }
      storage_block->rebuild();
    }

    // Insert tuples to fact table.
    for (tuple_id i = 0; i < kNumFactTuples; i += kBlockSize) {
      // Create block
      block_id block_id = storage_manager_->createBlock(*fact_table_, *fact_layout);
      storage_block = storage_manager_->getBlockMutable(block_id, *fact_table_);
      fact_table_->addBlock(block_id);

      // Insert tuples
      tuple_id block_bound = i + kBlockSize < kNumFactTuples ? i + kBlockSize : kNumFactTuples;
      for (tuple_id tid = i; tid < block_bound; ++tid) {
        // First attribute (long): a sequence id to join with dim_table.long. Each tuple has
        //                         exact one match.
        // Second attribute (int): a sequence id to join with dim_table.int. Each tuple in the
        //                         first kBlockSize tuples has mutiple matches. Other tuples
        //                         have no match.
        // Third attribute (char): an identical value to test Cartesian product.
        // Forth attribute (varchar): a sequence id to join with dim_table.var_char. Each tuple
        //                            has two matches.
        tuple.reset(createTuple(*fact_table_, tid, tid, 100, tid));
        EXPECT_TRUE(storage_block->insertTupleInBatch(*tuple));
      }
      storage_block->rebuild();
    }
  }

  void insertTuplesWithSingleAttributePartitions() {
    // Set PartitionScheme.
    dim_part_scheme_ = new PartitionScheme(
        new HashPartitionSchemeHeader(kMultiplePartitions, { dim_table_->getAttributeByName("long")->getID() },
                                      { kLong }, { 0 }));
    dim_table_->setPartitionScheme(dim_part_scheme_);

    fact_part_scheme_ = new PartitionScheme(
        new HashPartitionSchemeHeader(kMultiplePartitions, { fact_table_->getAttributeByName("long")->getID() },
                                      { kLong }, { 0 }));
    fact_table_->setPartitionScheme(fact_part_scheme_);

    // Create StorageLayout
    std::unique_ptr<StorageBlockLayout> dim_layout(createStorageLayout(*dim_table_));
    std::unique_ptr<StorageBlockLayout> fact_layout(createStorageLayout(*fact_table_));

    // Create blocks per partition. The index is the partition id.
    std::vector<MutableBlockReference> dim_partitioned_blocks;
    for (partition_id part_id = 0; part_id < kMultiplePartitions; ++part_id) {
      const block_id block = storage_manager_->createBlock(*dim_table_, *dim_layout);
      dim_part_scheme_->addBlockToPartition(block, part_id);
      // For a simpler teardown.
      dim_table_->addBlock(block);

      dim_partitioned_blocks.push_back(storage_manager_->getBlockMutable(block, *dim_table_));
    }

    // Insert tuples to dim table.
    for (tuple_id tid = 0; tid < kNumDimTuples; ++tid) {
      // First attribute (long): a sequence id.
      // Second attribute (int): a looped value to test duplicate keys.
      // Third attribute (char): an identical value to test Cartesian product.
      // Forth attribute (varchar): a value to test duplicate variable-length keys.
      unique_ptr<Tuple> tuple(createTuple(*dim_table_, tid, tid % kBlockSize, 100, tid / 2 * 2));
      EXPECT_TRUE(dim_partitioned_blocks[tid % kMultiplePartitions]->insertTupleInBatch(*tuple));
    }

    for (size_t i = 0; i < dim_partitioned_blocks.size(); ++i) {
      dim_partitioned_blocks[i]->rebuild();
    }

    // Create blocks per partition. The index is the partition id.
    std::vector<MutableBlockReference> fact_partitioned_blocks;
    for (partition_id part_id = 0; part_id < kMultiplePartitions; ++part_id) {
      const block_id block = storage_manager_->createBlock(*fact_table_, *fact_layout);
      fact_part_scheme_->addBlockToPartition(block, part_id);
      // For a simpler teardown.
      fact_table_->addBlock(block);

      fact_partitioned_blocks.push_back(storage_manager_->getBlockMutable(block, *fact_table_));
    }

    // Insert tuples to fact table.
    for (tuple_id tid = 0; tid < kNumFactTuples; ++tid) {
      // First attribute (long): a sequence id to join with dim_table.long. Each tuple has
      //                         exact one match.
      // Second attribute (int): a sequence id to join with dim_table.int. Each tuple in the
      //                         first kBlockSize tuples has mutiple matches. Other tuples
      //                         have no match.
      // Third attribute (char): an identical value to test Cartesian product.
      // Forth attribute (varchar): a sequence id to join with dim_table.var_char. Each tuple
      //                            has two matches.
      unique_ptr<Tuple> tuple(createTuple(*fact_table_, tid, tid, 100, tid));
      EXPECT_TRUE(fact_partitioned_blocks[tid % kMultiplePartitions]->insertTupleInBatch(*tuple));
    }

    for (size_t i = 0; i < fact_partitioned_blocks.size(); ++i) {
      fact_partitioned_blocks[i]->rebuild();
    }
  }

  void fetchAndExecuteWorkOrders(RelationalOperator *op) {
    // Note: We treat each operator as an individual query plan DAG. The
    // index for each operator should be set, so that the WorkOrdersContainer
    // class' checks about operator index are successful.
    op->setOperatorIndex(kOpIndex);
    WorkOrdersContainer container(1, 0);
    const std::size_t op_index = 0;
    op->getAllWorkOrders(&container,
                         query_context_.get(),
                         storage_manager_.get(),
                         foreman_client_id_,
                         &bus_);

    while (container.hasNormalWorkOrder(op_index)) {
      WorkOrder *work_order = container.getNormalWorkOrder(op_index);
      work_order->execute();
      delete work_order;
    }
  }

  // This map is needed for InsertDestination and some WorkOrders that send
  // messages to Foreman directly. To know the reason behind the design of this
  // map, see the note in InsertDestination.hpp.
  ClientIDMap *thread_id_map_;

  MessageBusImpl bus_;
  tmb::client_id foreman_client_id_;

  unique_ptr<QueryContext> query_context_;
  std::unique_ptr<StorageManager> storage_manager_;

  unique_ptr<CatalogDatabase> db_;
  // The following CatalogRelations are owned by db_.
  CatalogRelation *dim_table_, *fact_table_;
  // The following PartitionSchemes are owned by its own CatalogRelation, respectively.
  PartitionScheme *dim_part_scheme_ = nullptr, *fact_part_scheme_ = nullptr;
};

TEST_P(HashJoinOperatorTest, LongKeyHashJoinTest) {
  insertTuplesWithoutPartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::HashTable *hash_table_proto =
      query_context_proto.add_join_hash_tables()->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      if (TypedValue::HashIsReversible(kLong)) {
        hash_table_proto->set_hash_table_impl_type(
            serialization::HashTableImplType::SIMPLE_SCALAR_SEPARATE_CHAINING);
        break;
      } else {
        // Can't use SimpleScalarSeparateChainingHashTable for long keys on
        // this platform.
        return;
      }
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();

  hash_table_proto->add_key_types()->MergeFrom(long_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");

  // Create the builder operator.
  unique_ptr<BuildHashOperator> builder(
      new BuildHashOperator(kQueryId,
                            *dim_table_,
                            true /* is_stored */,
                            std::vector<attribute_id>(1, dim_col_long.getID()),
                            dim_col_long.getType().isNullable(),
                            kSinglePartition,
                            join_hash_table_index,
                            QueryContext::kInvalidPredicateId));

  // Create the prober operator with one selection attribute.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  ScalarAttribute scalar_attr(dim_col_long);
  query_context_proto.add_scalar_groups()->add_scalars()->MergeFrom(scalar_attr.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      std::vector<attribute_id>(1, fact_col_long.getID()),
      fact_col_long.getType().isNullable(),
      kSinglePartition,
      kNoRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));

  // Set up the QueryContext.
  query_context_.reset(new QueryContext(query_context_proto,
                                        *db_,
                                        storage_manager_.get(),
                                        foreman_client_id_,
                                        &bus_));

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;
  std::unique_ptr<std::size_t[]> counts(new std::size_t[kNumDimTuples]);
  std::memset(counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  DCHECK(query_context_);
  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples), num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    EXPECT_EQ(1u, counts[i]);
  }

  // Create cleaner operator.
  unique_ptr<DestroyHashOperator> cleaner(new DestroyHashOperator(kQueryId, kSinglePartition, join_hash_table_index));
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

TEST_P(HashJoinOperatorTest, IntDuplicateKeyHashJoinTest) {
  insertTuplesWithoutPartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::HashTable *hash_table_proto =
      query_context_proto.add_join_hash_tables()->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      if (TypedValue::HashIsReversible(kInt)) {
        hash_table_proto->set_hash_table_impl_type(
            serialization::HashTableImplType::SIMPLE_SCALAR_SEPARATE_CHAINING);
        break;
      } else {
        // Can't use SimpleScalarSeparateChainingHashTable for int keys on this
        // platform.
        return;
      }
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &int_type = IntType::InstanceNonNullable();

  hash_table_proto->add_key_types()->MergeFrom(int_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_int = *dim_table_->getAttributeByName("int");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_int = *fact_table_->getAttributeByName("int");

  // Create the builder operator.
  unique_ptr<BuildHashOperator> builder(
      new BuildHashOperator(kQueryId,
                            *dim_table_,
                            true /* is_stored */,
                            std::vector<attribute_id>(1, dim_col_int.getID()),
                            dim_col_int.getType().isNullable(),
                            kSinglePartition,
                            join_hash_table_index,
                            QueryContext::kInvalidPredicateId));

  // Create the prober operator with two selection attributes.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  serialization::QueryContext::ScalarGroup *scalar_group_proto = query_context_proto.add_scalar_groups();

  ScalarAttribute scalar_attr_dim(dim_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_dim.getProto());
  ScalarAttribute scalar_attr_fact(fact_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_fact.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "dim_long", long_type));
  result_table->addAttribute(new CatalogAttribute(result_table, "fact_long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      std::vector<attribute_id>(1, fact_col_int.getID()),
      fact_col_int.getType().isNullable(),
      kSinglePartition,
      kNoRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));


  // Set up the QueryContext.
  query_context_.reset(new QueryContext(query_context_proto,
                                        *db_,
                                        storage_manager_.get(),
                                        foreman_client_id_,
                                        &bus_));

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;

  std::unique_ptr<std::size_t[]> dim_counts(new std::size_t[kNumDimTuples]);
  std::memset(dim_counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  std::unique_ptr<std::size_t[]> fact_counts(new std::size_t[kNumFactTuples]);
  std::memset(fact_counts.get(), 0, sizeof(std::size_t) * kNumFactTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("dim_long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++dim_counts[value];

        typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("fact_long")->getID());
        value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumFactTuples));
        ++fact_counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples), num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    EXPECT_EQ(1u, dim_counts[i]);
  }
  for (tuple_id i = 0; i < kNumFactTuples; ++i) {
    if (i < kNumDimTuples % kBlockSize) {
      EXPECT_EQ(static_cast<std::size_t>((kNumDimTuples + kBlockSize - 1) / kBlockSize),
                fact_counts[i]);
    } else if (i < kBlockSize) {
      EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples / kBlockSize),
                fact_counts[i]);
    } else {
      EXPECT_EQ(0u, fact_counts[i]);
    }
  }

  // Create cleaner operator.
  unique_ptr<DestroyHashOperator> cleaner(new DestroyHashOperator(kQueryId, kSinglePartition, join_hash_table_index));
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

TEST_P(HashJoinOperatorTest, CharKeyCartesianProductHashJoinTest) {
  insertTuplesWithoutPartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::HashTable *hash_table_proto =
      query_context_proto.add_join_hash_tables()->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      // Can't use SimpleScalarSeparateChainingHashTable with CHAR(X) keys.
      return;
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &char_type = CharType::InstanceNonNullable(kCharLength);

  hash_table_proto->add_key_types()->MergeFrom(char_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_char = *dim_table_->getAttributeByName("char");
  const CatalogAttribute &fact_col_char = *fact_table_->getAttributeByName("char");

  // Create builder operator.
  unique_ptr<BuildHashOperator> builder(
      new BuildHashOperator(kQueryId,
                            *dim_table_,
                            true /* is_stored */,
                            std::vector<attribute_id>(1, dim_col_char.getID()),
                            dim_col_char.getType().isNullable(),
                            kSinglePartition,
                            join_hash_table_index,
                            QueryContext::kInvalidPredicateId));

  // Create prober operator with one selection attribute.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  ScalarAttribute scalar_attr(dim_col_long);
  query_context_proto.add_scalar_groups()->add_scalars()->MergeFrom(scalar_attr.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      std::vector<attribute_id>(1, fact_col_char.getID()),
      fact_col_char.getType().isNullable(),
      kSinglePartition,
      kNoRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));

  // Set up the QueryContext.
  query_context_.reset(new QueryContext(query_context_proto,
                                        *db_,
                                        storage_manager_.get(),
                                        foreman_client_id_,
                                        &bus_));

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;
  std::unique_ptr<std::size_t[]> counts(new std::size_t[kNumDimTuples]);
  std::memset(counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples * kNumFactTuples),
            num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    EXPECT_EQ(static_cast<std::size_t>(kNumFactTuples), counts[i]);
  }

  // Create cleaner operator.
  unique_ptr<DestroyHashOperator> cleaner(new DestroyHashOperator(kQueryId, kSinglePartition, join_hash_table_index));
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

TEST_P(HashJoinOperatorTest, VarCharDuplicateKeyHashJoinTest) {
  insertTuplesWithoutPartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::HashTable *hash_table_proto =
      query_context_proto.add_join_hash_tables()->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      // Can't use SimpleScalarSeparateChainingHashTable with VARCHAR(X) keys.
      return;
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);

  hash_table_proto->add_key_types()->MergeFrom(varchar_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_varchar = *dim_table_->getAttributeByName("varchar");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_varchar = *fact_table_->getAttributeByName("varchar");

  // Create builder operator.
  unique_ptr<BuildHashOperator> builder(
      new BuildHashOperator(kQueryId,
                            *dim_table_,
                            true /* is_stored */,
                            std::vector<attribute_id>(1, dim_col_varchar.getID()),
                            dim_col_varchar.getType().isNullable(),
                            kSinglePartition,
                            join_hash_table_index,
                            QueryContext::kInvalidPredicateId));

  // Create prober operator with two selection attributes.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  serialization::QueryContext::ScalarGroup *scalar_group_proto = query_context_proto.add_scalar_groups();

  ScalarAttribute scalar_attr_dim(dim_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_dim.getProto());
  ScalarAttribute scalar_attr_fact(fact_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_fact.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "dim_long", long_type));
  result_table->addAttribute(new CatalogAttribute(result_table, "fact_long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      std::vector<attribute_id>(1, fact_col_varchar.getID()),
      fact_col_varchar.getType().isNullable(),
      kSinglePartition,
      kNoRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));


  // Set up the QueryContext.
  query_context_.reset(new QueryContext(query_context_proto,
                                        *db_,
                                        storage_manager_.get(),
                                        foreman_client_id_,
                                        &bus_));

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;

  std::unique_ptr<std::size_t[]> dim_counts(new std::size_t[kNumDimTuples]);
  std::memset(dim_counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  std::unique_ptr<std::size_t[]> fact_counts(new std::size_t[kNumFactTuples]);
  std::memset(fact_counts.get(), 0, sizeof(std::size_t) * kNumFactTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("dim_long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++dim_counts[value];

        typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("fact_long")->getID());
        value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumFactTuples));
        ++fact_counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples), num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    EXPECT_EQ(1u, dim_counts[i]);
  }
  for (tuple_id i = 0; i < kNumFactTuples; ++i) {
    if (i >= kNumDimTuples) {
      EXPECT_EQ(0u, fact_counts[i]);
    } else {
      if (i % 2 == 0) {
        if (i == kNumDimTuples - 1) {
          EXPECT_EQ(1u, fact_counts[i]);
        } else {
          EXPECT_EQ(2u, fact_counts[i]);
        }
      } else if (i == kNumFactTuples) {
        EXPECT_EQ(0u, fact_counts[i]);
      }
    }
  }

  // Create the cleaner operator.
  unique_ptr<DestroyHashOperator> cleaner(new DestroyHashOperator(kQueryId, kSinglePartition, join_hash_table_index));
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

TEST_P(HashJoinOperatorTest, CompositeKeyHashJoinTest) {
  insertTuplesWithoutPartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::HashTable *hash_table_proto =
      query_context_proto.add_join_hash_tables()->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      // Can't use SimpleScalarSeparateChainingHashTable with composite keys.
      return;
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);

  hash_table_proto->add_key_types()->MergeFrom(long_type.getProto());
  hash_table_proto->add_key_types()->MergeFrom(varchar_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_varchar = *dim_table_->getAttributeByName("varchar");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_varchar = *fact_table_->getAttributeByName("varchar");

  // Create the builder operator.
  std::vector<attribute_id> dim_key_attrs;
  dim_key_attrs.push_back(dim_col_long.getID());
  dim_key_attrs.push_back(dim_col_varchar.getID());

  unique_ptr<BuildHashOperator> builder(
      new BuildHashOperator(kQueryId,
                            *dim_table_,
                            true /* is_stored */,
                            dim_key_attrs,
                            dim_col_long.getType().isNullable() || dim_col_varchar.getType().isNullable(),
                            kSinglePartition,
                            join_hash_table_index,
                            QueryContext::kInvalidPredicateId));

  // Create the prober operator with two selection attributes.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  serialization::QueryContext::ScalarGroup *scalar_group_proto = query_context_proto.add_scalar_groups();

  ScalarAttribute scalar_attr_dim(dim_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_dim.getProto());
  ScalarAttribute scalar_attr_fact(fact_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_fact.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "dim_long", long_type));
  result_table->addAttribute(new CatalogAttribute(result_table, "fact_long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  std::vector<attribute_id> fact_key_attrs;
  fact_key_attrs.push_back(fact_col_long.getID());
  fact_key_attrs.push_back(fact_col_varchar.getID());

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      fact_key_attrs,
      fact_col_long.getType().isNullable() ||
          fact_col_varchar.getType().isNullable(),
      kSinglePartition,
      kNoRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));

  // Set up the QueryContext.
  query_context_.reset(new QueryContext(query_context_proto,
                                        *db_,
                                        storage_manager_.get(),
                                        foreman_client_id_,
                                        &bus_));

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;

  std::unique_ptr<std::size_t[]> dim_counts(new std::size_t[kNumDimTuples]);
  std::memset(dim_counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  std::unique_ptr<std::size_t[]> fact_counts(new std::size_t[kNumFactTuples]);
  std::memset(fact_counts.get(), 0, sizeof(std::size_t) * kNumFactTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("dim_long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++dim_counts[value];

        typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("fact_long")->getID());
        value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumFactTuples));
        ++fact_counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples) / 2, num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    if (i & 0x1) {
      EXPECT_EQ(0u, dim_counts[i]);
    } else {
      EXPECT_EQ(1u, dim_counts[i]);
    }
  }
  for (tuple_id i = 0; i < kNumFactTuples; ++i) {
    if (i >= kNumDimTuples) {
      EXPECT_EQ(0u, fact_counts[i]);
    } else {
      if (i & 0x1) {
        EXPECT_EQ(0u, fact_counts[i]);
      } else {
        EXPECT_EQ(1u, fact_counts[i]);
      }
    }
  }

  // Create cleaner operator.
  unique_ptr<DestroyHashOperator> cleaner(new DestroyHashOperator(kQueryId, kSinglePartition, join_hash_table_index));
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

// Same as above test, but add an additional residual filter predicate.
TEST_P(HashJoinOperatorTest, CompositeKeyHashJoinWithResidualPredicateTest) {
  insertTuplesWithoutPartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::HashTable *hash_table_proto =
      query_context_proto.add_join_hash_tables()->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      // Can't use SimpleScalarSeparateChainingHashTable with composite keys.
      return;
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);

  hash_table_proto->add_key_types()->MergeFrom(long_type.getProto());
  hash_table_proto->add_key_types()->MergeFrom(varchar_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_varchar = *dim_table_->getAttributeByName("varchar");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_varchar = *fact_table_->getAttributeByName("varchar");

  // Create the builder operator.
  std::vector<attribute_id> dim_key_attrs;
  dim_key_attrs.push_back(dim_col_long.getID());
  dim_key_attrs.push_back(dim_col_varchar.getID());

  unique_ptr<BuildHashOperator> builder(
      new BuildHashOperator(kQueryId,
                            *dim_table_,
                            true /* is_stored */,
                            dim_key_attrs,
                            dim_col_long.getType().isNullable() || dim_col_varchar.getType().isNullable(),
                            kSinglePartition,
                            join_hash_table_index,
                            QueryContext::kInvalidPredicateId));

  // Create prober operator with two selection attributes.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  serialization::QueryContext::ScalarGroup *scalar_group_proto = query_context_proto.add_scalar_groups();

  ScalarAttribute scalar_attr_dim(dim_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_dim.getProto());
  ScalarAttribute scalar_attr_fact(fact_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_fact.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "dim_long", long_type));
  result_table->addAttribute(new CatalogAttribute(result_table, "fact_long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  // Include a residual predicate that selects a subset of the joined tuples.
  unique_ptr<Predicate> residual_pred(new ComparisonPredicate(
      ComparisonFactory::GetComparison(
          ComparisonID::kLess),
          new ScalarAttribute(dim_col_long),
          new ScalarLiteral(TypedValue(static_cast<std::int64_t>(15)), LongType::InstanceNonNullable())));

  std::vector<attribute_id> fact_key_attrs;
  fact_key_attrs.push_back(fact_col_long.getID());
  fact_key_attrs.push_back(fact_col_varchar.getID());

  const QueryContext::predicate_id residual_pred_index = query_context_proto.predicates_size();
  query_context_proto.add_predicates()->MergeFrom(residual_pred->getProto());

  unique_ptr<HashJoinOperator> prober(
      new HashJoinOperator(kQueryId,
                           *dim_table_,
                           *fact_table_,
                           true /* is_stored */,
                           fact_key_attrs,
                           fact_col_long.getType().isNullable() ||
                               fact_col_varchar.getType().isNullable(),
                           kSinglePartition,
                           kNoRepartition,
                           *result_table,
                           output_destination_index,
                           join_hash_table_index,
                           residual_pred_index,
                           selection_index));

  // Set up the QueryContext.
  query_context_.reset(new QueryContext(query_context_proto,
                                        *db_,
                                        storage_manager_.get(),
                                        foreman_client_id_,
                                        &bus_));

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;

  std::unique_ptr<std::size_t[]> dim_counts(new std::size_t[kNumDimTuples]);
  std::memset(dim_counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  std::unique_ptr<std::size_t[]> fact_counts(new std::size_t[kNumFactTuples]);
  std::memset(fact_counts.get(), 0, sizeof(std::size_t) * kNumFactTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("dim_long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++dim_counts[value];

        typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("fact_long")->getID());
        value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumFactTuples));
        ++fact_counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(8u, num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    if ((i & 0x1) || (i >= 15)) {
      EXPECT_EQ(0u, dim_counts[i]);
    } else {
      EXPECT_EQ(1u, dim_counts[i]);
    }
  }
  for (tuple_id i = 0; i < kNumFactTuples; ++i) {
    if (i >= 15) {
      EXPECT_EQ(0u, fact_counts[i]);
    } else {
      if (i & 0x1) {
        EXPECT_EQ(0u, fact_counts[i]);
      } else {
        EXPECT_EQ(1u, fact_counts[i]);
      }
    }
  }

  // Create cleaner operator.
  unique_ptr<DestroyHashOperator> cleaner(new DestroyHashOperator(kQueryId, kSinglePartition, join_hash_table_index));
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

// Hash join tests with single attribute partitions.
TEST_P(HashJoinOperatorTest, SingleAttributePartitionedLongKeyHashJoinTest) {
  insertTuplesWithSingleAttributePartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::QueryContext::HashTableContext *hash_table_context_proto =
      query_context_proto.add_join_hash_tables();
  hash_table_context_proto->set_num_partitions(kMultiplePartitions);

  serialization::HashTable *hash_table_proto =
      hash_table_context_proto->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      if (TypedValue::HashIsReversible(kLong)) {
        hash_table_proto->set_hash_table_impl_type(
            serialization::HashTableImplType::SIMPLE_SCALAR_SEPARATE_CHAINING);
        break;
      } else {
        // Can't use SimpleScalarSeparateChainingHashTable for long keys on
        // this platform.
        return;
      }
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();

  hash_table_proto->add_key_types()->MergeFrom(long_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");

  // Create the builder operator.
  unique_ptr<BuildHashOperator> builder(new BuildHashOperator(
      kQueryId,
      *dim_table_,
      true /* is_stored */,
      { dim_col_long.getID() },
      dim_col_long.getType().isNullable(),
      kMultiplePartitions,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId));

  // Create the prober operator with one selection attribute.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  ScalarAttribute scalar_attr(dim_col_long);
  query_context_proto.add_scalar_groups()->add_scalars()->MergeFrom(scalar_attr.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      { fact_col_long.getID() },
      fact_col_long.getType().isNullable(),
      kMultiplePartitions,
      kHasRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));

  // Set up the QueryContext.
  query_context_ =
      make_unique<QueryContext>(query_context_proto, *db_, storage_manager_.get(), foreman_client_id_, &bus_);

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;
  std::unique_ptr<std::size_t[]> counts(new std::size_t[kNumDimTuples]);
  std::memset(counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  DCHECK(query_context_);
  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples), num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    EXPECT_EQ(1u, counts[i]);
  }

  // Create cleaner operator.
  auto cleaner = make_unique<DestroyHashOperator>(kQueryId, kMultiplePartitions, join_hash_table_index);
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

TEST_P(HashJoinOperatorTest, SingleAttributePartitionedCompositeKeyHashJoinTest) {
  insertTuplesWithSingleAttributePartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::QueryContext::HashTableContext *hash_table_context_proto =
      query_context_proto.add_join_hash_tables();
  hash_table_context_proto->set_num_partitions(kMultiplePartitions);

  serialization::HashTable *hash_table_proto =
      hash_table_context_proto->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      // Can't use SimpleScalarSeparateChainingHashTable with composite keys.
      return;
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);

  hash_table_proto->add_key_types()->MergeFrom(long_type.getProto());
  hash_table_proto->add_key_types()->MergeFrom(varchar_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_varchar = *dim_table_->getAttributeByName("varchar");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_varchar = *fact_table_->getAttributeByName("varchar");

  // Create the builder operator.
  unique_ptr<BuildHashOperator> builder(new BuildHashOperator(
      kQueryId,
      *dim_table_,
      true /* is_stored */,
      { dim_col_long.getID(), dim_col_varchar.getID() },
      dim_col_long.getType().isNullable() || dim_col_varchar.getType().isNullable(),
      kMultiplePartitions,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId));

  // Create the prober operator with two selection attributes.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  serialization::QueryContext::ScalarGroup *scalar_group_proto = query_context_proto.add_scalar_groups();

  ScalarAttribute scalar_attr_dim(dim_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_dim.getProto());
  ScalarAttribute scalar_attr_fact(fact_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_fact.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "dim_long", long_type));
  result_table->addAttribute(new CatalogAttribute(result_table, "fact_long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      { fact_col_long.getID(), fact_col_varchar.getID() },
      fact_col_long.getType().isNullable() || fact_col_varchar.getType().isNullable(),
      kMultiplePartitions,
      kHasRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId /* residual_predicate_index */,
      selection_index));

  // Set up the QueryContext.
  query_context_ =
     make_unique<QueryContext>(query_context_proto, *db_, storage_manager_.get(), foreman_client_id_, &bus_);

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;

  std::unique_ptr<std::size_t[]> dim_counts(new std::size_t[kNumDimTuples]);
  std::memset(dim_counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  std::unique_ptr<std::size_t[]> fact_counts(new std::size_t[kNumFactTuples]);
  std::memset(fact_counts.get(), 0, sizeof(std::size_t) * kNumFactTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("dim_long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++dim_counts[value];

        typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("fact_long")->getID());
        value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumFactTuples));
        ++fact_counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(static_cast<std::size_t>(kNumDimTuples) / 2, num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    if (i & 0x1) {
      EXPECT_EQ(0u, dim_counts[i]);
    } else {
      EXPECT_EQ(1u, dim_counts[i]);
    }
  }
  for (tuple_id i = 0; i < kNumFactTuples; ++i) {
    if (i >= kNumDimTuples) {
      EXPECT_EQ(0u, fact_counts[i]);
    } else {
      if (i & 0x1) {
        EXPECT_EQ(0u, fact_counts[i]);
      } else {
        EXPECT_EQ(1u, fact_counts[i]);
      }
    }
  }

  // Create cleaner operator.
  auto cleaner = make_unique<DestroyHashOperator>(kQueryId, kMultiplePartitions, join_hash_table_index);
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

TEST_P(HashJoinOperatorTest, SingleAttributePartitionedCompositeKeyHashJoinWithResidualPredicateTest) {
  insertTuplesWithSingleAttributePartitions();

  // Setup the hash table proto in the query context proto.
  serialization::QueryContext query_context_proto;
  query_context_proto.set_query_id(kQueryId);

  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto.join_hash_tables_size();

  serialization::QueryContext::HashTableContext *hash_table_context_proto =
      query_context_proto.add_join_hash_tables();
  hash_table_context_proto->set_num_partitions(kMultiplePartitions);

  serialization::HashTable *hash_table_proto =
      hash_table_context_proto->mutable_join_hash_table();
  switch (GetParam()) {
    case HashTableImplType::kLinearOpenAddressing:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING);
      break;
    case HashTableImplType::kSeparateChaining:
      hash_table_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::SEPARATE_CHAINING);
      break;
    case HashTableImplType::kSimpleScalarSeparateChaining:
      // Can't use SimpleScalarSeparateChainingHashTable with composite keys.
      return;
    default:
      FATAL_ERROR("Unknown HashTable type requested for join.");
  }

  const Type &long_type = LongType::InstanceNonNullable();
  const Type &varchar_type = VarCharType::InstanceNonNullable(kCharLength);

  hash_table_proto->add_key_types()->MergeFrom(long_type.getProto());
  hash_table_proto->add_key_types()->MergeFrom(varchar_type.getProto());
  hash_table_proto->set_estimated_num_entries(kNumDimTuples);

  const CatalogAttribute &dim_col_long = *dim_table_->getAttributeByName("long");
  const CatalogAttribute &dim_col_varchar = *dim_table_->getAttributeByName("varchar");
  const CatalogAttribute &fact_col_long = *fact_table_->getAttributeByName("long");
  const CatalogAttribute &fact_col_varchar = *fact_table_->getAttributeByName("varchar");

  // Create the builder operator.
  unique_ptr<BuildHashOperator> builder(new BuildHashOperator(
      kQueryId,
      *dim_table_,
      true /* is_stored */,
      { dim_col_long.getID(), dim_col_varchar.getID() },
      dim_col_long.getType().isNullable() || dim_col_varchar.getType().isNullable(),
      kMultiplePartitions,
      join_hash_table_index,
      QueryContext::kInvalidPredicateId));

  // Create prober operator with two selection attributes.
  const QueryContext::scalar_group_id selection_index = query_context_proto.scalar_groups_size();
  serialization::QueryContext::ScalarGroup *scalar_group_proto = query_context_proto.add_scalar_groups();

  ScalarAttribute scalar_attr_dim(dim_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_dim.getProto());
  ScalarAttribute scalar_attr_fact(fact_col_long);
  scalar_group_proto->add_scalars()->MergeFrom(scalar_attr_fact.getProto());

  // Create result_table, owned by db_.
  CatalogRelation *result_table = new CatalogRelation(NULL, "result_table", 102);
  result_table->addAttribute(new CatalogAttribute(result_table, "dim_long", long_type));
  result_table->addAttribute(new CatalogAttribute(result_table, "fact_long", long_type));

  const relation_id output_relation_id = db_->addRelation(result_table);

  // Setup the InsertDestination proto in the query context proto.
  const QueryContext::insert_destination_id output_destination_index =
      query_context_proto.insert_destinations_size();
  serialization::InsertDestination *insert_destination_proto = query_context_proto.add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(serialization::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation_id);
  insert_destination_proto->set_relational_op_index(kOpIndex);

  // Include a residual predicate that selects a subset of the joined tuples.
  unique_ptr<Predicate> residual_pred(new ComparisonPredicate(
      ComparisonFactory::GetComparison(
          ComparisonID::kLess),
          new ScalarAttribute(dim_col_long),
          new ScalarLiteral(TypedValue(static_cast<std::int64_t>(15)), LongType::InstanceNonNullable())));

  const QueryContext::predicate_id residual_pred_index = query_context_proto.predicates_size();
  query_context_proto.add_predicates()->MergeFrom(residual_pred->getProto());

  unique_ptr<HashJoinOperator> prober(new HashJoinOperator(
      kQueryId,
      *dim_table_,
      *fact_table_,
      true /* is_stored */,
      { fact_col_long.getID(), fact_col_varchar.getID() },
      fact_col_long.getType().isNullable() || fact_col_varchar.getType().isNullable(),
      kMultiplePartitions,
      kHasRepartition,
      *result_table,
      output_destination_index,
      join_hash_table_index,
      residual_pred_index,
      selection_index));

  // Set up the QueryContext.
  query_context_ =
      make_unique<QueryContext>(query_context_proto, *db_, storage_manager_.get(), foreman_client_id_, &bus_);

  // Execute the operators.
  fetchAndExecuteWorkOrders(builder.get());
  fetchAndExecuteWorkOrders(prober.get());

  // Check result values
  // Note that the results might be in a different order.
  std::size_t num_result_tuples = 0;

  std::unique_ptr<std::size_t[]> dim_counts(new std::size_t[kNumDimTuples]);
  std::memset(dim_counts.get(), 0, sizeof(std::size_t) * kNumDimTuples);

  std::unique_ptr<std::size_t[]> fact_counts(new std::size_t[kNumFactTuples]);
  std::memset(fact_counts.get(), 0, sizeof(std::size_t) * kNumFactTuples);

  InsertDestination *insert_destination = query_context_->getInsertDestination(prober->getInsertDestinationID());
  DCHECK(insert_destination);

  const std::vector<block_id> result_blocks = insert_destination->getTouchedBlocks();
  for (std::size_t bid = 0; bid < result_blocks.size(); ++bid) {
    BlockReference result_block = storage_manager_->getBlock(result_blocks[bid],
                                                             insert_destination->getRelation());
    const TupleStorageSubBlock &result_tuple_sub_block = result_block->getTupleStorageSubBlock();
    num_result_tuples += result_tuple_sub_block.numTuples();
    for (tuple_id i = 0; i <= result_tuple_sub_block.getMaxTupleID(); ++i) {
      if (result_tuple_sub_block.hasTupleWithID(i)) {
        TypedValue typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("dim_long")->getID());
        std::int64_t value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumDimTuples));
        ++dim_counts[value];

        typed_value = result_tuple_sub_block.getAttributeValueTyped(
            i, result_table->getAttributeByName("fact_long")->getID());
        value = typed_value.getLiteral<std::int64_t>();
        ASSERT_GE(value, 0);
        ASSERT_LT(value, static_cast<std::int64_t>(kNumFactTuples));
        ++fact_counts[value];
      }
    }

    // Drop the block.
    result_block.release();
    storage_manager_->deleteBlockOrBlobFile(result_blocks[bid]);
  }
  EXPECT_EQ(8u, num_result_tuples);

  for (tuple_id i = 0; i < kNumDimTuples; ++i) {
    if ((i & 0x1) || (i >= 15)) {
      EXPECT_EQ(0u, dim_counts[i]);
    } else {
      EXPECT_EQ(1u, dim_counts[i]);
    }
  }
  for (tuple_id i = 0; i < kNumFactTuples; ++i) {
    if (i >= 15) {
      EXPECT_EQ(0u, fact_counts[i]);
    } else {
      if (i & 0x1) {
        EXPECT_EQ(0u, fact_counts[i]);
      } else {
        EXPECT_EQ(1u, fact_counts[i]);
      }
    }
  }

  // Create cleaner operator.
  auto cleaner = make_unique<DestroyHashOperator>(kQueryId, kMultiplePartitions, join_hash_table_index);
  fetchAndExecuteWorkOrders(cleaner.get());

  db_->dropRelationById(output_relation_id);
}

// Note: INSTANTIATE_TEST_CASE_P has variadic arguments part. If the variable argument part
//       is empty, C++11 standard says it should produce a warning. A warning is converted
//       to an error since we use -Werror as a compiler parameter. It causes Travis to build.
//       This is the reason that we must give an empty string argument as a last parameter
//       to supress warning that clang gives.
INSTANTIATE_TEST_CASE_P(
    HashTableImplType, HashJoinOperatorTest,
    ::testing::Values(
        HashTableImplType::kLinearOpenAddressing,
        HashTableImplType::kSeparateChaining,
        HashTableImplType::kSimpleScalarSeparateChaining),);  // NOLINT(whitespace/comma)

}  // namespace quickstep

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  // Honor FLAGS_buffer_pool_slots in StorageManager.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
