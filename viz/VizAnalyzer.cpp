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

#include "viz/VizAnalyzer.hpp"

#include <cstddef>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/UnaryExpression.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/Join.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/operations/unary_operations/DateExtractOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationID.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace viz {

namespace E = ::quickstep::optimizer::expressions;
namespace P = ::quickstep::optimizer::physical;

VizAnalyzer::VizAnalyzer(const optimizer::physical::PhysicalPtr &physical_plan,
                         const QueryPlan *execution_plan,
                         const std::vector<WorkOrderTimeEntry> *profiling_stats,
                         const CatalogRelation *query_result_relation,
                         const CatalogDatabase *catalog_database,
                         StorageManager *storage_manager)
    : physical_plan_(physical_plan),
      execution_plan_(execution_plan),
      profiling_stats_(profiling_stats),
      query_result_relation_(query_result_relation),
      catalog_database_(catalog_database),
      storage_manager_(storage_manager) {
  const std::vector<E::AttributeReferencePtr> physical_attrs =
      physical_plan->getOutputAttributes();
  std::size_t i = 0;
  for (const CatalogAttribute &catalog_attr : *query_result_relation) {
    CHECK_LT(i, physical_attrs.size());
    const E::AttributeReferencePtr &physical_attr = physical_attrs[i++];
    physical_to_catalog_attribute_map.emplace(physical_attr->id(), &catalog_attr);
    catalog_to_physical_attribute_map.emplace(catalog_attr.getID(), physical_attr);
  }
}

bool VizAnalyzer::findGroupByAttributes(
    std::vector<attribute_id> *group_by_attribute_ids) const {
  std::vector<E::AttributeReferencePtr> group_by_attributes;
  if (findGroupByAttributesInternal(physical_plan_, &group_by_attributes)) {
    CHECK(!group_by_attributes.empty());

    std::unordered_map<E::ExprId, attribute_id> attr_id_map;
    const std::vector<E::AttributeReferencePtr> query_result_attributes =
        physical_plan_->getOutputAttributes();
    for (std::size_t i = 0; i < query_result_attributes.size(); ++i) {
      attr_id_map.emplace(query_result_attributes[i]->id(), i);
    }

    for (const auto &attr : group_by_attributes) {
      const auto attr_it = attr_id_map.find(attr->id());
      CHECK(attr_it != attr_id_map.end());
      group_by_attribute_ids->emplace_back(attr_it->second);
    }
    return true;
  }
  return false;
}

bool VizAnalyzer::findGroupByAttributesInternal(
    const P::PhysicalPtr &physical_plan,
    std::vector<E::AttributeReferencePtr> *group_by_attributes) const {
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kTopLevelPlan:  // Fall through
    case P::PhysicalType::kSort:
    case P::PhysicalType::kSelection: {
      if (!findGroupByAttributesInternal(physical_plan->children().at(0),
                                         group_by_attributes)) {
        return false;
      }
      if (!E::SubsetOfExpressions(*group_by_attributes,
                                  physical_plan->getOutputAttributes())) {
        return false;
      }
      return true;
    }
    case P::PhysicalType::kAggregate: {
      const P::AggregatePtr &aggregate =
          std::static_pointer_cast<const P::Aggregate>(physical_plan);
      if (aggregate->grouping_expressions().empty()) {
        return false;
      }
      *group_by_attributes = E::ToRefVector(aggregate->grouping_expressions());
      return true;
    }
    default:
      return false;
  }
}

bool VizAnalyzer::isTime(const attribute_id attr_id,
                         std::string *format) const {
  const E::AttributeReferencePtr &attr =
      catalog_to_physical_attribute_map.at(attr_id);
  switch (attr->getValueType().getTypeID()) {
    case TypeID::kDate:
      *format = "%Y-%m-%d";
      return true;
    case TypeID::kDatetime:
      // TODO
      return false;
    case TypeID::kInt:  // Fall through
    case TypeID::kLong: {
      const E::ExprId expr_id =
          catalog_to_physical_attribute_map.at(attr_id)->id();
      return isExtractedTime(physical_plan_, expr_id, format);
    }
    default:
      break;
  }
  return false;
}

bool VizAnalyzer::isExtractedTime(const P::PhysicalPtr &physical_plan,
                                  const E::ExprId expr_id,
                                  std::string *format) const {
  E::ExpressionPtr gen_expr;
  if (findGenExpression(physical_plan_, expr_id, &gen_expr)) {
    E::UnaryExpressionPtr unary_expr;
    if (E::SomeUnaryExpression::MatchesWithConditionalCast(gen_expr, &unary_expr) &&
        unary_expr->operation().getUnaryOperationID() == UnaryOperationID::kDateExtract) {
      switch (static_cast<const DateExtractOperation &>(unary_expr->operation()).unit()) {
        case DateExtractUnit::kYear:
          *format = "%Y";
          return true;
        case DateExtractUnit::kMonth:
          *format = "%m";
          return true;
        case DateExtractUnit::kDay:
          *format = "%d";
          return true;
        case DateExtractUnit::kHour:
          *format = "%H";
          return true;
        case DateExtractUnit::kMinute:
          *format = "%M";
          return true;
        case DateExtractUnit::kSecond:
          *format = "%S";
          return true;
        default:
          break;
      }
    }
  }
  return false;
}


bool VizAnalyzer::findGenExpression(const P::PhysicalPtr &physical_plan,
                                    const E::ExprId expr_id,
                                    E::ExpressionPtr *expr) const {
  for (const P::PhysicalPtr &child : physical_plan->children()) {
    if (E::ContainsExprId(child->getOutputAttributes(), expr_id)) {
      return findGenExpression(child, expr_id, expr);
    }
  }

  const std::vector<E::NamedExpressionPtr> *expressions;
  switch (physical_plan->getPhysicalType()) {
    case P::PhysicalType::kAggregate:
      expressions =
          &std::static_pointer_cast<const P::Aggregate>(
              physical_plan)->grouping_expressions();
      break;
    case P::PhysicalType::kSelection:
      expressions =
          &std::static_pointer_cast<const P::Selection>(
              physical_plan)->project_expressions();
      break;
    case P::PhysicalType::kHashJoin:  // Fall through
    case P::PhysicalType::kNestedLoopsJoin: {
      expressions =
          &std::static_pointer_cast<const P::Join>(
              physical_plan)->project_expressions();
      break;
    }
    default:
      return false;
  }

  for (const auto &named_expr : *expressions) {
    E::AliasPtr alias;
    if (named_expr->id() == expr_id &&
        E::SomeAlias::MatchesWithConditionalCast(named_expr, &alias)) {
      *expr = alias->expression();
      return true;
    }
  }
  return false;
}

}  // namespace viz
}  // namespace quickstep
