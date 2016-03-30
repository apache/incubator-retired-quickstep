/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_PARSER_PARSE_STATEMENT_HPP_
#define QUICKSTEP_PARSER_PARSE_STATEMENT_HPP_

#include <memory>
#include <string>
#include <vector>

#include "parser/ParseAssignment.hpp"
#include "parser/ParseAttributeDefinition.hpp"
#include "parser/ParseBasicExpressions.hpp"
#include "parser/ParseBlockProperties.hpp"
#include "parser/ParseIndexProperties.hpp"
#include "parser/ParseKeyValue.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseSubqueryTableReference.hpp"
#include "parser/ParseTreeNode.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrList.hpp"
#include "utility/PtrVector.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Parser
 *  @{
 */

/**
 * @brief Abstract base class for all complete SQL commands.
 **/
class ParseStatement : public ParseTreeNode {
 public:
  /**
   * @brief The possible types of SQL statements.
   **/
  enum StatementType {
    kCreateTable,
    kCreateIndex,
    kDropTable,
    kSelect,
    kInsert,
    kCopyFrom,
    kUpdate,
    kDelete,
    kQuit,
    kCommand
  };

  /**
   * @brief Virtual destructor.
   **/
  ~ParseStatement() override {
  }

  /**
   * @brief Identify the type of this SQL statement.
   *
   * @return The type of this statement.
   **/
  virtual StatementType getStatementType() const = 0;

 protected:
  ParseStatement(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number) {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseStatement);
};

/**
 * @brief The parsed representation of a CREATE TABLE statement.
 **/
class ParseStatementCreateTable : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation_name The name of the relation to create.
   * @param attribute_definition_list The list of definitions for the
   *        attributes in the new relation, which becomes owned by this
   *        ParseStatementCreateTable.
   * @param opt_block_properties Optional physical properties of the block.
   **/
  ParseStatementCreateTable(const int line_number,
                            const int column_number,
                            ParseString *relation_name,
                            PtrList<ParseAttributeDefinition> *attribute_definition_list,
                            ParseBlockProperties *opt_block_properties)
      : ParseStatement(line_number, column_number),
        relation_name_(relation_name),
        attribute_definition_list_(attribute_definition_list),
        opt_block_properties_(opt_block_properties) {
  }

  ~ParseStatementCreateTable() override {
  }

  StatementType getStatementType() const override {
    return kCreateTable;
  }

  std::string getName() const override { return "CreateTableStatement"; }

  /**
   * @brief Get the name of the relation to create.
   *
   * @return The new relation's name.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

  /**
   * @brief Get the list of attribute definitions.
   *
   * @return The list of attribute definitions for the new relation.
   **/
  const PtrList<ParseAttributeDefinition>& attribute_definition_list() const {
    return *attribute_definition_list_;
  }

  /**
   * @brief Get a pointer to the BlockProperties.
   *
   * @return The BlockProperties or nullptr if not specified.
   **/
  const ParseBlockProperties* opt_block_properties() const {
    return opt_block_properties_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name_->value());

    container_child_field_names->push_back("attribute_list");
    container_child_fields->emplace_back();
    for (const ParseAttributeDefinition& attribute_definition : *attribute_definition_list_) {
      container_child_fields->back().push_back(&attribute_definition);
    }

    if (opt_block_properties_) {
      container_child_field_names->push_back("block_properties");
      container_child_fields->emplace_back();
      container_child_fields->back().push_back(opt_block_properties_.get());
    }
  }

 private:
  std::unique_ptr<ParseString> relation_name_;
  std::unique_ptr<PtrList<ParseAttributeDefinition> > attribute_definition_list_;
  std::unique_ptr<ParseBlockProperties> opt_block_properties_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementCreateTable);
};

  /**
   * @brief The parsed representation of a CREATE INDEX statement.
   **/
