/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SAMPLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SAMPLE_HPP_

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

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Sample;
typedef std::shared_ptr<const Sample> SamplePtr;

/**
 * @brief Sample operator that samples the data at a block level or 
 *        at the tuple level and outputs them as a new relation.
 */
class Sample : public Physical {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kSample; }

  std::string getName() const override { return " SAMPLE"; }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return input_->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return input_->getReferencedAttributes();
  }

  PhysicalPtr copyWithNewChildren(
    const std::vector<PhysicalPtr> &new_children) const override;

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @return the sample percentage.
   */
  int percentage() const {
    return percentage_;
  }

  /**
   * @return flag indicating if the sampling is block or tuple.
   */
  const bool is_block_sample() const {
    return is_block_sample_;
  }

  /**
   * @return Input physical node.
   */
  const PhysicalPtr& input() const { return children()[0]; }

  /**
   * @brief Creates a SamplePtr.
   * @param input The input node.
   * @param is_block_sample Flag indicating if the sample is block or tuple level
   * @param percentage The percentage of rows or blocks to be sampled 
   * @return An immutable Sample.
   */
  static SamplePtr Create(const PhysicalPtr &input,
                          const bool is_block_sample,
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
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  Sample(
      const PhysicalPtr &input,
      const bool is_block_sample,
      const int percentage)
      : percentage_(percentage),
        is_block_sample_(is_block_sample),
        input_(input) {
    addChild(input);
  }

  const int percentage_;
  const bool is_block_sample_;
  const PhysicalPtr input_;

  DISALLOW_COPY_AND_ASSIGN(Sample);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_SAMPLE_HPP_
