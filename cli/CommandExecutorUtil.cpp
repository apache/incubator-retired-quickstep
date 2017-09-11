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

#include "cli/CommandExecutorUtil.hpp"

#include <algorithm>
#include <cstddef>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/IndexScheme.hpp"
#include "catalog/PartitionScheme.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseString.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SqlError.hpp"
#include "utility/StringUtil.hpp"

using std::ostringstream;
using std::setw;
using std::size_t;
using std::string;
using std::vector;

namespace quickstep {
namespace cli {

namespace {

// Adding the max column width as 6 as the default initializer
// as the length of the word Column is 6 characters.
// This is used while describing the table.

constexpr int kInitMaxColumnWidth = 6;

}  // namespace

string ExecuteDescribeDatabase(
    const PtrVector<ParseString> &arguments,
    const CatalogDatabase &catalog_database) {
  // Column width initialized to 6 to take into account the header name
  // and the column value table
  int max_column_width = kInitMaxColumnWidth;
  vector<std::size_t> num_blocks;
  const CatalogRelation *relation = nullptr;
  if (arguments.empty()) {
    for (const CatalogRelation &rel : catalog_database) {
      max_column_width =
          std::max(static_cast<int>(rel.getName().length()), max_column_width);
      num_blocks.push_back(rel.size_blocks());
    }
  } else {
    const ParseString &table_name = arguments.front();
    const std::string &table_name_val = table_name.value();
    relation = catalog_database.getRelationByName(table_name_val);

    if (relation == nullptr) {
      THROW_SQL_ERROR_AT(&(arguments.front())) << " Unrecognized relation " << table_name_val;
    }
    max_column_width = std::max(static_cast<int>(relation->getName().length()),
                                    max_column_width);
    num_blocks.push_back(relation->size_blocks());
  }

  ostringstream oss;
  // Only if we have relations work on the printing logic.
  if (!catalog_database.empty()) {
    const std::size_t max_num_blocks = *std::max_element(num_blocks.begin(), num_blocks.end());
    const int max_num_blocks_digits = std::max(PrintToScreen::GetNumberOfDigits(max_num_blocks),
                                               kInitMaxColumnWidth + 2);

    oss << "       List of relations\n\n"
        << std::left << setw(max_column_width + 1) << " Name" << " |"
        << setw(kInitMaxColumnWidth) << " Type" << " |"
        << setw(max_num_blocks_digits) << " Blocks" << '\n'
        << PrintToScreen::GenerateHBar({ max_column_width + 1, kInitMaxColumnWidth + 1, max_num_blocks_digits + 1 });

    //  If there are no argument print the entire list of tables
    //  else print the particular table only.
    vector<std::size_t>::const_iterator num_blocks_it = num_blocks.begin();
    if (arguments.empty()) {
      for (const CatalogRelation &rel : catalog_database) {
        oss << ' ' << setw(max_column_width) << rel.getName() << " |"
            << setw(kInitMaxColumnWidth) << " table" << " | "
            << setw(max_num_blocks_digits - 1) << *num_blocks_it << '\n';
        ++num_blocks_it;
      }
    } else {
      oss << ' ' << setw(max_column_width) << relation->getName() << " |"
          << setw(kInitMaxColumnWidth) << " table" << " | "
          << setw(max_num_blocks_digits - 1) << *num_blocks_it << '\n';
      ++num_blocks_it;
    }
    oss << '\n';
  }

  return oss.str();
}

string ExecuteDescribeTable(
    const PtrVector<ParseString> &arguments,
    const CatalogDatabase &catalog_database) {
  const ParseString &table_name = arguments.front();
  const std::string &table_name_val = table_name.value();
  const CatalogRelation *relation =
      catalog_database.getRelationByName(table_name_val);
  if (relation == nullptr) {
    THROW_SQL_ERROR_AT(&(arguments.front())) << " Unrecognized relation "  << table_name_val;
  }

  int max_attr_column_width = kInitMaxColumnWidth;
  int max_type_column_width = kInitMaxColumnWidth;
  for (const CatalogAttribute &attr : *relation) {
    // Printed column needs to be wide enough to print:
    //   1. The attribute name (in the printed "header").
    //   2. Any value of the attribute's Type.
    max_attr_column_width =
        std::max(max_attr_column_width,
            static_cast<int>(attr.getDisplayName().length()));
    max_type_column_width =
        std::max(max_type_column_width,
            static_cast<int>(attr.getType().getName().length()));
  }

  ostringstream oss;
  oss << setw(kInitMaxColumnWidth) << "Table" << " \"" << table_name_val << "\"\n"
      << std::left << setw(max_attr_column_width + 1) << " Column" << " |"
      << setw(max_type_column_width + 1) << " Type" << '\n';

  // Add room for one extra character to allow spacing between the column ending and the vertical bar
  oss << PrintToScreen::GenerateHBar({ max_attr_column_width + 1, max_type_column_width + 1 });

  for (const CatalogAttribute &attr : *relation) {
    oss << ' ' << setw(max_attr_column_width) << attr.getDisplayName() << " | "
        << setw(max_type_column_width) << attr.getType().getName() << '\n';
  }

  if (relation->hasIndexScheme()) {
    oss << setw(kInitMaxColumnWidth + 2) << " Indexes" << '\n';
    for (const auto &index : relation->getIndexScheme()) {
      const IndexSubBlockDescription &index_description = index.second;
      oss << "  \"" << index.first << "\" "
          << index_description.IndexSubBlockType_Name(index_description.sub_block_type())
          << " ("
          << relation->getAttributeById(index_description.indexed_attribute_ids(0))->getDisplayName();
      for (int i = 1; i < index_description.indexed_attribute_ids_size(); ++i) {
        oss << ", " << relation->getAttributeById(index_description.indexed_attribute_ids(i))->getDisplayName();
      }
      oss << ")\n";
    }
  }

  if (relation->hasPartitionScheme()) {
    oss << setw(kInitMaxColumnWidth + 2) << " Partition Info\n  "
        << relation->getPartitionScheme()->toString(*relation);
  }

  return oss.str();
}

}  // namespace cli
}  // namespace quickstep