class ParseStatementCreateIndex : public ParseStatement {
 public:
    /**
     * @brief Constructor.
     *
     * @param index_name The name of the index to create.
     * @param relation_name The name of the relation to create index upon.
     * @param attribute_name_list A list of attributes of the relation
     *        on which the index has to be created. If specified as null,
     *        then index is created on all the attributes.
     * @param index_type The type of index to create.
     **/
    ParseStatementCreateIndex(const int line_number,
                              const int column_number,
                              ParseString *index_name,
                              ParseString *relation_name,
                              PtrList<ParseAttribute> *attribute_list,
                              ParseString *index_type)
        : ParseStatement(line_number, column_number),
          index_name_(index_name),
          relation_name_(relation_name),
          attribute_list_(attribute_list),
          index_type_(index_type) {
      initializeIndexType();
    }

    /**
     * @brief Constructor.
     *
     * @param index_name The name of the index to create.
     * @param relation_name The name of the relation to create index upon.
     * @param attribute_name_list A list of attributes of the relation
     *        on which the index has to be created. If specified as null,
     *        then index is created on all the attributes.
     * @param index_type The type of index to create.
     * @param index_properties_line_number
     * @param index_properties_column_number
     * @param opt_index_properties Optional index properties that were specified.
     **/
    ParseStatementCreateIndex(const int line_number,
                              const int column_number,
                              ParseString *index_name,
                              ParseString *relation_name,
                              PtrList<ParseAttribute> *attribute_list,
                              ParseString *index_type,
                              const int index_properties_line_number,
                              const int index_properties_column_number,
                              PtrList<ParseKeyValue> *opt_index_properties)
        : ParseStatement(line_number, column_number),
          index_name_(index_name),
          relation_name_(relation_name),
          attribute_list_(attribute_list),
          index_type_(index_type) {
      initializeIndexType();
      custom_properties_node_.reset(new ParseIndexProperties(index_properties_line_number,
                                                                   index_properties_column_number,
                                                                   opt_index_properties));
      index_properties_->addCustomProperties(custom_properties_node_->getKeyValueList());
    }

    ~ParseStatementCreateIndex() override {
    }

    StatementType getStatementType() const override {
      return kCreateIndex;
    }

    std::string getName() const override { return "CreateIndexStatement"; }

    /**
     * @brief Get the name of the index to create.
     *
     * @return The index's name.
     **/
    const ParseString* index_name() const {
      return index_name_.get();
    }

    /**
     * @brief Get the name of the relation to create index upon.
     *
     * @return The relation's name.
     **/
    const ParseString* relation_name() const {
      return relation_name_.get();
    }

    /**
     * @brief Get the list of attributes on which index is supposed to be defined.
     *
     * @return The list of attributes on which index is to be built.
     **/
    const PtrList<ParseAttribute>* attribute_list() const {
      return attribute_list_.get();
    }

    /**
     * @brief Get the type of the index to be created.
     *
     * @return The index's type.
     **/
    const ParseString* index_type() const {
      return index_type_.get();
    }

    /**
     * @brief Get the index properties associated with this index type.
     *
     * @return The index properties for this type.
     **/
    const IndexProperties* getIndexProperties() const {
      return index_properties_.get();
    }

    const ParseIndexProperties* getCustomPropertiesNode() const {
      return custom_properties_node_.get();
    }

    bool hasCustomProperties() const {
      return custom_properties_node_ != nullptr;
    }

 protected:
    void getFieldStringItems(
       std::vector<std::string> *inline_field_names,
       std::vector<std::string> *inline_field_values,
       std::vector<std::string> *non_container_child_field_names,
       std::vector<const ParseTreeNode*> *non_container_child_fields,
       std::vector<std::string> *container_child_field_names,
       std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
      inline_field_names->push_back("index_name");
      inline_field_values->push_back(index_name_->value());

      inline_field_names->push_back("relation_name");
      inline_field_values->push_back(relation_name_->value());

      inline_field_names->push_back("index_type");
      const int index_type_enum_val = std::stoi(index_type_->value());
      switch (index_type_enum_val) {
        case IndexSubBlockType::kCSBTree:
          inline_field_values->push_back("cs_b_tree");
          break;
        case IndexSubBlockType::kBloomFilter:
          inline_field_values->push_back("bloom_filter");
          break;
        case IndexSubBlockType::kSMA:
          inline_field_values->push_back("sma");
          break;
        default:
          inline_field_values->push_back("unkown");
      }

      if (attribute_list_ != nullptr) {
        container_child_field_names->push_back("attribute_list");
        container_child_fields->emplace_back();
        for (const ParseAttribute &attribute : *attribute_list_) {
          container_child_fields->back().push_back(&attribute);
        }
      }

      if (custom_properties_node_ != nullptr) {
        container_child_field_names->push_back("index_property_list");
        container_child_fields->emplace_back();
        container_child_fields->back().push_back(custom_properties_node_.get());
      }
    }

