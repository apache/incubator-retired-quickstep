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
#include "catalog/CatalogRelationStatistics.hpp"
#include "cli/CommandExecutorUtil.hpp"
#include "cli/Constants.hpp"
#include "cli/DropRelation.hpp"
#include "cli/PrintToScreen.hpp"
#include "expressions/aggregation/AggregateFunctionMax.hpp"
#include "expressions/aggregation/AggregateFunctionMin.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/SqlParserWrapper.hpp"
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
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

using std::fprintf;
using std::fputc;
using std::fputs;
using std::size_t;
using std::string;
using std::vector;

namespace tmb { class MessageBus; }

namespace quickstep {
namespace cli {
namespace {

/**
 * @brief A helper function that executes a SQL query to obtain a row of results.
 */
inline std::vector<TypedValue> ExecuteQueryForSingleRow(
    const tmb::client_id main_thread_client_id,
    const tmb::client_id foreman_client_id,
    const std::string &query_string,
    tmb::MessageBus *bus,
    StorageManager *storage_manager,
    QueryProcessor *query_processor,
    SqlParserWrapper *parser_wrapper) {
  parser_wrapper->feedNextBuffer(new std::string(query_string));

  ParseResult result = parser_wrapper->getNextStatement();
  DCHECK_EQ(ParseResult::kSuccess, result.condition);

  const ParseStatement &statement = *result.parsed_statement;
  const CatalogRelation *query_result_relation = nullptr;

  {
    // Generate the query plan.
    auto query_handle =
        std::make_unique<QueryHandle>(query_processor->query_id(),
                                      main_thread_client_id,
                                      statement.getPriority());
    query_processor->generateQueryHandle(statement, query_handle.get());
    DCHECK(query_handle->getQueryPlanMutable() != nullptr);
    query_result_relation = query_handle->getQueryResultRelation();
    DCHECK(query_result_relation != nullptr);

    // Use foreman to execute the query plan.
    QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
        main_thread_client_id, foreman_client_id, query_handle.release(), bus);
  }

  QueryExecutionUtil::ReceiveQueryCompletionMessage(main_thread_client_id, bus);

  // Retrieve the scalar result from the result relation.
  std::vector<TypedValue> values;
  {
    std::vector<block_id> blocks = query_result_relation->getBlocksSnapshot();
    DCHECK_EQ(1u, blocks.size());

    BlockReference block = storage_manager->getBlock(blocks[0], *query_result_relation);
    const TupleStorageSubBlock &tuple_store = block->getTupleStorageSubBlock();
    DCHECK_EQ(1, tuple_store.numTuples());

    const std::size_t num_columns = tuple_store.getRelation().size();
    if (tuple_store.isPacked()) {
      for (std::size_t i = 0; i < num_columns; ++i) {
        values.emplace_back(tuple_store.getAttributeValueTyped(0, i));
        values[i].ensureNotReference();
      }
    } else {
      std::unique_ptr<TupleIdSequence> existence_map(tuple_store.getExistenceMap());
      for (std::size_t i = 0; i < num_columns; ++i) {
        values.emplace_back(
            tuple_store.getAttributeValueTyped(*existence_map->begin(), i));
        values[i].ensureNotReference();
      }
    }
  }

  // Drop the result relation.
  DropRelation::Drop(*query_result_relation,
                     query_processor->getDefaultDatabase(),
                     storage_manager);

