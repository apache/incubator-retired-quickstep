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

#ifndef QUICKSTEP_PARSER_PARSE_COMMAND_HPP_
#define QUICKSTEP_PARSER_PARSE_COMMAND_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseTreeNode.hpp"
#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Abstract base class for all commands.
 * @details Commands are non-sql statements which can be issued to quickstep.
 *          They are entered into the CLI as '.command-name command-string\n'.
 */
class ParseCommand : public ParseStatement {
 public:
  /**
   * @brief The possible types of SQL statements.
   **/
  enum class CommandType {
    kHelp
  };

  /**
   * @brief All ParseCommands are ParseStatements of the type command.
   */
  StatementType getStatementType() const override {
    return kCommand;
  }

  /**
   * @brief Identify the type of this command.
   *
   * @return The type of this statement.
   **/
  virtual CommandType getCommandType() const = 0;

  /**
   * @brief Execute this command.
   */
  virtual void execute() = 0;

 protected:
  ParseCommand(const int line_number, const int column_number)
      : ParseStatement(line_number, column_number) {  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseCommand);
};

/**
 * @brief A help command.
 * @details Displays help messages to the user via standard out.
 * 
 */
class ParseHelpCommand : public ParseCommand {
  // A lazily-constructed map of help messages.
  // Maps SQL-keyword string (ex. "create"), to a description string.
  static std::map<const std::string, std::string> help_messages_;

 public:
  /**
   * @brief Constructor.
   * 
   * @param line_number Beginning line number.
   * @param column_number Beginning column number.
   * @param query Something which the user wishes to know more about.
   */
  ParseHelpCommand(int line_number,
                   int column_number,
                   ParseString *query)
      : ParseCommand(line_number, column_number),
        query_(query) { }

  /**
   * @return The name of this entity.
   */
  std::string getName() const override {
    return "HelpCommand";
  }

  /**
   * @return The type of this command.
   */
  CommandType getCommandType() const override {
    return ParseCommand::CommandType::kHelp;
  }

  /**
   * @return Returns a pointer to the user query.
   */
  const ParseString* getQuery() const {
    return query_.get();
  }

  /**
   * @brief Prints a help message to standard out.
   */
  void execute() override;

 protected:
  void getFieldStringItems(
       std::vector<std::string> *inline_field_names,
       std::vector<std::string> *inline_field_values,
       std::vector<std::string> *non_container_child_field_names,
       std::vector<const ParseTreeNode*> *non_container_child_fields,
       std::vector<std::string> *container_child_field_names,
       std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("query");
    inline_field_values->push_back(query_->value());
  }

 private:
  const std::map<const std::string, std::string>& getHelpMessageMap();

  std::unique_ptr<ParseString> query_;

  DISALLOW_COPY_AND_ASSIGN(ParseHelpCommand);
};

/** @} */

}  // namespace quickstep

#endif  /* QUICKSTEP_PARSER_PARSE_COMMAND_HPP_ */
