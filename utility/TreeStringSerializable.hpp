/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_UTILITY_TREE_STRING_SERIALIZABLE_HPP_
#define QUICKSTEP_UTILITY_TREE_STRING_SERIALIZABLE_HPP_

#include <cstddef>
#include <string>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Based class for a tree-structured type.
 *        It provides common logic in string serialization.
 *
 * @note This class is templated so that it may be used in both parser
 *       tree and optimizer trees. The parser tree uses unique_ptr to
 *       manage the ownership of fields, while the optimizer tree uses shared_ptr.
 *       TreeNodeType for a parser tree is a constant pointer, whereas it is
 *       a shared_ptr for an optimizer tree.
 */
template<class TreeNodeType>
class TreeStringSerializable {
 public:
  static const int kMaxLineWidth = 80;

  virtual ~TreeStringSerializable() {
  }

  /**
   * @return The node name in the tree-structured string.
   */
  virtual std::string getName() const = 0;

  /**
   * @return A tree-structured string representation.
   */
  std::string toString() const {
    return getNodeString(true /* is_root */,
                         true /* is_last */,
                         "" /* parent_prefix */,
                         "" /* node_name */);
  }

  /**
   * @return A short one-line string representation.
   */
  std::string getShortString() const {
    std::vector<std::string> inline_field_names;
    std::vector<std::string> inline_field_values;
    std::vector<std::string> non_container_child_field_names;
    std::vector<TreeNodeType> non_container_child_fields;
    std::vector<std::string> container_child_field_names;
    std::vector<std::vector<TreeNodeType>> container_child_fields;

    getFieldStringItems(&inline_field_names,
                        &inline_field_values,
                        &non_container_child_field_names,
                        &non_container_child_fields,
                        &container_child_field_names,
                        &container_child_fields);
    return getHeadString(true /* is_root */,
                         true /* is_last */,
                         "" /* prefix */,
                         "" /* node_name */,
                         inline_field_names,
                         inline_field_values,
                         false /* multi_line */);
  }

 protected:
  TreeStringSerializable() {
  }

  /**
   *
   * @brief Returns all the fields (the name and the value) that need to be shown in the output string. There are
   *        three types of fields:
   *        1) Inline fields are those that are not represented as tree nodes.
   *        2) Each non-container field corresponds to a tree node in the string representation. Each field corresponds
   *           to a single subtree.
   *        3) A container field contains a vector of non-container fields. They are all attached to a common child tree
   *           node.
   *
   * @param inline_field_names A list of inline field names.
   * @param inline_field_values A list of inline field string value representation.
   *            1:1 matching with inline_field_names.
   * @param non_container_child_field_names A list of names for non-container fields.
   * @param non_container_child_fields A list of non-container fields.
   * @param container_child_field_names A list of names for container fields.
   * @param container_child_fields A list of container fields.
   */
  virtual void getFieldStringItems(std::vector<std::string> *inline_field_names,
                                   std::vector<std::string> *inline_field_values,
                                   std::vector<std::string> *non_container_child_field_names,
                                   std::vector<TreeNodeType> *non_container_child_fields,
                                   std::vector<std::string> *container_child_field_names,
                                   std::vector<std::vector<TreeNodeType>> *container_child_fields) const = 0;

  /**
   * @brief Returns a tree-structured string representation.
   *
   * @param is_root Whether this node is a root.
   * @param is_last Whether this node is the last child node of its parent.
   * @param parent_prefix The prefix of the parent node.
   * @param node_name The name of this node in the parent node. This is different from getName() where the name is
   *            independent of the parent.
   * @return A tree-structured string representation.
   */
  std::string getNodeString(bool is_root,
                            bool is_last,
                            const std::string &parent_prefix,
                            const std::string &node_name) const {
    std::vector<std::string> inline_field_names;
    std::vector<std::string> inline_field_values;
    std::vector<std::string> non_container_child_field_names;
    std::vector<TreeNodeType> non_container_child_fields;
    std::vector<std::string> container_child_field_names;
    std::vector<std::vector<TreeNodeType>> container_child_fields;

    getFieldStringItems(&inline_field_names,
                        &inline_field_values,
                        &non_container_child_field_names,
                        &non_container_child_fields,
                        &container_child_field_names,
                        &container_child_fields);
    DCHECK_EQ(non_container_child_field_names.size(), non_container_child_fields.size());
    DCHECK_EQ(container_child_field_names.size(), container_child_field_names.size());
    DCHECK(!is_root || parent_prefix.empty());
    DCHECK(!is_root || is_last);

    std::string ret = getHeadString(is_root, is_last, parent_prefix, node_name, inline_field_names, inline_field_values,
                                    true /* multi_line */);
    ret.append("\n");

    std::string child_prefix;
    if (!is_root) {
      child_prefix = GetNewIndentedStringPrefix(is_last, parent_prefix);
    }

    // Append strings of non_container child fields.
    for (int i = 0; i < static_cast<int>(non_container_child_fields.size()) - 1; ++i) {
      ret.append(
          non_container_child_fields[i]->getNodeString(false /* is_root */,
                                                       false /* is_last */,
                                                       child_prefix,
                                                       non_container_child_field_names[i]));
    }

    if (!non_container_child_fields.empty()) {
      if (container_child_fields.empty()) {
        ret.append(
            non_container_child_fields.back()->getNodeString(false /* is_root */,
                                                             true /* is_last */,
                                                             child_prefix,
                                                             non_container_child_field_names.back()));
      } else {
        ret.append(
            non_container_child_fields.back()->getNodeString(false /* is_root */,
                                                             false /* is_last */,
                                                             child_prefix,
                                                             non_container_child_field_names.back()));
      }
    }

    // Append strings of container child fields.
    for (int i = 0; i < static_cast<int>(container_child_fields.size()) - 1; ++i) {
      ret.append(GetNodeListString(false /* is_last */,
                                   child_prefix,
                                   container_child_field_names[i],
                                   container_child_fields[i]));
    }

    if (!container_child_fields.empty()) {
      ret.append(
          GetNodeListString(true /* is_last */,
                            child_prefix,
                            container_child_field_names.back(),
                            container_child_fields.back()));
    }

    return ret;
  }

