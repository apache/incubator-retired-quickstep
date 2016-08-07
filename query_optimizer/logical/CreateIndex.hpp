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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATEINDEX_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATEINDEX_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class CreateIndex;
typedef std::shared_ptr<const CreateIndex> CreateIndexPtr;

/**
 * @brief Represents an operation that creates a new index.
 */
class CreateIndex : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kCreateIndex; }

  std::string getName() const override { return "CreateIndex"; }

  /**
   * @return The input that produces the relation to create index upon.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return The name of the index to be created.
   */
  const std::string& index_name() const { return index_name_; }

  /**
   * @return The list of attributes to build index upon.
   */
  const std::vector<expressions::AttributeReferencePtr>& index_attributes() const {
    return index_attributes_;
  }

  /**
   * @return Shared pointer to the index properties.
   */
  std::shared_ptr<const IndexSubBlockDescription> index_description() const {
    return index_description_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], index_name_, index_attributes_, index_description_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return index_attributes_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return index_attributes_;
  }

  /**
   * @brief Creates a CreateIndex logical node that represents an operation to
   *        create a new index.
   *
   * @param input The input produces the relation to create index upon.
   * @param index_name The name of the index to create.
   * @param index_attributes Set of attributes to create index upon.
   * @param index_description A proto block describing the set of properties for this index
   * @return An immutable CreateIndex node.
   */
  static CreateIndexPtr Create(
      const LogicalPtr &input,
      const std::string &index_name,
      const std::vector<expressions::AttributeReferencePtr> &index_attributes,
      const std::shared_ptr<const IndexSubBlockDescription> &index_description) {
    return CreateIndexPtr(new CreateIndex(input, index_name, index_attributes, index_description));
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
  CreateIndex(
      const LogicalPtr &input,
      const std::string &index_name,
      const std::vector<expressions::AttributeReferencePtr> &index_attributes,
      const std::shared_ptr<const IndexSubBlockDescription> &index_description)
      : input_(input),
        index_name_(index_name),
        index_attributes_(index_attributes),
        index_description_(index_description) {
    addChild(input_);
  }

  const LogicalPtr input_;
  const std::string index_name_;
  const std::vector<expressions::AttributeReferencePtr> index_attributes_;
  std::shared_ptr<const IndexSubBlockDescription> index_description_;

  DISALLOW_COPY_AND_ASSIGN(CreateIndex);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATEINDEX_HPP_ */
