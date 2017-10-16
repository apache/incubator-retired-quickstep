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

#include "query_optimizer/ExecutionGenerator.hpp"

#include <algorithm>
#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <numeric>
#include <string>
#include <type_traits>
#include <unordered_map>

#include "query_optimizer/QueryOptimizerConfig.h"  // For QUICKSTEP_DISTRIBUTED.

#ifdef QUICKSTEP_DISTRIBUTED
#include <unordered_set>
#endif

#include <utility>
#include <vector>

#ifdef QUICKSTEP_DISTRIBUTED
#include "catalog/Catalog.pb.h"
#endif

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "catalog/PartitionScheme.hpp"
#include "catalog/PartitionSchemeHeader.hpp"
#include "cli/Flags.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunction.pb.h"
#include "expressions/aggregation/AggregationID.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/window_aggregation/WindowAggregateFunction.hpp"
#include "expressions/window_aggregation/WindowAggregateFunction.pb.h"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/LIPFilterGenerator.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/cost_model/SimpleCostModel.hpp"
#include "query_optimizer/cost_model/StarSchemaSimpleCostModel.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/expressions/WindowAggregateFunction.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/CopyFrom.hpp"
#include "query_optimizer/physical/CopyTo.hpp"
#include "query_optimizer/physical/CreateIndex.hpp"
#include "query_optimizer/physical/CreateTable.hpp"
#include "query_optimizer/physical/CrossReferenceCoalesceAggregate.hpp"
#include "query_optimizer/physical/DeleteTuples.hpp"
#include "query_optimizer/physical/DropTable.hpp"
#include "query_optimizer/physical/FilterJoin.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/InsertSelection.hpp"
#include "query_optimizer/physical/InsertTuple.hpp"
#include "query_optimizer/physical/LIPFilterConfiguration.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/physical/PatternMatcher.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "query_optimizer/physical/Sample.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/SharedSubplanReference.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/UnionAll.hpp"
#include "query_optimizer/physical/UpdateTable.hpp"
#include "query_optimizer/physical/WindowAggregate.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildAggregationExistenceMapOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/BuildLIPFilterOperator.hpp"
#include "relational_operators/CreateIndexOperator.hpp"
#include "relational_operators/CreateTableOperator.hpp"
#include "relational_operators/DeleteOperator.hpp"
#include "relational_operators/DestroyAggregationStateOperator.hpp"
#include "relational_operators/DestroyHashOperator.hpp"
#include "relational_operators/DropTableOperator.hpp"
#include "relational_operators/FinalizeAggregationOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/InitializeAggregationOperator.hpp"
#include "relational_operators/InsertOperator.hpp"
#include "relational_operators/NestedLoopsJoinOperator.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SampleOperator.hpp"
#include "relational_operators/SaveBlocksOperator.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "relational_operators/SortMergeRunOperator.hpp"
#include "relational_operators/SortRunGenerationOperator.hpp"
#include "relational_operators/TableExportOperator.hpp"
#include "relational_operators/TableGeneratorOperator.hpp"
#include "relational_operators/TextScanOperator.hpp"
#include "relational_operators/UnionAllOperator.hpp"
#include "relational_operators/UpdateOperator.hpp"
#include "relational_operators/WindowAggregationOperator.hpp"
#include "storage/AggregationOperationState.pb.h"
#include "storage/HashTable.pb.h"
#include "storage/HashTableFactory.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "storage/SubBlockTypeRegistry.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"
#include "types/containers/Tuple.pb.h"
#include "utility/BarrieredReadWriteConcurrentBitVector.hpp"
#include "utility/SqlError.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

