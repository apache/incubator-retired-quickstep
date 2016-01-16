/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/Sort.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

namespace E = ::quickstep::optimizer::expressions;

LogicalPtr Sort::copyWithNewChildren(
    const std::vector<LogicalPtr> &new_children) const {
  DCHECK_EQ(getNumChildren(), new_children.size());
  return Create(new_children[0], sort_attributes_, sort_ascending_, nulls_first_flags_, limit_);
}

void Sort::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  inline_field_names->push_back("is_ascending");
  std::string ascending_list("[");
  for (bool is_ascending : sort_ascending_) {
    if (is_ascending) {
      ascending_list.append("true,");
    } else {
      ascending_list.append("false,");
    }
  }
  ascending_list.pop_back();
  ascending_list.append("]");
  inline_field_values->push_back(ascending_list);

  inline_field_names->push_back("nulls_first");
  std::string nulls_first_flags("[");
  for (bool nulls_first_flag : nulls_first_flags_) {
    if (nulls_first_flag) {
      nulls_first_flags.append("true,");
    } else {
      nulls_first_flags.append("false,");
    }
  }
  nulls_first_flags.pop_back();
  nulls_first_flags.append("]");
  inline_field_values->push_back(nulls_first_flags);

  container_child_field_names->push_back("sort_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(sort_attributes_));
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
