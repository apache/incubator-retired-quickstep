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

#include "query_optimizer/strategy/OneToOne.hpp"

#include <memory>
#include <vector>

#include "query_optimizer/LogicalToPhysicalMapper.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/logical/CopyFrom.hpp"
#include "query_optimizer/logical/CreateIndex.hpp"
#include "query_optimizer/logical/CreateTable.hpp"
#include "query_optimizer/logical/DeleteTuples.hpp"
#include "query_optimizer/logical/DropTable.hpp"
#include "query_optimizer/logical/InsertTuple.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "query_optimizer/logical/Sample.hpp"
#include "query_optimizer/logical/SharedSubplanReference.hpp"
#include "query_optimizer/logical/Sort.hpp"
#include "query_optimizer/logical/TableGenerator.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/logical/TopLevelPlan.hpp"
#include "query_optimizer/logical/UpdateTable.hpp"
#include "query_optimizer/physical/CopyFrom.hpp"
#include "query_optimizer/physical/CreateIndex.hpp"
#include "query_optimizer/physical/CreateTable.hpp"
#include "query_optimizer/physical/DeleteTuples.hpp"
#include "query_optimizer/physical/DropTable.hpp"
#include "query_optimizer/physical/InsertTuple.hpp"
#include "query_optimizer/physical/Sample.hpp"
#include "query_optimizer/physical/SharedSubplanReference.hpp"
#include "query_optimizer/physical/Sort.hpp"
#include "query_optimizer/physical/TableGenerator.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/physical/UpdateTable.hpp"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

bool OneToOne::generatePlan(const L::LogicalPtr &logical_input,
                            P::PhysicalPtr *physical_output) {
  switch (logical_input->getLogicalType()) {
    case L::LogicalType::kTopLevelPlan: {
      const L::TopLevelPlanPtr top_level_plan = std::static_pointer_cast<const L::TopLevelPlan>(logical_input);
      const P::PhysicalPtr main_physical_plan =
          physical_mapper_->createOrGetPhysicalFromLogical(top_level_plan->plan());
      std::vector<P::PhysicalPtr> shared_physical_subplans;
      for (const L::LogicalPtr &shared_logical_subplan : top_level_plan->shared_subplans()) {
        shared_physical_subplans.emplace_back(
            physical_mapper_->createOrGetPhysicalFromLogical(shared_logical_subplan));
      }
      *physical_output = P::TopLevelPlan::Create(main_physical_plan,
                                                 shared_physical_subplans);
      return true;
    }
    case L::LogicalType::kSharedSubplanReference: {
      const L::SharedSubplanReferencePtr shared_subplan_reference =
          std::static_pointer_cast<const L::SharedSubplanReference>(logical_input);
      *physical_output = P::SharedSubplanReference::Create(shared_subplan_reference->subplan_id(),
                                                           shared_subplan_reference->output_attributes());
      return true;
    }
    case L::LogicalType::kTableReference: {
      const L::TableReferencePtr table_reference =
          std::static_pointer_cast<const L::TableReference>(logical_input);
      *physical_output = P::TableReference::Create(table_reference->catalog_relation(),
                                                   table_reference->relation_alias(),
                                                   table_reference->attribute_list());
      return true;
    }
    case L::LogicalType::kCopyFrom: {
      const L::CopyFromPtr copy_from =
          std::static_pointer_cast<const L::CopyFrom>(logical_input);
      *physical_output = P::CopyFrom::Create(
          copy_from->catalog_relation(), copy_from->file_name(),
          copy_from->column_delimiter(), copy_from->escape_strings());
      return true;
    }
    case L::LogicalType::kCreateIndex: {
      const L::CreateIndexPtr create_index =
          std::static_pointer_cast<const L::CreateIndex>(logical_input);
      *physical_output = P::CreateIndex::Create(
          physical_mapper_->createOrGetPhysicalFromLogical(create_index->input()), create_index->index_name());
      return true;
    }
    case L::LogicalType::kCreateTable: {
      const L::CreateTablePtr create_table =
          std::static_pointer_cast<const L::CreateTable>(logical_input);
      *physical_output = P::CreateTable::Create(create_table->relation_name(),
                                                create_table->attributes(),
                                                create_table->block_properties());
      return true;
    }
    case L::LogicalType::kDeleteTuples: {
      const L::DeleteTuplesPtr delete_tuples =
          std::static_pointer_cast<const L::DeleteTuples>(logical_input);
      *physical_output = P::DeleteTuples::Create(
          physical_mapper_->createOrGetPhysicalFromLogical(delete_tuples->input()),
          delete_tuples->predicate());
      return true;
    }
    case L::LogicalType::kDropTable: {
      const L::DropTablePtr drop_table =
          std::static_pointer_cast<const L::DropTable>(logical_input);
      *physical_output = P::DropTable::Create(drop_table->catalog_relation());
      return true;
    }
    case L::LogicalType::kInsertTuple: {
      const L::InsertTuplePtr insert_tuple =
          std::static_pointer_cast<const L::InsertTuple>(logical_input);
      *physical_output = P::InsertTuple::Create(
          physical_mapper_->createOrGetPhysicalFromLogical(insert_tuple->input()),
          insert_tuple->column_values());
      return true;
    }
    case L::LogicalType::kSample: {
      const L::SamplePtr sample =
          std::static_pointer_cast<const L::Sample>(logical_input);
      *physical_output = P::Sample::Create(
          physical_mapper_->createOrGetPhysicalFromLogical(sample->input()),
          sample->is_block_sample(),
          sample->percentage());
      return true;
    }
    case L::LogicalType::kSort: {
      const L::Sort *sort =
          static_cast<const L::Sort*>(logical_input.get());

      const P::PhysicalPtr physical_input =
          physical_mapper_->createOrGetPhysicalFromLogical(sort->input());

      // Find non-sort attributes.
      const std::vector<E::AttributeReferencePtr> input_attributes =
          physical_input->getOutputAttributes();
      E::UnorderedNamedExpressionSet sort_attributes_set(sort->sort_attributes().begin(),
                                                         sort->sort_attributes().end());
      std::vector<E::AttributeReferencePtr> non_sort_attributes;
      for (const E::AttributeReferencePtr &input_attribute : input_attributes) {
        if (sort_attributes_set.find(input_attribute) == sort_attributes_set.end()) {
          non_sort_attributes.emplace_back(input_attribute);
        }
      }

      *physical_output = P::Sort::Create(
          physical_mapper_->createOrGetPhysicalFromLogical(sort->input()),
          sort->sort_attributes(),
          non_sort_attributes,
          sort->sort_ascending(),
          sort->nulls_first_flags(),
          sort->limit());
      return true;
    }
    case L::LogicalType::kTableGenerator: {
      const L::TableGeneratorPtr table_generator =
          std::static_pointer_cast<const L::TableGenerator>(logical_input);
      *physical_output = P::TableGenerator::Create(
          table_generator->generator_function_handle(),
          table_generator->table_alias(),
          table_generator->attribute_list());
      return true;
    }
    case L::LogicalType::kUpdateTable: {
      const L::UpdateTablePtr update_table =
          std::static_pointer_cast<const L::UpdateTable>(logical_input);
      *physical_output = P::UpdateTable::Create(
          physical_mapper_->createOrGetPhysicalFromLogical(update_table->input()),
          update_table->assignees(),
          update_table->assignment_expressions(),
          update_table->predicate());
      return true;
    }
    default:
      return false;
  }
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
