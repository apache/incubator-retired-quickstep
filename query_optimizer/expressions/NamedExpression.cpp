/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/expressions/NamedExpression.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "types/Type.hpp"

namespace quickstep {
namespace optimizer {
namespace expressions {

void NamedExpression::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("id");
  inline_field_values->push_back(std::to_string(id_));

  inline_field_names->push_back("name");
  inline_field_values->push_back(attribute_name_);

  if (attribute_name_ != attribute_alias_) {
    inline_field_names->push_back("alias");
    inline_field_values->push_back(attribute_alias_);
  }

  inline_field_names->push_back("relation");
  inline_field_values->push_back(relation_name_);

  inline_field_names->push_back("type");
  inline_field_values->push_back(getValueType().getName());
}

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep
