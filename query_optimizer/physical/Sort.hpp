/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SORT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SORT_HPP_

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
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

class Sort;
typedef std::shared_ptr<const Sort> SortPtr;

/**
 * @brief Sort operator.
 */
class Sort : public Physical {
 public:
  PhysicalType getPhysicalType() const override {
    return PhysicalType::kSort;
  }

  std::string getName() const override {
    return "Sort";
  }

  const PhysicalPtr& input() const {
    return input_;
  }

  /**
   * @return The sorting attributes.
   */
  const std::vector<expressions::AttributeReferencePtr>& sort_attributes() const {
    return sort_attributes_;
  }

  /**
   * @brief The order direction for every sorting attributes.
   */
  const std::vector<bool>& sort_ascending() const {
    return sort_ascending_;
  }

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

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return input_->getOutputAttributes();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    std::vector<expressions::AttributeReferencePtr> referenced_attributes =
        sort_attributes_;
    referenced_attributes.insert(referenced_attributes.end(),
                                 non_sort_attributes_.begin(),
                                 non_sort_attributes_.end());
    return referenced_attributes;
  }

  PhysicalPtr copyWithNewChildren(const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(new_children[0],
                  sort_attributes_,
                  non_sort_attributes_,
                  sort_ascending_,
                  nulls_first_flags_,
                  limit_);
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_attributes,
      PhysicalPtr *output) const override;

  /**
   * @brief Creates a physical Sort operator.
   *
   * @param input The input to the Sort.
   * @param sort_attributes The attributes as the ordering keys.
   * @param non_sort_attributes The output attributes other than sort_attributes.
   * @param sort_ascending The vector of ordering directions.
   * @param nulls_first_flags The vector of ordering directions for null values.
   * @param limit The number of output rows. -1 for a full table sort.
   *
   * @return An immutable physical Sort.
   */
  static SortPtr Create(const PhysicalPtr &input,
                        const std::vector<expressions::AttributeReferencePtr> &sort_attributes,
                        const std::vector<expressions::AttributeReferencePtr> &non_sort_attributes,
                        const std::vector<bool> &sort_ascending,
                        const std::vector<bool> &nulls_first_flags,
                        const int limit) {
    DCHECK_EQ(sort_attributes.size(), sort_ascending.size());
    DCHECK_EQ(sort_attributes.size(), nulls_first_flags.size());

    return SortPtr(new Sort(input,
                            sort_attributes,
                            non_sort_attributes,
                            sort_ascending,
                            nulls_first_flags,
                            limit));
  }

 protected:
  void getFieldStringItems(std::vector<std::string> *inline_field_names,
                           std::vector<std::string> *inline_field_values,
                           std::vector<std::string> *non_container_child_field_names,
                           std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
                           std::vector<std::string> *container_child_field_names,
                           std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  Sort(const PhysicalPtr &input,
       const std::vector<expressions::AttributeReferencePtr> &sort_attributes,
       const std::vector<expressions::AttributeReferencePtr> &non_sort_attributes,
       const std::vector<bool> &sort_ascending,
       const std::vector<bool> &nulls_first_flags,
       const int limit)
      : input_(input),
        sort_attributes_(sort_attributes),
        non_sort_attributes_(non_sort_attributes),
        sort_ascending_(sort_ascending),
        nulls_first_flags_(nulls_first_flags),
        limit_(limit) {
    addChild(input);
  }

  PhysicalPtr input_;
  std::vector<expressions::AttributeReferencePtr> sort_attributes_;
  // non_sort_attributes_ is present only for Rule PruneColumns,
  // which need attributes referenced by Sort (i.e. sort_attributes_)
  // and others by its ancestors (i.e. non_sort_attributes_).
  // The output attributes by Sort may contain additional non-sorting attributes,
  // since the input physical of the Sort may not allow column pruning.
  std::vector<expressions::AttributeReferencePtr> non_sort_attributes_;
  std::vector<bool> sort_ascending_;
  std::vector<bool> nulls_first_flags_;
  int limit_;

  DISALLOW_COPY_AND_ASSIGN(Sort);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_SORT_HPP_ */
