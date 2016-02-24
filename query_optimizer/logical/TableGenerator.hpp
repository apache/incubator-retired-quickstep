/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TABLE_GENERATOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TABLE_GENERATOR_HPP_

#include <memory>
#include <string>
#include <vector>

#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

using quickstep::GeneratorFunctionHandle;

/** \addtogroup OptimizerLogical
 *  @{
 */

class TableGenerator;
typedef std::shared_ptr<const TableGenerator> TableGeneratorPtr;

/**
 * @brief Leaf logical node that represents a table that will be populated
 *        by a generator function.
 *
 * @note For how to add a new generator function, see the documentation of
 *       GeneratorFunction and GeneratorFunctionHandle.
 */
class TableGenerator : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kTableGenerator; }

  std::string getName() const override { return "TableGenerator"; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    return TableGeneratorPtr(
        new TableGenerator(generator_function_handle_,
                           table_alias_,
                           attribute_list_));
  }

  /**
   * @return The reference to the generator function handle.
   */
  const GeneratorFunctionHandlePtr &generator_function_handle() const {
    return generator_function_handle_;
  }

  /**
   * @return The alias name of this table.
   */
  const std::string &table_alias() const {
    return table_alias_;
  }

  /**
   * @return The reference to the output attributes of this table.
   */
  const std::vector<expressions::AttributeReferencePtr> &attribute_list() const {
    return attribute_list_;
  }

  std::vector<E::AttributeReferencePtr> getOutputAttributes() const override {
    return attribute_list_;
  }

  std::vector<E::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *
          container_child_fields) const override {
    inline_field_names->push_back("function_name");
    inline_field_values->push_back(generator_function_handle_->getName());

    if (table_alias_ != generator_function_handle_->getName()) {
      inline_field_names->push_back("table_alias");
      inline_field_values->push_back(table_alias_);
    }

    container_child_field_names->push_back("");
    container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(attribute_list_));
  }

  /**
   * @brief Creates a logical TableGenerator node.
   *
   * @param generator_function_handle The shared_ptr reference to a generator
            function handle.
   * @param table_alias The alias name of this table.
   * @param optimizer_context The OptimizerContext for the query
   * @return An immutable TableGenerator.
   */
  static TableGeneratorPtr Create(
      const GeneratorFunctionHandlePtr &generator_function_handle,
      const std::string &table_alias,
      OptimizerContext *optimizer_context) {
    return TableGeneratorPtr(
        new TableGenerator(generator_function_handle,
                           table_alias,
                           optimizer_context));
  }

 private:
  TableGenerator(const GeneratorFunctionHandlePtr &generator_function_handle,
                 const std::string &table_alias,
                 OptimizerContext *optimizer_context)
      : generator_function_handle_(generator_function_handle),
        table_alias_(table_alias) {
    int num_attrs = generator_function_handle->getNumberOfOutputColumns();
    const std::string &table_name = generator_function_handle->getName();

    for (int i = 0; i < num_attrs; ++i) {
      attribute_list_.emplace_back(
        E::AttributeReference::Create(
            optimizer_context->nextExprId(),
            generator_function_handle->getOutputColumnName(i),
            table_alias,
            table_name,
            generator_function_handle->getOutputColumnType(i)));
    }
  }

  TableGenerator(const GeneratorFunctionHandlePtr &generator_function_handle,
                 const std::string &table_alias,
                 const std::vector<E::AttributeReferencePtr> &attribute_list)
      : generator_function_handle_(generator_function_handle),
        table_alias_(table_alias),
        attribute_list_(attribute_list) {
  }

  GeneratorFunctionHandlePtr generator_function_handle_;
  std::string table_alias_;
  std::vector<E::AttributeReferencePtr> attribute_list_;

  DISALLOW_COPY_AND_ASSIGN(TableGenerator);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_TABLE_GENERATOR_HPP_ */
