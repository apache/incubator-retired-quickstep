/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "query_optimizer/ExecutionGenerator.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunction.pb.h"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryContext.pb.h"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/cost_model/SimpleCostModel.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/physical/CopyFrom.hpp"
#include "query_optimizer/physical/CreateIndex.hpp"
#include "query_optimizer/physical/CreateTable.hpp"
#include "query_optimizer/physical/DeleteTuples.hpp"
#include "query_optimizer/physical/DropTable.hpp"
#include "query_optimizer/physical/HashJoin.hpp"
#include "query_optimizer/physical/InsertTuple.hpp"
#include "query_optimizer/physical/NestedLoopsJoin.hpp"
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
#include "query_optimizer/physical/UpdateTable.hpp"
#include "relational_operators/AggregationOperator.hpp"
#include "relational_operators/BuildHashOperator.hpp"
#include "relational_operators/CreateIndexOperator.hpp"
#include "relational_operators/CreateTableOperator.hpp"
#include "relational_operators/DeleteOperator.hpp"
#include "relational_operators/DestroyHashOperator.hpp"
#include "relational_operators/DropTableOperator.hpp"
#include "relational_operators/FinalizeAggregationOperator.hpp"
#include "relational_operators/HashJoinOperator.hpp"
#include "relational_operators/InsertOperator.hpp"
#include "relational_operators/NestedLoopsJoinOperator.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/SampleOperator.hpp"
#include "relational_operators/SaveBlocksOperator.hpp"
#include "relational_operators/SelectOperator.hpp"
#include "relational_operators/SortMergeRunOperator.hpp"
#include "relational_operators/SortRunGenerationOperator.hpp"
#include "relational_operators/TableGeneratorOperator.hpp"
#include "relational_operators/TextScanOperator.hpp"
#include "relational_operators/UpdateOperator.hpp"
#include "storage/AggregationOperationState.pb.h"
#include "storage/HashTable.pb.h"
#include "storage/HashTableFactory.hpp"
#include "storage/InsertDestination.pb.h"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypedValue.hpp"
#include "types/TypedValue.pb.h"
#include "types/containers/Tuple.pb.h"
#include "utility/SqlError.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

using std::move;
using std::static_pointer_cast;
using std::unique_ptr;
using std::unordered_map;
using std::vector;

namespace quickstep {
namespace optimizer {

DEFINE_string(join_hashtable_type, "SeparateChaining",
              "HashTable implementation to use for hash joins (valid options "
              "are SeparateChaining or LinearOpenAddressing)");
static const bool join_hashtable_type_dummy
    = gflags::RegisterFlagValidator(&FLAGS_join_hashtable_type,
                                    &ValidateHashTableImplTypeString);

DEFINE_string(aggregate_hashtable_type, "LinearOpenAddressing",
              "HashTable implementation to use for aggregates with GROUP BY "
              "(valid options are SeparateChaining or LinearOpenAddressing)");
static const bool aggregate_hashtable_type_dummy
    = gflags::RegisterFlagValidator(&FLAGS_aggregate_hashtable_type,
                                    &ValidateHashTableImplTypeString);

DEFINE_bool(parallelize_load, false, "Parallelize loading data files.");

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;
namespace S = ::quickstep::serialization;

constexpr QueryPlan::DAGNodeIndex ExecutionGenerator::CatalogRelationInfo::kInvalidOperatorIndex;

void ExecutionGenerator::generatePlan(const P::PhysicalPtr &physical_plan) {
  CHECK(P::SomeTopLevelPlan::MatchesWithConditionalCast(physical_plan, &top_level_physical_plan_))
      << "The physical plan must be rooted by a TopLevelPlan";

  const CatalogRelation *result_relation = nullptr;

  try {
    for (const P::PhysicalPtr &shared_subplan : top_level_physical_plan_->shared_subplans()) {
      generatePlanInternal(shared_subplan);
    }
    generatePlanInternal(top_level_physical_plan_->plan());

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
          optimizer_context_->catalog_database()->getRelationByIdMutable(result_relation->getID()));
      continue;
    }
    const QueryPlan::DAGNodeIndex drop_table_index =
        execution_plan_->addRelationalOperator(
            new DropTableOperator(*temporary_relation,
                                  optimizer_context_->catalog_database(),
                                  false /* only_drop_blocks */));
    DCHECK(!temporary_relation_info.isStoredRelation());
    execution_plan_->addDependenciesForDropOperator(
        drop_table_index,
        temporary_relation_info.producer_operator_index);
  }
}