 private:
    std::unique_ptr<ParseString> index_name_;
    std::unique_ptr<ParseString> relation_name_;
    std::unique_ptr<PtrList<ParseAttribute>> attribute_list_;
    std::unique_ptr<ParseString> index_type_;
    std::unique_ptr<IndexProperties> index_properties_;
    // Optional custom properties for the index can be specified during creation.
    std::unique_ptr<const ParseIndexProperties> custom_properties_node_;

    void initializeIndexType() {
      const int index_type_enum_val = std::stoi(index_type_->value());
      switch (index_type_enum_val) {
        case IndexSubBlockType::kBloomFilter:
          index_properties_.reset(new BloomFilterIndexProperties());
          break;
        case IndexSubBlockType::kCSBTree:
          index_properties_.reset(new CSBTreeIndexProperties());
          break;
        case IndexSubBlockType::kSMA:
          LOG(FATAL) << "Currently cannot create this index subblock type using CREATE INDEX.";
        default:
          LOG(FATAL) << "Unknown index subblock type.";
          break;
      }
    }

    DISALLOW_COPY_AND_ASSIGN(ParseStatementCreateIndex);
};

/**
 * @brief The parsed representation of a DROP TABLE statement.
 **/
class ParseStatementDropTable : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to drop.
   **/
  ParseStatementDropTable(const int line_number, const int column_number, ParseString *relation_name)
      : ParseStatement(line_number, column_number),
        relation_name_(relation_name) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementDropTable() override {
  }

  StatementType getStatementType() const override {
    return kDropTable;
  }

  std::string getName() const override { return "DropTableStatement"; }

  /**
   * @brief Get the name of the relation to drop.
   *
   * @return The name of the relation to drop.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name_->value());
  }

 private:
  std::unique_ptr<ParseString> relation_name_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementDropTable);
};

/**
 * @brief The parsed representation of a SELECT statement.
 **/
class ParseStatementSelect : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   * @note Takes ownership of all pointers.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param select_query The top-level SELECT query.
   * @param with_clause The WITH clause of common table query expressions.
   **/
  ParseStatementSelect(const int line_number,
                       const int column_number,
                       ParseSelect *select_query,
                       PtrVector<ParseSubqueryTableReference> *with_clause)
      : ParseStatement(line_number, column_number),
        select_query_(select_query),
        with_clause_(with_clause) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementSelect() override {
  }

  StatementType getStatementType() const override {
    return kSelect;
  }

  std::string getName() const override { return "SelectStatement"; }

  /**
   * @return Gets the top-level SELECT query.
   */
  const ParseSelect* select_query() const {
    return select_query_.get();
  }

  /**
   * @brief Gets the WITH table queries.
   *
   * @return The parsed WITH table list.
   */
  const PtrVector<ParseSubqueryTableReference>* with_clause() const {
    return with_clause_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    non_container_child_field_names->push_back("select_query");
    non_container_child_fields->push_back(select_query_.get());

    if (with_clause_ != nullptr && !with_clause_->empty()) {
      container_child_field_names->push_back("with_clause");
      container_child_fields->emplace_back();
      for (const ParseSubqueryTableReference &common_subquery : *with_clause_) {
        container_child_fields->back().push_back(&common_subquery);
      }
    }
  }

 private:
  std::unique_ptr<ParseSelect> select_query_;
  std::unique_ptr<PtrVector<ParseSubqueryTableReference>> with_clause_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementSelect);
};

/**
 * @brief The parsed representation of an INSERT statement.
 *
 * This is an abstract class where each of its subclass represents a concrete
 * type of insert operation.
 **/
class ParseStatementInsert : public ParseStatement {
 public:
  enum class InsertType {
    kTuple = 0,
    kSelection
  };

  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to insert into.
   **/
  ParseStatementInsert(const int line_number,
                       const int column_number,
                       const ParseString *relation_name)
      : ParseStatement(line_number, column_number),
        relation_name_(relation_name) {
  }