using std::atomic;
using std::make_unique;
using std::move;
using std::pair;
using std::static_pointer_cast;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace quickstep {
namespace optimizer {

DEFINE_string(join_hashtable_type, "SeparateChaining",
              "HashTable implementation to use for hash joins (valid options "
              "are SeparateChaining or LinearOpenAddressing)");
static const volatile bool join_hashtable_type_dummy
    = gflags::RegisterFlagValidator(&FLAGS_join_hashtable_type,
                                    &ValidateHashTableImplTypeString);

DEFINE_string(aggregate_hashtable_type, "SeparateChaining",
              "HashTable implementation to use for aggregates with GROUP BY "
              "(valid options are SeparateChaining or LinearOpenAddressing)");
static const volatile bool aggregate_hashtable_type_dummy
    = gflags::RegisterFlagValidator(&FLAGS_aggregate_hashtable_type,
                                    &ValidateHashTableImplTypeString);

DEFINE_bool(parallelize_load, true, "Parallelize loading data files.");

static bool ValidateNumAggregationPartitions(const char *flagname, int value) {
  return value > 0;
}
DEFINE_int32(num_aggregation_partitions,
             41,
             "The number of partitions in PartitionedHashTablePool used for "
             "performing the aggregation");
static const volatile bool num_aggregation_partitions_dummy
    = gflags::RegisterFlagValidator(&FLAGS_num_aggregation_partitions, &ValidateNumAggregationPartitions);

DEFINE_uint64(partition_aggregation_num_groups_threshold,
              100000,
              "The threshold used for deciding whether the aggregation is done "
              "in a partitioned way or not");

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;
namespace S = ::quickstep::serialization;

namespace {

size_t CacheLineAlignedBytes(const size_t actual_bytes) {
  return (actual_bytes + kCacheLineBytes - 1) / kCacheLineBytes * kCacheLineBytes;
}

size_t CalculateNumInitializationPartitionsForCollisionFreeVectorTable(const size_t memory_size) {
  // At least 1 partition, at most (#workers * 2) partitions.
  return std::max(1uL, std::min(memory_size / kCollisonFreeVectorInitBlobSize,
                                static_cast<size_t>(2 * FLAGS_num_workers)));
}

void CalculateCollisionFreeAggregationInfo(
    const size_t num_entries, const vector<pair<AggregationID, vector<const Type *>>> &group_by_aggrs_info,
    S::CollisionFreeVectorInfo *collision_free_vector_info) {
  size_t memory_size = CacheLineAlignedBytes(
      BarrieredReadWriteConcurrentBitVector::BytesNeeded(num_entries));

  for (std::size_t i = 0; i < group_by_aggrs_info.size(); ++i) {
    const auto &group_by_aggr_info = group_by_aggrs_info[i];

    size_t state_size = 0;
    switch (group_by_aggr_info.first) {
      case AggregationID::kCount: {
        state_size = sizeof(atomic<size_t>);
        break;
      }
      case AggregationID::kSum: {
        const vector<const Type *> &argument_types = group_by_aggr_info.second;
        DCHECK_EQ(1u, argument_types.size());
        switch (argument_types.front()->getTypeID()) {
          case TypeID::kInt:
          case TypeID::kLong:
            state_size = sizeof(atomic<std::int64_t>);
            break;
          case TypeID::kFloat:
          case TypeID::kDouble:
            state_size = sizeof(atomic<double>);
            break;
          default:
            LOG(FATAL) << "No support by CollisionFreeVector";
        }
        break;
      }
      default:
        LOG(FATAL) << "No support by CollisionFreeVector";
    }

    collision_free_vector_info->add_state_offsets(memory_size);
    memory_size += CacheLineAlignedBytes(state_size * num_entries);
  }

  collision_free_vector_info->set_memory_size(memory_size);
  collision_free_vector_info->set_num_init_partitions(
      CalculateNumInitializationPartitionsForCollisionFreeVectorTable(memory_size));
}

size_t CalculateNumFinalizationPartitionsForCollisionFreeVectorTable(const size_t num_entries) {
  // Set finalization segment size as 4096 entries.
  constexpr size_t kFinalizeSegmentSize = 4uL * 1024L;

  // At least 1 partition, at most (#workers * 2) partitions.
  return std::max(1uL, std::min(num_entries / kFinalizeSegmentSize,
                                static_cast<size_t>(2 * FLAGS_num_workers)));
}

bool CheckAggregatePartitioned(const std::size_t num_aggregate_functions,
                               const std::vector<bool> &is_distincts,
                               const std::vector<attribute_id> &group_by_attrs,
                               const std::size_t estimated_num_groups) {
  // If there's no aggregation, return false.
  if (num_aggregate_functions == 0) {
    return false;
  }
  // If there is only only aggregate function, we allow distinct aggregation.
  // Otherwise it can't be partitioned with distinct aggregation.
  if (num_aggregate_functions > 1) {
    for (const bool distinct : is_distincts) {
      if (distinct) {
        return false;
      }
    }
  }
  // There's no distinct aggregation involved, Check if there's at least one
  // GROUP BY operation.
  if (group_by_attrs.empty()) {
    return false;
  }

  // Currently we require that all the group-by keys are ScalarAttributes for
  // the convenient of implementing copy elision.
  // TODO(jianqiao): relax this requirement.
  for (const attribute_id group_by_attr : group_by_attrs) {
    if (group_by_attr == kInvalidAttributeID) {
      return false;
    }
  }

  // Currently we always use partitioned aggregation to parallelize distinct
  // aggregation.
  const bool all_distinct = std::accumulate(is_distincts.begin(), is_distincts.end(),
                                            !is_distincts.empty(), std::logical_and<bool>());
  if (all_distinct) {
    return true;
  }

  // There are GROUP BYs without DISTINCT. Check if the estimated number of
  // groups is large enough to warrant a partitioned aggregation.
  return estimated_num_groups >= FLAGS_partition_aggregation_num_groups_threshold;
}

}  // namespace

constexpr QueryPlan::DAGNodeIndex ExecutionGenerator::CatalogRelationInfo::kInvalidOperatorIndex;

void ExecutionGenerator::generatePlan(const P::PhysicalPtr &physical_plan) {
  CHECK(P::SomeTopLevelPlan::MatchesWithConditionalCast(physical_plan, &top_level_physical_plan_))
      << "The physical plan must be rooted by a TopLevelPlan";

  cost_model_for_aggregation_.reset(
      new cost::StarSchemaSimpleCostModel(top_level_physical_plan_->shared_subplans()));
  cost_model_for_hash_join_.reset(
      new cost::SimpleCostModel(top_level_physical_plan_->shared_subplans()));

  const auto &lip_filter_configuration =
      top_level_physical_plan_->lip_filter_configuration();
  if (lip_filter_configuration != nullptr) {
    lip_filter_generator_.reset(new LIPFilterGenerator(lip_filter_configuration));
  }

  const CatalogRelation *result_relation = nullptr;

  try {
    for (const P::PhysicalPtr &shared_subplan : top_level_physical_plan_->shared_subplans()) {
      generatePlanInternal(shared_subplan);
    }
    generatePlanInternal(top_level_physical_plan_->plan());

    // Deploy LIPFilters if enabled.
    if (lip_filter_generator_ != nullptr) {
      lip_filter_generator_->deployLIPFilters(execution_plan_, query_context_proto_);
    }

    // Set the query result relation if the input plan exists in physical_to_execution_map_,
    // which indicates the plan is the result of a SELECT query.
    const std::unordered_map<P::PhysicalPtr, CatalogRelationInfo>::const_iterator it =
        physical_to_output_relation_map_.find(top_level_physical_plan_->plan());
    if (it != physical_to_output_relation_map_.end()) {
      result_relation = it->second.relation;
    }
  } catch (...) {
    // Drop all temporary relations.
    dropAllTemporaryRelations();
    throw;
  }

  // Add one DropTableOperator per temporary relation, except for the result relation, if any.
  // NOTE(zuyu): the Cli shell drops the result relation after printing, if enabled.
  for (const CatalogRelationInfo &temporary_relation_info : temporary_relation_info_vec_) {
    const CatalogRelation *temporary_relation = temporary_relation_info.relation;
    if (temporary_relation == result_relation) {
      query_handle_->setQueryResultRelation(
          catalog_database_->getRelationByIdMutable(result_relation->getID()));
      continue;
    }
    const QueryPlan::DAGNodeIndex drop_table_index =
        execution_plan_->addRelationalOperator(
            new DropTableOperator(query_handle_->query_id(),
                                  *temporary_relation,
                                  catalog_database_,
                                  false /* only_drop_blocks */));
    DCHECK(!temporary_relation_info.isStoredRelation());
    execution_plan_->addDependenciesForDropOperator(
        drop_table_index,
        temporary_relation_info.producer_operator_index);
  }

#ifdef QUICKSTEP_DISTRIBUTED
  catalog_database_cache_proto_->set_name(catalog_database_->getName());

  LOG(INFO) << "CatalogDatabaseCache proto has " << referenced_relation_ids_.size() << " relation(s)";
  for (const relation_id rel_id : referenced_relation_ids_) {
    const CatalogRelationSchema &relation =
        catalog_database_->getRelationSchemaById(rel_id);
    LOG(INFO) << "RelationSchema " << rel_id
              << ", name: " << relation.getName()
              << ", " << relation.size()  << " attribute(s)";
    catalog_database_cache_proto_->add_relations()->MergeFrom(relation.getProto());
  }
#endif
}

void ExecutionGenerator::generatePlanInternal(
    const P::PhysicalPtr &physical_plan) {
  // Generate the execution plan in bottom-up.
  for (const P::PhysicalPtr &child : physical_plan->children()) {
    generatePlanInternal(child);
  }

  // If enabled, collect attribute substitution map for LIPFilterGenerator.
  if (lip_filter_generator_ != nullptr) {
    lip_filter_generator_->registerAttributeMap(physical_plan, attribute_substitution_map_);
  }

  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kAggregate:
      return convertAggregate(
          std::static_pointer_cast<const P::Aggregate>(physical_plan));
    case P::PhysicalType::kCrossReferenceCoalesceAggregate:
      return convertCrossReferenceCoalesceAggregate(
          std::static_pointer_cast<const P::CrossReferenceCoalesceAggregate>(physical_plan));
    case P::PhysicalType::kCopyFrom:
      return convertCopyFrom(
          std::static_pointer_cast<const P::CopyFrom>(physical_plan));
    case P::PhysicalType::kCopyTo:
      return convertCopyTo(
          std::static_pointer_cast<const P::CopyTo>(physical_plan));
    case P::PhysicalType::kCreateIndex:
      return convertCreateIndex(
          std::static_pointer_cast<const P::CreateIndex>(physical_plan));
    case P::PhysicalType::kCreateTable:
      return convertCreateTable(
          std::static_pointer_cast<const P::CreateTable>(physical_plan));
    case P::PhysicalType::kDeleteTuples:
      return convertDeleteTuples(
          std::static_pointer_cast<const P::DeleteTuples>(physical_plan));
    case P::PhysicalType::kDropTable:
      return convertDropTable(
          std::static_pointer_cast<const P::DropTable>(physical_plan));
    case P::PhysicalType::kFilterJoin:
      return convertFilterJoin(
          std::static_pointer_cast<const P::FilterJoin>(physical_plan));
    case P::PhysicalType::kHashJoin:
      return convertHashJoin(
          std::static_pointer_cast<const P::HashJoin>(physical_plan));
    case P::PhysicalType::kInsertSelection:
      return convertInsertSelection(
          std::static_pointer_cast<const P::InsertSelection>(physical_plan));
    case P::PhysicalType::kInsertTuple:
      return convertInsertTuple(
          std::static_pointer_cast<const P::InsertTuple>(physical_plan));
    case P::PhysicalType::kNestedLoopsJoin:
      return convertNestedLoopsJoin(
          std::static_pointer_cast<const P::NestedLoopsJoin>(physical_plan));
    case P::PhysicalType::kSample:
      return convertSample(
          std::static_pointer_cast<const P::Sample>(physical_plan));
    case P::PhysicalType::kSelection:
      return convertSelection(
          std::static_pointer_cast<const P::Selection>(physical_plan));
    case P::PhysicalType::kSharedSubplanReference:
      return convertSharedSubplanReference(
          std::static_pointer_cast<const P::SharedSubplanReference>(physical_plan));
    case P::PhysicalType::kSort:
      return convertSort(
          std::static_pointer_cast<const P::Sort>(physical_plan));
    case P::PhysicalType::kTableGenerator:
      return convertTableGenerator(
          std::static_pointer_cast<const P::TableGenerator>(physical_plan));
    case P::PhysicalType::kTableReference:
      return convertTableReference(
          std::static_pointer_cast<const P::TableReference>(physical_plan));
    case P::PhysicalType::kUnionAll:
      return convertUnionAll(
          std::static_pointer_cast<const P::UnionAll>(physical_plan));
    case P::PhysicalType::kUpdateTable:
      return convertUpdateTable(
          std::static_pointer_cast<const P::UpdateTable>(physical_plan));
    case P::PhysicalType::kWindowAggregate:
      return convertWindowAggregate(
          std::static_pointer_cast<const P::WindowAggregate>(physical_plan));
    default:
      LOG(FATAL) << "Unknown physical plan node "
                 << physical_plan->getShortString();
  }
}

std::string ExecutionGenerator::getNewRelationName() {
  std::ostringstream out;
  out << OptimizerContext::kInternalTemporaryRelationNamePrefix
      << query_handle_->query_id() << "_" << rel_id_;
  ++rel_id_;
  return out.str();
}

void ExecutionGenerator::createTemporaryCatalogRelation(
    const P::PhysicalPtr &physical,
    const CatalogRelation **catalog_relation_output,
    S::InsertDestination *insert_destination_proto) {
  std::unique_ptr<CatalogRelation> catalog_relation(
      new CatalogRelation(catalog_database_,
                          getNewRelationName(),
                          -1 /* id */,
                          true /* is_temporary*/));
  attribute_id aid = 0;
  for (const E::NamedExpressionPtr &project_expression :
       physical->getOutputAttributes()) {
    // The attribute name is simply set to the attribute id to make it distinct.
    std::unique_ptr<CatalogAttribute> catalog_attribute(
        new CatalogAttribute(catalog_relation.get(),
                             std::to_string(aid),
                             project_expression->getValueType(),
                             aid,
                             project_expression->attribute_alias()));
    attribute_substitution_map_[project_expression->id()] =
        catalog_attribute.get();
    catalog_relation->addAttribute(catalog_attribute.release());
    ++aid;
  }

  const P::PartitionSchemeHeader *partition_scheme_header = physical->getOutputPartitionSchemeHeader();
  if (partition_scheme_header) {
    PartitionSchemeHeader::PartitionAttributeIds output_partition_attr_ids;
    for (const auto &partition_equivalent_expr_ids : partition_scheme_header->partition_expr_ids) {
      DCHECK(!partition_equivalent_expr_ids.empty());
      const E::ExprId partition_expr_id = *partition_equivalent_expr_ids.begin();
      DCHECK(attribute_substitution_map_.find(partition_expr_id) != attribute_substitution_map_.end());
      output_partition_attr_ids.push_back(attribute_substitution_map_[partition_expr_id]->getID());
    }

    const size_t num_partition = partition_scheme_header->num_partitions;
    unique_ptr<PartitionSchemeHeader> output_partition_scheme_header;
    switch (partition_scheme_header->partition_type) {
      case P::PartitionSchemeHeader::PartitionType::kHash:
        output_partition_scheme_header =
            make_unique<HashPartitionSchemeHeader>(num_partition, move(output_partition_attr_ids));
        break;
      case P::PartitionSchemeHeader::PartitionType::kRandom:
        output_partition_scheme_header =
            make_unique<RandomPartitionSchemeHeader>(num_partition);
        break;
      case P::PartitionSchemeHeader::PartitionType::kRange:
        LOG(FATAL) << "Unimplemented";
      default:
        LOG(FATAL) << "Unknown partition type";
    }
    auto output_partition_scheme = make_unique<PartitionScheme>(output_partition_scheme_header.release());

    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::PARTITION_AWARE);
    insert_destination_proto->MutableExtension(S::PartitionAwareInsertDestination::partition_scheme)
        ->MergeFrom(output_partition_scheme->getProto());

    catalog_relation->setPartitionScheme(output_partition_scheme.release());
  } else {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);
  }

  *catalog_relation_output = catalog_relation.get();
  const relation_id output_rel_id = catalog_database_->addRelation(
      catalog_relation.release());

  insert_destination_proto->set_relation_id(output_rel_id);

#ifdef QUICKSTEP_DISTRIBUTED
  referenced_relation_ids_.insert(output_rel_id);
#endif
}

void ExecutionGenerator::dropAllTemporaryRelations() {
  for (const CatalogRelationInfo &temporary_relation_info :
       temporary_relation_info_vec_) {
    DCHECK_EQ(temporary_relation_info.relation->size_blocks(), 0u);
    catalog_database_->dropRelationById(temporary_relation_info.relation->getID());
  }
}

void ExecutionGenerator::convertNamedExpressions(
    const std::vector<E::NamedExpressionPtr> &named_expressions,
    S::QueryContext::ScalarGroup *scalar_group_proto) {
  for (const E::NamedExpressionPtr &project_expression : named_expressions) {
    unique_ptr<const Scalar> execution_scalar;
    E::AliasPtr alias;
    if (E::SomeAlias::MatchesWithConditionalCast(project_expression, &alias)) {
      E::ScalarPtr scalar;
      // We have not added aggregate expressions yet,
      // so all child expressions of an Alias should be a Scalar.
      CHECK(E::SomeScalar::MatchesWithConditionalCast(alias->expression(), &scalar))
          << alias->toString();
      execution_scalar.reset(scalar->concretize(attribute_substitution_map_));
    } else {
      execution_scalar.reset(project_expression->concretize(attribute_substitution_map_));
    }

    scalar_group_proto->add_scalars()->CopyFrom(execution_scalar->getProto());
  }
}

Predicate* ExecutionGenerator::convertPredicate(
    const expressions::PredicatePtr &optimizer_predicate) const {
  return optimizer_predicate->concretize(attribute_substitution_map_);
}

void ExecutionGenerator::convertTableReference(
    const P::TableReferencePtr &physical_table_reference) {
  // TableReference is not converted to an execution operator;
  // instead it just provides CatalogRelation info for its
  // parent (e.g. the substitution map from an AttributeReference
  // to a CatalogAttribute).
  const CatalogRelation *catalog_relation = physical_table_reference->relation();

#ifdef QUICKSTEP_DISTRIBUTED
  referenced_relation_ids_.insert(catalog_relation->getID());
#endif

  const std::vector<E::AttributeReferencePtr> &attribute_references =
      physical_table_reference->attribute_list();
  DCHECK_EQ(attribute_references.size(), catalog_relation->size());

  for (CatalogRelation::size_type i = 0; i < catalog_relation->size(); ++i) {
    attribute_substitution_map_.emplace(attribute_references[i]->id(),
                                        catalog_relation->getAttributeById(i));
  }
  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_table_reference),
      std::forward_as_tuple(CatalogRelationInfo::kInvalidOperatorIndex,
                            catalog_relation));
}

