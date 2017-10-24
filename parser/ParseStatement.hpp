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
#include "parser/ParsePartitionClause.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParsePriority.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseSetOperation.hpp"
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
    kCommand = 0,
    kCopy,
    kCreateIndex,
    kCreateTable,
    kDelete,
    kDropTable,
    kInsert,
    kQuit,
    kSetOperation,
    kUpdate
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

  /**
   * @brief Get the priority of the SQL statement. Note that the priority is
   *        an unsigned non-zero integer.
   *
   * @return The priority of the SQL statement. The default priority is 1.
   **/
  virtual const std::uint64_t getPriority() const {
    return 1;
  }

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
                            ParseBlockProperties *opt_block_properties,
                            ParsePartitionClause *opt_partition_clause)
      : ParseStatement(line_number, column_number),
        relation_name_(relation_name),
        attribute_definition_list_(attribute_definition_list),
        opt_block_properties_(opt_block_properties),
        opt_partition_clause_(opt_partition_clause) {
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

  /**
   * @brief Get a pointer to the PartitionClause.
   *
   * @return The PartitionClause or nullptr if not specified.
   **/
  const ParsePartitionClause* opt_partition_clause() const {
    return opt_partition_clause_.get();
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

    if (opt_partition_clause_) {
      container_child_field_names->push_back("partition_clause");
      container_child_fields->emplace_back();
      container_child_fields->back().push_back(opt_partition_clause_.get());
    }
  }

 private:
  std::unique_ptr<ParseString> relation_name_;
  std::unique_ptr<PtrList<ParseAttributeDefinition> > attribute_definition_list_;
  std::unique_ptr<ParseBlockProperties> opt_block_properties_;
  std::unique_ptr<ParsePartitionClause> opt_partition_clause_;

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
        case IndexSubBlockType::kBitWeavingV:  // Fall through.
        case IndexSubBlockType::kBitWeavingH:
          inline_field_values->push_back("bitweaving");
          break;
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
          inline_field_values->push_back("unknown");
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
        case IndexSubBlockType::kBitWeavingV:  // Fall through.
        case IndexSubBlockType::kBitWeavingH:
          index_properties_.reset(new BitWeavingIndexProperties());
          break;
        case IndexSubBlockType::kBloomFilter:
          index_properties_.reset(new BloomFilterIndexProperties());
          break;
        case IndexSubBlockType::kCSBTree:
          index_properties_.reset(new CSBTreeIndexProperties());
          break;
        case IndexSubBlockType::kSMA:
          index_properties_.reset(new SMAIndexProperties());
          break;
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
 * @brief The parsed representation of an UNION/INTERSECT/SELECT statement.
 **/
class ParseStatementSetOperation : public ParseStatement {
 public:
  /**
   * @brief Constructor.
   * @note Takes ownership of all pointers.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param set_operation_query The top level set operation query
   * @param with_clause The WITH clause of common table query expressions.
   * @param priority_clause The PRIORITY clause of this query. If not valid or
   *        not present, this is NULL.
   **/
  ParseStatementSetOperation(const int line_number,
                             const int column_number,
                             ParseSetOperation *set_operation_query,
                             PtrVector<ParseSubqueryTableReference> *with_clause,
                             ParsePriority *priority_clause)
      : ParseStatement(line_number, column_number),
        set_operation_query_(set_operation_query),
        with_clause_(with_clause),
        priority_clause_(priority_clause) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementSetOperation() override {
  }

  StatementType getStatementType() const override {
    return kSetOperation;
  }

  std::string getName() const override { return "SetOperationStatement"; }

  /**
   * @return Get the top-level set operation query.
   */
  const ParseSetOperation* set_operation_query() const {
    return set_operation_query_.get();
  }

  /**
   * @brief Get the WITH table queries.
   *
   * @return The parsed WITH table list.
   */
  const PtrVector<ParseSubqueryTableReference>* with_clause() const {
    return with_clause_.get();
  }

  const std::uint64_t getPriority() const override {
    if (priority_clause_ != nullptr) {
      DCHECK(priority_clause_->priority_expression() != nullptr);
      return priority_clause_->priority_expression()->long_value();
    }
    return 1;
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const ParseTreeNode*> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const ParseTreeNode*>> *container_child_fields) const override {
    non_container_child_field_names->push_back("set_operation_query");
    non_container_child_fields->push_back(set_operation_query_.get());

    if (with_clause_ != nullptr && !with_clause_->empty()) {
      container_child_field_names->push_back("with_clause");
      container_child_fields->emplace_back();
      for (const ParseSubqueryTableReference &common_subquery : *with_clause_) {
        container_child_fields->back().push_back(&common_subquery);
      }
    }

    if (priority_clause_ != nullptr) {
      non_container_child_field_names->push_back("priority");
      non_container_child_fields->push_back(priority_clause_.get());
    }
  }

 private:
  std::unique_ptr<ParseSetOperation> set_operation_query_;
  std::unique_ptr<PtrVector<ParseSubqueryTableReference>> with_clause_;
  std::unique_ptr<ParsePriority> priority_clause_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementSetOperation);
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
                            PtrList<PtrList<ParseScalarLiteral>> *literal_values_list)
      : ParseStatementInsert(line_number, column_number, relation_name),
        literal_values_(literal_values_list) {
  }

  ~ParseStatementInsertTuple() override {
  }

  InsertType getInsertType() const override {
    return InsertType::kTuple;
  }

  /**
   * @brief Get the list of list of parsed literal attribute values to insert.
   *
   * @return The list of lists of literal values to insert.
   **/
  const PtrList<PtrList<ParseScalarLiteral>>& getLiteralValues() const {
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

    for (const PtrList<ParseScalarLiteral>& literal_values_single_tuple : *literal_values_) {
      container_child_field_names->push_back("tuple");
      container_child_fields->emplace_back();
      for (const ParseScalarLiteral& literal_value : literal_values_single_tuple) {
        container_child_fields->back().push_back(&literal_value);
      }
    }
  }

 private:
  std::unique_ptr<PtrList<PtrList<ParseScalarLiteral>>> literal_values_;

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
 * @brief The parsed representation of a COPY FROM/COPY TO statement.
 **/