  /**
   * @brief Get the insert type of this insert statement.
   *
   * @return The insert type of this insert statement.
   */
  virtual InsertType getInsertType() const = 0;

  std::string getName() const override { return "InsertStatement"; }

  StatementType getStatementType() const override {
    return kInsert;
  }

  /**
   * @brief Get the name of the relation to insert into.
   *
   * @return The name of the relation to insert into.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

 private:
  std::unique_ptr<const ParseString> relation_name_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementInsert);
};


/**
 * @brief The parsed representation of an INSERT ... VALUES ... statement.
 **/
class ParseStatementInsertTuple : public ParseStatementInsert {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to insert into.
   * @param literal_values A list of literal values (in attribute-definition
   *        order) to insert into the specified relation as a new tuple.
   *        Becomes owned by this ParseStatementInsert.
   **/
  ParseStatementInsertTuple(const int line_number,
                            const int column_number,
                            const ParseString *relation_name,
                            PtrList<ParseScalarLiteral> *literal_values)
      : ParseStatementInsert(line_number, column_number, relation_name),
        literal_values_(literal_values) {
  }

  ~ParseStatementInsertTuple() override {
  }

  InsertType getInsertType() const override {
    return InsertType::kTuple;
  }

  /**
   * @brief Get the parsed literal attribute values to insert.
   *
   * @return The list of literal values to insert.
   **/
  const PtrList<ParseScalarLiteral>& getLiteralValues() const {
    return *literal_values_;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name()->value());

    container_child_field_names->push_back("tuple");
    container_child_fields->emplace_back();
    for (const ParseScalarLiteral& literal_value : *literal_values_) {
      container_child_fields->back().push_back(&literal_value);
    }
  }

 private:
  std::unique_ptr<PtrList<ParseScalarLiteral> > literal_values_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementInsertTuple);
};

/**
 * @brief The parsed representation of an INSERT ... SELECT ... statement.
 **/
class ParseStatementInsertSelection : public ParseStatementInsert {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to insert into.
   * @param select_query The SELECT query for generating insertion tuples.
   * @param with_clause The WITH clause of common table query expressions.
   **/
  ParseStatementInsertSelection(const int line_number,
                                const int column_number,
                                const ParseString *relation_name,
                                ParseSelect *select_query,
                                PtrVector<ParseSubqueryTableReference> *with_clause)
      : ParseStatementInsert(line_number, column_number, relation_name),
        select_query_(select_query),
        with_clause_(with_clause) {
  }

  ~ParseStatementInsertSelection() override {
  }

  InsertType getInsertType() const override {
    return InsertType::kSelection;
  }

  /**
   * @return Gets the SELECT query.
   */
  const ParseSelect* select_query() const {
    return select_query_.get();
  }

  /**
   * @brief Gets the WITH table queries.
   *
   * @return The parsed WITH table list.
   */
  const PtrVector<ParseSubqueryTableReference>* with_clause() const {
    return with_clause_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name()->value());

    non_container_child_field_names->push_back("select_query");
    non_container_child_fields->push_back(select_query_.get());

    if (with_clause_ != nullptr && !with_clause_->empty()) {
      container_child_field_names->push_back("with_clause");
      container_child_fields->emplace_back();
      for (const ParseSubqueryTableReference &common_subquery : *with_clause_) {
        container_child_fields->back().push_back(&common_subquery);
      }
    }
  }

 private:
  std::unique_ptr<ParseSelect> select_query_;
  std::unique_ptr<PtrVector<ParseSubqueryTableReference>> with_clause_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementInsertSelection);
};

/**
 * @brief Optional parameters for a COPY FROM statement.
 **/
struct ParseCopyFromParams : public ParseTreeNode {
  /**
   * @brief Constructor, sets default values.
   **/
  ParseCopyFromParams(const int line_number, const int column_number)
      : ParseTreeNode(line_number, column_number),
        escape_strings(true) {
  }

  std::string getName() const override { return "CopyFromParams"; }