void ExecutionGenerator::convertSample(const P::SamplePtr &physical_sample) {
  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto =
      query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_sample,
                                 &output_relation,
                                 insert_destination_proto);

  // Create and add a Sample operator.
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_sample->input());
  DCHECK(input_relation_info != nullptr);

  SampleOperator *sample_op =
      new SampleOperator(query_handle_->query_id(),
                         *input_relation_info->relation,
                         *output_relation,
                         insert_destination_index,
                         input_relation_info->isStoredRelation(),
                         physical_sample->is_block_sample(),
                         physical_sample->percentage());
  const QueryPlan::DAGNodeIndex sample_index =
      execution_plan_->addRelationalOperator(sample_op);
  insert_destination_proto->set_relational_op_index(sample_index);

  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(sample_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_sample),
      std::forward_as_tuple(sample_index,
                            output_relation));
  temporary_relation_info_vec_.emplace_back(sample_index, output_relation);
}

bool ExecutionGenerator::convertSimpleProjection(
    const QueryContext::scalar_group_id project_expressions_group_index,
    std::vector<attribute_id> *attributes) const {
  const S::QueryContext::ScalarGroup &scalar_group_proto =
      query_context_proto_->scalar_groups(project_expressions_group_index);

  for (int i = 0; i < scalar_group_proto.scalars_size(); ++i) {
    if (scalar_group_proto.scalars(i).data_source() != S::Scalar::ATTRIBUTE) {
      return false;
    }
  }

  for (int i = 0; i < scalar_group_proto.scalars_size(); ++i) {
    attributes->push_back(
      scalar_group_proto.scalars(i).GetExtension(S::ScalarAttribute::attribute_id));
  }

  return true;
}

void ExecutionGenerator::convertSelection(
    const P::SelectionPtr &physical_selection) {
  const P::PhysicalPtr input = physical_selection->input();
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(input);
  DCHECK(input_relation_info != nullptr);
  const CatalogRelation &input_relation = *input_relation_info->relation;

  // Check if the Selection is only for renaming columns.
  if (physical_selection->filter_predicate() == nullptr) {
    const P::PartitionSchemeHeader *physical_select_partition_scheme_header =
        physical_selection->getOutputPartitionSchemeHeader();
    const P::PartitionSchemeHeader *physical_input_partition_scheme_header = input->getOutputPartitionSchemeHeader();

    const bool are_same_physical_partition_scheme_headers =
        (!physical_select_partition_scheme_header && !physical_input_partition_scheme_header) ||
        (physical_select_partition_scheme_header && physical_input_partition_scheme_header &&
         physical_select_partition_scheme_header->equal(*physical_input_partition_scheme_header));

    const std::vector<E::AttributeReferencePtr> input_attributes = input->getOutputAttributes();

    const std::vector<E::NamedExpressionPtr> &project_expressions =
        physical_selection->project_expressions();
    if (project_expressions.size() == input_attributes.size() && are_same_physical_partition_scheme_headers) {
      bool has_different_attrs = false;
      for (std::size_t attr_idx = 0; attr_idx < input_attributes.size(); ++attr_idx) {
        if (project_expressions[attr_idx]->id() != input_attributes[attr_idx]->id()) {
          has_different_attrs = true;
          break;
        }
      }
      if (!has_different_attrs) {
        if (!input_relation_info->isStoredRelation()) {
          CatalogRelation *catalog_relation =
              const_cast<CatalogRelation*>(input_relation_info->relation);
          for (std::size_t attr_idx = 0; attr_idx < project_expressions.size(); ++attr_idx) {
            CatalogAttribute *catalog_attribute =
                catalog_relation->getAttributeByIdMutable(attr_idx);
            DCHECK(catalog_attribute != nullptr);
            catalog_attribute->setDisplayName(
                project_expressions[attr_idx]->attribute_alias());
          }
          physical_to_output_relation_map_.emplace(physical_selection,
                                                   *input_relation_info);
          return;
        }
      }
    }
  }

  // Convert the project expressions proto.
  const QueryContext::scalar_group_id project_expressions_group_index =
      query_context_proto_->scalar_groups_size();
  convertNamedExpressions(physical_selection->project_expressions(),
                          query_context_proto_->add_scalar_groups());

  // Convert the predicate proto.
  QueryContext::predicate_id execution_predicate_index = QueryContext::kInvalidPredicateId;
  if (physical_selection->filter_predicate()) {
    execution_predicate_index = query_context_proto_->predicates_size();

    unique_ptr<const Predicate> execution_predicate(convertPredicate(physical_selection->filter_predicate()));
    query_context_proto_->add_predicates()->CopyFrom(execution_predicate->getProto());
  }

  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_selection,
                                 &output_relation,
                                 insert_destination_proto);

  // Create and add a Select operator.
  const bool has_repartition = physical_selection->hasRepartition();

  // Use the "simple" form of the selection operator (a pure projection that
  // doesn't require any expression evaluation or intermediate copies) if
  // possible.
  std::vector<attribute_id> attributes;
  SelectOperator *op =
      convertSimpleProjection(project_expressions_group_index, &attributes)
          ? new SelectOperator(query_handle_->query_id(),
                               input_relation,
                               has_repartition,
                               *output_relation,
                               insert_destination_index,
                               execution_predicate_index,
                               move(attributes),
                               input_relation_info->isStoredRelation())
          : new SelectOperator(query_handle_->query_id(),
                               input_relation,
                               has_repartition,
                               *output_relation,
                               insert_destination_index,
                               execution_predicate_index,
                               project_expressions_group_index,
                               input_relation_info->isStoredRelation());

  const QueryPlan::DAGNodeIndex select_index =
      execution_plan_->addRelationalOperator(op);
  insert_destination_proto->set_relational_op_index(select_index);

  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(select_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_selection),
      std::forward_as_tuple(select_index,
                            output_relation));
  temporary_relation_info_vec_.emplace_back(select_index, output_relation);

  if (lip_filter_generator_ != nullptr) {
    lip_filter_generator_->addSelectionInfo(physical_selection, select_index);
  }
}

void ExecutionGenerator::convertSharedSubplanReference(const physical::SharedSubplanReferencePtr &physical_plan) {
  const std::unordered_map<physical::PhysicalPtr, CatalogRelationInfo>::const_iterator found_it =
      physical_to_output_relation_map_.find(
          top_level_physical_plan_->shared_subplan_at(physical_plan->subplan_id()));
  if (found_it != physical_to_output_relation_map_.end()) {
    physical_to_output_relation_map_.emplace(physical_plan, found_it->second);

    // Propagate the (ExprId -> CatalogAttribute) mapping.
    const std::vector<E::AttributeReferencePtr> &referenced_attributes =
        physical_plan->referenced_attributes();
    const std::vector<E::AttributeReferencePtr> &output_attributes =
        physical_plan->output_attributes();
    for (std::size_t i = 0; i < referenced_attributes.size(); ++i) {
      attribute_substitution_map_[output_attributes[i]->id()] =
          attribute_substitution_map_[referenced_attributes[i]->id()];
    }
  }
}

void ExecutionGenerator::convertFilterJoin(const P::FilterJoinPtr &physical_plan) {
  P::PhysicalPtr probe_physical = physical_plan->left();
  P::PhysicalPtr build_physical = physical_plan->right();

  // Let B denote the build side child. If B is also a FilterJoin, then the
  // actual "concrete" input relation is B's probe side child, and B's build
  // side becomes a LIPFilter that is attached to the BuildLIPFilterOperator
  // created below.
  P::FilterJoinPtr filter_join;
  if (P::SomeFilterJoin::MatchesWithConditionalCast(build_physical, &filter_join)) {
    build_physical = filter_join->left();
    DCHECK(build_physical->getPhysicalType() != P::PhysicalType::kFilterJoin);
  }

  // Convert the predicate proto.
  QueryContext::predicate_id build_side_predicate_index = QueryContext::kInvalidPredicateId;
  if (physical_plan->build_side_filter_predicate()) {
    build_side_predicate_index = query_context_proto_->predicates_size();

    std::unique_ptr<const Predicate> build_side_predicate(
        convertPredicate(physical_plan->build_side_filter_predicate()));
    query_context_proto_->add_predicates()->CopyFrom(build_side_predicate->getProto());
  }

  const CatalogRelationInfo *probe_relation_info =
      findRelationInfoOutputByPhysical(probe_physical);
  const CatalogRelationInfo *build_relation_info =
      findRelationInfoOutputByPhysical(build_physical);

  const CatalogRelation &build_relation = *build_relation_info->relation;

  // Create a BuildLIPFilterOperator for the FilterJoin. This operator builds
  // LIP filters that are applied properly in downstream operators to achieve
  // the filter-join semantics.
  const QueryPlan::DAGNodeIndex build_filter_operator_index =
      execution_plan_->addRelationalOperator(
          new BuildLIPFilterOperator(
              query_handle_->query_id(),
              build_relation,
              build_side_predicate_index,
              build_relation_info->isStoredRelation()));

  if (!build_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(build_filter_operator_index,
                                         build_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_plan),
      std::forward_as_tuple(probe_relation_info->producer_operator_index,
                            probe_relation_info->relation));

  DCHECK(lip_filter_generator_ != nullptr);
  lip_filter_generator_->addFilterJoinInfo(physical_plan,
                                           build_filter_operator_index);
}

