/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "parser/ParseHaving.hpp"

#include <string>
#include <vector>

#include "parser/ParsePredicate.hpp"

namespace quickstep {

void ParseHaving::getFieldStringItems(std::vector<std::string> *inline_field_names,
                                      std::vector<std::string> *inline_field_values,
                                      std::vector<std::string> *non_container_child_field_names,
                                      std::vector<const ParseTreeNode*> *non_container_child_fields,
                                      std::vector<std::string> *container_child_field_names,
                                      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const {
  non_container_child_field_names->push_back("");
  non_container_child_fields->push_back(having_predicate_.get());
}

}  // namespace quickstep
