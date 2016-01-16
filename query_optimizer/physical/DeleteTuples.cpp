/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/physical/DeleteTuples.hpp"

#include <string>
#include <vector>

#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Predicate.hpp"

namespace quickstep {
namespace optimizer {
namespace physical {

std::vector<expressions::AttributeReferencePtr> DeleteTuples::getReferencedAttributes() const {
  if (predicate_ != nullptr) {
    return predicate_->getReferencedAttributes();
  } else {
    return std::vector<expressions::AttributeReferencePtr>();
  }
}

void DeleteTuples::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields)
    const {
  non_container_child_field_names->push_back("input");
  non_container_child_fields->push_back(input_);

  if (predicate_ != nullptr) {
    non_container_child_field_names->push_back("predicate");
    non_container_child_fields->push_back(predicate_);
  }
}

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep
