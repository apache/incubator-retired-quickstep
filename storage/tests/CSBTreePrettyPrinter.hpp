/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_TESTS_CSBTREE_PRETTY_PRINTER_HPP_
#define QUICKSTEP_STORAGE_TESTS_CSBTREE_PRETTY_PRINTER_HPP_

#include <cstdint>
#include <iostream>

#include "utility/Macros.hpp"

namespace quickstep {

class CSBTreeIndexSubBlock;
class CSBTreePrettyPrinter;

std::ostream& operator<<(std::ostream &out, const CSBTreePrettyPrinter &printer);

class CSBTreePrettyPrinter {
 public:
  explicit CSBTreePrettyPrinter(const CSBTreeIndexSubBlock &csbtree_index)
      : csbtree_index_(csbtree_index) {
  }

 private:
  void putToStream(std::ostream *stream) const;

  void printNode(const int node_group_number,
                 const uint16_t node_number,
                 const int node_depth,
                 std::ostream *stream) const;
  void printKey(const void* key, std::ostream *stream) const;
  static void advanceLine(const int depth, std::ostream *stream);

  const CSBTreeIndexSubBlock &csbtree_index_;

  friend std::ostream& operator<<(std::ostream &out, const CSBTreePrettyPrinter &printer);

  DISALLOW_COPY_AND_ASSIGN(CSBTreePrettyPrinter);
};

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TESTS_CSBTREE_PRETTY_PRINTER_HPP_