void ExecutionGenerator::convertHashJoin(const P::HashJoinPtr &physical_plan) {
  // HashJoin is converted to three operators:
  //     BuildHash, HashJoin, DestroyHash. The second is the primary operator.

  P::PhysicalPtr probe_physical = physical_plan->left();
  P::PhysicalPtr build_physical = physical_plan->right();

  std::vector<attribute_id> probe_attribute_ids;
  std::vector<attribute_id> build_attribute_ids;

  std::size_t build_cardinality =
      cost_model_for_hash_join_->estimateCardinality(build_physical);

  bool any_probe_attributes_nullable = false;
  bool any_build_attributes_nullable = false;

  const std::vector<E::AttributeReferencePtr> &left_join_attributes =
      physical_plan->left_join_attributes();
  for (const E::AttributeReferencePtr &left_join_attribute : left_join_attributes) {
    const CatalogAttribute *probe_catalog_attribute
        = attribute_substitution_map_[left_join_attribute->id()];
    probe_attribute_ids.emplace_back(probe_catalog_attribute->getID());

    if (probe_catalog_attribute->getType().isNullable()) {
      any_probe_attributes_nullable = true;
    }
  }

  const std::vector<E::AttributeReferencePtr> &right_join_attributes =
      physical_plan->right_join_attributes();
  for (const E::AttributeReferencePtr &right_join_attribute : right_join_attributes) {
    const CatalogAttribute *build_catalog_attribute
        = attribute_substitution_map_[right_join_attribute->id()];
    build_attribute_ids.emplace_back(build_catalog_attribute->getID());

    if (build_catalog_attribute->getType().isNullable()) {
      any_build_attributes_nullable = true;
    }
  }

  // Remember key types for call to SimplifyHashTableImplTypeProto() below.
  std::vector<const Type*> key_types;
  for (std::vector<E::AttributeReferencePtr>::size_type attr_idx = 0;
       attr_idx < left_join_attributes.size();
       ++attr_idx) {
    const Type &left_attribute_type = left_join_attributes[attr_idx]->getValueType();
    const Type &right_attribute_type = right_join_attributes[attr_idx]->getValueType();
    if (left_attribute_type.getTypeID() != right_attribute_type.getTypeID()) {
      THROW_SQL_ERROR() << "Equality join predicate between two attributes of different types "
                           "is not allowed in HashJoin";
    }
    key_types.push_back(&left_attribute_type);
  }

  // Convert the residual predicate proto.
  QueryContext::predicate_id residual_predicate_index = QueryContext::kInvalidPredicateId;
  if (physical_plan->residual_predicate()) {
    residual_predicate_index = query_context_proto_->predicates_size();

    unique_ptr<const Predicate> residual_predicate(convertPredicate(physical_plan->residual_predicate()));
    query_context_proto_->add_predicates()->CopyFrom(residual_predicate->getProto());
  }

  // Convert the project expressions proto.
  const QueryContext::scalar_group_id project_expressions_group_index =
      query_context_proto_->scalar_groups_size();
  convertNamedExpressions(physical_plan->project_expressions(),
                          query_context_proto_->add_scalar_groups());

  const CatalogRelationInfo *build_relation_info =
      findRelationInfoOutputByPhysical(build_physical);
  const CatalogRelationInfo *probe_relation_info =
      findRelationInfoOutputByPhysical(probe_physical);

  // Create a vector that indicates whether each project expression is using
  // attributes from the build relation as input. This information is required
  // by the current implementation of hash left outer join
  std::unique_ptr<std::vector<bool>> is_selection_on_build;
  if (physical_plan->join_type() == P::HashJoin::JoinType::kLeftOuterJoin) {
    is_selection_on_build.reset(
        new std::vector<bool>(
            E::MarkExpressionsReferingAnyAttribute(
                physical_plan->project_expressions(),
                build_physical->getOutputAttributes())));
  }

  const CatalogRelation *build_relation = build_relation_info->relation;
  const CatalogRelation *probe_relation = probe_relation_info->relation;

  // FIXME(quickstep-team): Add support for self-join.
  if (build_relation == probe_relation) {
    THROW_SQL_ERROR() << "Self-join is not supported";
  }

  // Create join hash table proto.
  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto_->join_hash_tables_size();
  S::QueryContext::HashTableContext *hash_table_context_proto =
      query_context_proto_->add_join_hash_tables();

  const std::size_t probe_num_partitions = probe_relation->getNumPartitions();
  hash_table_context_proto->set_num_partitions(probe_num_partitions);

  S::HashTable *hash_table_proto = hash_table_context_proto->mutable_join_hash_table();

  // SimplifyHashTableImplTypeProto() switches the hash table implementation
  // from SeparateChaining to SimpleScalarSeparateChaining when there is a
  // single scalar key type with a reversible hash function.
  hash_table_proto->set_hash_table_impl_type(
      SimplifyHashTableImplTypeProto(
          HashTableImplTypeProtoFromString(FLAGS_join_hashtable_type),
          key_types));

  for (const attribute_id build_attribute : build_attribute_ids) {
    hash_table_proto->add_key_types()->CopyFrom(
        build_relation->getAttributeById(build_attribute)->getType().getProto());
  }

  hash_table_proto->set_estimated_num_entries(build_cardinality);

  // Create three operators.
  const QueryPlan::DAGNodeIndex build_operator_index =
      execution_plan_->addRelationalOperator(
          new BuildHashOperator(
              query_handle_->query_id(),
              *build_relation,
              build_relation_info->isStoredRelation(),
              build_attribute_ids,
              any_build_attributes_nullable,
              probe_num_partitions,
              join_hash_table_index));

  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_plan,
                                 &output_relation,
                                 insert_destination_proto);

  // Get JoinType
  HashJoinOperator::JoinType join_type;
  switch (physical_plan->join_type()) {
    case P::HashJoin::JoinType::kInnerJoin:
      join_type = HashJoinOperator::JoinType::kInnerJoin;
      break;
    case P::HashJoin::JoinType::kLeftSemiJoin:
      join_type = HashJoinOperator::JoinType::kLeftSemiJoin;
      break;
    case P::HashJoin::JoinType::kLeftAntiJoin:
      join_type = HashJoinOperator::JoinType::kLeftAntiJoin;
      break;
    case P::HashJoin::JoinType::kLeftOuterJoin:
      join_type = HashJoinOperator::JoinType::kLeftOuterJoin;
      break;
    default:
      LOG(FATAL) << "Invalid physical::HashJoin::JoinType: "
                 << static_cast<typename std::underlying_type<P::HashJoin::JoinType>::type>(
                        physical_plan->join_type());
  }

  // Create hash join operator
  const QueryPlan::DAGNodeIndex join_operator_index =
      execution_plan_->addRelationalOperator(
          new HashJoinOperator(
              query_handle_->query_id(),
              *build_relation,
              *probe_relation,
              probe_relation_info->isStoredRelation(),
              probe_attribute_ids,
              any_probe_attributes_nullable,
              probe_num_partitions,
              physical_plan->hasRepartition(),
              *output_relation,
              insert_destination_index,
              join_hash_table_index,
              residual_predicate_index,
              project_expressions_group_index,
              is_selection_on_build.get(),
              join_type));
  insert_destination_proto->set_relational_op_index(join_operator_index);

  const QueryPlan::DAGNodeIndex destroy_operator_index =
      execution_plan_->addRelationalOperator(new DestroyHashOperator(
          query_handle_->query_id(), probe_num_partitions, join_hash_table_index));

  if (!build_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(build_operator_index,
                                         build_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
    // Add the dependency for the producer operator of the build relation
    // to prevent the build relation from being destroyed until after the join
    // is complete (see QueryPlan::addDependenciesForDropOperator(), which
    // makes the drop operator for the temporary relation dependent on all its
    // consumers having finished).
    execution_plan_->addDirectDependency(join_operator_index,
                                         build_relation_info->producer_operator_index,
                                         true /* is_pipeline_breaker */);
  }
  if (!probe_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(join_operator_index,
                                         probe_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  execution_plan_->addDirectDependency(join_operator_index,
                                       build_operator_index,
                                       true /* is_pipeline_breaker */);
  execution_plan_->addDirectDependency(destroy_operator_index,
                                       join_operator_index,
                                       true /* is_pipeline_breaker */);

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_plan),
      std::forward_as_tuple(join_operator_index,
                            output_relation));
  temporary_relation_info_vec_.emplace_back(join_operator_index, output_relation);

  if (lip_filter_generator_ != nullptr) {
    lip_filter_generator_->addHashJoinInfo(physical_plan,
                                           build_operator_index,
                                           join_operator_index);
  }
}

void ExecutionGenerator::convertNestedLoopsJoin(
    const P::NestedLoopsJoinPtr &physical_plan) {
  // NestedLoopsJoin is converted to a NestedLoopsJoin operator.

  // Convert the join predicate proto.
  const QueryContext::predicate_id execution_join_predicate_index = query_context_proto_->predicates_size();
  if (physical_plan->join_predicate()) {
    unique_ptr<const Predicate> execution_join_predicate(convertPredicate(physical_plan->join_predicate()));
    query_context_proto_->add_predicates()->CopyFrom(execution_join_predicate->getProto());
  } else {
    query_context_proto_->add_predicates()->set_predicate_type(S::Predicate::TRUE);
  }

  // Convert the project expressions proto.
  const QueryContext::scalar_group_id project_expressions_group_index =
      query_context_proto_->scalar_groups_size();
  convertNamedExpressions(physical_plan->project_expressions(),
                          query_context_proto_->add_scalar_groups());

  const CatalogRelationInfo *left_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->left());
  const CatalogRelation &left_relation = *left_relation_info->relation;
  const CatalogRelationInfo *right_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->right());
  const CatalogRelation &right_relation = *right_relation_info->relation;

  // FIXME(quickstep-team): Add support for self-join.
  if (left_relation.getID() == right_relation.getID()) {
    THROW_SQL_ERROR() << "NestedLoopsJoin does not support self-join yet";
  }

  const std::size_t num_partitions = left_relation.getNumPartitions();

#ifdef QUICKSTEP_DEBUG
  if (right_relation.hasPartitionScheme()) {
    DCHECK_EQ(num_partitions, right_relation.getNumPartitions());
  }
#endif

  const std::size_t nested_loops_join_index =
      query_context_proto_->num_partitions_for_nested_loops_joins_size();
  query_context_proto_->add_num_partitions_for_nested_loops_joins(num_partitions);

  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_plan,
                                 &output_relation,
                                 insert_destination_proto);

  // Create and add a NestedLoopsJoin operator.
  const QueryPlan::DAGNodeIndex join_operator_index =
      execution_plan_->addRelationalOperator(
          new NestedLoopsJoinOperator(query_handle_->query_id(),
                                      nested_loops_join_index,
                                      left_relation,
                                      right_relation,
                                      num_partitions,
                                      physical_plan->hasRepartition(),
                                      *output_relation,
                                      insert_destination_index,
                                      execution_join_predicate_index,
                                      project_expressions_group_index,
                                      left_relation_info->isStoredRelation(),
                                      right_relation_info->isStoredRelation()));
  insert_destination_proto->set_relational_op_index(join_operator_index);

  if (!left_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(join_operator_index,
                                         left_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  if (!right_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(join_operator_index,
                                         right_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_plan),
      std::forward_as_tuple(join_operator_index,
                            output_relation));
  temporary_relation_info_vec_.emplace_back(join_operator_index, output_relation);
}

void ExecutionGenerator::convertCopyFrom(
    const P::CopyFromPtr &physical_plan) {
  // CopyFrom is converted to a TextScan and a SaveBlocks.

  const CatalogRelation *output_relation = physical_plan->catalog_relation();
  const relation_id output_rel_id = output_relation->getID();

#ifdef QUICKSTEP_DISTRIBUTED
  referenced_relation_ids_.insert(output_rel_id);
#endif

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();

  if (output_relation->hasPartitionScheme()) {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::PARTITION_AWARE);
    insert_destination_proto->MutableExtension(S::PartitionAwareInsertDestination::partition_scheme)
        ->MergeFrom(output_relation->getPartitionScheme()->getProto());
  } else {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);

    const vector<block_id> blocks(output_relation->getBlocksSnapshot());
    for (const block_id block : blocks) {
      insert_destination_proto->AddExtension(S::BlockPoolInsertDestination::blocks, block);
    }
  }

  insert_destination_proto->set_relation_id(output_rel_id);
  insert_destination_proto->mutable_layout()->MergeFrom(
      output_relation->getDefaultStorageBlockLayout().getDescription());

  const QueryPlan::DAGNodeIndex scan_operator_index =
      execution_plan_->addRelationalOperator(
          new TextScanOperator(
              query_handle_->query_id(),
              physical_plan->file_name(),
              physical_plan->options(),
              *output_relation,
              insert_destination_index));
  insert_destination_proto->set_relational_op_index(scan_operator_index);

  CatalogRelation *mutable_output_relation =
      catalog_database_->getRelationByIdMutable(output_rel_id);
  const QueryPlan::DAGNodeIndex save_blocks_operator_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator(query_handle_->query_id(), mutable_output_relation));
  execution_plan_->addDirectDependency(save_blocks_operator_index,
                                       scan_operator_index,
                                       false /* is_pipeline_breaker */);
}

