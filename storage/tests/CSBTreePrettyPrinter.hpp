/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
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