  /**
   * @brief Creates a tree node with a list of child tree nodes:
   *            +-NodeList
   *              +- Item1
   *              +- Item2
   *              +- Item3
   *
   * @param is_last Whether this is the last child node of the parent.
   * @param prefix The initial prefix of this node.
   * @param node_name The node name.
   * @param node_list The list of nodes.
   * @return A tree node string with a list of .
   */
  static std::string GetNodeListString(bool is_last,
                                       const std::string &prefix,
                                       const std::string &node_name,
                                       const std::vector<TreeNodeType> &node_list) {
    std::string ret;
    std::string item_prefix = prefix;
    if (!node_name.empty()) {
      ret = item_prefix;
      ret.append("+-").append(node_name).append("=").append("\n");
      // Indent the child fields only if the node name is not empty.
      item_prefix = GetNewIndentedStringPrefix(is_last, prefix);
    }

    if (node_list.empty()) {
      ret.append(item_prefix).append("+-[]\n");
      return ret;
    }

    for (int i = 0; i < static_cast<int>(node_list.size()) - 1; ++i) {
      // Do not provide the node name so that there will not be an extra indent.
      ret.append(node_list[i]->getNodeString(false /* is_root */,
                                             false /* is_last */,
                                             item_prefix,
                                             "" /* node_name */));
    }
    if (!node_name.empty()) {
      ret.append(node_list.back()->getNodeString(false /* is_root */,
                                                 true /* is_last */,
                                                 item_prefix,
                                                 "" /* node_name */));
    } else {
      ret.append(node_list.back()->getNodeString(false /* is_root */,
                                                 (node_name.empty() ? is_last : true),
                                                 item_prefix, "" /* node_name */));
    }
    return ret;
  }

  /**
   * @brief Returns the node head string with the form like NodeName[inline_field=value1,inline_field=value2].
   *
   * @param is_root Whether this node is the root.
   * @param is_last Whether this node is the last child of its parent.
   * @param parent_prefix The prefix of its parent node.
   * @param node_name The node name in the parent node.
   * @param field_names The names of inline fields.
   * @param field_values The string values of the inline fields.
   * @param multi_line Whether the returned string is allowed to have multiple lines.
   * @return The node head string.
   */
  std::string getHeadString(bool is_root,
                            bool is_last,
                            const std::string &parent_prefix,
                            const std::string &node_name,
                            const std::vector<std::string> &field_names,
                            const std::vector<std::string> &field_values,
                            bool multi_line) const {
    DCHECK_EQ(field_names.size(), field_values.size());
    std::string ret;

    const std::string name = getName();
    std::string prefix = GetNewIndentedStringPrefix(is_last, parent_prefix);
    std::string current_line = parent_prefix;
    if (!is_root) {
      current_line.append("+-");
    }

    if (!node_name.empty()) {
      current_line.append(node_name).append("=");
      if (current_line.size() + name.size() > kMaxLineWidth && multi_line) {
        ret.append(current_line).append("\n");
        current_line = prefix;
      }
    }

    current_line.append(name);

    if (!field_names.empty()) {
      // "[" and "]" are considered as inline fields
      // so that we can add newlines for them.
      AppendInlineString(prefix, "[" /* inline_field */, multi_line, &current_line, &ret);
      for (size_t i = 0; i < field_names.size(); ++i) {
        std::string inline_field = field_names[i];
        inline_field.append("=").append(field_values[i]);
        if (i < field_names.size() - 1) {
          inline_field.append(",");
        }
        AppendInlineString(prefix, inline_field, multi_line, &current_line, &ret);
      }
      AppendInlineString(prefix, "]" /* inline_field */, false /* multi_line */, &current_line, &ret);
    }

    if (!current_line.empty()) {
      ret.append(current_line);
    }

    return ret;
  }

  /**
   * @brief Appends an inline field to \p output if the length of the current line is greater than MAX_LINE_WIDTH.
   *        Otherwise updates \p current_line.
   */
  static void AppendInlineString(const std::string &prefix,
                                 const std::string &inline_field,
                                 bool multi_line,
                                 std::string *current_line,
                                 std::string *output) {
    if (current_line->size() + inline_field.size() > kMaxLineWidth && multi_line) {
      output->append(*current_line).append("\n");
      *current_line = prefix;
    }
    current_line->append(inline_field);
  }

  /**
   * @brief Returns a new prefix string indented by 2.
   *
   * @param is_last Whether this node is the last child of the parent.
   * @param prefix The current prefix.
   * @return A new prefix string indented by 2.
   */
  static std::string GetNewIndentedStringPrefix(bool is_last, const std::string &prefix) {
    std::string new_prefix(prefix);
    if (!is_last) {
      new_prefix.append("|").append(" ");
    } else {
      new_prefix.append(2, ' ');
    }
    return new_prefix;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(TreeStringSerializable);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_UTILITY_TREE_STRING_SERIALIZABLE_HPP_ */