void ExecutionGenerator::convertCopyTo(const P::CopyToPtr &physical_plan) {
  // CopyTo is converted to a TableExport operator.

  const CatalogRelation *input_relation;
  bool input_relation_is_stored;

  const P::PhysicalPtr &input = physical_plan->input();
  P::TableReferencePtr table_reference;
  const CatalogRelationInfo *input_relation_info = nullptr;
  if (P::SomeTableReference::MatchesWithConditionalCast(input, &table_reference)) {
    input_relation = table_reference->relation();
    input_relation_is_stored = true;
  } else {
    input_relation_info = findRelationInfoOutputByPhysical(input);
    input_relation = input_relation_info->relation;
    input_relation_is_stored = false;
  }

  DCHECK(input_relation != nullptr);
  const QueryPlan::DAGNodeIndex table_export_operator_index =
      execution_plan_->addRelationalOperator(
          new TableExportOperator(query_handle_->query_id(),
                                  *input_relation,
                                  input_relation_is_stored,
                                  physical_plan->file_name(),
                                  physical_plan->options()));
  if (!input_relation_is_stored) {
    DCHECK(input_relation_info != nullptr);
    execution_plan_->addDirectDependency(table_export_operator_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
}

void ExecutionGenerator::convertCreateIndex(
  const P::CreateIndexPtr &physical_plan) {
  // CreateIndex is converted to a CreateIndex operator.
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  CatalogRelation *input_relation =
      catalog_database_->getRelationByIdMutable(
            input_relation_info->relation->getID());

  // Check if any index with the specified name already exists.
  if (input_relation->hasIndexWithName(physical_plan->index_name())) {
    THROW_SQL_ERROR() << "The relation " << input_relation->getName()
        << " already has an index named "<< physical_plan->index_name();
  }

  DCHECK_GT(physical_plan->index_attributes().size(), 0u);

  // Convert attribute references to a vector of pointers to catalog attributes.
  std::vector<const CatalogAttribute*> index_attributes;
  for (const E::AttributeReferencePtr &attribute : physical_plan->index_attributes()) {
    const CatalogAttribute *catalog_attribute
        = input_relation->getAttributeByName(attribute->attribute_name());
    DCHECK(catalog_attribute != nullptr);
    index_attributes.emplace_back(catalog_attribute);
  }

  // Create a copy of index description and add all the specified attributes to it.
  IndexSubBlockDescription index_description(*physical_plan->index_description());
  for (const CatalogAttribute* catalog_attribute : index_attributes) {
    index_description.add_indexed_attribute_ids(catalog_attribute->getID());
  }
  if (input_relation->hasIndexWithDescription(index_description)) {
    // Check if the given index description already exists in the relation.
    THROW_SQL_ERROR() << "The relation " << input_relation->getName()
        << " already defines this index on the given attribute(s).";
  }
  if (!SubBlockTypeRegistry::IndexDescriptionIsValid(*input_relation, index_description)) {
    // Check if the given index description is valid.
    THROW_SQL_ERROR() << "The index with given properties cannot be created.";
  }
  execution_plan_->addRelationalOperator(
      new CreateIndexOperator(query_handle_->query_id(),
                              input_relation,
                              physical_plan->index_name(),
                              std::move(index_description)));
}

void ExecutionGenerator::convertCreateTable(
    const P::CreateTablePtr &physical_plan) {
  // CreateTable is converted to a CreateTable operator.

  std::unique_ptr<CatalogRelation> catalog_relation(new CatalogRelation(
      catalog_database_,
      physical_plan->relation_name(),
      -1 /* id */,
      false /* is_temporary*/));
  attribute_id aid = 0;
  for (const E::AttributeReferencePtr &attribute :
       physical_plan->attributes()) {
    std::unique_ptr<CatalogAttribute> catalog_attribute(new CatalogAttribute(
        catalog_relation.get(),
        attribute->attribute_name(),
        attribute->getValueType(),
        aid,
        attribute->attribute_alias()));
    catalog_relation->addAttribute(catalog_attribute.release());
    ++aid;
  }

  // If specified, set the physical block type as the users'. Otherwise,
  // the system uses the default layout.
  if (physical_plan->block_properties()) {
    if (!StorageBlockLayout::DescriptionIsValid(*catalog_relation,
                                                *physical_plan->block_properties())) {
      THROW_SQL_ERROR() << "BLOCKPROPERTIES is invalid.";
    }

    std::unique_ptr<StorageBlockLayout> layout(
        new StorageBlockLayout(*catalog_relation, *physical_plan->block_properties()));
    layout->finalize();
    catalog_relation->setDefaultStorageBlockLayout(layout.release());
  }

  if (physical_plan->partition_scheme_header_proto()) {
    catalog_relation->setPartitionScheme(new PartitionScheme(
        PartitionSchemeHeader::ReconstructFromProto(*physical_plan->partition_scheme_header_proto())));
  }

  execution_plan_->addRelationalOperator(
      new CreateTableOperator(query_handle_->query_id(),
                              catalog_relation.release(),
                              catalog_database_));
}

void ExecutionGenerator::convertDeleteTuples(
    const P::DeleteTuplesPtr &physical_plan) {
  // If there is a selection predicate and the predicate value
  // is not statically true, DeleteTuples is converted to
  // a DeleteOperator and a SaveBlocksOperator; if there is not
  // a selection predicate or the predicate value is statically true,
  // it is converted to a DropTableOperator; otherwise, the predicate
  // value is statically false, so no operator needs to be created.

  unique_ptr<const Predicate> execution_predicate;
  if (physical_plan->predicate()) {
    execution_predicate.reset(convertPredicate(physical_plan->predicate()));
  }

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  DCHECK(input_relation_info != nullptr);

  const CatalogRelation *input_relation = input_relation_info->relation;

  if (execution_predicate == nullptr ||
      (execution_predicate->hasStaticResult() &&
       execution_predicate->getStaticResult())) {
    const QueryPlan::DAGNodeIndex drop_table_index =
        execution_plan_->addRelationalOperator(
            new DropTableOperator(query_handle_->query_id(),
                                  *input_relation,
                                  catalog_database_,
                                  true /* only_drop_blocks */));
    if (!input_relation_info->isStoredRelation()) {
      execution_plan_->addDirectDependency(drop_table_index,
                                           input_relation_info->producer_operator_index,
                                           true /* is_pipeline_breaker */);
    }
  } else if (!execution_predicate->hasStaticResult()) {
    const QueryContext::predicate_id execution_predicate_index = query_context_proto_->predicates_size();
    query_context_proto_->add_predicates()->CopyFrom(execution_predicate->getProto());

    const QueryPlan::DAGNodeIndex delete_tuples_index =
        execution_plan_->addRelationalOperator(
            new DeleteOperator(query_handle_->query_id(),
                               *input_relation,
                               execution_predicate_index,
                               input_relation_info->isStoredRelation()));

    if (!input_relation_info->isStoredRelation()) {
      execution_plan_->addDirectDependency(delete_tuples_index,
                                           input_relation_info->producer_operator_index,
                                           false /* is_pipeline_breaker */);
    }

    CatalogRelation *mutable_relation =
        catalog_database_->getRelationByIdMutable(input_relation->getID());
    const QueryPlan::DAGNodeIndex save_blocks_index =
        execution_plan_->addRelationalOperator(
            new SaveBlocksOperator(query_handle_->query_id(), mutable_relation));
    execution_plan_->addDirectDependency(save_blocks_index,
                                         delete_tuples_index,
                                         false /* is_pipeline_breaker */);
  }
}

void ExecutionGenerator::convertDropTable(
    const P::DropTablePtr &physical_plan) {
  // DropTable is converted to a DropTable operator.
  const CatalogRelation &catalog_relation = *physical_plan->catalog_relation();

#ifdef QUICKSTEP_DISTRIBUTED
  referenced_relation_ids_.insert(catalog_relation.getID());
#endif

  execution_plan_->addRelationalOperator(
      new DropTableOperator(query_handle_->query_id(),
                            catalog_relation,
                            catalog_database_));
}

void ExecutionGenerator::convertInsertTuple(
    const P::InsertTuplePtr &physical_plan) {
  // InsertTuple is converted to an Insert and a SaveBlocks.
#ifdef QUICKSTEP_DISTRIBUTED
  query_handle_->set_is_single_node_query();
#endif  // QUICKSTEP_DISTRIBUTED

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  const CatalogRelation &input_relation =
      *catalog_database_->getRelationById(
          input_relation_info->relation->getID());

  // Construct the tuple proto to be inserted.
  const QueryContext::tuple_id tuple_index = query_context_proto_->tuples_size();

  S::Tuple *tuple_proto = query_context_proto_->add_tuples();
  for (const E::ScalarLiteralPtr &literal : physical_plan->column_values()) {
    tuple_proto->add_attribute_values()->CopyFrom(
        literal->value().toTypedValue().getProto());
  }

  // FIXME(qzeng): A better way is using a traits struct to look up whether a storage
  //               block supports ad-hoc insertion instead of hard-coding the block types.
  const StorageBlockLayout &storage_block_layout =
      input_relation.getDefaultStorageBlockLayout();
  if (storage_block_layout.getDescription().tuple_store_description().sub_block_type() ==
      TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE ||
      storage_block_layout.getDescription().tuple_store_description().sub_block_type() ==
            TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE) {
    THROW_SQL_ERROR() << "INSERT statement is not supported for the relation "
                      << input_relation.getName()
                      << ", because its storage blocks do not support ad-hoc insertion";
  }

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();

  insert_destination_proto->set_relation_id(input_relation.getID());
  insert_destination_proto->mutable_layout()->MergeFrom(
      input_relation.getDefaultStorageBlockLayout().getDescription());

  if (input_relation.hasPartitionScheme()) {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::PARTITION_AWARE);
    insert_destination_proto->MutableExtension(S::PartitionAwareInsertDestination::partition_scheme)
        ->MergeFrom(input_relation.getPartitionScheme()->getProto());
  } else {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);

    const vector<block_id> blocks(input_relation.getBlocksSnapshot());
    for (const block_id block : blocks) {
      insert_destination_proto->AddExtension(S::BlockPoolInsertDestination::blocks, block);
    }
  }

  const QueryPlan::DAGNodeIndex insert_operator_index =
      execution_plan_->addRelationalOperator(
          new InsertOperator(query_handle_->query_id(),
                             input_relation,
                             insert_destination_index,
                             tuple_index));
  insert_destination_proto->set_relational_op_index(insert_operator_index);

  CatalogRelation *mutable_relation =
      catalog_database_->getRelationByIdMutable(input_relation.getID());
  const QueryPlan::DAGNodeIndex save_blocks_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator(query_handle_->query_id(), mutable_relation));
  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(insert_operator_index,
                                         input_relation_info->producer_operator_index,
                                         true /* is_pipeline_breaker */);
  }
  execution_plan_->addDirectDependency(save_blocks_index,
                                       insert_operator_index,
                                       false /* is_pipeline_breaker */);
}

