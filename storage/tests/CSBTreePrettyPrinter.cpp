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

#include "storage/tests/CSBTreePrettyPrinter.hpp"

#include <cstdint>
#include <iostream>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/CSBTreeIndexSubBlock.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"

using std::uint16_t;

namespace quickstep {

std::ostream& operator<<(std::ostream &out, const CSBTreePrettyPrinter &printer) {
  printer.putToStream(&out);
  return out;
}

void CSBTreePrettyPrinter::putToStream(std::ostream *stream) const {
  printNode(csbtree_index_.getRootNodeGroupNumber(), 0, 0, stream);
}

void CSBTreePrettyPrinter::printNode(const int node_group_number,
                                     const uint16_t node_number,
                                     const int node_depth,
                                     std::ostream *stream) const {
  advanceLine(node_depth, stream);
  const char *node_ptr = static_cast<const char*>(csbtree_index_.getNode(node_group_number, node_number));
  const CSBTreeIndexSubBlock::NodeHeader *node_header
      = reinterpret_cast<const CSBTreeIndexSubBlock::NodeHeader*>(node_ptr);
  *stream << "Node ";
  if (node_header->is_leaf) {
    *stream << "(leaf, ";
    *stream << "num_keys: " << node_header->num_keys;
    *stream << ", right_sibling: ";
    if (node_header->node_group_reference == CSBTreeIndexSubBlock::kNodeGroupNextLeaf) {
      *stream << "next in group";
    } else if (node_header->node_group_reference == CSBTreeIndexSubBlock::kNodeGroupNone) {
      *stream << "none";
    } else {
      *stream << node_header->node_group_reference;
    }
    *stream << ")\n";

    for (uint16_t key_num = 0;
         key_num < node_header->num_keys;
         ++key_num) {
      advanceLine(node_depth, stream);
      printKey(node_ptr
                   + sizeof(CSBTreeIndexSubBlock::NodeHeader)
                   + key_num * csbtree_index_.key_tuple_id_pair_length_bytes_,
               stream);
      *stream << " -> ";
      *stream << *reinterpret_cast<const tuple_id*>(node_ptr
                                                    + sizeof(CSBTreeIndexSubBlock::NodeHeader)
                                                    + key_num * csbtree_index_.key_tuple_id_pair_length_bytes_
                                                    + csbtree_index_.key_length_bytes_);
      *stream << "\n";
    }
  } else {
    *stream << "(internal, ";
    *stream << "num_keys: " << node_header->num_keys;
    *stream << ", child: " << node_header->node_group_reference;
    *stream << ")\n";

    printNode(node_header->node_group_reference,
              0,
              node_depth + 1,
              stream);
    for (uint16_t key_num = 0;
         key_num < node_header->num_keys;
         ++key_num) {
      advanceLine(node_depth, stream);
      *stream << "<";
      printKey(node_ptr
                   + sizeof(CSBTreeIndexSubBlock::NodeHeader)
                   + key_num * csbtree_index_.key_length_bytes_,
               stream);
      *stream << ">\n";
      printNode(node_header->node_group_reference,
                key_num + 1,
                node_depth + 1,
                stream);
    }
  }
}

void CSBTreePrettyPrinter::printKey(const void* key, std::ostream *stream) const {
  const char *key_ptr = static_cast<const char*>(key);
  for (std::vector<attribute_id>::const_iterator subkey_it = csbtree_index_.indexed_attribute_ids_.begin();
       subkey_it != csbtree_index_.indexed_attribute_ids_.end();
       ++subkey_it) {
    const Type &subkey_type = csbtree_index_.relation_.getAttributeById(*subkey_it)->getType();
    TypedValue typed_key = subkey_type.makeValue(key_ptr, csbtree_index_.key_length_bytes_);
    *stream << subkey_type.printValueToString(typed_key);
    if (subkey_it != csbtree_index_.indexed_attribute_ids_.end() - 1) {
      *stream << ", ";
    }
    key_ptr += subkey_type.maximumByteLength();
  }
}

void CSBTreePrettyPrinter::advanceLine(const int depth, std::ostream *stream) {
  for (int print_depth = 0; print_depth < depth; ++print_depth) {
    *stream << "    ";
  }
}

}  // namespace quickstep
