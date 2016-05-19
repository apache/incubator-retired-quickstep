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

#ifndef QUICKSTEP_CLI_COMMAND_COMMAND_EXECUTOR_HPP_
#define QUICKSTEP_CLI_COMMAND_COMMAND_EXECUTOR_HPP_

#include <cstdio>
#include <string>

using std::fprintf;
using std::fputc;
using std::string;

namespace quickstep {

class CatalogDatabase;
class Foreman;
class ParseStatement;
class QueryProcessor;
class StorageManager;

namespace cli {

/** \addtogroup CLI
 *  @{
 */

// Adding the max column width as 6  as the default initializer
// as the length of the word Column is 6 characters.
// This is used while describing the table.
constexpr int kInitMaxColumnWidth = 6;

constexpr char kDescribeDatabaseCommand[] = "\\dt";
constexpr char kDescribeTableCommand[] = "\\d";
constexpr char kAnalyzeCommand[] = "\\analyze";

/**
  * @brief Executes the command by calling the command handler.
  *
  * @param statement The parsed statement from the cli.
  * @param catalog_database The catalog information about the current database.
  * @param storage_manager The current StorageManager.
  * @param query_processor The query processor to generate plans for SQL queries.
  * @param foreman The foreman to execute query plans.
  * @param out The stream where the output of the command has to be redirected to.
*/
void executeCommand(const ParseStatement &statement,
                    const CatalogDatabase &catalog_database,
                    StorageManager *storage_manager,
                    QueryProcessor *query_processor,
                    Foreman *foreman,
                    FILE *out);

/** @} */

}  // namespace cli
}  // namespace quickstep

#endif  // QUICKSTEP_CLI_COMMAND_COMMAND_EXECUTOR_HPP_