void ExecutionGenerator::convertInsertSelection(
    const P::InsertSelectionPtr &physical_plan) {
  // InsertSelection is converted to a Select and a SaveBlocks.

  const CatalogRelationInfo *destination_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->destination());
  const CatalogRelation &destination_relation = *destination_relation_info->relation;

  // FIXME(qzeng): A better way is using a traits struct to look up whether a storage
  //               block supports ad-hoc insertion instead of hard-coding the block types.
  const StorageBlockLayout &storage_block_layout =
      destination_relation.getDefaultStorageBlockLayout();
  if (storage_block_layout.getDescription().tuple_store_description().sub_block_type() ==
          TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE
      || storage_block_layout.getDescription().tuple_store_description().sub_block_type() ==
             TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE) {
    THROW_SQL_ERROR() << "INSERT statement is not supported for the relation "
                      << destination_relation.getName()
                      << ", because its storage blocks do not support ad-hoc insertion";
  }

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  insert_destination_proto->set_relation_id(destination_relation.getID());
  insert_destination_proto->mutable_layout()->MergeFrom(
      destination_relation.getDefaultStorageBlockLayout().getDescription());

  if (destination_relation.hasPartitionScheme()) {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::PARTITION_AWARE);
    insert_destination_proto->MutableExtension(S::PartitionAwareInsertDestination::partition_scheme)
        ->MergeFrom(destination_relation.getPartitionScheme()->getProto());
  } else {
    insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);

    const vector<block_id> blocks(destination_relation.getBlocksSnapshot());
    for (const block_id block : blocks) {
      insert_destination_proto->AddExtension(S::BlockPoolInsertDestination::blocks, block);
    }
  }

  const CatalogRelationInfo *selection_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->selection());
  const CatalogRelation &selection_relation = *selection_relation_info->relation;

  // Prepare the attributes, which are output columns of the selection relation.
  std::vector<attribute_id> attributes;
  for (E::AttributeReferencePtr attr_ref : physical_plan->selection()->getOutputAttributes()) {
    unique_ptr<const Scalar> attribute(attr_ref->concretize(attribute_substitution_map_));

    DCHECK_EQ(Scalar::kAttribute, attribute->getDataSource());
    attributes.push_back(
        static_cast<const ScalarAttribute*>(attribute.get())->getAttribute().getID());
  }

  const P::PartitionSchemeHeader *input_physical_partition_scheme_header =
      physical_plan->selection()->getOutputPartitionSchemeHeader();
  const P::PartitionSchemeHeader *output_physical_partition_scheme_header =
      physical_plan->destination()->getOutputPartitionSchemeHeader();
  const bool has_repartition =
      (input_physical_partition_scheme_header && output_physical_partition_scheme_header)
          ? input_physical_partition_scheme_header->equal(*output_physical_partition_scheme_header)
          : (input_physical_partition_scheme_header || output_physical_partition_scheme_header);

  // Create the select operator.
  // TODO(jianqiao): This select operator is actually redundant. That is,
  // we may directly set physical_plan_->selection()'s output relation to be
  // destination_relation, instead of creating an intermediate selection_relation
  // and then copy the data into destination_relation. One way to achieve this
  // optimization is to enable specifying a specific output relation for each
  // physical plan by modifying class Physical.
  SelectOperator *insert_selection_op =
      new SelectOperator(query_handle_->query_id(),
                         selection_relation,
                         has_repartition,
                         destination_relation,
                         insert_destination_index,
                         QueryContext::kInvalidPredicateId,
                         move(attributes),
                         selection_relation_info->isStoredRelation());

  const QueryPlan::DAGNodeIndex insert_selection_index =
      execution_plan_->addRelationalOperator(insert_selection_op);
  insert_destination_proto->set_relational_op_index(insert_selection_index);

  CatalogRelation *mutable_relation =
      catalog_database_->getRelationByIdMutable(destination_relation.getID());
  const QueryPlan::DAGNodeIndex save_blocks_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator(query_handle_->query_id(), mutable_relation));

  if (!selection_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(insert_selection_index,
                                         selection_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  execution_plan_->addDirectDependency(save_blocks_index,
                                       insert_selection_index,
                                       false /* is_pipeline_breaker */);
}

void ExecutionGenerator::convertUnionAll(
    const P::UnionAllPtr &physical_unionall) {
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto =
      query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_unionall,
                                 &output_relation,
                                 insert_destination_proto);

  const std::vector<P::PhysicalPtr> &operands = physical_unionall->operands();
  std::vector<const CatalogRelation*> input_relations;
  std::vector<bool> is_stored_relation;
  std::vector<std::vector<attribute_id>> select_attribute_ids;
  std::vector<QueryPlan::DAGNodeIndex> dependency_operator_index;

  for (const auto &operand : operands) {
    const CatalogRelationInfo *input_relation_info =
        findRelationInfoOutputByPhysical(operand);
    DCHECK(input_relation_info != nullptr);
    input_relations.push_back(input_relation_info->relation);
    is_stored_relation.push_back(input_relation_info->isStoredRelation());
    dependency_operator_index.push_back(input_relation_info->producer_operator_index);

    const QueryContext::scalar_group_id project_expressions_group_index =
        query_context_proto_->scalar_groups_size();
    convertNamedExpressions(
        E::ToNamedExpressions(operand->getOutputAttributes()),
        query_context_proto_->add_scalar_groups());
    std::vector<attribute_id> select_attribute_id;
    convertSimpleProjection(project_expressions_group_index, &select_attribute_id);
    select_attribute_ids.push_back(std::move(select_attribute_id));
  }

  UnionAllOperator *union_all =
      new UnionAllOperator(query_handle_->query_id(),
                           input_relations,
                           *output_relation,
                           insert_destination_index,
                           is_stored_relation,
                           select_attribute_ids);

  const QueryPlan::DAGNodeIndex union_all_index =
      execution_plan_->addRelationalOperator(union_all);
  insert_destination_proto->set_relational_op_index(union_all_index);

  for (std::size_t relation_id = 0; relation_id < is_stored_relation.size(); ++relation_id) {
    if (!is_stored_relation[relation_id]) {
      execution_plan_->addDirectDependency(union_all_index,
                                           dependency_operator_index[relation_id],
                                           false /* is_pipeline_breaker */);
    }
  }

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_unionall),
      std::forward_as_tuple(union_all_index,
                            output_relation));
  temporary_relation_info_vec_.emplace_back(union_all_index, output_relation);
}

void ExecutionGenerator::convertUpdateTable(
    const P::UpdateTablePtr &physical_plan) {
  // UpdateTable is converted to an Update and a SaveBlocks.

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  DCHECK(input_relation_info != nullptr);

  const CatalogRelation *input_relation = input_relation_info->relation;
  const relation_id input_rel_id = input_relation->getID();

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id relocation_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *relocation_destination_proto = query_context_proto_->add_insert_destinations();

  if (input_relation->hasPartitionScheme()) {
    relocation_destination_proto->set_insert_destination_type(S::InsertDestinationType::PARTITION_AWARE);
    relocation_destination_proto->MutableExtension(S::PartitionAwareInsertDestination::partition_scheme)
        ->MergeFrom(input_relation->getPartitionScheme()->getProto());
  } else {
    relocation_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);
  }
  relocation_destination_proto->set_relation_id(input_rel_id);

  // Convert the predicate proto.
  QueryContext::predicate_id execution_predicate_index = QueryContext::kInvalidPredicateId;
  if (physical_plan->predicate()) {
    execution_predicate_index = query_context_proto_->predicates_size();

    unique_ptr<const Predicate> execution_predicate(convertPredicate(physical_plan->predicate()));
    query_context_proto_->add_predicates()->CopyFrom(execution_predicate->getProto());
  }

  // Convert assignment expressions as a UpdateGroup proto.
  const vector<E::AttributeReferencePtr> &assignees = physical_plan->assignees();
  const vector<E::ScalarPtr> &assignment_expressions = physical_plan->assignment_expressions();

  DCHECK_EQ(assignees.size(), assignment_expressions.size())
      << physical_plan->toString();

  const QueryContext::update_group_id update_group_index = query_context_proto_->update_groups_size();
  S::QueryContext::UpdateGroup *update_group_proto = query_context_proto_->add_update_groups();
  update_group_proto->set_relation_id(input_rel_id);

  for (vector<E::AttributeReferencePtr>::size_type i = 0; i < assignees.size(); ++i) {
    unique_ptr<const Scalar> attribute(
        assignees[i]->concretize(attribute_substitution_map_));
    DCHECK_EQ(Scalar::kAttribute, attribute->getDataSource())
        << assignees[i]->toString();

    S::QueryContext::UpdateGroup::UpdateAssignment *update_assignment_proto =
        update_group_proto->add_update_assignments();

    update_assignment_proto->set_attribute_id(
        static_cast<const ScalarAttribute*>(attribute.get())->getAttribute().getID());

    unique_ptr<const Scalar> value(
        assignment_expressions[i]->concretize(attribute_substitution_map_));
    update_assignment_proto->mutable_scalar()->CopyFrom(value->getProto());
  }

  const QueryPlan::DAGNodeIndex update_operator_index =
      execution_plan_->addRelationalOperator(new UpdateOperator(
          query_handle_->query_id(),
          *input_relation,
          relocation_destination_index,
          execution_predicate_index,
          update_group_index));
  relocation_destination_proto->set_relational_op_index(update_operator_index);

  const QueryPlan::DAGNodeIndex save_blocks_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator(query_handle_->query_id(),
                                 catalog_database_->getRelationByIdMutable(input_rel_id)));
  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(update_operator_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  execution_plan_->addDirectDependency(save_blocks_index,
                                       update_operator_index,
                                       false /* is_pipeline_breaker */);
}

