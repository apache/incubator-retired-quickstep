/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/TableReference.hpp"

#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

namespace E = ::quickstep::optimizer::expressions;

void TableReference::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("relation");
  inline_field_values->push_back(relation_->getName());

  if (alias_ != relation_->getName()) {
    inline_field_names->push_back("alias");
    inline_field_values->push_back(alias_);
  }

  container_child_field_names->push_back("");
  container_child_fields->push_back(CastSharedPtrVector<OptimizerTreeBase>(attribute_list_));
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
