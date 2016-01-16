/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/BinaryJoin.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

void BinaryJoin::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("left");
  non_container_child_field_names->push_back("right");

  non_container_child_fields->push_back(left_);
  non_container_child_fields->push_back(right_);

  container_child_field_names->push_back("project_expressions");
  container_child_fields->push_back(
      CastSharedPtrVector<OptimizerTreeBase>(project_expressions()));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