void ExecutionGenerator::convertAggregate(
    const P::AggregatePtr &physical_plan) {
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  const CatalogRelation &input_relation = *input_relation_info->relation;
  const size_t num_partitions = input_relation.getNumPartitions();

  // Create aggr state proto.
  const QueryContext::aggregation_state_id aggr_state_index =
      query_context_proto_->aggregation_states_size();
  S::QueryContext::AggregationOperationStateContext *aggr_state_context_proto =
      query_context_proto_->add_aggregation_states();
  aggr_state_context_proto->set_num_partitions(num_partitions);

  S::AggregationOperationState *aggr_state_proto =
      aggr_state_context_proto->mutable_aggregation_state();
  aggr_state_proto->set_relation_id(input_relation.getID());

  bool use_parallel_initialization = false;

  std::vector<const Type*> group_by_types;
  std::vector<attribute_id> group_by_attrs;
  for (const E::NamedExpressionPtr &grouping_expression : physical_plan->grouping_expressions()) {
    unique_ptr<const Scalar> execution_group_by_expression;
    E::AliasPtr alias;
    if (E::SomeAlias::MatchesWithConditionalCast(grouping_expression, &alias)) {
      E::ScalarPtr scalar;
      // NOTE(zuyu): For aggregate expressions, all child expressions of an
      // Alias should be a Scalar.
      CHECK(E::SomeScalar::MatchesWithConditionalCast(alias->expression(), &scalar))
          << alias->toString();
      execution_group_by_expression.reset(scalar->concretize(attribute_substitution_map_));
    } else {
      execution_group_by_expression.reset(
          grouping_expression->concretize(attribute_substitution_map_));
    }
    aggr_state_proto->add_group_by_expressions()->MergeFrom(execution_group_by_expression->getProto());
    group_by_types.push_back(&execution_group_by_expression->getType());
    group_by_attrs.push_back(execution_group_by_expression->getAttributeIdForValueAccessor());
  }

  const auto &aggregate_expressions = physical_plan->aggregate_expressions();
  vector<bool> is_distincts;
  vector<pair<AggregationID, vector<const Type *>>> group_by_aggrs_info;
  for (const E::AliasPtr &named_aggregate_expression : aggregate_expressions) {
    const E::AggregateFunctionPtr unnamed_aggregate_expression =
        std::static_pointer_cast<const E::AggregateFunction>(named_aggregate_expression->expression());

    // Add a new entry in 'aggregates'.
    S::Aggregate *aggr_proto = aggr_state_proto->add_aggregates();

    // Set the AggregateFunction.
    const AggregateFunction &aggr_func = unnamed_aggregate_expression->getAggregate();
    aggr_proto->mutable_function()->MergeFrom(aggr_func.getProto());

    // Add each of the aggregate's arguments.
    vector<const Type *> argument_types;
    for (const E::ScalarPtr &argument : unnamed_aggregate_expression->getArguments()) {
      unique_ptr<const Scalar> concretized_argument(argument->concretize(attribute_substitution_map_));
      argument_types.push_back(&concretized_argument->getType());

      aggr_proto->add_argument()->MergeFrom(concretized_argument->getProto());
    }

    if (!group_by_types.empty()) {
      group_by_aggrs_info.emplace_back(aggr_func.getAggregationID(), move(argument_types));
    }

    // Set whether it is a DISTINCT aggregation.
    const bool is_distinct = unnamed_aggregate_expression->is_distinct();
    aggr_proto->set_is_distinct(is_distinct);
    is_distincts.push_back(is_distinct);

    // Add distinctify hash table impl type if it is a DISTINCT aggregation.
    if (unnamed_aggregate_expression->is_distinct()) {
      const std::vector<E::ScalarPtr> &arguments = unnamed_aggregate_expression->getArguments();
      DCHECK_GE(arguments.size(), 1u);
      // Right now only SeparateChaining implementation is supported.
      aggr_state_proto->add_distinctify_hash_table_impl_types(
          serialization::HashTableImplType::SEPARATE_CHAINING);
    }
  }

  bool aggr_state_is_partitioned = false;
  std::size_t aggr_state_num_partitions = 1u;
  if (!group_by_types.empty()) {
    const std::size_t estimated_num_groups =
        cost_model_for_aggregation_->estimateNumGroupsForAggregate(physical_plan);

    std::size_t max_num_groups;
    if (cost_model_for_aggregation_
            ->canUseCollisionFreeAggregation(physical_plan,
                                             estimated_num_groups,
                                             &max_num_groups)) {
      // First option: use array-based aggregation if applicable.
      aggr_state_proto->set_hash_table_impl_type(
          serialization::HashTableImplType::COLLISION_FREE_VECTOR);
      aggr_state_proto->set_estimated_num_entries(max_num_groups);
      use_parallel_initialization = true;
      aggr_state_num_partitions = CalculateNumFinalizationPartitionsForCollisionFreeVectorTable(max_num_groups);

      DCHECK(!group_by_aggrs_info.empty());
      CalculateCollisionFreeAggregationInfo(max_num_groups, group_by_aggrs_info,
                                            aggr_state_proto->mutable_collision_free_vector_info());
    } else {
      if (cost_model_for_aggregation_->canUseTwoPhaseCompactKeyAggregation(
              physical_plan, estimated_num_groups)) {
        // Second option: use thread-private compact-key aggregation if applicable.
        aggr_state_proto->set_hash_table_impl_type(
            serialization::HashTableImplType::THREAD_PRIVATE_COMPACT_KEY);
      } else {
        // Otherwise, use SeparateChaining.
        aggr_state_proto->set_hash_table_impl_type(
            serialization::HashTableImplType::SEPARATE_CHAINING);
        if (CheckAggregatePartitioned(aggregate_expressions.size(), is_distincts, group_by_attrs,
                                      estimated_num_groups)) {
          aggr_state_is_partitioned = true;
          aggr_state_num_partitions = FLAGS_num_aggregation_partitions;
        }
      }
      aggr_state_proto->set_estimated_num_entries(std::max(16uL, estimated_num_groups));
    }
  } else {
    aggr_state_proto->set_estimated_num_entries(1uL);
  }
  aggr_state_proto->set_is_partitioned(aggr_state_is_partitioned);
  aggr_state_proto->set_num_partitions(aggr_state_num_partitions);

  if (physical_plan->filter_predicate() != nullptr) {
    unique_ptr<const Predicate> predicate(convertPredicate(physical_plan->filter_predicate()));
    aggr_state_proto->mutable_predicate()->MergeFrom(predicate->getProto());
  }

  const QueryPlan::DAGNodeIndex aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new AggregationOperator(
              query_handle_->query_id(),
              input_relation,
              input_relation_info->isStoredRelation(),
              aggr_state_index,
              num_partitions));

  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(aggregation_operator_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }

  if (use_parallel_initialization) {
    DCHECK(aggr_state_proto->has_collision_free_vector_info());

    const QueryPlan::DAGNodeIndex initialize_aggregation_operator_index =
        execution_plan_->addRelationalOperator(
            new InitializeAggregationOperator(
                query_handle_->query_id(),
                aggr_state_index,
                num_partitions,
                aggr_state_proto->collision_free_vector_info().num_init_partitions()));

    execution_plan_->addDirectDependency(aggregation_operator_index,
                                         initialize_aggregation_operator_index,
                                         true /* is_pipeline_breaker */);
  }

  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_plan,
                                 &output_relation,
                                 insert_destination_proto);

  const QueryPlan::DAGNodeIndex finalize_aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new FinalizeAggregationOperator(query_handle_->query_id(),
                                          aggr_state_index,
                                          num_partitions,
                                          physical_plan->hasRepartition(),
                                          aggr_state_num_partitions,
                                          *output_relation,
                                          insert_destination_index));

  insert_destination_proto->set_relational_op_index(finalize_aggregation_operator_index);

  execution_plan_->addDirectDependency(finalize_aggregation_operator_index,
                                       aggregation_operator_index,
                                       true /* is_pipeline_breaker */);

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_plan),
      std::forward_as_tuple(finalize_aggregation_operator_index, output_relation));
  temporary_relation_info_vec_.emplace_back(finalize_aggregation_operator_index,
                                            output_relation);

  const QueryPlan::DAGNodeIndex destroy_aggregation_state_operator_index =
      execution_plan_->addRelationalOperator(
          new DestroyAggregationStateOperator(query_handle_->query_id(),
                                              aggr_state_index,
                                              num_partitions));

  execution_plan_->addDirectDependency(destroy_aggregation_state_operator_index,
                                       finalize_aggregation_operator_index,
                                       true);

  if (lip_filter_generator_ != nullptr) {
    lip_filter_generator_->addAggregateInfo(physical_plan,
                                            aggregation_operator_index);
  }
}

void ExecutionGenerator::convertCrossReferenceCoalesceAggregate(
    const P::CrossReferenceCoalesceAggregatePtr &physical_plan) {
  DCHECK_EQ(1u, physical_plan->left_join_attributes().size());
  DCHECK_EQ(1u, physical_plan->right_join_attributes().size());

  const CatalogRelationInfo *left_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->left_child());
  const CatalogRelationInfo *right_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->right_child());
  const CatalogRelation &right_relation = *right_relation_info->relation;

  // TODO(quickstep-team): Support partitioned aggregation.
  CHECK(!right_relation.hasPartitionScheme());
  const std::size_t num_partitions = 1u;

  // Create aggr state proto.
  const QueryContext::aggregation_state_id aggr_state_index =
      query_context_proto_->aggregation_states_size();
  S::QueryContext::AggregationOperationStateContext *aggr_state_context_proto =
      query_context_proto_->add_aggregation_states();
  aggr_state_context_proto->set_num_partitions(num_partitions);

  S::AggregationOperationState *aggr_state_proto =
      aggr_state_context_proto->mutable_aggregation_state();
  aggr_state_proto->set_relation_id(right_relation.getID());

  // Group by the right join attribute.
  std::unique_ptr<const Scalar> execution_group_by_expression(
      physical_plan->right_join_attributes().front()->concretize(
          attribute_substitution_map_));
  aggr_state_proto->add_group_by_expressions()->MergeFrom(
      execution_group_by_expression->getProto());

  aggr_state_proto->set_hash_table_impl_type(
      serialization::HashTableImplType::COLLISION_FREE_VECTOR);

  const size_t estimated_num_entries = physical_plan->group_by_key_value_range();
  aggr_state_proto->set_estimated_num_entries(estimated_num_entries);

  const size_t aggr_state_num_partitions =
      CalculateNumFinalizationPartitionsForCollisionFreeVectorTable(estimated_num_entries);
  aggr_state_proto->set_num_partitions(aggr_state_num_partitions);

  if (physical_plan->right_filter_predicate() != nullptr) {
    std::unique_ptr<const Predicate> predicate(
        convertPredicate(physical_plan->right_filter_predicate()));
    aggr_state_proto->mutable_predicate()->MergeFrom(predicate->getProto());
  }

  vector<pair<AggregationID, vector<const Type *>>> group_by_aggrs_info;
  for (const E::AliasPtr &named_aggregate_expression : physical_plan->aggregate_expressions()) {
    const E::AggregateFunctionPtr unnamed_aggregate_expression =
        std::static_pointer_cast<const E::AggregateFunction>(named_aggregate_expression->expression());

    // Add a new entry in 'aggregates'.
    S::Aggregate *aggr_proto = aggr_state_proto->add_aggregates();

    // Set the AggregateFunction.
    const AggregateFunction &aggr_func = unnamed_aggregate_expression->getAggregate();
    aggr_proto->mutable_function()->MergeFrom(aggr_func.getProto());

    // Add each of the aggregate's arguments.
    vector<const Type *> argument_types;
    for (const E::ScalarPtr &argument : unnamed_aggregate_expression->getArguments()) {
      unique_ptr<const Scalar> concretized_argument(argument->concretize(attribute_substitution_map_));
      argument_types.push_back(&concretized_argument->getType());

      aggr_proto->add_argument()->MergeFrom(concretized_argument->getProto());
    }

    group_by_aggrs_info.emplace_back(aggr_func.getAggregationID(), move(argument_types));

    // Set whether it is a DISTINCT aggregation.
    DCHECK(!unnamed_aggregate_expression->is_distinct());
    aggr_proto->set_is_distinct(false);
  }

  CalculateCollisionFreeAggregationInfo(estimated_num_entries, group_by_aggrs_info,
                                        aggr_state_proto->mutable_collision_free_vector_info());

  const QueryPlan::DAGNodeIndex initialize_aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new InitializeAggregationOperator(
              query_handle_->query_id(),
              aggr_state_index,
              num_partitions,
              aggr_state_proto->collision_free_vector_info().num_init_partitions()));

  const QueryPlan::DAGNodeIndex build_aggregation_existence_map_operator_index =
      execution_plan_->addRelationalOperator(
          new BuildAggregationExistenceMapOperator(
              query_handle_->query_id(),
              *left_relation_info->relation,
              physical_plan->left_join_attributes().front()->id(),
              left_relation_info->isStoredRelation(),
              aggr_state_index,
              num_partitions));

  if (!left_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(build_aggregation_existence_map_operator_index,
                                         left_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }

  const QueryPlan::DAGNodeIndex aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new AggregationOperator(
              query_handle_->query_id(),
              right_relation,
              right_relation_info->isStoredRelation(),
              aggr_state_index,
              num_partitions));

  if (!right_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(aggregation_operator_index,
                                         right_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }

  // Build aggregation existence map once initialization is done.
  execution_plan_->addDirectDependency(build_aggregation_existence_map_operator_index,
                                       initialize_aggregation_operator_index,
                                       true /* is_pipeline_breaker */);

  // Start aggregation after building existence map.
  execution_plan_->addDirectDependency(aggregation_operator_index,
                                       build_aggregation_existence_map_operator_index,
                                       true /* is_pipeline_breaker */);


  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_plan,
                                 &output_relation,
                                 insert_destination_proto);

  const QueryPlan::DAGNodeIndex finalize_aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new FinalizeAggregationOperator(query_handle_->query_id(),
                                          aggr_state_index,
                                          num_partitions,
                                          physical_plan->hasRepartition(),
                                          aggr_state_num_partitions,
                                          *output_relation,
                                          insert_destination_index));

  insert_destination_proto->set_relational_op_index(finalize_aggregation_operator_index);

  execution_plan_->addDirectDependency(finalize_aggregation_operator_index,
                                       aggregation_operator_index,
                                       true /* is_pipeline_breaker */);

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_plan),
      std::forward_as_tuple(finalize_aggregation_operator_index, output_relation));
  temporary_relation_info_vec_.emplace_back(finalize_aggregation_operator_index,
                                            output_relation);

  const QueryPlan::DAGNodeIndex destroy_aggregation_state_operator_index =
      execution_plan_->addRelationalOperator(
          new DestroyAggregationStateOperator(query_handle_->query_id(),
                                              aggr_state_index,
                                              num_partitions));

  execution_plan_->addDirectDependency(destroy_aggregation_state_operator_index,
                                       finalize_aggregation_operator_index,
                                       true);
}

