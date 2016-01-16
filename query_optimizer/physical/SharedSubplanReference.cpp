/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/SharedSubplanReference.hpp"

#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "utility/Cast.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

void SharedSubplanReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("subplan_id");
  inline_field_values->push_back(std::to_string(subplan_id_));

  container_child_field_names->push_back("output_attributes");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(output_attributes_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
