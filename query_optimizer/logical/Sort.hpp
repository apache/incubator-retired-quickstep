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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SORT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SORT_HPP_

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Sort;
typedef std::shared_ptr<const Sort> SortPtr;

/**
 * @brief Sort operator that sorts the input tuples by a subset of attributes.
 */
class Sort : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kSort; }

  std::string getName() const override { return "Sort"; }

  /**
   * @return The input logical node.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @return The sorting attributes.
   */
  const std::vector<expressions::AttributeReferencePtr>& sort_attributes() const {
    return sort_attributes_;
  }

  /**
   * @return The sorting directions.
   */
  const std::vector<bool>& sort_ascending() const { return sort_ascending_; }

  /**
   * @brief The order directions for null values.
   */
  const std::vector<bool>& nulls_first_flags() const {
    return nulls_first_flags_;
  }

  /**
   * @return The number of output sorted rows. -1 for a full table sort.
   */
  int limit() const {
    return limit_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return input_->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return sort_attributes_;
  }
  /**
   * @brief Creates a Sort logical node.
   *
   * @param input The input to the Sort.
   * @param sort_attributes The sorting attributes.
   * @param sort_ascending The vector of ordering directions.
   * @param nulls_first_flags The vector of ordering directions for null values.
   * @param limit The number of output rows. -1 for a full table sort.
   *
   * @return An immutable Sort.
   */
  static SortPtr Create(
      const LogicalPtr &input,
      const std::vector<expressions::AttributeReferencePtr> &sort_attributes,
      const std::vector<bool> &sort_ascending,
      const std::vector<bool> &nulls_first_flags,
      const int limit) {
    return SortPtr(new Sort(input,
                            sort_attributes,
                            sort_ascending,
                            nulls_first_flags,
                            limit));
  }

  static SortPtr Create(
      const LogicalPtr &input,
      std::vector<expressions::AttributeReferencePtr> &&sort_attributes,
      std::vector<bool> &&sort_ascending,
      std::vector<bool> &&nulls_first_flags,
      const int limit) {
    return SortPtr(new Sort(input,
                            std::move(sort_attributes),
                            std::move(sort_ascending),
                            std::move(nulls_first_flags),
                            limit));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *
          container_child_fields) const override;

 private:
  Sort(const LogicalPtr &input,
       const std::vector<expressions::AttributeReferencePtr> &sort_attributes,
       const std::vector<bool> &sort_ascending,
       const std::vector<bool> &nulls_first_flags,
       const int limit)
      : input_(input),
        sort_attributes_(sort_attributes),
        sort_ascending_(sort_ascending),
        nulls_first_flags_(nulls_first_flags),
        limit_(limit) {
    addChild(input_);
  }

  Sort(const LogicalPtr &input,
       std::vector<expressions::AttributeReferencePtr> &&sort_attributes,
       std::vector<bool> &&sort_ascending,
       std::vector<bool> &&nulls_first_flags,
       const int limit)
      : input_(input),
        sort_attributes_(std::move(sort_attributes)),
        sort_ascending_(std::move(sort_ascending)),
        nulls_first_flags_(std::move(nulls_first_flags)),
        limit_(limit) {
    addChild(input_);
  }

  const LogicalPtr input_;
  const std::vector<expressions::AttributeReferencePtr> sort_attributes_;
  // Has 1:1 matching with <sort_expressions_>.
  const std::vector<bool> sort_ascending_;
  const std::vector<bool> nulls_first_flags_;
  const int limit_;

  DISALLOW_COPY_AND_ASSIGN(Sort);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SORT_HPP_ */
