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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SAMPLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SAMPLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Sample;
typedef std::shared_ptr<const Sample> SamplePtr;

/**
 * @brief Sample operator that samples the data at a block level or
 *        at the tuple level and outputs them as a new relation.
 */
class Sample : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kSample; }

  std::string getName() const override { return "SAMPLE"; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override;

  std::vector<expressions::AttributeReferencePtr>
      getOutputAttributes() const override {
    return input_->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr>
      getReferencedAttributes() const override {
    return input_->getReferencedAttributes();
  }

  /**
   * @return The percentage of data to be sampled.
   */
  int percentage() const {
    return percentage_;
  }

  /**
   * @return Flag indicating the sample type.
   */
  bool is_block_sample() const {
    return is_block_sample_;
  }

  /**
   * @return The input operator.
   */
  const LogicalPtr& input() const { return input_; }

  /**
   * @brief Creates a Sample operator that samples the data from the
   *        relation produced by \p input.
   * @param input The input operator to this Sample.
   * @param is_block_sample Flag indicating if the sampling is either a block or
   *                        tuple level sampling
   * @param percentage The percentage of tuples/blocks to be sampled
   *
   * @return An immutable Sample.
   */
  static SamplePtr Create(const LogicalPtr &input, const bool is_block_sample,
                          const int percentage) {
    return SamplePtr(new Sample(input, is_block_sample, percentage));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>>
          *container_child_fields) const override;

 private:
  Sample(const LogicalPtr &input,
         const bool is_block_sample,
         const int percentage)
      : input_(input),
        is_block_sample_(is_block_sample),
        percentage_(percentage) {
     addChild(input);
  }

  const LogicalPtr input_;
  const bool is_block_sample_;
  const int percentage_;

  DISALLOW_COPY_AND_ASSIGN(Sample);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_SAMPLE_HPP_