  return values;
}

/**
 * @brief A helper function that executes a SQL query to obtain a scalar result.
 */
inline TypedValue ExecuteQueryForSingleResult(
    const tmb::client_id main_thread_client_id,
    const tmb::client_id foreman_client_id,
    const std::string &query_string,
    tmb::MessageBus *bus,
    StorageManager *storage_manager,
    QueryProcessor *query_processor,
    SqlParserWrapper *parser_wrapper) {
  std::vector<TypedValue> results =
      ExecuteQueryForSingleRow(main_thread_client_id,
                               foreman_client_id,
                               query_string,
                               bus,
                               storage_manager,
                               query_processor,
                               parser_wrapper);
  DCHECK_EQ(1u, results.size());
  return results[0];
}

void ExecuteAnalyze(const PtrVector<ParseString> &arguments,
                    const tmb::client_id main_thread_client_id,
                    const tmb::client_id foreman_client_id,
                    MessageBus *bus,
                    StorageManager *storage_manager,
                    QueryProcessor *query_processor,
                    FILE *out) {
  const CatalogDatabase &database = *query_processor->getDefaultDatabase();

  std::unique_ptr<SqlParserWrapper> parser_wrapper(new SqlParserWrapper());
  std::vector<std::reference_wrapper<const CatalogRelation>> relations;
  if (arguments.empty()) {
    relations.insert(relations.begin(), database.begin(), database.end());
  } else {
    for (const auto &rel_name : arguments) {
      const CatalogRelation *rel = database.getRelationByName(rel_name.value());
      if (rel == nullptr) {
        THROW_SQL_ERROR_AT(&rel_name) << "Table does not exist";
      } else {
        relations.emplace_back(*rel);
      }
    }
  }

  // Analyze each relation in the database.
  for (const CatalogRelation &relation : relations) {
    fprintf(out, "Analyzing %s ... ", relation.getName().c_str());
    fflush(out);

    CatalogRelation *mutable_relation =
        query_processor->getDefaultDatabase()->getRelationByIdMutable(relation.getID());
    CatalogRelationStatistics *mutable_stat =
        mutable_relation->getStatisticsMutable();

    const std::string rel_name = EscapeQuotes(relation.getName(), '"');

    // Get the number of distinct values for each column.
    for (const CatalogAttribute &attribute : relation) {
      const std::string attr_name = EscapeQuotes(attribute.getName(), '"');
      const Type &attr_type = attribute.getType();
      bool is_min_applicable =
          AggregateFunctionMin::Instance().canApplyToTypes({&attr_type});
      bool is_max_applicable =
          AggregateFunctionMax::Instance().canApplyToTypes({&attr_type});

      // NOTE(jianqiao): Note that the relation name and the attribute names may
      // contain non-letter characters, e.g. CREATE TABLE "with space"("1" int).
      // So here we need to format the names with double quotes (").
      std::string query_string = "SELECT COUNT(DISTINCT \"";
      query_string.append(attr_name);
      query_string.append("\")");
      if (is_min_applicable) {
        query_string.append(", MIN(\"");
        query_string.append(attr_name);
        query_string.append("\")");
      }
      if (is_max_applicable) {
        query_string.append(", MAX(\"");
        query_string.append(attr_name);
        query_string.append("\")");
      }
      query_string.append(" FROM \"");
      query_string.append(rel_name);
      query_string.append("\";");

      std::vector<TypedValue> results =
          ExecuteQueryForSingleRow(main_thread_client_id,
                                   foreman_client_id,
                                   query_string,
                                   bus,
                                   storage_manager,
                                   query_processor,
                                   parser_wrapper.get());

      auto results_it = results.begin();
      DCHECK_EQ(TypeID::kLong, results_it->getTypeID());

      const attribute_id attr_id = attribute.getID();
      mutable_stat->setNumDistinctValues(attr_id,
                                         results_it->getLiteral<std::int64_t>());
      if (is_min_applicable) {
        ++results_it;
        mutable_stat->setMinValue(attr_id, *results_it);
      }
      if (is_max_applicable) {
        ++results_it;
        mutable_stat->setMaxValue(attr_id, *results_it);
      }
    }

    // Get the number of tuples for the relation.
    std::string query_string = "SELECT COUNT(*) FROM \"";
    query_string.append(rel_name);
    query_string.append("\";");

    TypedValue num_tuples =
        ExecuteQueryForSingleResult(main_thread_client_id,
                                    foreman_client_id,
                                    query_string,
                                    bus,
                                    storage_manager,
                                    query_processor,
                                    parser_wrapper.get());

    DCHECK_EQ(TypeID::kLong, num_tuples.getTypeID());
    mutable_stat->setNumTuples(num_tuples.getLiteral<std::int64_t>());

    mutable_stat->setExactness(true);

    fprintf(out, "done\n");
    fflush(out);
  }
  query_processor->markCatalogAltered();
  query_processor->saveCatalog();
}

}  // namespace

void executeCommand(const ParseStatement &statement,
                    const CatalogDatabase &catalog_database,
                    const tmb::client_id main_thread_client_id,
                    const tmb::client_id foreman_client_id,
                    MessageBus *bus,
                    StorageManager *storage_manager,
                    QueryProcessor *query_processor,
                    FILE *out) {
  const ParseCommand &command = static_cast<const ParseCommand &>(statement);
  const PtrVector<ParseString> &arguments = *(command.arguments());
  const std::string &command_str = command.command()->value();
  if (command_str == kDescribeDatabaseCommand) {
    const string database_description = ExecuteDescribeDatabase(arguments, catalog_database);
    fprintf(out, "%s", database_description.c_str());
  } else if (command_str == kDescribeTableCommand) {
    if (arguments.empty()) {
      const string database_description = ExecuteDescribeDatabase(arguments, catalog_database);
      fprintf(out, "%s", database_description.c_str());
    } else {
      const string table_description = ExecuteDescribeTable(arguments, catalog_database);
      fprintf(out, "%s", table_description.c_str());
    }
  } else if (command_str == kAnalyzeCommand) {
    ExecuteAnalyze(arguments,
                   main_thread_client_id,
                   foreman_client_id,
                   bus,
                   storage_manager,
                   query_processor, out);
  } else {
    THROW_SQL_ERROR_AT(command.command()) << "Invalid Command";
  }
}

}  // namespace cli
}  // namespace quickstep