  /**
   * @brief Sets the column delimiter.
   *
   * @param delimiter_in The column delimiter string.
   */
  void set_delimiter(ParseString* delimiter_in) {
    delimiter.reset(delimiter_in);
  }

  /**
   * @brief The string which terminates individual attribute values in the
   *        input file. Can be NULL.
   **/
  std::unique_ptr<ParseString> delimiter;

  /**
   * @brief If true, replace C-style escape sequences in strings from the input
   *        text file.
   **/
  bool escape_strings;

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    if (delimiter != nullptr) {
      inline_field_names->push_back("delimiter");
      inline_field_values->push_back(delimiter->value());
    }

    inline_field_names->push_back("escape_string");
    inline_field_values->push_back(escape_strings ? "true" : "false");
  }
};

/**
 * @brief The parsed representation of a COPY FROM statement.
 **/
class ParseStatementCopyFrom : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to insert into.
   * @param source_filename The name of the text file to bulk insert from.
   * @param params The optional parameters of the COPY FROM statement (should
   *        be supplied with defaults if not otherwise set).
   **/
  ParseStatementCopyFrom(const int line_number,
                         const int column_number,
                         ParseString *relation_name,
                         ParseString *source_filename,
                         ParseCopyFromParams *params)
      : ParseStatement(line_number, column_number),
        relation_name_(relation_name),
        source_filename_(source_filename),
        params_(params) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementCopyFrom() override {
  }

  StatementType getStatementType() const override {
    return kCopyFrom;
  }

  std::string getName() const override { return "CopyFromStatement"; }

  /**
   * @brief Get the name of the relation to insert into.
   *
   * @return The name of the relation to insert into.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

  /**
   * @brief Get the name of the text file to copy from.
   *
   * @return The name of the text file to copy from.
   **/
  const ParseString* source_filename() const {
    return source_filename_.get();
  }

  /**
   * @brief Get the additional COPY FROM parameters.
   *
   * @return The string which terminates individual attribute values in the
   *         input file.
   **/
  const ParseCopyFromParams* params() const {
    return params_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name_->value());

    inline_field_names->push_back("source_file");
    inline_field_values->push_back(source_filename_->value());

    if (params_ != nullptr) {
      non_container_child_field_names->push_back("params");
      non_container_child_fields->push_back(params_.get());
    }
  }

 private:
  std::unique_ptr<ParseString> relation_name_;
  std::unique_ptr<ParseString> source_filename_;
  std::unique_ptr<ParseCopyFromParams> params_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementCopyFrom);
};


/**
 * @brief The parsed representation of an UPDATE statement.
 **/
class ParseStatementUpdate : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to update.
   * @param assignments A list of assignment for the attributes in the
   *        relation, which becomes owned by this ParseStatementUpdate.
   * @param where_predicate An optional predicate from a WHERE clause in the
   *        UPDATE statement (may be NULL if no predicate). Becomes owned by
   *        this ParseStatementUpdate if non-NULL.
   **/
  ParseStatementUpdate(const int line_number,
                       const int column_number,
                       ParseString *relation_name,
                       PtrList<ParseAssignment> *assignments,
                       ParsePredicate *where_predicate)
      : ParseStatement(line_number, column_number),
        relation_name_(relation_name),
        assignments_(assignments),
        where_predicate_(where_predicate) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementUpdate() override {
  }

  StatementType getStatementType() const override {
    return kUpdate;
  }

  std::string getName() const override { return "UpdateStatement"; }

  /**
   * @brief Get the name of the relation to update.
   *
   * @return The name of the relation to update.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

  /**
   * @brief Get the assignments to perform in the update.
   *
   * @return A list of assignments to perform in the update.
   **/
  const PtrList<ParseAssignment>& assignments() const {
    return *assignments_;
  }

  /**
   * @brief Determine whether this update statement has a WHERE predicate.
   *
   * @return Whether there is a WHERE predicate in this statement.
   **/
  bool hasWherePredicate() const {
    return where_predicate_.get() != nullptr;
  }

  /**
   * @brief Get the where predicate.
   * @warning Always call hasWherePredicate() first.
   *
   * @return The where predicate for this update.
   **/
  const ParsePredicate& where_predicate() const {
    DCHECK(hasWherePredicate());
    return *where_predicate_;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name_->value());

    container_child_field_names->push_back("assignment");
    container_child_fields->emplace_back();
    for (const ParseAssignment& assignment_item : *assignments_) {
      container_child_fields->back().push_back(&assignment_item);
    }

    if (where_predicate_ != nullptr) {
      non_container_child_field_names->push_back("where_predicate");
      non_container_child_fields->push_back(where_predicate_.get());
    }
  }

 private:
  std::unique_ptr<ParseString> relation_name_;
  std::unique_ptr<PtrList<ParseAssignment> > assignments_;
  std::unique_ptr<ParsePredicate> where_predicate_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementUpdate);
};

