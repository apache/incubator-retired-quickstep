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

#include "cli/PrintToScreen.hpp"

#include <cstddef>
#include <cstdio>
#include <cmath>
#include <memory>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"

using std::fprintf;
using std::fputc;
using std::size_t;
using std::vector;

namespace quickstep {

DEFINE_bool(printing_enabled, true,
            "If true, print query results to screen normally. If false, skip "
            "printing output (e.g. for benchmarking).");

int PrintToScreen::GetNumberOfDigits(int number) {
  if (number > 0) {
    return static_cast<int>(log10 (number)) + 1;
  } else if (number < 0) {
    return static_cast<int>(log10 ( abs(number) )) + 2;
  } else {
    return 1;
  }
}

void PrintToScreen::PrintRelation(const CatalogRelation &relation,
                                  StorageManager *storage_manager,
                                  FILE *out) {
  if (!FLAGS_printing_enabled) {
    return;
  }

  vector<int> column_widths;
  column_widths.reserve(relation.size());

  for (CatalogRelation::const_iterator attr_it = relation.begin();
       attr_it != relation.end();
       ++attr_it) {
    // Printed column needs to be wide enough to print:
    //   1. The attribute name (in the printed "header").
    //   2. Any value of the attribute's Type.
    //   3. If the attribute's Type is nullable, the 4-character string "NULL".
    // We pick the largest of these 3 widths as the column width.
    int column_width = static_cast<int>(attr_it->getDisplayName().length());
    column_width = column_width < attr_it->getType().getPrintWidth()
                   ? attr_it->getType().getPrintWidth()
                   : column_width;
    column_width = attr_it->getType().isNullable() && (column_width < 4)
                   ? 4
                   : column_width;
    column_widths.push_back(column_width);
  }

  printHBar(column_widths, out);

  fputc('|', out);
  vector<int>::const_iterator width_it = column_widths.begin();
  CatalogRelation::const_iterator attr_it = relation.begin();
  for (; width_it != column_widths.end(); ++width_it, ++attr_it) {
    fprintf(out,
            "%-*s|",
            *width_it,
            attr_it->getDisplayName().c_str());
  }
  fputc('\n', out);

  printHBar(column_widths, out);

  std::vector<block_id> blocks = relation.getBlocksSnapshot();
  for (const block_id current_block_id : blocks) {
    BlockReference block = storage_manager->getBlock(current_block_id, relation);
    const TupleStorageSubBlock &tuple_store = block->getTupleStorageSubBlock();

    if (tuple_store.isPacked()) {
      for (tuple_id tid = 0; tid <= tuple_store.getMaxTupleID(); ++tid) {
        printTuple(tuple_store, tid, column_widths, out);
      }
    } else {
      std::unique_ptr<TupleIdSequence> existence_map(tuple_store.getExistenceMap());
      for (tuple_id tid : *existence_map) {
        printTuple(tuple_store, tid, column_widths, out);
      }
    }
  }

  printHBar(column_widths, out);
}

void PrintToScreen::printHBar(const vector<int> &column_widths,
                              FILE *out) {
  fputc('+', out);
  for (const int width : column_widths) {
    for (int i = 0; i < width; ++i) {
      fputc('-', out);
    }
    fputc('+', out);
  }
  fputc('\n', out);
}

void PrintToScreen::printTuple(const TupleStorageSubBlock &tuple_store,
                               const tuple_id tid,
                               const vector<int> &column_widths,
                               FILE *out) {
  DEBUG_ASSERT(tuple_store.hasTupleWithID(tid));
  fputc('|', out);

  const CatalogRelationSchema &relation = tuple_store.getRelation();
  vector<int>::const_iterator width_it = column_widths.begin();
  CatalogRelation::const_iterator attr_it = relation.begin();
  for (; attr_it != relation.end(); ++attr_it, ++width_it) {
    TypedValue value(tuple_store.getAttributeValueTyped(tid, attr_it->getID()));
    if (value.isNull()) {
      fprintf(out,
              "%*s",
              *width_it,
              "NULL");
    } else {
      attr_it->getType().printValueToFile(value, out, *width_it);
    }

    fputc('|', out);
  }
  fputc('\n', out);
}

std::size_t PrintToScreen::GetNumTuplesInRelation(
    const CatalogRelation &relation, StorageManager *storage_manager) {
  const std::vector<block_id> &blocks = relation.getBlocksSnapshot();
  std::size_t total_num_tuples = 0;
  for (block_id block : blocks) {
    total_num_tuples +=
        storage_manager->getBlock(block, relation)->getNumTuples();
  }
  return total_num_tuples;
}

void PrintToScreen::PrintOutputSize(const CatalogRelation &relation,
                                    StorageManager *storage_manager,
                                    FILE *out) {
  const std::size_t num_rows = GetNumTuplesInRelation(relation, storage_manager);
  fprintf(out,
          "(%lu %s)\n",
          num_rows,
          (num_rows == 1) ? "row" : "rows");
}

}  // namespace quickstep
