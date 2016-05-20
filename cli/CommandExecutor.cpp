/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "cli/CommandExecutor.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/Foreman.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryPlan.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleIdSequence.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/Type.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SqlError.hpp"

#include "glog/logging.h"

using std::fprintf;
using std::fputc;
using std::fputs;
using std::size_t;
using std::string;
using std::vector;

namespace quickstep {
namespace cli {
namespace {

namespace C = ::quickstep::cli;

void executeDescribeDatabase(
    const PtrVector<ParseString> *arguments,
    const CatalogDatabase &catalog_database,
    StorageManager *storage_manager,
    FILE *out) {
  // Column width initialized to 6 to take into account the header name
  // and the column value table
  int max_column_width = C::kInitMaxColumnWidth;
  vector<std::size_t> num_tuples;
  vector<std::size_t> num_blocks;
  const CatalogRelation *relation = nullptr;
  if (arguments->size() == 0) {
    for (const CatalogRelation &rel : catalog_database) {
      max_column_width =
          std::max(static_cast<int>(rel.getName().length()), max_column_width);
      num_blocks.push_back(rel.size_blocks());
      num_tuples.push_back(
          PrintToScreen::GetNumTuplesInRelation(rel, storage_manager));
    }
  } else {
    const ParseString &table_name = arguments->front();
    const std::string &table_name_val = table_name.value();
    relation = catalog_database.getRelationByName(table_name_val);

    if (relation == nullptr) {
      THROW_SQL_ERROR_AT(&(arguments->front())) << " Unrecognized relation " << table_name_val;
    }
    max_column_width = std::max(static_cast<int>(relation->getName().length()),
                                    max_column_width);
    num_blocks.push_back(relation->size_blocks());
   num_tuples.push_back(PrintToScreen::GetNumTuplesInRelation(
        *relation,
        storage_manager));
  }
  // Only if we have relations work on the printing logic.
  if (catalog_database.size() > 0) {
    const std::size_t max_num_blocks = *std::max_element(num_blocks.begin(), num_blocks.end());
    const std::size_t max_num_rows = *std::max_element(num_tuples.begin(), num_tuples.end());
    const int max_num_rows_digits = std::max(PrintToScreen::GetNumberOfDigits(max_num_rows),
                                      C::kInitMaxColumnWidth);
    const int max_num_blocks_digits = std::max(PrintToScreen::GetNumberOfDigits(max_num_blocks),
                                      C::kInitMaxColumnWidth+2);

    vector<int> column_widths;
    column_widths.push_back(max_column_width +1);
    column_widths.push_back(C::kInitMaxColumnWidth + 1);
    column_widths.push_back(max_num_blocks_digits + 1);
    column_widths.push_back(max_num_rows_digits + 1);
    fputs("       List of relations\n\n", out);
    fprintf(out, "%-*s |", max_column_width+1, " Name");
    fprintf(out, "%-*s |", C::kInitMaxColumnWidth, " Type");
    fprintf(out, "%-*s |", max_num_blocks_digits, " Blocks");
    fprintf(out, "%-*s\n", max_num_rows_digits, " Rows");
    PrintToScreen::printHBar(column_widths, out);
    //  If there are no argument print the entire list of tables
    //  else print the particular table only.
    vector<std::size_t>::const_iterator num_tuples_it = num_tuples.begin();
    vector<std::size_t>::const_iterator num_blocks_it = num_blocks.begin();
    if (arguments->size() == 0) {
      for (const CatalogRelation &rel : catalog_database) {
        fprintf(out, " %-*s |", max_column_width, rel.getName().c_str());
        fprintf(out, " %-*s |", C::kInitMaxColumnWidth - 1, "table");
        fprintf(out, " %-*lu |", max_num_blocks_digits - 1, *num_blocks_it);
        fprintf(out, " %-*lu\n", max_num_rows_digits - 1, *num_tuples_it);
        ++num_tuples_it;
        ++num_blocks_it;
      }
    } else {
      fprintf(out, " %-*s |", max_column_width, relation->getName().c_str());
      fprintf(out, " %-*s |", C::kInitMaxColumnWidth -1, "table");
      fprintf(out, " %-*lu |", max_num_blocks_digits - 1, *num_blocks_it);
      fprintf(out, " %-*lu\n", max_num_rows_digits - 1, *num_tuples_it);
      ++num_tuples_it;
      ++num_blocks_it;
    }
    fputc('\n', out);
  }
}

void executeDescribeTable(
    const PtrVector<ParseString> *arguments,
    const CatalogDatabase &catalog_database, FILE *out) {
  const ParseString &table_name = arguments->front();
  const std::string &table_name_val = table_name.value();
  const CatalogRelation *relation =
      catalog_database.getRelationByName(table_name_val);
  if (relation == nullptr) {
    THROW_SQL_ERROR_AT(&(arguments->front())) << " Unrecognized relation "  << table_name_val;
  }
  vector<int> column_widths;
  int max_attr_column_width = C::kInitMaxColumnWidth;
  int max_type_column_width = C::kInitMaxColumnWidth;

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
  // Add room for one extra character to allow spacing between the column ending and the vertical bar
  column_widths.push_back(max_attr_column_width+1);
  column_widths.push_back(max_type_column_width+1);

  fprintf(out, "%*s \"%s\"\n", C::kInitMaxColumnWidth, "Table", table_name_val.c_str());
  fprintf(out, "%-*s |", max_attr_column_width+1, " Column");
  fprintf(out, "%-*s\n", max_type_column_width+1, " Type");
  PrintToScreen::printHBar(column_widths, out);
  for (const CatalogAttribute &attr : *relation) {
    fprintf(out, " %-*s |", max_attr_column_width,
            attr.getDisplayName().c_str());
    fprintf(out, " %-*s\n", max_type_column_width,
            attr.getType().getName().c_str());
  }
  // TODO(rogers): Add handlers for partitioning information.
  if (relation->hasIndexScheme()) {
    fprintf(out, "%*s\n", C::kInitMaxColumnWidth+2, " Indexes");
    const quickstep::IndexScheme &index_scheme = relation->getIndexScheme();
    for (auto index_it = index_scheme.begin(); index_it != index_scheme.end();
         ++index_it) {
      fprintf(out, "  \"%-*s\" %s", static_cast<int>(index_it->first.length()),
              index_it->first.c_str(),
              index_it->second.IndexSubBlockType_Name(
                  index_it->second.sub_block_type()).c_str());
      fputc(' ', out);
      fputc('(', out);
      fprintf(out, "%s", relation->getAttributeById(index_it->second.indexed_attribute_ids(0))
                             ->getDisplayName().c_str());
      for (std::size_t i = 1; i < static_cast<std::size_t>(index_it->second.indexed_attribute_ids_size()); ++i) {
        const char *attribute_display_name = relation->getAttributeById(
                                                 index_it->second.indexed_attribute_ids(i))
                                                     ->getDisplayName().c_str();
        fprintf(out, ", %s", attribute_display_name);
      }
      fputc(')', out);
      fputc('\n', out);
    }
  }
}

/**
 * @brief A helper function that executes a SQL query to obtain a scalar result.
 */
inline TypedValue executeQueryForSingleResult(const std::string &query_string,
                                               StorageManager *storage_manager,
                                               QueryProcessor *query_processor,
                                               SqlParserWrapper *parser_wrapper,
                                               Foreman *foreman) {
  parser_wrapper->feedNextBuffer(new std::string(query_string));

  ParseResult result = parser_wrapper->getNextStatement();
  DCHECK(result.condition == ParseResult::kSuccess);

  // Generate the query plan.
  std::unique_ptr<QueryHandle> query_handle(
      query_processor->generateQueryHandle(*result.parsed_statement));
  DCHECK(query_handle->getQueryPlanMutable() != nullptr);

  // Use foreman to execute the query plan.
  foreman->setQueryPlan(query_handle->getQueryPlanMutable()->getQueryPlanDAGMutable());
  foreman->reconstructQueryContextFromProto(query_handle->getQueryContextProto());

  foreman->start();
  foreman->join();

  // Retrieve the scalar result from the result relation.
  const CatalogRelation *query_result_relation = query_handle->getQueryResultRelation();
  DCHECK(query_result_relation != nullptr);

  TypedValue value;
  {
    std::vector<block_id> blocks = query_result_relation->getBlocksSnapshot();
    DCHECK_EQ(1u, blocks.size());
    BlockReference block = storage_manager->getBlock(blocks[0], *query_result_relation);
    const TupleStorageSubBlock &tuple_store = block->getTupleStorageSubBlock();
    DCHECK_EQ(1, tuple_store.numTuples());
    DCHECK_EQ(1u, tuple_store.getRelation().size());

    if (tuple_store.isPacked()) {
      value = tuple_store.getAttributeValueTyped(0, 0);
    } else {
      std::unique_ptr<TupleIdSequence> existence_map(tuple_store.getExistenceMap());
      value = tuple_store.getAttributeValueTyped(*existence_map->begin(), 0);
    }
    value.ensureNotReference();
  }

  // Drop the result relation.
  DropRelation::Drop(*query_result_relation,
                     query_processor->getDefaultDatabase(),
                     query_processor->getStorageManager());

  return value;
}

void executeAnalyze(QueryProcessor *query_processor,
                    Foreman *foreman,
                    FILE *out) {
  const CatalogDatabase &database = *query_processor->getDefaultDatabase();
  StorageManager *storage_manager = query_processor->getStorageManager();

  std::unique_ptr<SqlParserWrapper> parser_wrapper(new SqlParserWrapper());
  std::vector<std::reference_wrapper<const CatalogRelation>> relations(
      database.begin(), database.end());

  // Analyze each relation in the database.
  for (const CatalogRelation &relation : relations) {
    fprintf(out, "Analyzing %s ... ", relation.getName().c_str());
    fflush(out);

    CatalogRelation *mutable_relation =
        query_processor->getDefaultDatabase()->getRelationByIdMutable(relation.getID());

    // Get the number of distinct values for each column.
    for (const CatalogAttribute &attribute : relation) {
      std::string query_string = "SELECT COUNT(DISTINCT ";
      query_string.append(attribute.getName());
      query_string.append(") FROM ");
      query_string.append(relation.getName());
      query_string.append(";");

      TypedValue num_distinct_values =
          executeQueryForSingleResult(query_string,
                                      storage_manager,
                                      query_processor,
                                      parser_wrapper.get(),
                                      foreman);

      DCHECK(num_distinct_values.getTypeID() == TypeID::kLong);
      mutable_relation->getStatisticsMutable()->setNumDistinctValues(
          attribute.getID(),
          num_distinct_values.getLiteral<std::int64_t>());
    }

    // Get the number of tuples for the relation.
    std::string query_string = "SELECT COUNT(*) FROM ";
    query_string.append(relation.getName());
    query_string.append(";");

    TypedValue num_tuples =
        executeQueryForSingleResult(query_string,
                                    storage_manager,
                                    query_processor,
                                    parser_wrapper.get(),
                                    foreman);

    DCHECK(num_tuples.getTypeID() == TypeID::kLong);
    mutable_relation->getStatisticsMutable()->setNumTuples(
        num_tuples.getLiteral<std::int64_t>());

    fprintf(out, "done\n");
    fflush(out);
  }
  query_processor->markCatalogAltered();
  query_processor->saveCatalog();
}

}  // namespace

void executeCommand(const ParseStatement &statement,
                    const CatalogDatabase &catalog_database,
                    StorageManager *storage_manager,
                    QueryProcessor *query_processor,
                    Foreman *foreman,
                    FILE *out) {
  const ParseCommand &command = static_cast<const ParseCommand &>(statement);
  const PtrVector<ParseString> *arguments = command.arguments();
  const std::string &command_str = command.command()->value();
  if (command_str == C::kDescribeDatabaseCommand) {
    executeDescribeDatabase(arguments, catalog_database, storage_manager, out);
  } else if (command_str == C::kDescribeTableCommand) {
    if (arguments->size() == 0) {
      executeDescribeDatabase(arguments, catalog_database, storage_manager, out);
    } else {
      executeDescribeTable(arguments, catalog_database, out);
    }
  } else if (command_str == C::kAnalyzeCommand) {
    executeAnalyze(query_processor, foreman, out);
  } else {
    THROW_SQL_ERROR_AT(command.command()) << "Invalid Command";
  }
}
}  // namespace cli
}  // namespace quickstep
