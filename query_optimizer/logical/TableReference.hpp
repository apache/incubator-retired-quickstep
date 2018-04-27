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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_TABLE_REFERENCE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_TABLE_REFERENCE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {

class OptimizerContext;

namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class TableReference;
typedef std::shared_ptr<const TableReference> TableReferencePtr;

/**
 * @brief Leaf logical operator that represents a reference to
 *        a catalog relation.
 */
class TableReference : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kTableReference; }

  std::string getName() const override { return "TableReference"; }

  /**
   * @return The catalog relation which the operator scans.
   */
  const CatalogRelation* catalog_relation() const { return catalog_relation_; }

  /**
   * @return The relation alias.
   */
  const std::string& relation_alias() const { return relation_alias_; }

  /**
   * @return The references to the attributes in the relation.
   */
  const std::vector<expressions::AttributeReferencePtr>& attribute_list() const {
    return attribute_list_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return attribute_list_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return getOutputAttributes();
  }

  /**
   * @brief Generates a AttributeRerference for each CatalogAttribute in the \p
   *        catalog_relation and creates a TableReference on it.
   *
   * @param catalog_relation The catalog relation to be scanned.
   * @param relation_alias The relation alias. This is stored here for the printing purpose only.
   * @param optimizer_context The OptimizerContext for the query.
   * @return An immutable TableReference.
   */
  static TableReferencePtr Create(const CatalogRelation *catalog_relation,
                                  const std::string &relation_alias,
                                  OptimizerContext *optimizer_context) {
    DCHECK(!relation_alias.empty());
    return TableReferencePtr(
        new TableReference(catalog_relation, relation_alias, optimizer_context));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  TableReference(const CatalogRelation *catalog_relation,
                 const std::string &relation_alias,
                 OptimizerContext *optimizer_context);

  // Constructor where the attribute list is explicitly given.
  TableReference(
      const CatalogRelation *catalog_relation,
      const std::string &relation_alias,
      const std::vector<expressions::AttributeReferencePtr> &attribute_list)
      : catalog_relation_(catalog_relation),
        relation_alias_(relation_alias),
        attribute_list_(attribute_list) {}

  const CatalogRelation *catalog_relation_;
  const std::string relation_alias_;
  std::vector<expressions::AttributeReferencePtr> attribute_list_;

  DISALLOW_COPY_AND_ASSIGN(TableReference);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_LOGICAL_TABLE_REFERENCE_HPP_ */