void ExecutionGenerator::generatePlanInternal(
    const P::PhysicalPtr &physical_plan) {
  // Generate the execution plan in bottom-up.
  for (const P::PhysicalPtr &child : physical_plan->children()) {
    generatePlanInternal(child);
  }

  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kAggregate:
      return convertAggregate(
          std::static_pointer_cast<const P::Aggregate>(physical_plan));
    case P::PhysicalType::kCopyFrom:
      return convertCopyFrom(
          std::static_pointer_cast<const P::CopyFrom>(physical_plan));
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
    case P::PhysicalType::kHashJoin:
      return convertHashJoin(
          std::static_pointer_cast<const P::HashJoin>(physical_plan));
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
    case P::PhysicalType::kUpdateTable:
      return convertUpdateTable(
          std::static_pointer_cast<const P::UpdateTable>(physical_plan));
    default:
      LOG(FATAL) << "Unknown physical plan node "
                 << physical_plan->getShortString();
  }
}

std::string ExecutionGenerator::getNewRelationName() {
  std::ostringstream out;
  out << OptimizerContext::kInternalTemporaryRelationNamePrefix
      << optimizer_context_->query_id() << "_" << rel_id_;
  ++rel_id_;
  return out.str();
}

void ExecutionGenerator::createTemporaryCatalogRelation(
    const P::PhysicalPtr &physical,
    const CatalogRelation **catalog_relation_output,
    S::InsertDestination *insert_destination_proto) {
  std::unique_ptr<CatalogRelation> catalog_relation(
      new CatalogRelation(optimizer_context_->catalog_database(),
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

  *catalog_relation_output = catalog_relation.get();
  const relation_id output_rel_id = optimizer_context_->catalog_database()->addRelation(
      catalog_relation.release());

  insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_rel_id);
}

void ExecutionGenerator::dropAllTemporaryRelations() {
  CatalogDatabase *catalog_database = optimizer_context_->catalog_database();
  for (const CatalogRelationInfo &temporary_relation_info :
       temporary_relation_info_vec_) {
    DCHECK_EQ(temporary_relation_info.relation->size_blocks(), 0u);
    catalog_database->dropRelationById(temporary_relation_info.relation->getID());
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

  SampleOperator *sample_op = new SampleOperator(*input_relation_info->relation,
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
  // Check if the Selection is only for renaming columns.
  if (physical_selection->filter_predicate() == nullptr) {
    const std::vector<E::AttributeReferencePtr> input_attributes =
        physical_selection->input()->getOutputAttributes();
    const std::vector<E::NamedExpressionPtr> &project_expressions =
        physical_selection->project_expressions();
    if (project_expressions.size() == input_attributes.size()) {
      bool has_different_attrs = false;
      for (std::size_t attr_idx = 0; attr_idx < input_attributes.size(); ++attr_idx) {
        if (project_expressions[attr_idx]->id() != input_attributes[attr_idx]->id()) {
          has_different_attrs = true;
          break;
        }
      }
      if (!has_different_attrs) {
        const std::unordered_map<P::PhysicalPtr, CatalogRelationInfo>::const_iterator input_catalog_rel_it =
            physical_to_output_relation_map_.find(physical_selection->input());
        DCHECK(input_catalog_rel_it != physical_to_output_relation_map_.end());
        if (!input_catalog_rel_it->second.isStoredRelation()) {
          CatalogRelation *catalog_relation =
              const_cast<CatalogRelation*>(input_catalog_rel_it->second.relation);
          for (std::size_t attr_idx = 0; attr_idx < project_expressions.size(); ++attr_idx) {
            CatalogAttribute *catalog_attribute =
                catalog_relation->getAttributeByIdMutable(attr_idx);
            DCHECK(catalog_attribute != nullptr);
            catalog_attribute->setDisplayName(
                project_expressions[attr_idx]->attribute_alias());
          }
          physical_to_output_relation_map_.emplace(physical_selection,
                                                   input_catalog_rel_it->second);
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
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_selection->input());
  DCHECK(input_relation_info != nullptr);

  // Use the "simple" form of the selection operator (a pure projection that
  // doesn't require any expression evaluation or intermediate copies) if
  // possible.
  std::unique_ptr<std::vector<attribute_id>> attributes(new std::vector<attribute_id>());
  SelectOperator *op
      = convertSimpleProjection(project_expressions_group_index, attributes.get())
        ? new SelectOperator(*input_relation_info->relation,
                             *output_relation,
                             insert_destination_index,
                             execution_predicate_index,
                             attributes.release(),
                             input_relation_info->isStoredRelation())
        : new SelectOperator(*input_relation_info->relation,
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
}

void ExecutionGenerator::convertSharedSubplanReference(const physical::SharedSubplanReferencePtr &physical_plan) {
  const std::unordered_map<physical::PhysicalPtr, CatalogRelationInfo>::const_iterator found_it =
      physical_to_output_relation_map_.find(
          top_level_physical_plan_->shared_subplan_at(physical_plan->subplan_id()));
  if (found_it != physical_to_output_relation_map_.end()) {
    physical_to_output_relation_map_.emplace(physical_plan, found_it->second);
  }
}

void ExecutionGenerator::convertHashJoin(const P::HashJoinPtr &physical_plan) {
  // HashJoin is converted to three operators:
  //     BuildHash, HashJoin, DestroyHash. The second is the primary operator.

  P::PhysicalPtr probe_physical = physical_plan->left();
  P::PhysicalPtr build_physical = physical_plan->right();

  std::vector<attribute_id> probe_attribute_ids;
  std::vector<attribute_id> build_attribute_ids;

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

  // Choose the smaller table as the inner build table,
  // and the other one as the outer probe table.
  std::size_t probe_cardinality = cost_model_->estimateCardinality(probe_physical);
  std::size_t build_cardinality = cost_model_->estimateCardinality(build_physical);
  if (probe_cardinality < build_cardinality) {
    // Switch the probe and build physical nodes.
    std::swap(probe_physical, build_physical);
    std::swap(probe_cardinality, build_cardinality);
    std::swap(probe_attribute_ids, build_attribute_ids);
    std::swap(any_probe_attributes_nullable, any_build_attributes_nullable);
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
  const CatalogRelationInfo *probe_operator_info =
      findRelationInfoOutputByPhysical(probe_physical);

  // FIXME(quickstep-team): Add support for self-join.
  if (build_relation_info->relation == probe_operator_info->relation) {
    THROW_SQL_ERROR() << "Self-join is not supported";
  }

  // Create join hash table proto.
  const QueryContext::join_hash_table_id join_hash_table_index =
      query_context_proto_->join_hash_tables_size();
  S::HashTable *hash_table_proto = query_context_proto_->add_join_hash_tables();

  // SimplifyHashTableImplTypeProto() switches the hash table implementation
  // from SeparateChaining to SimpleScalarSeparateChaining when there is a
  // single scalar key type with a reversible hash function.
  hash_table_proto->set_hash_table_impl_type(
      SimplifyHashTableImplTypeProto(
          HashTableImplTypeProtoFromString(FLAGS_join_hashtable_type),
          key_types));

  const CatalogRelationSchema *build_relation = build_relation_info->relation;
  for (const attribute_id build_attribute : build_attribute_ids) {
    hash_table_proto->add_key_types()->CopyFrom(
        build_relation->getAttributeById(build_attribute)->getType().getProto());
  }

  hash_table_proto->set_estimated_num_entries(build_cardinality);

  // Create three operators.
  const QueryPlan::DAGNodeIndex build_operator_index =
      execution_plan_->addRelationalOperator(
          new BuildHashOperator(
              *build_relation_info->relation,
              build_relation_info->isStoredRelation(),
              build_attribute_ids,
              any_build_attributes_nullable,
              join_hash_table_index));

  // Create InsertDestination proto.
  const CatalogRelation *output_relation = nullptr;
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();
  createTemporaryCatalogRelation(physical_plan,
                                 &output_relation,
                                 insert_destination_proto);

  const QueryPlan::DAGNodeIndex join_operator_index =
      execution_plan_->addRelationalOperator(
          new HashJoinOperator(
              *build_relation_info->relation,
              *probe_operator_info->relation,
              probe_operator_info->isStoredRelation(),
              probe_attribute_ids,
              any_probe_attributes_nullable,
              *output_relation,
              insert_destination_index,
              join_hash_table_index,
              residual_predicate_index,
              project_expressions_group_index));
  insert_destination_proto->set_relational_op_index(join_operator_index);

  const QueryPlan::DAGNodeIndex destroy_operator_index =
      execution_plan_->addRelationalOperator(
          new DestroyHashOperator(join_hash_table_index));

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
  if (!probe_operator_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(join_operator_index,
                                         probe_operator_info->producer_operator_index,
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
  const CatalogRelationInfo *right_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->right());

  // FIXME(quickstep-team): Add support for self-join.
  if (left_relation_info->relation == right_relation_info->relation) {
    THROW_SQL_ERROR() << "NestedLoopsJoin does not support self-join yet";
  }

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
      execution_plan_->addRelationalOperator(new NestedLoopsJoinOperator(
          *left_relation_info->relation,
          *right_relation_info->relation,
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

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(output_relation->getID());

  const vector<block_id> blocks(physical_plan->catalog_relation()->getBlocksSnapshot());
  for (const block_id block : blocks) {
    insert_destination_proto->AddExtension(S::BlockPoolInsertDestination::blocks, block);
  }

  const QueryPlan::DAGNodeIndex scan_operator_index =
      execution_plan_->addRelationalOperator(
          new TextScanOperator(
              physical_plan->file_name(),
              physical_plan->column_delimiter(),
              physical_plan->escape_strings(),
              FLAGS_parallelize_load,
              *output_relation,
              insert_destination_index));
  insert_destination_proto->set_relational_op_index(scan_operator_index);

  const QueryPlan::DAGNodeIndex save_blocks_operator_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator());
  execution_plan_->addDirectDependency(save_blocks_operator_index,
                                       scan_operator_index,
                                       false /* is_pipeline_breaker */);
}


void ExecutionGenerator::convertCreateIndex(
  const P::CreateIndexPtr &physical_plan) {
  // CreateIndex is converted to a CreateIndex operator.
  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  CatalogRelation *input_relation =
      optimizer_context_->catalog_database()->getRelationByIdMutable(
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

  // Corresponding to each attribute, create a copy of index description.
  std::vector<IndexSubBlockDescription> index_descriptions;
  for (const CatalogAttribute* catalog_attribute : index_attributes) {
    IndexSubBlockDescription index_description(*physical_plan->index_description());
    index_description.add_indexed_attribute_ids(catalog_attribute->getID());
    if (input_relation->hasIndexWithDescription(index_description)) {
      // Check if the given index description already exists in the relation.
      THROW_SQL_ERROR() << "The relation " << input_relation->getName()
          << " already defines this index on "<< catalog_attribute->getName();
    } else if (!index_description.IsInitialized()) {
      // Check if the given index description is valid.
      THROW_SQL_ERROR() << "The index with given properties cannot be created.";
    } else {
      index_descriptions.push_back(std::move(index_description));
    }
  }

  execution_plan_->addRelationalOperator(new CreateIndexOperator(input_relation,
                                                                 optimizer_context_->catalog_database(),
                                                                 physical_plan->index_name(),
                                                                 index_descriptions));
}

void ExecutionGenerator::convertCreateTable(
    const P::CreateTablePtr &physical_plan) {
  // CreateTable is converted to a CreateTable operator.

  std::unique_ptr<CatalogRelation> catalog_relation(new CatalogRelation(
      optimizer_context_->catalog_database(),
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

  execution_plan_->addRelationalOperator(
      new CreateTableOperator(catalog_relation.release(),
                              optimizer_context_->catalog_database()));
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
  if (execution_predicate == nullptr ||
      (execution_predicate->hasStaticResult() &&
       execution_predicate->getStaticResult())) {
    const QueryPlan::DAGNodeIndex drop_table_index =
        execution_plan_->addRelationalOperator(
            new DropTableOperator(*input_relation_info->relation,
                                  optimizer_context_->catalog_database(),
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
        execution_plan_->addRelationalOperator(new DeleteOperator(
            *input_relation_info->relation,
            execution_predicate_index,
            input_relation_info->isStoredRelation()));
    if (!input_relation_info->isStoredRelation()) {
      execution_plan_->addDirectDependency(delete_tuples_index,
                                           input_relation_info->producer_operator_index,
                                           false /* is_pipeline_breaker */);
    }

    const QueryPlan::DAGNodeIndex save_blocks_index =
        execution_plan_->addRelationalOperator(
            new SaveBlocksOperator());
    execution_plan_->addDirectDependency(save_blocks_index,
                                         delete_tuples_index,
                                         false /* is_pipeline_breaker */);
  }
}

void ExecutionGenerator::convertDropTable(
    const P::DropTablePtr &physical_plan) {
  // DropTable is converted to a DropTable operator.
  execution_plan_->addRelationalOperator(
      new DropTableOperator(*physical_plan->catalog_relation(),
                            optimizer_context_->catalog_database()));
}

void ExecutionGenerator::convertInsertTuple(
    const P::InsertTuplePtr &physical_plan) {
  // InsertTuple is converted to an Insert and a SaveBlocks.

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  CatalogRelation *input_relation =
      optimizer_context_->catalog_database()->getRelationByIdMutable(
          input_relation_info->relation->getID());

  // Construct the tuple proto to be inserted.
  const QueryContext::tuple_id tuple_index = query_context_proto_->tuples_size();

  S::Tuple *tuple_proto = query_context_proto_->add_tuples();
  for (const E::ScalarLiteralPtr &literal : physical_plan->column_values()) {
    tuple_proto->add_attribute_values()->CopyFrom(literal->value().getProto());
  }

  // FIXME(qzeng): A better way is using a traits struct to look up whether a storage
  //               block supports ad-hoc insertion instead of hard-coding the block types.
  const StorageBlockLayout &storage_block_layout =
      input_relation->getDefaultStorageBlockLayout();
  if (storage_block_layout.getDescription().tuple_store_description().sub_block_type() ==
      TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE ||
      storage_block_layout.getDescription().tuple_store_description().sub_block_type() ==
            TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE) {
    THROW_SQL_ERROR() << "INSERT statement is not supported for the relation "
                      << input_relation->getName()
                      << ", because its storage blocks do not support ad-hoc insertion";
  }

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id insert_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *insert_destination_proto = query_context_proto_->add_insert_destinations();

  insert_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);
  insert_destination_proto->set_relation_id(input_relation->getID());

  const vector<block_id> blocks(input_relation->getBlocksSnapshot());
  for (const block_id block : blocks) {
    insert_destination_proto->AddExtension(S::BlockPoolInsertDestination::blocks, block);
  }


  const QueryPlan::DAGNodeIndex insert_operator_index =
      execution_plan_->addRelationalOperator(
          new InsertOperator(*input_relation,
                             insert_destination_index,
                             tuple_index));
  insert_destination_proto->set_relational_op_index(insert_operator_index);

  const QueryPlan::DAGNodeIndex save_blocks_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator());
  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(insert_operator_index,
                                         input_relation_info->producer_operator_index,
                                         true /* is_pipeline_breaker */);
  }
  execution_plan_->addDirectDependency(save_blocks_index,
                                       insert_operator_index,
                                       false /* is_pipeline_breaker */);
}

void ExecutionGenerator::convertUpdateTable(
    const P::UpdateTablePtr &physical_plan) {
  // UpdateTable is converted to an Update and a SaveBlocks.

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  DCHECK(input_relation_info != nullptr);

  const relation_id input_rel_id = input_relation_info->relation->getID();

  // Create InsertDestination proto.
  const QueryContext::insert_destination_id relocation_destination_index =
      query_context_proto_->insert_destinations_size();
  S::InsertDestination *relocation_destination_proto = query_context_proto_->add_insert_destinations();

  relocation_destination_proto->set_insert_destination_type(S::InsertDestinationType::BLOCK_POOL);
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
      execution_plan_->addRelationalOperator(
          new UpdateOperator(
              *optimizer_context_->catalog_database()->getRelationByIdMutable(input_rel_id),
              relocation_destination_index,
              execution_predicate_index,
              update_group_index));
  relocation_destination_proto->set_relational_op_index(update_operator_index);

  const QueryPlan::DAGNodeIndex save_blocks_index =
      execution_plan_->addRelationalOperator(
          new SaveBlocksOperator());
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
  // TODO(quickstep-team): The AggregationOperator and FinalizeAggregationOperator
  //                       should be able to handle GROUP BY without aggregate expressions,
  //                       or we may implement a DISTINCT operator.
  if (physical_plan->aggregate_expressions().empty()) {
    THROW_SQL_ERROR() << "GROUP BY without any aggregate expression is not supported yet";
  }

  // Create aggr state proto.
  const QueryContext::aggregation_state_id aggr_state_index =
      query_context_proto_->aggregation_states_size();
  S::AggregationOperationState *aggr_state_proto = query_context_proto_->add_aggregation_states();

  const CatalogRelationInfo *input_relation_info =
      findRelationInfoOutputByPhysical(physical_plan->input());
  aggr_state_proto->set_relation_id(input_relation_info->relation->getID());

  for (const E::AliasPtr &named_aggregate_expression : physical_plan->aggregate_expressions()) {
    const E::AggregateFunctionPtr unnamed_aggregate_expression =
        std::static_pointer_cast<const E::AggregateFunction>(named_aggregate_expression->expression());

    // Add a new entry in 'aggregates'.
    S::Aggregate *aggr_proto = aggr_state_proto->add_aggregates();

    // Set the AggregateFunction.
    aggr_proto->mutable_function()->CopyFrom(
        unnamed_aggregate_expression->getAggregate().getProto());

    // Add each of the aggregate's arguments.
    for (const E::ScalarPtr &argument : unnamed_aggregate_expression->getArguments()) {
      unique_ptr<const Scalar> concretized_argument(argument->concretize(attribute_substitution_map_));
      aggr_proto->add_argument()->CopyFrom(concretized_argument->getProto());
    }
  }

  std::vector<const Type*> group_by_types;
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
    aggr_state_proto->add_group_by_expressions()->CopyFrom(execution_group_by_expression->getProto());
    group_by_types.push_back(&execution_group_by_expression->getType());
  }

  if (physical_plan->filter_predicate() != nullptr) {
    unique_ptr<const Predicate> predicate(convertPredicate(physical_plan->filter_predicate()));
    aggr_state_proto->mutable_predicate()->CopyFrom(predicate->getProto());
  }

  aggr_state_proto->set_estimated_num_entries(cost_model_->estimateCardinality(physical_plan));

  if (!group_by_types.empty()) {
    // SimplifyHashTableImplTypeProto() switches the hash table implementation
    // from SeparateChaining to SimpleScalarSeparateChaining when there is a
    // single scalar key type with a reversible hash function.
    aggr_state_proto->set_hash_table_impl_type(
        SimplifyHashTableImplTypeProto(
            HashTableImplTypeProtoFromString(FLAGS_aggregate_hashtable_type),
            group_by_types));
  }

  const QueryPlan::DAGNodeIndex aggregation_operator_index =
      execution_plan_->addRelationalOperator(
          new AggregationOperator(
              *input_relation_info->relation,
              input_relation_info->isStoredRelation(),
              aggr_state_index));
  if (!input_relation_info->isStoredRelation()) {
    execution_plan_->addDirectDependency(aggregation_operator_index,
                                         input_relation_info->producer_operator_index,
                                         false /* is_pipeline_breaker */);
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
          new FinalizeAggregationOperator(aggr_state_index,
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
      execution_plan_->addRelationalOperator(
          new SortRunGenerationOperator(*input_relation_info->relation,
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
      execution_plan_->addRelationalOperator(
          new SortMergeRunOperator(*initial_runs_relation,
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
              *merged_runs_relation,
              optimizer_context_->catalog_database(),
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
      new TableGeneratorOperator(*output_relation,
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

}  // namespace optimizer
}  // namespace quickstep
