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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CREATETABLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CREATETABLE_HPP_

#include <memory>
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
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class CreateTable;
typedef std::shared_ptr<const CreateTable> CreateTablePtr;

/**
 * @brief Creates a table.
 */
class CreateTable : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kCreateTable;
  }

  std::string getName() const override { return "CreateTable"; }

  /**
   * @return The name of the relation to be inserted.
   */
  const std::string& relation_name() const { return relation_name_; }

  /**
   * @return Attribute list of the relation.
   */
  const std::vector<expressions::AttributeReferencePtr>& attributes() const {
    return attributes_;
  }

  /**
   * @return Shared pointer to the block properties.
   */
  const std::shared_ptr<const StorageBlockLayoutDescription> block_properties() const {
    return block_properties_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(relation_name_, attributes_, block_properties_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return attributes_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return attributes_;
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates a CreateTable physical node that represents an operation to
   *        create a new table.
   *
   * @param relation_name The name of the relation to be inserted.
   * @param attributes Schema of the relation.
   * @param block_properties The optional proto message describing the block.
   * @return An immutable CreateTable node.
   */
  static CreateTablePtr Create(
      const std::string &relation_name,
      const std::vector<expressions::AttributeReferencePtr> &attributes,
      const std::shared_ptr<const StorageBlockLayoutDescription> &block_properties) {
    return CreateTablePtr(new CreateTable(relation_name, attributes, block_properties));
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
  CreateTable(
      const std::string &relation_name,
      const std::vector<expressions::AttributeReferencePtr> &attributes,
      const std::shared_ptr<const StorageBlockLayoutDescription> &block_properties)
      : relation_name_(relation_name),
        attributes_(attributes),
        block_properties_(block_properties),
        block_properties_representation_(
            getOptimizerRepresentationForProto<OptimizerTreeBaseNodePtr>(block_properties_.get())) {}

  std::string relation_name_;
  std::vector<expressions::AttributeReferencePtr> attributes_;
  std::shared_ptr<const StorageBlockLayoutDescription> block_properties_;
  std::shared_ptr<const OptimizerProtoRepresentation<OptimizerTreeBaseNodePtr> > block_properties_representation_;

  DISALLOW_COPY_AND_ASSIGN(CreateTable);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CREATETABLE_HPP_ */
