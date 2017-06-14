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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TABLE_REFERENCE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TABLE_REFERENCE_HPP_

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

struct PartitionSchemeHeader;
class TableReference;
typedef std::shared_ptr<const TableReference> TableReferencePtr;

/**
 * @brief Leaf physical node that represents a reference to a catalog relation.
 */
class TableReference : public Physical {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kTableReference; }

  std::string getName() const override { return "TableReference"; }

  /**
   * @return The input catalog relation.
   */
  const CatalogRelation* relation() const { return relation_; }

  /**
   * @return The list of references to the relation attributes.
   */
  const std::vector<expressions::AttributeReferencePtr>& attribute_list() const {
    return attribute_list_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return attribute_list_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    // The project attributes cannot be changed in a TableReference.
    return false;
  }

  /**
   * @brief Creates a TableReference.
   *
   * @param relation The referenced catalog relation.
   * @param alias The relation alias in the query. It is used only for printing.
   * @param attribute_list The list of AttributeReferences of the relation.
   * @return An immutable TableReference.
   */
  static TableReferencePtr Create(
      const CatalogRelation *relation,
      const std::string &alias,
      const std::vector<expressions::AttributeReferencePtr> &attribute_list);

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  TableReference(
      const CatalogRelation *relation, const std::string &alias,
      const std::vector<expressions::AttributeReferencePtr> &attribute_list,
      PartitionSchemeHeader *partition_scheme_header)
      : Physical(partition_scheme_header),
        relation_(relation),
        alias_(alias),
        attribute_list_(attribute_list) {}

  const CatalogRelation *relation_;
  const std::string alias_;
  const std::vector<expressions::AttributeReferencePtr> attribute_list_;

  DISALLOW_COPY_AND_ASSIGN(TableReference);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TABLE_REFERENCE_HPP_ */
