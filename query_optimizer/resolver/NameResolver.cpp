/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "query_optimizer/resolver/NameResolver.hpp"

#include <map>
#include <string>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "parser/ParseString.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/StringUtil.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace resolver {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

E::AttributeReferencePtr NameResolver::RelationInfo::findAttributeByName(
    const ParseString *parse_attr_node) const {
  E::AttributeReferencePtr attribute;
  const std::string lower_attr_name = ToLower(parse_attr_node->value());
  const std::map<std::string, int>::const_iterator found_it =
      name_to_attribute_index_map.find(lower_attr_name);
  if (found_it != name_to_attribute_index_map.end()) {
    if (found_it->second == -1) {
      THROW_SQL_ERROR_AT(parse_attr_node)
          << "Ambiguous attribute " << lower_attr_name;
    }
    attribute = attributes[found_it->second];
  }
  return attribute;
}

void NameResolver::addRelation(
    const ParseString *parse_rel_name,
    const L::LogicalPtr &logical) {
  DCHECK(parse_rel_name != nullptr);
  relations_.emplace_back(new RelationInfo(logical));
  const std::string lower_rel_name = ToLower(parse_rel_name->value());
  if (rel_name_to_rel_info_map_.find(lower_rel_name) !=
      rel_name_to_rel_info_map_.end()) {
    THROW_SQL_ERROR_AT(parse_rel_name) << "Relation alias " << lower_rel_name
                                       << " appears more than once";
  }
  rel_name_to_rel_info_map_[lower_rel_name] = relations_.back().get();
}

E::AttributeReferencePtr NameResolver::lookup(
    const ParseString *parse_attr_node,
    const ParseString *parse_rel_node) const {
  E::AttributeReferencePtr attribute;
  if (parse_rel_node == nullptr) {
    // If the relation name is not given, search all visible relations.
    for (const std::unique_ptr<RelationInfo> &item : relations_) {
      E::AttributeReferencePtr found_attribute =
          item->findAttributeByName(parse_attr_node);
      if (found_attribute != nullptr) {
        // More than one relation has the same attribute name.
        if (attribute != nullptr) {
          THROW_SQL_ERROR_AT(parse_attr_node) << "Ambiguous attribute "
                                              << parse_attr_node->value();
        }
        attribute = found_attribute;
      }
    }
  } else {
    const std::map<std::string, const RelationInfo *>::const_iterator found_it =
        rel_name_to_rel_info_map_.find(ToLower(parse_rel_node->value()));
    if (found_it != rel_name_to_rel_info_map_.end()) {
      attribute = found_it->second->findAttributeByName(parse_attr_node);
    } else {
      THROW_SQL_ERROR_AT(parse_rel_node) << "Unrecognized relation "
                                         << parse_rel_node->value();
    }
  }
  if (attribute == nullptr) {
    THROW_SQL_ERROR_AT(parse_attr_node) << "Unrecognized attribute "
                                        << parse_attr_node->value();
  }
  return attribute;
}

std::vector<E::AttributeReferencePtr> NameResolver::getVisibleAttributeReferences() const {
  std::vector<E::AttributeReferencePtr> referenced_attributes;
  for (const std::unique_ptr<RelationInfo> &rel : relations_) {
    referenced_attributes.insert(referenced_attributes.end(),
                                 rel->attributes.begin(),
                                 rel->attributes.end());
  }
  return referenced_attributes;
}

}  // namespace resolver
}  // namespace optimizer
}  // namespace quickstep
