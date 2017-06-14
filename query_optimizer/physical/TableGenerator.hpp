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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TABLE_GENERATOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TABLE_GENERATOR_HPP_

#include <memory>
#include <string>
#include <vector>

#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

/** \addtogroup OptimizerPhysical
 *  @{
 */

class TableGenerator;
typedef std::shared_ptr<const TableGenerator> TableGeneratorPtr;

/**
 * @brief Leaf physical node that represents a table that will be populated
 *        by a generator function.
 */
class TableGenerator : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kTableGenerator;
  }

  std::string getName() const override { return "TableGenerator"; }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    return TableGeneratorPtr(
        new TableGenerator(generator_function_handle_,
                           table_alias_,
                           attribute_list_));
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    // The number of output columns is fixed for generator functions.
    return false;
  }

  /**
   * @return The reference to the generator function handle.
   */
  const GeneratorFunctionHandlePtr& generator_function_handle() const {
    return generator_function_handle_;
  }

  /**
   * @return The alias name of this table.
   */
  const std::string& table_alias() const {
    return table_alias_;
  }

  const std::vector<expressions::AttributeReferencePtr>& attribute_list() const {
    return attribute_list_;
  }

  std::vector<E::AttributeReferencePtr> getOutputAttributes() const override {
    return attribute_list_;
  }

  std::vector<E::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  PhysicalPtr copyWithNewOutputPartitionSchemeHeader(
      PartitionSchemeHeader *partition_scheme_header) const override {
    return TableGeneratorPtr(
        new TableGenerator(generator_function_handle_, table_alias_, attribute_list_, partition_scheme_header));
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

    if (partition_scheme_header_) {
      inline_field_names->push_back("output_partition_scheme_header");
      inline_field_values->push_back(partition_scheme_header_->toString());
    }

    container_child_field_names->push_back("");
    container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(attribute_list_));
  }

  /**
   * @brief Creates a table generator that generates a table.
   *
   * @param A generator function handle.
   * @return An immutable TableGenerator.
   */
  static TableGeneratorPtr Create(
      const GeneratorFunctionHandlePtr &generator_function_handle,
      const std::string &table_alias,
      const std::vector<E::AttributeReferencePtr> &attribute_list) {
    return TableGeneratorPtr(
        new TableGenerator(generator_function_handle,
                           table_alias,
                           attribute_list));
  }

 private:
  TableGenerator(const GeneratorFunctionHandlePtr &generator_function_handle,
                 const std::string &table_alias,
                 const std::vector<E::AttributeReferencePtr> &attribute_list,
                 PartitionSchemeHeader *partition_scheme_header = nullptr)
      : Physical(partition_scheme_header),
        generator_function_handle_(generator_function_handle),
        table_alias_(table_alias),
        attribute_list_(attribute_list) {
  }

  GeneratorFunctionHandlePtr generator_function_handle_;
  std::string table_alias_;
  std::vector<E::AttributeReferencePtr> attribute_list_;

  DISALLOW_COPY_AND_ASSIGN(TableGenerator);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_TABLE_GENERATOR_HPP_ */
