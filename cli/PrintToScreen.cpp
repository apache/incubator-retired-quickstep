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

#include "cli/PrintToScreen.hpp"

#include <cstddef>
#include <cstdio>
#include <memory>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/Type.hpp"
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


void PrintToScreen::computeColumnWidths() {
  column_widths_.reserve(relation_.size());
  for (CatalogRelation::const_iterator attr_it = relation_.begin();
       attr_it != relation_.end();
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
    column_widths_.push_back(column_width);
  }
}

void PrintToScreen::printRelation(FILE *out) {

	if (!FLAGS_printing_enabled) {
	    return;
	  }

	  printHeader(out);

	  std::vector<block_id> blocks = relation_.getBlocksSnapshot();
	  for (const block_id current_block_id : blocks) {
	    printBlock(current_block_id, out);
	  }

	  printHBar(out);
}


void PrintToScreen::printHeader(FILE *out) const {
  if (!FLAGS_printing_enabled) {
    return;
  }

  printHBar(out);

  fputc('|', out);
  vector<int>::const_iterator width_it = column_widths_.begin();
  CatalogRelation::const_iterator attr_it = relation_.begin();
  for (; width_it != column_widths_.end(); ++width_it, ++attr_it) {
    fprintf(out, "%-*s|", *width_it, attr_it->getDisplayName().c_str());
  }
  fputc('\n', out);

  printHBar(out);
}

void PrintToScreen::printBlock(block_id bid, FILE *out) {
  if (!FLAGS_printing_enabled) {
    return;
  }

  BlockReference block = storage_manager_->getBlock(bid, relation_);
  const TupleStorageSubBlock &tuple_store = block->getTupleStorageSubBlock();

  if (tuple_store.isPacked()) {
    for (tuple_id tid = 0; tid <= tuple_store.getMaxTupleID(); ++tid) {
      printTuple(tuple_store, tid, out);
    }
  } else {
    std::unique_ptr<TupleIdSequence> existence_map(
        tuple_store.getExistenceMap());
    for (tuple_id tid : *existence_map) {
      printTuple(tuple_store, tid, out);
    }
  }
}




void PrintToScreen::printHBar(FILE *out) const {
  fputc('+', out);
  for (const int width : column_widths_) {
    for (int i = 0; i < width; ++i) {
      fputc('-', out);
    }
    fputc('+', out);
  }
  fputc('\n', out);
}

void PrintToScreen::printTuple(const TupleStorageSubBlock &tuple_store,
                               const tuple_id tid,
                               FILE *out) const {
  DEBUG_ASSERT(tuple_store.hasTupleWithID(tid));
  fputc('|', out);

  const CatalogRelationSchema &relation = tuple_store.getRelation();
  vector<int>::const_iterator width_it = column_widths_.begin();
  CatalogRelation::const_iterator attr_it = relation.begin();
  for (; attr_it != relation.end(); ++attr_it, ++width_it) {
    TypedValue value(
        tuple_store.getAttributeValueTyped(tid, attr_it->getID()));
    if (value.isNull()) {
      fprintf(out, "%*s", *width_it, "NULL");
    } else {
      attr_it->getType().printValueToFile(value, out, *width_it);
    }

    fputc('|', out);
  }
  fputc('\n', out);
}

}  // namespace quickstep