void ExecutionGenerator::convertSort(const P::SortPtr &physical_sort) {
  // Create sort configuration for run generation.
  vector<bool> sort_ordering(physical_sort->sort_ascending());
  vector<bool> sort_null_ordering(physical_sort->nulls_first_flags());
  PtrVector<Scalar> sort_run_gen_attributes;
  for (const E::AttributeReferencePtr &sort_attribute :
       physical_sort->sort_attributes()) {
    sort_run_gen_attributes.push_back(
        sort_attribute->concretize(attribute_substitution_map_));
  }
  const SortConfiguration sort_run_gen_config(sort_run_gen_attributes,
                                              std::move(sort_ordering),
                                              std::move(sort_null_ordering));
  const QueryContext::sort_config_id sort_run_gen_config_id =
      query_context_proto_->sort_configs_size();
  S::SortConfiguration *sort_run_gen_config_proto =
      query_context_proto_->add_sort_configs();
  sort_run_gen_config_proto->CopyFrom(sort_run_gen_config.getProto());

  // Create SortRunGenerationOperator.
  const CatalogRelation *initial_runs_relation;
  const QueryContext::insert_destination_id initial_runs_destination_id =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *initial_runs_destination_proto =
      query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(
      physical_sort, &initial_runs_relation, initial_runs_destination_proto);

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_sort->input());
  const QueryPlan::DAGNodeIndex run_generator_index =
      execution_plan_->addRelationalOperator(new SortRunGenerationOperator(
          query_handle_->query_id(),
          *input_relation_info->relation,
          *initial_runs_relation,
          initial_runs_destination_id,
          sort_run_gen_config_id,
          input_relation_info->isStoredRelation()));
  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(run_generator_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
  }
  temporary_relation_info_vec_.emplace_back(run_generator_index,
                                            initial_runs_relation);
  initial_runs_destination_proto->set_relational_op_index(run_generator_index);

  // Create sort configuration for run merging.
  sort_ordering = physical_sort->sort_ascending();
  sort_null_ordering = physical_sort->nulls_first_flags();
  PtrVector<Scalar> sort_merge_run_attributes;
  for (const E::AttributeReferencePtr &sort_attribute :
       physical_sort->sort_attributes()) {
    sort_merge_run_attributes.push_back(
        sort_attribute->concretize(attribute_substitution_map_));
  }
  const SortConfiguration sort_merge_run_config(sort_merge_run_attributes,
                                                std::move(sort_ordering),
                                                std::move(sort_null_ordering));
  const QueryContext::sort_config_id sort_merge_run_config_id =
      query_context_proto_->sort_configs_size();
  S::SortConfiguration *sort_merge_run_config_proto =
      query_context_proto_->add_sort_configs();
  sort_merge_run_config_proto->CopyFrom(sort_merge_run_config.getProto());

  // Create SortMergeRunOperator.
  const CatalogRelation *merged_runs_relation;
  const QueryContext::insert_destination_id merged_runs_destination_id =
    query_context_proto_->insert_destinations_size();
  S::InsertDestination *merged_runs_destination_proto =
    query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_sort,
                                 &merged_runs_relation,
                                 merged_runs_destination_proto);
  const CatalogRelation *sorted_relation;
  const QueryContext::insert_destination_id sorted_output_destination_id =
    query_context_proto_->insert_destinations_size();
  S::InsertDestination *sorted_output_destination_proto =
    query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_sort,
                                 &sorted_relation,
                                 sorted_output_destination_proto);

  // TODO(qzeng): Make the merge factor configurable.
  const QueryPlan::DAGNodeIndex merge_run_operator_index =
      execution_plan_->addRelationalOperator(new SortMergeRunOperator(
          query_handle_->query_id(),
          *initial_runs_relation,
          *sorted_relation,
          sorted_output_destination_id,
          *merged_runs_relation,
          merged_runs_destination_id,
          sort_merge_run_config_id,
          64 /* merge_factor */,
          physical_sort->limit(),
          false /* input_relation_is_stored */));

  execution_plan_->addDirectDependency(merge_run_operator_index,
                                       run_generator_index,
                                       false /* is_pipeline_breaker */);
  merged_runs_destination_proto->set_relational_op_index(merge_run_operator_index);
  sorted_output_destination_proto->set_relational_op_index(merge_run_operator_index);

  // Do not add merged_runs_relation into 'temporary_relation_info_vec_'
  // and create the DropTableOperator for it at the end. Instead, add the drop
  // operator right here, because the relation won't be used by any other operator.
  const QueryPlan::DAGNodeIndex drop_merged_runs_index =
      execution_plan_->addRelationalOperator(
          new DropTableOperator(
              query_handle_->query_id(),
              *merged_runs_relation,
              catalog_database_,
              false /* only_drop_blocks */));
  execution_plan_->addDirectDependency(
      drop_merged_runs_index,
      merge_run_operator_index,
      true /* is_pipeline_breaker */);

  temporary_relation_info_vec_.emplace_back(merge_run_operator_index,
                                            sorted_relation);
  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_sort),
      std::forward_as_tuple(merge_run_operator_index,
                            sorted_relation));
}

void ExecutionGenerator::convertTableGenerator(
    const P::TableGeneratorPtr &physical_tablegen) {
  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto =
      query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_tablegen,
                                 &output_relation,
                                 insert_destination_proto);

  // Create GeneratorFunctionHandle proto
  const QueryContext::generator_function_id generator_function_index =
      query_context_proto_->generator_functions_size();
  query_context_proto_->add_generator_functions()->CopyFrom(
      physical_tablegen->generator_function_handle()->getProto());

  TableGeneratorOperator *op =
      new TableGeneratorOperator(query_handle_->query_id(),
                                 *output_relation,
                                 insert_destination_index,
                                 generator_function_index);

  const QueryPlan::DAGNodeIndex tablegen_index =
      execution_plan_->addRelationalOperator(op);
  insert_destination_proto->set_relational_op_index(tablegen_index);

  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_tablegen),
      std::forward_as_tuple(tablegen_index,
                            output_relation));
  temporary_relation_info_vec_.emplace_back(tablegen_index, output_relation);
}

void ExecutionGenerator::convertWindowAggregate(
    const P::WindowAggregatePtr &physical_plan) {
  // Create window_aggregation_operation_state proto.
  const QueryContext::window_aggregation_state_id window_aggr_state_index =
      query_context_proto_->window_aggregation_states_size();
  S::WindowAggregationOperationState *window_aggr_state_proto =
      query_context_proto_->add_window_aggregation_states();

  // Get input.
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  const CatalogRelation &input_relation = *input_relation_info->relation;
  DCHECK_EQ(1u, input_relation.getNumPartitions());
  window_aggr_state_proto->set_input_relation_id(input_relation.getID());

  // Get window aggregate function expression.
  const E::AliasPtr &named_window_aggregate_expression =
      physical_plan->window_aggregate_expression();
  const E::WindowAggregateFunctionPtr &window_aggregate_function =
      std::static_pointer_cast<const E::WindowAggregateFunction>(
          named_window_aggregate_expression->expression());

  // Set the WindowAggregateFunction.
  window_aggr_state_proto->mutable_function()->MergeFrom(
      window_aggregate_function->window_aggregate().getProto());

  // Set the arguments.
  for (const E::ScalarPtr &argument : window_aggregate_function->arguments()) {
    unique_ptr<const Scalar> concretized_argument(argument->concretize(attribute_substitution_map_));
    window_aggr_state_proto->add_arguments()->MergeFrom(concretized_argument->getProto());
  }

  // Set partition keys.
  const E::WindowInfo &window_info = window_aggregate_function->window_info();
  for (const E::ScalarPtr &partition_by_attribute
      : window_info.partition_by_attributes) {
    unique_ptr<const Scalar> concretized_partition_by_attribute(
        partition_by_attribute->concretize(attribute_substitution_map_));
    window_aggr_state_proto->add_partition_by_attributes()
        ->MergeFrom(concretized_partition_by_attribute->getProto());
  }

  // Set order keys.
  for (const E::ScalarPtr &order_by_attribute
      : window_info.order_by_attributes) {
    unique_ptr<const Scalar> concretized_order_by_attribute(
        order_by_attribute->concretize(attribute_substitution_map_));
    window_aggr_state_proto->add_order_by_attributes()
        ->MergeFrom(concretized_order_by_attribute->getProto());
  }

  // Set window frame info.
  if (window_info.frame_info == nullptr) {
    // If the frame is not specified, use the default setting:
    //   1. If ORDER BY key is specified, use cumulative aggregation:
    //      ROWS BETWEEN UNBOUNDED PRECEDING AND CURRENT ROW.
    //   2. If ORDER BY key is not specified either, use the whole partition:
    //      ROWS BETWEEN UNBOUNDED PRECEDING AND UNBOUNDED FOLLOWING.
    window_aggr_state_proto->set_is_row(true);  // frame mode: ROWS.
    window_aggr_state_proto->set_num_preceding(-1);  // UNBOUNDED PRECEDING.
    window_aggr_state_proto->set_num_following(
        window_info.order_by_attributes.empty()
            ? -1  // UNBOUNDED FOLLOWING.
            : 0);  // CURRENT ROW.
  } else {
    const E::WindowFrameInfo *window_frame_info = window_info.frame_info;
    window_aggr_state_proto->set_is_row(window_frame_info->is_row);
    window_aggr_state_proto->set_num_preceding(window_frame_info->num_preceding);
    window_aggr_state_proto->set_num_following(window_frame_info->num_following);
  }

  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_plan,
                                 &output_relation,
                                 insert_destination_proto);

  const QueryPlan::DAGNodeIndex window_aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new WindowAggregationOperator(query_handle_->query_id(),
                                        input_relation,
                                        *output_relation,
                                        window_aggr_state_index,
                                        insert_destination_index));

  // TODO(Shixuan): Once parallelism is introduced, the is_pipeline_breaker
  //                could be set to false.
  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(window_aggregation_operator_index,
                                         input_relation_info->producer_operator_index,
                                         true /* is_pipeline_breaker */);
  }

  insert_destination_proto->set_relational_op_index(window_aggregation_operator_index);

  // Add to map and temp_relation_info_vec.
  physical_to_output_relation_map_.emplace(
      std::piecewise_construct,
      std::forward_as_tuple(physical_plan),
      std::forward_as_tuple(window_aggregation_operator_index, output_relation));
  temporary_relation_info_vec_.emplace_back(window_aggregation_operator_index,
                                            output_relation);
}

}  // namespace optimizer
}  // namespace quickstep
