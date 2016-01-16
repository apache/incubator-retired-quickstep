/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/InsertTuple.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

void InsertTuple::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  container_child_field_names->push_back("column_values");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(column_values_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
