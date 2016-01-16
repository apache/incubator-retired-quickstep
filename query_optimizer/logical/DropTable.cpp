/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/logical/DropTable.hpp"

#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerTree.hpp"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace logical {

void DropTable::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const {
  inline_field_names->push_back("relation");
  inline_field_values->push_back(catalog_relation_->getName());
}

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep
