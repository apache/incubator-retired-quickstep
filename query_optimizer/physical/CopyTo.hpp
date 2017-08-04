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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_COPY_TO_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_COPY_TO_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/BulkIoConfiguration.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class CopyTo;
typedef std::shared_ptr<const CopyTo> CopyToPtr;

/**
 * @brief Represents an operation that copies data from a relation to a text file.
 */
class CopyTo : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kCopyTo;
  }

  std::string getName() const override {
    return "CopyTo";
  }

  /**
   * @return The input relation whose data is to be exported.
   */
  const PhysicalPtr& input() const {
    return input_;
  }

  /**
   * @return The name of the file to write the data to.
   */
  const std::string& file_name() const {
    return file_name_;
  }

  /**
   * @return The options for this COPY TO statement.
   */
  const BulkIoConfigurationPtr& options() const {
    return options_;
  }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(1u, new_children.size());
    return Create(new_children.front(), file_name_, options_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return input_->getOutputAttributes();
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates a CopyTo physical node.
   *
   * @param input The input relation whose data is to be exported.
   * @param file_name The name of the file to write the data to.
   * @param options The options for this COPY TO statement.
   * @return An immutable CopyTo physical node.
   */
  static CopyToPtr Create(const PhysicalPtr &input,
                          const std::string &file_name,
                          const BulkIoConfigurationPtr &options) {
    return CopyToPtr(new CopyTo(input, file_name, options));
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
  CopyTo(const PhysicalPtr &input,
         const std::string &file_name,
         const BulkIoConfigurationPtr &options)
      : input_(input),
        file_name_(file_name),
        options_(options) {
    addChild(input);
  }

  const PhysicalPtr input_;
  const std::string file_name_;
  const BulkIoConfigurationPtr options_;

  DISALLOW_COPY_AND_ASSIGN(CopyTo);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_COPY_TO_HPP_