/**
 * @brief The parsed representation of a DELETE statement.
 **/
class ParseStatementDelete : public ParseStatement {
 public:
  /**
   * @brief Constructor
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param relation_name The name of the relation to delete from.
   * @param where_predicate An optional predicate from a WHERE clause in the
   *        DELETE statement (may be NULL if no predicate). Becomes owned by
   *        this ParseStatementDelete if non-NULL.
   **/
  ParseStatementDelete(const int line_number,
                       const int column_number,
                       ParseString *relation_name,
                       ParsePredicate *where_predicate)
      : ParseStatement(line_number, column_number), relation_name_(relation_name), where_predicate_(where_predicate) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementDelete() override {
  }

  StatementType getStatementType() const override {
    return kDelete;
  }

  std::string getName() const override { return "DeleteStatement"; }

  /**
   * @brief Get the name of the relation to delete from.
   *
   * @return The name of the relation to delete from.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

  /**
   * @brief Get the where predicate.
   *
   * @return The where predicate for this delete statement.
   **/
  const ParsePredicate* where_predicate() const {
    return where_predicate_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("relation_name");
    inline_field_values->push_back(relation_name_->value());

    if (where_predicate_ != nullptr) {
      non_container_child_field_names->push_back("where_predicate");
      non_container_child_fields->push_back(where_predicate_.get());
    }
  }

 private:
  std::unique_ptr<ParseString> relation_name_;
  std::unique_ptr<ParsePredicate> where_predicate_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementDelete);
};

/**
 * @brief The parsed representation of a QUIT statement, which terminates a
 *        quickstep session.
 **/
class ParseStatementQuit : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   */
  ParseStatementQuit(const int line_number, const int column_number)
      : ParseStatement(line_number, column_number) {
  }

  StatementType getStatementType() const override {
    return kQuit;
  }

  std::string getName() const override { return "QuitStatement"; }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(ParseStatementQuit);
};

/**
 * @brief Class to hold the parsed command name and an optional argument string.
 * @details Commands are non-sql statements which can be issued to quickstep.
 *          They are entered into the CLI as '.command-name command-string\n'.
 *          The command string is split up into words using whitespace as
 *          a delimiter.
 */
class ParseCommand : public ParseStatement {
 public:
  ParseCommand(const int line_number,
               const int column_number,
               ParseString *command,
               PtrVector<ParseString> *arguments)
      : ParseStatement(line_number, column_number),
        command_(command),
        arguments_(arguments) {  }

  /**
   * @return The name of this class.
   */
  std::string getName() const override {
    return "ParseCommand";
  }

  /**
   * @brief All ParseCommands are ParseStatements of the type command.
   */
  StatementType getStatementType() const override {
    return kCommand;
  }

  /**
   * @return The name of the command.
   */
  const ParseString* command() const {
    return command_.get();
  }

  /**
   * @return The optional argument strings to the command. Possibly empty.
   */
  const PtrVector<ParseString>* arguments() const {
    return arguments_.get();
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    inline_field_names->push_back("command");
    inline_field_values->push_back(command_->value());

    for (const ParseString &argument : *arguments_) {
      non_container_child_field_names->push_back("argument");
      non_container_child_fields->push_back(&argument);
    }
  }

 private:
  std::unique_ptr<ParseString> command_;
  std::unique_ptr<PtrVector<ParseString>> arguments_;

  DISALLOW_COPY_AND_ASSIGN(ParseCommand);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_PARSER_PARSE_STATEMENT_HPP_
