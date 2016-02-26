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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATEINDEX_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATEINDEX_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
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

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0], index_name_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  /**
   * @brief Creates a CreateIndex logical node that represents an operation to
   *        create a new index.
   *
   * @param input The input produces the relation to create index upon.
   * @param index_name The name of the index to create.
   * @return An immutable CreateIndex node.
   */
  static CreateIndexPtr Create(
      const LogicalPtr &input,
      const std::string &relation_name) {
    return CreateIndexPtr(new CreateIndex(input, relation_name));
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
      const std::string &index_name)
      : input_(input), index_name_(index_name) {
        addChild(input_);
  }

  LogicalPtr input_;
  std::string index_name_;

  DISALLOW_COPY_AND_ASSIGN(CreateIndex);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATEINDEX_HPP_ */