class ParseStatementCopy : public ParseStatement {
 public:
  /**
   * @brief Copy direction (FROM text file/TO text file).
   */
  enum CopyDirection {
    kFrom = 0,
    kTo
  };

  /**
   * @brief Constructor for a copy statement that copies a stored relation FROM
   *        a text file (or multiple text files, in the case that the file name
   *        is a GLOB pattern) / TO a text file.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param direction The copy direction (FROM/TO).
   * @param relation_name The name of the relation.
   * @param file_name The name of the file.
   * @param params The optional parameters of the COPY statement.
   **/
  ParseStatementCopy(const int line_number,
                     const int column_number,
                     const CopyDirection direction,
                     ParseString *relation_name,
                     ParseString *file_name,
                     PtrList<ParseKeyValue> *params)
      : ParseStatement(line_number, column_number),
        direction_(direction),
        relation_name_(relation_name),
        file_name_(file_name),
        params_(params) {
  }

  /**
   * @brief Constructor for a copy statement that copies the result table of a
   *        query TO a text file.
   *
   * @param line_number Line number of the first token of this node in the SQL statement.
   * @param column_number Column number of the first token of this node in the SQL statement.
   * @param set_operation_query The set operation query.
   * @param with_clause The WITH clause of common table query expressions.
   * @param file_name The name of the file.
   * @param params The optional parameters of the COPY statement.
   **/
  ParseStatementCopy(const int line_number,
                     const int column_number,
                     ParseSetOperation *set_operation_query,
                     PtrVector<ParseSubqueryTableReference> *with_clause,
                     ParseString *file_name,
                     PtrList<ParseKeyValue> *params)
      : ParseStatement(line_number, column_number),
        direction_(kTo),
        set_operation_query_(set_operation_query),
        with_clause_(with_clause),
        file_name_(file_name),
        params_(params) {
  }

  /**
   * @brief Destructor.
   */
  ~ParseStatementCopy() override {
  }

  StatementType getStatementType() const override {
    return kCopy;
  }

  std::string getName() const override {
    return "CopyStatement";
  }

  /**
   * @brief Get the direction (FROM text file/TO text file) of the COPY statement.
   *
   * return The direction of the COPY statement.
   */
  const CopyDirection getCopyDirection() const {
    return direction_;
  }

  /**
   * @brief Get the name of the relation.
   *
   * @return The name of the relation.
   **/
  const ParseString* relation_name() const {
    return relation_name_.get();
  }

  /**
   * @brief Get the set operation query.
   *
   * @return The set operation query.
   */
  const ParseSetOperation* set_operation_query() const {
    return set_operation_query_.get();
  }

  /**
   * @brief Get the WITH table queries.
   *
   * @return The parsed WITH table list.
   */
  const PtrVector<ParseSubqueryTableReference>* with_clause() const {
    return with_clause_.get();
  }

  /**
   * @brief Get the name of the text file to import from/export to.
   *
   * @return The name of the text file.
   **/
  const ParseString* file_name() const {
    return file_name_.get();
  }

  /**
   * @brief Get the additional COPY parameters.
   *
   * @return The additional COPY parameters.
   **/
  const PtrList<ParseKeyValue>* params() const {
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
    inline_field_names->push_back("direction");
    inline_field_values->push_back(direction_ == kFrom ? "FROM" : "TO");

    inline_field_names->push_back("file");
    inline_field_values->push_back(file_name_->value());

    if (relation_name_ != nullptr) {
      inline_field_names->push_back("relation_name");
      inline_field_values->push_back(relation_name_->value());
    }

    if (set_operation_query_ != nullptr) {
      non_container_child_field_names->push_back("set_operation_query");
      non_container_child_fields->push_back(set_operation_query_.get());
    }

    if (with_clause_ != nullptr && !with_clause_->empty()) {
      container_child_field_names->push_back("with_clause");
      container_child_fields->emplace_back();
      for (const ParseSubqueryTableReference &common_subquery : *with_clause_) {
        container_child_fields->back().push_back(&common_subquery);
      }
    }

    if (params_ != nullptr) {
      container_child_field_names->push_back("params");
      container_child_fields->emplace_back();
      for (const ParseKeyValue &param : *params_) {
        container_child_fields->back().push_back(&param);
      }
    }
  }

 private:
  const CopyDirection direction_;

  // NOTE(jianqiao):
  // (1) Either relation_name_ or set_operation_query_ has non-null value.
  // (2) set_operation_query_ must be null for COPY FROM statement.
  std::unique_ptr<ParseString> relation_name_;
  std::unique_ptr<ParseSetOperation> set_operation_query_;

  std::unique_ptr<PtrVector<ParseSubqueryTableReference>> with_clause_;
  std::unique_ptr<ParseString> file_name_;
  std::unique_ptr<PtrList<ParseKeyValue>> params_;

  DISALLOW_COPY_AND_ASSIGN(ParseStatementCopy);
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
