/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RESOLVER_NAME_RESOLVER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RESOLVER_NAME_RESOLVER_HPP_

#include <map>
#include <string>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {

class CatalogDatabase;
class ParseString;

namespace optimizer {
namespace resolver {

/** \addtogroup QueryResolver
 *  @{
 */

/**
 * @brief Manages name scopes and resolves identifiers (e.g. relation and
 *        attribute names) in a parse tree.
 */
class NameResolver {
 public:
  /**
   * @brief Constructor.
   */
  NameResolver() {}

  /**
   * @brief Adds the attributes of the relation produced by \p logical
   *        into the name scope.
   * @exception Throws a SqlError if the relation name already exists.
   * @note Does not take ownership of \p parse_rel_name.
   *
   * @param parse_rel_name The parse node of the relation name. Cannot be NULL.
   * @param logical The logical operator that outputs the relation.
   */
  void addRelation(
      const ParseString *parse_rel_name,
      const logical::LogicalPtr &logical);

  /**
   * @brief Returns the AttributeReference with the given attribute name and
   *        relation name.
   * @exception Throws a SqlError if the relation/attribute is not found or the
   *            attribute name is ambiguous.
   * @note Does not take ownership of the two arguments.
   *
   * @param parse_attr_node The parser node for the attribute name. Cannot be
   *                        NULL.
   * @param parse_rel_node The parser node for the relation name. Maybe NULL.
   * @return The AttributeReference.
   */
  expressions::AttributeReferencePtr lookup(
      const ParseString *parse_attr_node,
      const ParseString *parse_rel_node) const;

  /**
   * @return All AttributeReferences in the current name scope.
   */
  std::vector<expressions::AttributeReferencePtr> getVisibleAttributeReferences() const;

  /**
   * @brief Determines whether \p name is an internal name.
   *        All internal names are empty.
   *
   * @param name The string name to check.
   * @return True if \p name is an internal name.
   */
  static bool IsInternalName(const std::string &name) { return name.empty(); }

  /**
   * @brief Generates an internal alias from \p name.
   *
   * @param name The base string to compose the internal alias.
   * @return An internal alias.
   */
  static std::string GenerateInternalAlias(const std::string &name) {
    return std::string("$").append(name);
  }

 private:
  /**
   * @brief Info of a relation and its visible attributes in the name scope.
   */
  struct RelationInfo {
    explicit RelationInfo(const logical::LogicalPtr &logical_in)
        : logical(logical_in),
          attributes(logical->getOutputAttributes()) {
      int current_attribute_index = 0;
      for (const expressions::AttributeReferencePtr &attribute : attributes) {
        if (!IsInternalName(attribute->attribute_name())) {
          const std::string lower_attr_name =
              ToLower(attribute->attribute_name());
          if (name_to_attribute_index_map.find(lower_attr_name) !=
              name_to_attribute_index_map.end()) {
            // Use -1 to indicate an ambiguous attribute name.
            name_to_attribute_index_map[lower_attr_name] = -1;
          } else {
            name_to_attribute_index_map[lower_attr_name] =
                current_attribute_index;
          }
        }
        ++current_attribute_index;
      }
    }

    // Finds an attribute with the value of <parse_attr_node> from the current
    // relation. Returns NULL if no attribute with the given name is found.
    expressions::AttributeReferencePtr findAttributeByName(const ParseString *parse_attr_node) const;

    logical::LogicalPtr logical;

    std::vector<expressions::AttributeReferencePtr> attributes;

    // Map from attribute name to the index in <attributes>.
    // For an ambiguous attribute, the value is -1.
    std::map<std::string, int> name_to_attribute_index_map;
  };

  /**
   * @brief Relation info in the name scope.
   */
  std::vector<std::unique_ptr<RelationInfo>> relations_;

  /**
   * @brief Map from relation name to relation info.
   */
  std::map<std::string, const RelationInfo *> rel_name_to_rel_info_map_;

  DISALLOW_COPY_AND_ASSIGN(NameResolver);
};

/** @} */

}  // namespace resolver
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RESOLVER_NAME_RESOLVER_HPP_ */
