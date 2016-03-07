/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CREATEINDEX_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CREATEINDEX_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class CreateIndex;
typedef std::shared_ptr<const CreateIndex> CreateIndexPtr;

/**
 * @brief Creates an index.
 */
class CreateIndex : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kCreateIndex;
  }

  std::string getName() const override { return "CreateIndex"; }

  /**
   * @return The input that produces the relation to create index upon.
   */
  const PhysicalPtr& input() const { return input_; }

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
  const std::shared_ptr<const IndexSubBlockDescription> index_description() const {
    return index_description_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], index_name_, index_attributes_, index_description_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return index_attributes_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return index_attributes_;
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates a CreateIndex physical node that represents an operation to
   *        create a new index.
   *
   * @param input The input produces the relation to create index upon.
   * @param index_name The name of the index to create.
   * @param index_attributes Set of attributes to create index upon.
   * @param index_description A proto block describing the set of properties for this index.
   * @return An immutable CreateIndex node.
   */
  static CreateIndexPtr Create(
      const PhysicalPtr &input,
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
      const PhysicalPtr &input,
      const std::string &index_name,
      const std::vector<expressions::AttributeReferencePtr> &index_attributes,
      const std::shared_ptr<const IndexSubBlockDescription> &index_description)
      : input_(input),
        index_name_(index_name),
        index_attributes_(index_attributes),
        index_description_(index_description) {
      addChild(input_);
  }

  PhysicalPtr input_;
  std::string index_name_;
  std::vector<expressions::AttributeReferencePtr> index_attributes_;
  std::shared_ptr<const IndexSubBlockDescription> index_description_;

  DISALLOW_COPY_AND_ASSIGN(CreateIndex);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_CREATEINDEX_HPP_ */
