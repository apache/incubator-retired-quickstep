/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "query_optimizer/resolver/Resolver.hpp"

#include <map>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <utility>

#include "catalog/CatalogDatabase.hpp"
#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/table_generator/GeneratorFunction.hpp"
#include "expressions/table_generator/GeneratorFunctionFactory.hpp"
#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "parser/ParseAssignment.hpp"
#include "parser/ParseBasicExpressions.hpp"
#include "parser/ParseBlockProperties.hpp"
#include "parser/ParseExpression.hpp"
#include "parser/ParseGeneratorTableReference.hpp"
#include "parser/ParseGroupBy.hpp"
#include "parser/ParseHaving.hpp"
#include "parser/ParseLimit.hpp"
#include "parser/ParseLiteralValue.hpp"
#include "parser/ParseOrderBy.hpp"
#include "parser/ParsePredicate.hpp"
#include "parser/ParseSelect.hpp"
#include "parser/ParseSelectionClause.hpp"
#include "parser/ParseSimpleTableReference.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/ParseString.hpp"
#include "parser/ParseSubqueryExpression.hpp"
#include "parser/ParseSubqueryTableReference.hpp"
#include "parser/ParseTableReference.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/Validator.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/Cast.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/LogicalAnd.hpp"
#include "query_optimizer/expressions/LogicalNot.hpp"
#include "query_optimizer/expressions/LogicalOr.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/PatternMatcher.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/PredicateLiteral.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/expressions/UnaryExpression.hpp"
#include "query_optimizer/logical/Aggregate.hpp"
#include "query_optimizer/logical/CopyFrom.hpp"
#include "query_optimizer/logical/CreateIndex.hpp"
#include "query_optimizer/logical/CreateTable.hpp"
#include "query_optimizer/logical/DeleteTuples.hpp"
#include "query_optimizer/logical/DropTable.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/InsertTuple.hpp"
#include "query_optimizer/logical/MultiwayCartesianJoin.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/Sample.hpp"
#include "query_optimizer/logical/SharedSubplanReference.hpp"
#include "query_optimizer/logical/Sort.hpp"
#include "query_optimizer/logical/TableGenerator.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/logical/TopLevelPlan.hpp"
#include "query_optimizer/logical/UpdateTable.hpp"
#include "query_optimizer/resolver/NameResolver.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/StorageConstants.hpp"
#include "types/IntType.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeFactory.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/PtrList.hpp"
#include "utility/SqlError.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {
namespace optimizer {
namespace resolver {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;

struct Resolver::ExpressionResolutionInfo {
  /**
   * @brief Constructs an ExpressionResolutionInfo that disallows aggregate
   *        expressions.
   *
   * @param name_resolver_in The name resolver.
   * @param clause_name The name of the current clause. This is only used to
   *                    display errors when there is an aggregate expression.
   * @param select_list_info info for SELECT-list expressions. NULL if
   *                         references to SELECT-list expressions are not
   *                         allowed.
   */
  ExpressionResolutionInfo(const NameResolver &name_resolver_in,
                           const std::string &clause_name,
                           SelectListInfo *select_list_info_in)
      : name_resolver(name_resolver_in),
        not_allow_aggregate_here(clause_name),
        select_list_info(select_list_info_in) {}

  /**
   * @brief Constructs an ExpressionResolutionInfo that allows aggregate
   *        expressions. Does not take ownership of
   *        \p query_aggregation_info_in.
   *
   * @param name_resolver_in The name resolver.
   * @param query_aggregation_info_in Container for aggregate expressions.
   * @param select_list_info info for SELECT-list expressions. NULL if
   *                         references to SELECT-list expressions are not
   *                         allowed.
   */
  ExpressionResolutionInfo(const NameResolver &name_resolver_in,
                           QueryAggregationInfo *query_aggregation_info_in,
                           SelectListInfo *select_list_info_in)
      : name_resolver(name_resolver_in),
        query_aggregation_info(query_aggregation_info_in),
        select_list_info(select_list_info_in) {}

  /**
   * @brief Constructor an ExpressionResolutionInfo with parameters inherited
   *        from another ExpressionResolutionInfo.
   *
   * @param parent The ExpressionResolutionInfo where the parameters are
   *               inherited from.
   */
  explicit ExpressionResolutionInfo(const ExpressionResolutionInfo &parent)
      : name_resolver(parent.name_resolver),
        not_allow_aggregate_here(parent.not_allow_aggregate_here),
        query_aggregation_info(parent.query_aggregation_info),
        select_list_info(parent.select_list_info) {}

  /**
   * @return True if the expression contains an aggregate function.
   */
  bool hasAggregate() const { return parse_aggregate_expression != nullptr; }

  const NameResolver &name_resolver;
  // Empty if aggregations are allowed.
  const std::string not_allow_aggregate_here;

  // Can be NULL if aggregations are not allowed.
  QueryAggregationInfo *query_aggregation_info = nullptr;
  // Can be NULL if alias references to SELECT-list expressions are not allowed.
  SelectListInfo *select_list_info = nullptr;
  // The first aggregate in the expression.
  const ParseTreeNode *parse_aggregate_expression = nullptr;
};

struct Resolver::QueryAggregationInfo {
  explicit QueryAggregationInfo(bool has_group_by_in)
      : has_group_by(has_group_by_in) {}

  // Whether the current query block has a GROUP BY.
  const bool has_group_by;
  // Alias expressions that wraps aggregate functions.
  std::vector<E::AliasPtr> aggregate_expressions;
};

struct Resolver::SelectListInfo {
 public:
  /**
   * @brief Constructor.
   *
   * @param select_list_expressions_in SELECT-list expressions.
   * @param has_aggregate_per_expression_in
   *            For each SELECT-list expression, indicates whether it contains
   *            an aggregate.
   */
  SelectListInfo(
      const std::vector<E::NamedExpressionPtr> &select_list_expressions_in,
      const std::vector<bool> &has_aggregate_per_expression_in)
      : select_list_expressions(select_list_expressions_in),
        has_aggregate_per_expression(has_aggregate_per_expression_in),
        is_referenced(select_list_expressions.size(), false) {
    for (std::vector<E::NamedExpressionPtr>::size_type idx = 0;
         idx < select_list_expressions.size();
         ++idx) {
      if (!NameResolver::IsInternalName(
              select_list_expressions[idx]->attribute_alias())) {
        const std::string lower_alias =
            ToLower(select_list_expressions[idx]->attribute_alias());
        if (alias_to_expression_map.find(lower_alias) !=
            alias_to_expression_map.end()) {
          alias_to_expression_map[lower_alias] = -1;
        } else {
          alias_to_expression_map[lower_alias] = idx;
        }
      }
    }
  }

  /**
   * @return The map from each alias to the 0-based position of the
   *         corresponding expression in the <select_list_expressions>.
   */
  const std::map<std::string, int> &getAliasMap() const {
    return alias_to_expression_map;
  }

  const std::vector<E::NamedExpressionPtr> &select_list_expressions;

  // Has 1:1 matching with <select_list_expressions>.
  // True if the corresponding expression has an aggregate.
  const std::vector<bool> &has_aggregate_per_expression;

  // Has 1:1 matching with <select_list_expressions>.
  // True if the corresponding expression is referenced by an
  // ordinal number or an alias.
  std::vector<bool> is_referenced;

 private:
  // Map from the alias to the position of the corresponding expression.
  // Map to -1 if the alias is ambiguous.
  std::map<std::string, int> alias_to_expression_map;
};

L::LogicalPtr Resolver::resolve(const ParseStatement &parse_query) {
  switch (parse_query.getStatementType()) {
    case ParseStatement::kCopyFrom:
      context_->set_is_catalog_changed();
      logical_plan_ = resolveCopyFrom(
          static_cast<const ParseStatementCopyFrom&>(parse_query));
      break;
    case ParseStatement::kCreateTable:
      context_->set_is_catalog_changed();
      logical_plan_ = resolveCreateTable(
          static_cast<const ParseStatementCreateTable&>(parse_query));
      break;
    case ParseStatement::kCreateIndex:
      context_->set_is_catalog_changed();
      logical_plan_ = resolveCreateIndex(
          static_cast<const ParseStatementCreateIndex&>(parse_query));
      break;
    case ParseStatement::kDelete:
      context_->set_is_catalog_changed();
      logical_plan_ =
          resolveDelete(static_cast<const ParseStatementDelete&>(parse_query));
      break;
    case ParseStatement::kDropTable:
      context_->set_is_catalog_changed();
      logical_plan_ = resolveDropTable(
          static_cast<const ParseStatementDropTable&>(parse_query));
      break;
    case ParseStatement::kInsert:
      context_->set_is_catalog_changed();
      logical_plan_ =
          resolveInsert(static_cast<const ParseStatementInsert&>(parse_query));
      break;
    case ParseStatement::kSelect: {
      const ParseStatementSelect &select_statement =
          static_cast<const ParseStatementSelect&>(parse_query);
      if (select_statement.with_clause() != nullptr) {
        int index = 0;
        for (const ParseSubqueryTableReference &with_table_reference : *select_statement.with_clause()) {
          NameResolver name_resolver;
          with_queries_info_.with_query_plans.emplace_back(
              resolveTableReference(with_table_reference, &name_resolver));

          const ParseString *reference_alias = with_table_reference.table_reference_signature()->table_alias();
          const std::string lower_alias_name = ToLower(reference_alias->value());
          if (with_queries_info_.with_query_name_to_vector_position.find(lower_alias_name)
                  != with_queries_info_.with_query_name_to_vector_position.end()) {
            THROW_SQL_ERROR_AT(reference_alias)
                << "WITH query name " << reference_alias->value()
                << " is specified more than once";
          }

          with_queries_info_.with_query_name_to_vector_position.emplace(lower_alias_name, index);
          with_queries_info_.unreferenced_query_indexes.insert(index);
          ++index;
        }
      }
      logical_plan_ =
          resolveSelect(*select_statement.select_query(),
                        "" /* select_name */);
      if (select_statement.with_clause() != nullptr) {
        // Report an error if there is a WITH query that is not actually used.
        if (!with_queries_info_.unreferenced_query_indexes.empty()) {
          int unreferenced_with_query_index = *with_queries_info_.unreferenced_query_indexes.begin();
          const ParseSubqueryTableReference &unreferenced_with_query =
              (*select_statement.with_clause())[unreferenced_with_query_index];
          THROW_SQL_ERROR_AT(&unreferenced_with_query)
              << "WITH query "
              << unreferenced_with_query.table_reference_signature()->table_alias()->value()
              << " is defined but not used";
        }
      }
      break;
    }
    case ParseStatement::kUpdate:
      context_->set_is_catalog_changed();
      logical_plan_ =
          resolveUpdate(static_cast<const ParseStatementUpdate&>(parse_query));
      break;
    default:
      LOG(FATAL) << "Unhandled query statement:" << parse_query.toString();
  }

  logical_plan_ = L::TopLevelPlan::Create(logical_plan_,
                                          with_queries_info_.with_query_plans);

#ifdef QUICKSTEP_DEBUG
  Validate(logical_plan_);
#endif

  return logical_plan_;
}

L::LogicalPtr Resolver::resolveCopyFrom(
    const ParseStatementCopyFrom &copy_from_statement) {
  // Default parameters.
  std::string column_delimiter_ = "\t";
  bool escape_strings_ = true;

  const ParseCopyFromParams *params = copy_from_statement.params();
  if (params != nullptr) {
    if (params->delimiter != nullptr) {
      column_delimiter_ = params->delimiter->value();
      if (column_delimiter_.size() != 1) {
        THROW_SQL_ERROR_AT(params->delimiter)
            << "DELIMITER is not a single character";
      }
    }
    escape_strings_ = params->escape_strings;
  }

  return L::CopyFrom::Create(resolveRelationName(copy_from_statement.relation_name()),
                             copy_from_statement.source_filename()->value(),
                             column_delimiter_[0],
                             escape_strings_);
}

L::LogicalPtr Resolver::resolveCreateTable(
    const ParseStatementCreateTable &create_table_statement) {
  // Resolve relation name.
  const std::string relation_name =
      create_table_statement.relation_name()->value();
  if (context_->catalog_database()->hasRelationWithName(relation_name)) {
    THROW_SQL_ERROR_AT(create_table_statement.relation_name())
        << "Relation " << create_table_statement.relation_name()->value()
        << " already exists";
  }
  if (relation_name.compare(0,
                            std::strlen(OptimizerContext::kInternalTemporaryRelationNamePrefix),
                            OptimizerContext::kInternalTemporaryRelationNamePrefix) == 0) {
    THROW_SQL_ERROR_AT(create_table_statement.relation_name())
        << "Relation name cannot start with "
        << OptimizerContext::kInternalTemporaryRelationNamePrefix;
  }

  // Resolve attribute definitions.
  std::vector<E::AttributeReferencePtr> attributes;
  std::set<std::string> attribute_name_set;
  for (const ParseAttributeDefinition &attribute_definition :
       create_table_statement.attribute_definition_list()) {
    const std::string lower_attribute_name =
        ToLower(attribute_definition.name()->value());
    if (attribute_name_set.find(lower_attribute_name) !=
        attribute_name_set.end()) {
      THROW_SQL_ERROR_AT(attribute_definition.name())
          << "Column " << attribute_definition.name()->value()
          << " is specified more than once";
    }
    attributes.emplace_back(
        E::AttributeReference::Create(context_->nextExprId(),
                                      attribute_definition.name()->value(),
                                      attribute_definition.name()->value(),
                                      relation_name,
                                      attribute_definition.data_type().getType()));
    attribute_name_set.insert(lower_attribute_name);
  }

  std::shared_ptr<const StorageBlockLayoutDescription>
      block_properties(resolveBlockProperties(create_table_statement));

  return L::CreateTable::Create(relation_name, attributes, block_properties);
}

StorageBlockLayoutDescription* Resolver::resolveBlockProperties(
    const ParseStatementCreateTable &create_table_statement) {
  const ParseBlockProperties *block_properties
      = create_table_statement.opt_block_properties();
  // If there are no block properties.
  if (block_properties == nullptr) {
    return nullptr;
  }
  // Check for error conditions.
  const ParseKeyValue *repeated_key_value
      = block_properties->getFirstRepeatedKeyValue();
  if (repeated_key_value != nullptr) {
    THROW_SQL_ERROR_AT(repeated_key_value)
        << "Properties must be specified at most once.";
  }
  const ParseKeyValue *invalid_key_value
      = block_properties->getFirstInvalidKeyValue();
  if (invalid_key_value != nullptr) {
    THROW_SQL_ERROR_AT(invalid_key_value)
        << "Unrecognized property name.";
  }

  // Begin resolution of properties.
  std::unique_ptr<StorageBlockLayoutDescription>
      storage_block_description(new StorageBlockLayoutDescription());
  TupleStorageSubBlockDescription *description =
      storage_block_description->mutable_tuple_store_description();

  // Resolve TYPE property.
  // The type of the block will determine these:
  bool block_requires_sort = false;
  bool block_requires_compress = false;

  const ParseString *type_parse_string = block_properties->getType();
  if (type_parse_string == nullptr) {
    THROW_SQL_ERROR_AT(block_properties)
        << "TYPE property must be specified and be a string.";
  }
  const std::string type_string = ToLower(type_parse_string->value());
  if (type_string.compare("rowstore") == 0) {
    description->set_sub_block_type(
        quickstep::TupleStorageSubBlockDescription::PACKED_ROW_STORE);
  } else if (type_string.compare("split_rowstore") == 0) {
    description->set_sub_block_type(
        quickstep::TupleStorageSubBlockDescription::SPLIT_ROW_STORE);
  } else if (type_string.compare("columnstore") == 0) {
    description->set_sub_block_type(
        quickstep::TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
    block_requires_sort = true;
  } else if (type_string.compare("compressed_rowstore") == 0) {
    description->set_sub_block_type(
        quickstep::TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
    block_requires_compress = true;
  } else if (type_string.compare("compressed_columnstore") == 0) {
    description->set_sub_block_type(
        quickstep::TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
    block_requires_sort = true;
    block_requires_compress = true;
  } else {
    THROW_SQL_ERROR_AT(type_parse_string) << "Unrecognized storage type.";
  }

  // Helper lambda function which will be used in COMPRESS and SORT resolution.
  // Returns the column id from the name of the given attribute. Returns -1 if
  // the attribute is not found.
  auto columnIdFromAttributeName = [&create_table_statement](
      const std::string& attribute_name) -> int {
    const std::string search_name = ToLower(attribute_name);
    int i = 0;
    for (const ParseAttributeDefinition &attribute_definition :
     create_table_statement.attribute_definition_list()) {
      const std::string lower_attribute_name =
        ToLower(attribute_definition.name()->value());
      if (lower_attribute_name.compare(search_name) == 0) {
        return i;
      }
      i++;
    }
    return -1;
  };

  // Resolve the SORT property.
  const ParseString *sort_parse_string = block_properties->getSort();
  if (block_requires_sort) {
    if (sort_parse_string == nullptr) {
      THROW_SQL_ERROR_AT(type_parse_string)
          << "The SORT property must be specified as an attribute name.";
    } else {
      const std::string &sort_name = sort_parse_string->value();
      // Lookup the name and map to a column id.
      int sort_id = columnIdFromAttributeName(sort_name);
      if (sort_id == -1) {
        THROW_SQL_ERROR_AT(sort_parse_string)
          << "The SORT property did not match any attribute name.";
      } else {
        if (description->sub_block_type() ==
            TupleStorageSubBlockDescription::BASIC_COLUMN_STORE) {
          description->SetExtension(
              BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id, sort_id);
        } else if (description->sub_block_type() ==
            TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE) {
          description->SetExtension(
              CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id, sort_id);
        }
      }
    }
  } else {
    if (sort_parse_string != nullptr) {
      THROW_SQL_ERROR_AT(sort_parse_string)
          << "The SORT property does not apply to this block type.";
    }
  }
  // Resolve the COMPRESS property.
  if (block_requires_compress) {
    std::vector<std::size_t> compressed_column_ids;
    // If we compress all the columns in the relation.
    if (block_properties->compressAll()) {
      for (std::size_t attribute_id = 0;
           attribute_id < create_table_statement.attribute_definition_list().size();
           ++attribute_id) {
        compressed_column_ids.push_back(attribute_id);
      }
    } else {
      // Otherwise search through the relation, mapping attribute names to their id.
      const PtrList<ParseString> *compress_parse_strings
          = block_properties->getCompressed();
      if (compress_parse_strings == nullptr) {
        THROW_SQL_ERROR_AT(block_properties)
          << "The COMPRESS property must be specified as ALL or a list of attributes.";
      }
      for (const ParseString &compressed_attribute_name : *compress_parse_strings) {
        int column_id = columnIdFromAttributeName(compressed_attribute_name.value());
        if (column_id == -1) {
          THROW_SQL_ERROR_AT(&compressed_attribute_name)
              << "The given attribute was not found.";
        } else {
          compressed_column_ids.push_back(static_cast<std::size_t>(column_id));
        }
      }
    }
    // Add the found column ids to the proto message.
    for (std::size_t column_id : compressed_column_ids) {
      if (description->sub_block_type() ==
          TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE) {
        description->AddExtension(
            CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, column_id);
      } else if (description->sub_block_type() ==
          TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE) {
        description->AddExtension(
            CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id, column_id);
      }
    }
  } else {
    // If the user specified COMPRESS but the block type did not require it, throw.
    if (block_properties->compressAll() ||
        block_properties->getCompressed() != nullptr) {
      THROW_SQL_ERROR_AT(type_parse_string)
          << "The COMPRESS property does not apply to this block type.";
    }
  }
  // Resolve the Block size (size -> # of slots).
  std::int64_t slots = 1;  // The default.
  if (block_properties->hasBlockSizeMb()) {
    std::int64_t blocksizemb = block_properties->getBlockSizeMbValue();
    if (blocksizemb == -1) {
      // Indicates an error condition if the property is present but getter returns -1.
      THROW_SQL_ERROR_AT(block_properties->getBlockSizeMb())
          << "The BLOCKSIZEMB property must be an integer.";
    }
    slots = (blocksizemb * 1000000) / kSlotSizeBytes;
    DLOG(INFO) << "Resolver using BLOCKSIZEMB of " << slots << " slots"
        << " which is " << (slots * kSlotSizeBytes) << " bytes versus"
        << " user requested " << (blocksizemb * 1000000) << " bytes.";
    // 1Gb is the max size.
    const std::int64_t max_size_slots = 1000000000 / kSlotSizeBytes;
    // TODO(marc) The upper bound is arbitrary.
    if (slots < 1 || slots > max_size_slots) {
      THROW_SQL_ERROR_AT(block_properties->getBlockSizeMb())
        << "The BLOCKSIZEMB property must be between 2Mb and 1000Mb.";
    }
  }
  storage_block_description->set_num_slots(slots);

  return storage_block_description.release();
}

L::LogicalPtr Resolver::resolveCreateIndex(
    const ParseStatementCreateIndex &create_index_statement) {
  // Resolve relation reference
  const L::LogicalPtr input = resolveSimpleTableReference(
      *create_index_statement.relation_name(), nullptr /* reference_alias */);

  const std::string &index_name = create_index_statement.index_name()->value();

  // Resolve attribute references
  const PtrList<ParseAttribute> *index_attributes = create_index_statement.attribute_list();
  const std::vector<E::AttributeReferencePtr> &relation_attributes =
      input->getOutputAttributes();
  std::vector<E::AttributeReferencePtr> resolved_attributes;
  if (index_attributes == nullptr) {
    // specify to build index on all the attributes, if no attribute was specified
    for (const E::AttributeReferencePtr &relation_attribute : relation_attributes) {
      resolved_attributes.emplace_back(relation_attribute);
    }
  } else {
    // otherwise specify to build index on the attributes that were given
    for (const ParseAttribute &index_attribute : *index_attributes) {
      bool is_resolved = false;
      for (const E::AttributeReferencePtr &relation_attribute : relation_attributes) {
        const std::string &relation_attr_name = relation_attribute->attribute_name();
        const std::string &index_attr_name = index_attribute.attr_name()->value();
        if (relation_attr_name.compare(index_attr_name) == 0) {
          is_resolved = true;
          resolved_attributes.emplace_back(relation_attribute);
          break;
        }
      }
      if (!is_resolved) {
        THROW_SQL_ERROR_AT(&index_attribute)<< "Attribute "<< index_attribute.attr_name()->value()
        << " is undefined for the relation "<< create_index_statement.relation_name()->value();
      }
    }
  }

  // Resolve index properties
  std::shared_ptr<const IndexSubBlockDescription> index_description_shared;
  if (create_index_statement.getIndexProperties()->isIndexDescriptionValid()) {
    // create a deep copy of the index description and pass its ownership to the shared ptr
    std::unique_ptr<IndexSubBlockDescription> index_description(new IndexSubBlockDescription());
    index_description->CopyFrom(*create_index_statement.getIndexProperties()->getIndexDescription());
    index_description_shared.reset(index_description.release());
    DCHECK(index_description_shared != nullptr);
  } else {
    if (create_index_statement.getIndexProperties()->getInvalidPropertyNode() != nullptr) {
      // if exact location is known in the parser, the error is thrown at that specific node
      THROW_SQL_ERROR_AT(create_index_statement.getIndexProperties()->getInvalidPropertyNode())
        << create_index_statement.getIndexProperties()->getReasonForInvalidIndexDescription();
    } else {
      // else the error is thrown at the index name node
      THROW_SQL_ERROR_AT(create_index_statement.index_type())
        << create_index_statement.getIndexProperties()->getReasonForInvalidIndexDescription();
    }
  }

  return L::CreateIndex::Create(input, index_name, resolved_attributes, index_description_shared);
}

L::LogicalPtr Resolver::resolveDelete(
    const ParseStatementDelete &delete_tuples_statement) {
  NameResolver name_resolver;

  const L::LogicalPtr input_logical = resolveSimpleTableReference(
      *delete_tuples_statement.relation_name(), nullptr /* reference alias */);
  name_resolver.addRelation(delete_tuples_statement.relation_name(),
                            input_logical);

  // Resolve the predicate to select tuples to be deleted.
  E::PredicatePtr predicate;
  if (delete_tuples_statement.where_predicate() != nullptr) {
    ExpressionResolutionInfo expr_resolution_info(
        name_resolver,
        "WHERE clause" /* clause_name */,
        nullptr /* select_list_info */);
    predicate = resolvePredicate(*delete_tuples_statement.where_predicate(),
                                 &expr_resolution_info);
  }

  return L::DeleteTuples::Create(input_logical, predicate);
}

L::LogicalPtr Resolver::resolveDropTable(
    const ParseStatementDropTable &drop_table_statement) {
  return L::DropTable::Create(
      resolveRelationName(drop_table_statement.relation_name()));
}

L::LogicalPtr Resolver::resolveInsert(
    const ParseStatementInsert &insert_statement) {
  NameResolver name_resolver;

  const L::LogicalPtr input_logical = resolveSimpleTableReference(
      *insert_statement.relation_name(), nullptr /* reference alias */);
  name_resolver.addRelation(insert_statement.relation_name(),
                            input_logical);

  // Resolve column values.
  const std::vector<E::AttributeReferencePtr> relation_attributes =
      input_logical->getOutputAttributes();
  const PtrList<ParseScalarLiteral> &parse_column_values =
      insert_statement.getLiteralValues();
  DCHECK_GT(parse_column_values.size(), 0u);

  if (parse_column_values.size() > relation_attributes.size()) {
    THROW_SQL_ERROR_AT(insert_statement.relation_name())
        << "The relation " << insert_statement.relation_name()->value()
        << " has " << std::to_string(relation_attributes.size())
        << " columns, but " << std::to_string(parse_column_values.size())
        << " values are provided";
  }

  std::vector<E::ScalarLiteralPtr> resolved_column_values;
  std::vector<E::AttributeReferencePtr>::size_type aid = 0;
  for (const ParseScalarLiteral &parse_literal_value : parse_column_values) {
    E::ScalarLiteralPtr resolved_literal_value;
    ExpressionResolutionInfo expr_resolution_info(
        name_resolver,
        "INSERT statement" /* clause_name */,
        nullptr /* select_list_info */);
    // When resolving the literal, use the attribute's Type as a hint.
    CHECK(E::SomeScalarLiteral::MatchesWithConditionalCast(
        resolveExpression(parse_literal_value,
                          &(relation_attributes[aid]->getValueType()),
                          &expr_resolution_info),
        &resolved_literal_value));

    // Check that the resolved Type is safely coercible to the attribute's
    // Type.
    if (!relation_attributes[aid]->getValueType().isSafelyCoercibleFrom(
            resolved_literal_value->getValueType())) {
      THROW_SQL_ERROR_AT(&parse_literal_value)
          << "The assigned value for the column "
          << relation_attributes[aid]->attribute_name() << " has the type "
          << resolved_literal_value->getValueType().getName()
          << ", which cannot be safely coerced to the column's type "
          << relation_attributes[aid]->getValueType().getName();
    }

    // If the Type is not exactly right (but is safely coercible), coerce it.
    if (!resolved_literal_value->getValueType().equals(
            relation_attributes[aid]->getValueType())) {
      resolved_literal_value = E::ScalarLiteral::Create(
          relation_attributes[aid]->getValueType().coerceValue(
              resolved_literal_value->value(),
              resolved_literal_value->getValueType()),
          relation_attributes[aid]->getValueType());
    }

    resolved_column_values.push_back(resolved_literal_value);
    ++aid;
  }

  while (aid < relation_attributes.size()) {
    if (!relation_attributes[aid]->getValueType().isNullable()) {
      THROW_SQL_ERROR_AT(insert_statement.relation_name())
          << "Must assign a non-NULL value to column "
          << relation_attributes[aid]->attribute_name();
    }
    // Create a NULL value.
    resolved_column_values.push_back(E::ScalarLiteral::Create(
        relation_attributes[aid]->getValueType().makeNullValue(),
        relation_attributes[aid]->getValueType()));
    ++aid;
  }

  return L::InsertTuple::Create(input_logical, resolved_column_values);
}

L::LogicalPtr Resolver::resolveUpdate(
    const ParseStatementUpdate &update_statement) {
  NameResolver name_resolver;
  const L::LogicalPtr input = resolveSimpleTableReference(
      *update_statement.relation_name(), nullptr /* reference_alias */);
  name_resolver.addRelation(update_statement.relation_name(),
                            input);

  // Resolve the assigned attributes and expressions.
  std::vector<E::AttributeReferencePtr> assignees;
  std::vector<E::ScalarPtr> assignment_expressions;
  std::set<E::ExprId> assignee_ids;
  for (const ParseAssignment &assignment : update_statement.assignments()) {
    const E::AttributeReferencePtr attribute = name_resolver.lookup(
        assignment.attr_name(), nullptr /* parse_rel_node */);
    ExpressionResolutionInfo expr_resolution_info(
        name_resolver,
        "SET clause" /* clause_name */,
        nullptr /* select_list_info */);

    // Resolve the assignment expression using the assigned attribute's Type as
    // a hint.
    E::ScalarPtr assignment_expression =
        resolveExpression(assignment.value(),
                          &(attribute->getValueType()),
                          &expr_resolution_info);

    if (!attribute->getValueType().isSafelyCoercibleFrom(
            assignment_expression->getValueType())) {
      THROW_SQL_ERROR_AT(&assignment)
          << "The assigned value for the column "
          << attribute->attribute_name() << " has the type "
          << assignment_expression->getValueType().getName()
          << ", which cannot be safely coerced to the column's type "
          << attribute->getValueType().getName();
    }

    // Coerce the assignment expression if its Type is not equal to that of the
    // assigned attribute.
    if (!assignment_expression->getValueType().equals(attribute->getValueType())) {
      assignment_expression =
          E::Cast::Create(assignment_expression, attribute->getValueType());
    }
    if (assignee_ids.find(attribute->id()) != assignee_ids.end()) {
      THROW_SQL_ERROR_AT(&assignment) << "Multiple assignments to the column "
                                      << attribute->attribute_name();
    }
    assignees.push_back(attribute);
    assignment_expressions.push_back(assignment_expression);
    assignee_ids.insert(attribute->id());
  }

  // Resolve the WHERE predicate.
  expressions::PredicatePtr resolved_where_predicate;
  if (update_statement.hasWherePredicate()) {
    ExpressionResolutionInfo expr_resolution_info(
        name_resolver, "WHERE clause" /* clause_name */,
        nullptr /* select_list_info */);
    resolved_where_predicate = resolvePredicate(
        update_statement.where_predicate(), &expr_resolution_info);
  }

  return L::UpdateTable::Create(input,
                                assignees,
                                assignment_expressions,
                                resolved_where_predicate);
}

L::LogicalPtr Resolver::resolveSelect(
    const ParseSelect &select_query,
    const std::string &select_name) {
  // Create a new name scope. We currently do not support correlated query.
  std::unique_ptr<NameResolver> name_resolver(new NameResolver());

  // Resolve FROM clause.
  L::LogicalPtr logical_plan;
  logical_plan =
      resolveFromClause(select_query.from_list(), name_resolver.get());

  // Resolve WHERE clause.
  if (select_query.hasWherePredicate()) {
    ExpressionResolutionInfo expr_resolution_info(
        *name_resolver, "WHERE clause" /* clause_name */,
        nullptr /* select_list_info */);
    const ParsePredicate &parse_predicate = select_query.where_predicate();
    logical_plan = L::Filter::Create(
        logical_plan, resolvePredicate(parse_predicate, &expr_resolution_info));
  }

  QueryAggregationInfo query_aggregation_info(
      (select_query.group_by() != nullptr));

  // Resolve SELECT-list clause.
  std::vector<E::NamedExpressionPtr> select_list_expressions;
  std::vector<bool> has_aggregate_per_expression;
  resolveSelectClause(select_query.selection(),
                      select_name,
                      *name_resolver,
                      &query_aggregation_info,
                      &select_list_expressions,
                      &has_aggregate_per_expression);
  DCHECK_EQ(has_aggregate_per_expression.size(),
            select_list_expressions.size());

  SelectListInfo select_list_info(select_list_expressions,
                                  has_aggregate_per_expression);

  // Resolve GROUP BY.
  std::vector<E::NamedExpressionPtr> group_by_expressions;
  if (select_query.group_by() != nullptr) {
    for (const ParseExpression &unresolved_group_by_expression :
         *select_query.group_by()->grouping_expressions()) {
      ExpressionResolutionInfo expr_resolution_info(
          *name_resolver, "GROUP BY clause" /* clause_name */,
          &select_list_info);
      E::ScalarPtr group_by_scalar = resolveExpression(
          unresolved_group_by_expression,
          nullptr,  // No Type hint.
          &expr_resolution_info);

      // Rewrite it to a reference to a SELECT-list expression
      // if it is an ordinal reference.
      rewriteIfOrdinalReference(&unresolved_group_by_expression,
                                expr_resolution_info,
                                &select_list_info,
                                &group_by_scalar);

      if (group_by_scalar->isConstant()) {
        THROW_SQL_ERROR_AT(&unresolved_group_by_expression)
            << "Constant expression not allowed in GROUP BY";
      }

      // If the group by expression is not a named expression,
      // wrap it with an Alias.
      E::NamedExpressionPtr group_by_expression;
      if (!E::SomeNamedExpression::MatchesWithConditionalCast(group_by_scalar,
                                                              &group_by_expression)) {
        const std::string internal_alias =
            GenerateGroupingAttributeAlias(group_by_expressions.size());
        group_by_expression = E::Alias::Create(
            context_->nextExprId(),
            group_by_scalar,
            "" /* attribute_name */,
            internal_alias,
            "$groupby" /* relation_name */);
      }
      group_by_expressions.push_back(group_by_expression);
    }
  }

  // Resolve HAVING.
  E::PredicatePtr having_predicate;
  if (select_query.having() != nullptr) {
    ExpressionResolutionInfo expr_resolution_info(
        *name_resolver, &query_aggregation_info, &select_list_info);
    having_predicate = resolvePredicate(
        *select_query.having()->having_predicate(), &expr_resolution_info);
  }

  // Resolve ORDER BY.
  std::vector<E::ScalarPtr> order_by_expressions;
  // True if ascending.
  std::vector<bool> order_by_directions;
  std::vector<bool> nulls_first;
  if (select_query.order_by() != nullptr) {
    for (const ParseOrderByItem &order_by_item :
         *select_query.order_by()->order_by_items()) {
      ExpressionResolutionInfo expr_resolution_info(
          *name_resolver, &query_aggregation_info, &select_list_info);
      E::ScalarPtr order_by_scalar = resolveExpression(
          *order_by_item.ordering_expression(),
          nullptr,  // No Type hint.
          &expr_resolution_info);

      // Rewrite it to a reference to a SELECT-list expression
      // if it is an ordinal reference.
      rewriteIfOrdinalReference(&order_by_item, expr_resolution_info,
                                &select_list_info, &order_by_scalar);

      if (order_by_scalar->isConstant()) {
        THROW_SQL_ERROR_AT(&order_by_item)
            << "Constant expression not allowed in ORDER BY";
      }

      order_by_expressions.push_back(order_by_scalar);
      order_by_directions.push_back(order_by_item.is_ascending());
      nulls_first.push_back(order_by_item.nulls_first());
    }
  }

  // If we have GROUP BY, at least one aggregate expression or HAVING,
  // we need an Aggregate node. If there is a HAVING but no GROUP BY,
  // the implicit GROUP BY is GROUP BY () with no grouping columns.
  const bool need_aggregate =
      (!query_aggregation_info.aggregate_expressions.empty() ||
       !group_by_expressions.empty() || having_predicate != nullptr);

  // If we have GROUP BY, at least one aggregate expression or HAVING,
  // validate each SELECT-list, HAVING and ORDER BY expressions that
  // they do not reference an attribute that is neither a GROUP BY column or
  // an aggregate expression.
  if (need_aggregate) {
    std::unordered_set<E::ExprId> visible_expr_id_set;
    for (const E::AliasPtr &aggregate_expression :
         query_aggregation_info.aggregate_expressions) {
      visible_expr_id_set.emplace(aggregate_expression->id());
    }
    for (const E::NamedExpressionPtr &group_by_expression :
         group_by_expressions) {
      visible_expr_id_set.emplace(group_by_expression->id());
    }

    // First check all SELECT-list expressions.
    validateSelectExpressionsForAggregation(select_query.selection(),
                                            select_list_expressions,
                                            visible_expr_id_set);

    // Now we have validated all SELECT-list expressions,
    // add them to the visible expression set.
    for (const E::NamedExpressionPtr &select_list_expression :
         select_list_expressions) {
      visible_expr_id_set.emplace(select_list_expression->id());
    }

    // Next check the HAVING predicate and ordering expressions.
    if (select_query.having() != nullptr) {
      validateExpressionForAggregation(
          select_query.having()->having_predicate(), having_predicate,
          visible_expr_id_set);
    }
    if (select_query.order_by() != nullptr) {
      validateOrderingExpressionsForAggregation(*select_query.order_by(),
                                                order_by_expressions,
                                                visible_expr_id_set);
    }
  }

  // Add a Project if we need to precompute some SELECT-list columns
  // before Aggregate.
  appendProjectIfNeedPrecomputationBeforeAggregation(
      select_list_info, &select_list_expressions, &logical_plan);

  if (need_aggregate) {
    // Add an aggregate node.
    logical_plan =
        L::Aggregate::Create(logical_plan, group_by_expressions,
                             query_aggregation_info.aggregate_expressions);

    // Add a Project if we need to precompute some SELECT-list columns
    // after Aggregate.
    appendProjectIfNeedPrecomputationAfterAggregation(
        select_list_info, &select_list_expressions, &logical_plan);
  }

  if (having_predicate != nullptr) {
    logical_plan = L::Filter::Create(logical_plan, having_predicate);
  }

  if (!order_by_expressions.empty()) {
    // Here we may need another Project, because an ordering expression
    // may reference a SELECT expression at a non-top level.
    // Wrap a Project on the Sort if there is some non-attribute
    // ordering expression.
    std::vector<E::NamedExpressionPtr> wrap_project_expressions;
    std::vector<E::AttributeReferencePtr> order_by_attributes;
    for (const E::ScalarPtr &order_by_expression : order_by_expressions) {
      E::AttributeReferencePtr order_by_attribute;
      if (!E::SomeAttributeReference::MatchesWithConditionalCast(order_by_expression,
                                                                 &order_by_attribute)) {
        const std::string internal_alias =
            GenerateOrderingAttributeAlias(wrap_project_expressions.size());
        const E::AliasPtr computed_order_by_expression =
            E::Alias::Create(context_->nextExprId(),
                             order_by_expression,
                             "" /* attribute_name */,
                             internal_alias,
                             "$orderby" /* relation_name */);
        wrap_project_expressions.push_back(computed_order_by_expression);
        order_by_attribute = E::ToRef(computed_order_by_expression);
      }
      order_by_attributes.push_back(order_by_attribute);
    }

    if (!wrap_project_expressions.empty()) {
      const std::vector<expressions::AttributeReferencePtr> child_project_attributes =
          logical_plan->getOutputAttributes();
      wrap_project_expressions.insert(wrap_project_expressions.end(),
                                      child_project_attributes.begin(),
                                      child_project_attributes.end());
      logical_plan = L::Project::Create(logical_plan, wrap_project_expressions);
    }

    if (select_query.limit() != nullptr) {
      logical_plan =
          L::Sort::Create(logical_plan,
                          order_by_attributes,
                          order_by_directions,
                          nulls_first,
                          select_query.limit()->limit_expression()->long_value());
    } else {
      logical_plan =
          L::Sort::Create(logical_plan,
                          order_by_attributes,
                          order_by_directions,
                          nulls_first,
                          -1 /* limit */);
    }
  } else if (select_query.limit() != nullptr) {
    THROW_SQL_ERROR_AT(select_query.limit())
        << "LIMIT is not supported without ORDER BY";
  }

  logical_plan = L::Project::Create(logical_plan, select_list_expressions);

  return logical_plan;
}

void Resolver::appendProjectIfNeedPrecomputationBeforeAggregation(
    const SelectListInfo &select_list_info,
    std::vector<E::NamedExpressionPtr> *select_list_expressions,
    L::LogicalPtr *logical_plan) {
  DCHECK_EQ(select_list_info.select_list_expressions.size(),
            select_list_expressions->size());

  std::vector<E::NamedExpressionPtr> precomputed_expressions;
  for (std::vector<E::NamedExpressionPtr>::size_type idx = 0;
       idx < select_list_expressions->size();
       ++idx) {
    // Do not precompute aggregations.
    if (select_list_info.is_referenced[idx] &&
        !select_list_info.has_aggregate_per_expression[idx]) {
      precomputed_expressions.push_back((*select_list_expressions)[idx]);
      (*select_list_expressions)[idx] =
          E::ToRef((*select_list_expressions)[idx]);
    }
  }
  if (!precomputed_expressions.empty()) {
    const std::vector<expressions::AttributeReferencePtr> child_project_attributes =
        (*logical_plan)->getOutputAttributes();
    precomputed_expressions.insert(precomputed_expressions.end(),
                                   child_project_attributes.begin(),
                                   child_project_attributes.end());
    *logical_plan = L::Project::Create(*logical_plan, precomputed_expressions);
  }
}

void Resolver::appendProjectIfNeedPrecomputationAfterAggregation(
    const SelectListInfo &select_list_info,
    std::vector<expressions::NamedExpressionPtr> *select_list_expressions,
    logical::LogicalPtr *logical_plan) {
  DCHECK_EQ(select_list_info.select_list_expressions.size(),
            select_list_expressions->size());

  std::vector<E::NamedExpressionPtr> precomputed_expressions;
  for (std::vector<E::NamedExpressionPtr>::size_type idx = 0; idx < select_list_expressions->size(); ++idx) {
    // Precompute expressions that contain aggregations and
    // is not a simple wrapper of an attribute reference..
    if (select_list_info.is_referenced[idx] &&
        select_list_info.has_aggregate_per_expression[idx]) {
      E::AliasPtr select_alias;
      CHECK(E::SomeAlias::MatchesWithConditionalCast((*select_list_expressions)[idx],
                                                     &select_alias));
      if (!E::SomeAttributeReference::Matches(select_alias->expression())) {
        precomputed_expressions.push_back((*select_list_expressions)[idx]);
        (*select_list_expressions)[idx] =
            E::ToRef((*select_list_expressions)[idx]);
      }
    }
  }
  if (!precomputed_expressions.empty()) {
    const std::vector<expressions::AttributeReferencePtr> child_project_attributes =
        (*logical_plan)->getOutputAttributes();
    precomputed_expressions.insert(precomputed_expressions.end(),
                                   child_project_attributes.begin(),
                                   child_project_attributes.end());
    *logical_plan = L::Project::Create(*logical_plan, precomputed_expressions);
  }
}

void Resolver::validateSelectExpressionsForAggregation(
    const ParseSelectionClause &parse_selection,
    const std::vector<E::NamedExpressionPtr> &select_list_expressions,
    const std::unordered_set<E::ExprId> &visible_expr_id_set) const {
  if (parse_selection.getSelectionType() == ParseSelectionClause::kStar) {
    for (const E::NamedExpressionPtr &select_list_expression :
         select_list_expressions) {
      validateExpressionForAggregation(&parse_selection,
                                       select_list_expression,
                                       visible_expr_id_set);
    }
  } else {
    DCHECK_EQ(select_list_expressions.size(),
              static_cast<const ParseSelectionList&>(parse_selection)
                  .select_item_list()
                  .size());
    PtrList<ParseSelectionItem>::const_iterator parse_item_it =
        static_cast<const ParseSelectionList&>(parse_selection)
            .select_item_list()
            .begin();
    std::vector<E::NamedExpressionPtr>::const_iterator select_expression_it =
        select_list_expressions.begin();
    while (select_expression_it != select_list_expressions.end()) {
      // Look down the expression tree only if the SELECT expression
      // itself is not a group by column.
      if (visible_expr_id_set.find((*select_expression_it)->id()) ==
          visible_expr_id_set.end()) {
        validateExpressionForAggregation(
            &(*parse_item_it), *select_expression_it, visible_expr_id_set);
      }
      ++select_expression_it;
      ++parse_item_it;
    }
  }
}

void Resolver::validateOrderingExpressionsForAggregation(
    const ParseOrderBy &parse_order_by,
    const std::vector<E::ScalarPtr> &order_by_expressions,
    const std::unordered_set<E::ExprId> &visible_expr_id_set) const {
  DCHECK_EQ(parse_order_by.order_by_items()->size(),
            order_by_expressions.size());
  PtrList<ParseOrderByItem>::const_iterator parse_item_it =
      parse_order_by.order_by_items()->begin();
  std::vector<E::ScalarPtr>::const_iterator order_by_expression_it =
      order_by_expressions.begin();
  while (order_by_expression_it != order_by_expressions.end()) {
    validateExpressionForAggregation(&(*parse_item_it),
                                     *order_by_expression_it,
                                     visible_expr_id_set);
    ++parse_item_it;
    ++order_by_expression_it;
  }
}

void Resolver::validateExpressionForAggregation(
    const ParseTreeNode *location, const E::ExpressionPtr &expression,
    const std::unordered_set<E::ExprId> &visible_expr_id_set) const {
  if (expression->getNumChildren() > 0) {
    for (const E::ExpressionPtr &child : expression->children()) {
      validateExpressionForAggregation(location, child, visible_expr_id_set);
    }
  } else {
    E::AttributeReferencePtr attribute_reference;
    if (E::SomeAttributeReference::MatchesWithConditionalCast(expression, &attribute_reference) &&
        visible_expr_id_set.find(attribute_reference->id()) ==
            visible_expr_id_set.end()) {
      THROW_SQL_ERROR_AT(location)
          << "Expression contains an attribute "
          << attribute_reference->attribute_alias()
          << ", which is neither a GROUP BY expression nor in an aggregate "
             "expression";
    }
  }
}

L::LogicalPtr Resolver::resolveFromClause(
    const PtrList<ParseTableReference> &from_list,
    NameResolver *name_resolver) {
  std::vector<L::LogicalPtr> from_logical_list;
  for (const ParseTableReference &from_parse_item : from_list) {
    L::LogicalPtr from_logical_item =
        resolveTableReference(from_parse_item, name_resolver);
    from_logical_list.emplace_back(from_logical_item);
  }

  if (from_logical_list.size() > 1u) {
    return L::MultiwayCartesianJoin::Create(from_logical_list);
  } else {
    return from_logical_list[0];
  }
}

L::LogicalPtr Resolver::resolveTableReference(const ParseTableReference &table_reference,
                                              NameResolver *name_resolver) {
  L::LogicalPtr logical_plan;
  const ParseString *reference_alias = nullptr;
  const ParseTableReferenceSignature *reference_signature = table_reference.table_reference_signature();

  switch (table_reference.getTableReferenceType()) {
    case ParseTableReference::kSimpleTableReference: {
      const ParseSimpleTableReference &simple_table_reference =
          static_cast<const ParseSimpleTableReference&>(table_reference);

      if (reference_signature == nullptr) {
        reference_alias = simple_table_reference.table_name();
      } else {
        DCHECK(reference_signature->table_alias() != nullptr);
        reference_alias = reference_signature->table_alias();
      }

      logical_plan = resolveSimpleTableReference(*simple_table_reference.table_name(), reference_alias);

      if (reference_signature != nullptr && reference_signature->column_aliases() != nullptr) {
        logical_plan = RenameOutputColumns(logical_plan, *reference_signature);
      }
      // Create a logical sample operator. Applicable only if \p the table is materialized.
      // Sampling from subquery table references is not supported.
      if (simple_table_reference.sample() != nullptr) {
        logical_plan = L::Sample::Create(logical_plan,
        simple_table_reference.sample()->is_block_sample(),
        simple_table_reference.sample()->percentage()->long_value());
      }
      name_resolver->addRelation(reference_alias, logical_plan);
      break;
    }
    case ParseTableReference::kGeneratorTableReference: {
      const ParseGeneratorTableReference &generator_table_reference =
          static_cast<const ParseGeneratorTableReference&>(table_reference);

      if (reference_signature == nullptr) {
        reference_alias = generator_table_reference.generator_function()->name();
      } else {
        DCHECK(reference_signature->table_alias() != nullptr);
        reference_alias = reference_signature->table_alias();
      }

      logical_plan = resolveGeneratorTableReference(generator_table_reference,
                                                    reference_alias);

      if (reference_signature != nullptr && reference_signature->column_aliases() != nullptr) {
        logical_plan = RenameOutputColumns(logical_plan, *reference_signature);
      }

      name_resolver->addRelation(reference_alias, logical_plan);
      break;
    }
    case ParseTableReference::kSubqueryTableReference: {
      // Any un-named subquery in FROM should has been caught in the parser.
      DCHECK(reference_signature != nullptr)
          << "Subquery expressions in FROM must be explicitly named";
      DCHECK(reference_signature->table_alias() != nullptr);

      reference_alias = reference_signature->table_alias();
      logical_plan = resolveSelect(
          *static_cast<const ParseSubqueryTableReference&>(table_reference).subquery_expr()->query(),
          reference_alias->value());

      if (reference_signature->column_aliases() != nullptr) {
        logical_plan = RenameOutputColumns(logical_plan, *reference_signature);
      }

      name_resolver->addRelation(reference_alias, logical_plan);
      break;
    }
    default:
      LOG(FATAL) << "Unhandeled table reference " << table_reference.toString();
  }

  return logical_plan;
}

L::LogicalPtr Resolver::RenameOutputColumns(
    const L::LogicalPtr &logical_plan,
    const ParseTableReferenceSignature &table_signature) {
  const PtrList<ParseString> *column_aliases =
      table_signature.column_aliases();
  const std::vector<E::AttributeReferencePtr> output_attributes =
      logical_plan->getOutputAttributes();

  DCHECK(column_aliases != nullptr);

  // Rename each attribute to its column alias.
  std::vector<E::NamedExpressionPtr> project_expressions;
  if (column_aliases->size() != output_attributes.size()) {
    THROW_SQL_ERROR_AT(&table_signature)
        << "The number of named columns is different from the number of table columns ("
        << std::to_string(column_aliases->size()) << " vs. " << std::to_string(output_attributes.size()) << ")";
  }

  int attr_index = 0;
  for (const ParseString &column_alias : *column_aliases) {
    project_expressions.emplace_back(
        E::Alias::Create(
            context_->nextExprId(),
            output_attributes[attr_index],
            column_alias.value(),
            column_alias.value()));
    ++attr_index;
  }

  return L::Project::Create(logical_plan, project_expressions);
}

const CatalogRelation* Resolver::resolveRelationName(
    const ParseString *relation_name) {
  const CatalogRelation *relation =
      context_->catalog_database()->getRelationByName(
          ToLower(relation_name->value()));
  if (relation == nullptr) {
    THROW_SQL_ERROR_AT(relation_name) << "Unrecognized relation "
                                      << relation_name->value();
  }
  return relation;
}

L::LogicalPtr Resolver::resolveSimpleTableReference(
    const ParseString &table_name, const ParseString *reference_alias) {
  // First look up the name in the subqueries.
  const std::string lower_table_name = ToLower(table_name.value());
  const std::unordered_map<std::string, int>::const_iterator subplan_it =
      with_queries_info_.with_query_name_to_vector_position.find(lower_table_name);
  if (subplan_it != with_queries_info_.with_query_name_to_vector_position.end()) {
    with_queries_info_.unreferenced_query_indexes.erase(subplan_it->second);
    return L::SharedSubplanReference::Create(
        subplan_it->second,
        with_queries_info_.with_query_plans[subplan_it->second]->getOutputAttributes());
  }

  // Then look up the name in the database.
  const CatalogRelation *relation = resolveRelationName(&table_name);
  const ParseString *scoped_table_name;
  if (reference_alias == nullptr) {
    scoped_table_name = &table_name;
  } else {
    scoped_table_name = reference_alias;
  }

  return L::TableReference::Create(relation, scoped_table_name->value(), context_);
}

L::LogicalPtr Resolver::resolveGeneratorTableReference(
    const ParseGeneratorTableReference &table_reference,
    const ParseString *reference_alias) {
  const ParseString *func_name = table_reference.generator_function()->name();

  // Resolve the generator function
  const quickstep::GeneratorFunction *func_template =
      GeneratorFunctionFactory::Instance().getByName(func_name->value());
  if (func_template == nullptr) {
      THROW_SQL_ERROR_AT(func_name)
          << "Generator function " << func_name->value() << " not found";
  }

  // Check that all arguments are constant literals, also convert them into a
  // list of TypedValue's.
  const PtrList<ParseExpression> *func_args = table_reference.generator_function()->arguments();
  std::vector<TypedValue> concretized_args;
  if (func_args != nullptr) {
    for (const ParseExpression& arg : *func_args) {
      if (arg.getExpressionType() != ParseExpression::kScalarLiteral) {
        THROW_SQL_ERROR_AT(&arg)
            << "Argument(s) to a generator function can only be constant literals";
      }
      const ParseScalarLiteral &scalar_literal_arg = static_cast<const ParseScalarLiteral&>(arg);
      const Type* dumb_concretized_type;
      concretized_args.emplace_back(
          scalar_literal_arg.literal_value()->concretize(nullptr, &dumb_concretized_type));
    }
  }

  // Concretize the generator function with the arguments.
  quickstep::GeneratorFunctionHandle *func_handle = nullptr;
  try {
    func_handle = func_template->createHandle(concretized_args);
  } catch (const std::exception &e) {
    DCHECK(func_handle == nullptr);
    THROW_SQL_ERROR_AT(table_reference.generator_function()) << e.what();
  }
  if (func_handle == nullptr) {
    THROW_SQL_ERROR_AT(table_reference.generator_function())
        << "Invalid arguments";
  }

  return L::TableGenerator::Create(quickstep::GeneratorFunctionHandlePtr(func_handle),
                                   reference_alias->value(),
                                   context_);
}


void Resolver::resolveSelectClause(
    const ParseSelectionClause &parse_selection,
    const std::string &select_name,
    const NameResolver &name_resolver,
    QueryAggregationInfo *query_aggregation_info,
    std::vector<expressions::NamedExpressionPtr> *project_expressions,
    std::vector<bool> *has_aggregate_per_expression) {
  project_expressions->clear();
  switch (parse_selection.getSelectionType()) {
    case ParseSelectionClause::kStar: {
      // Add all visible attributes.
      *project_expressions = E::ToNamedExpressions(name_resolver.getVisibleAttributeReferences());
      has_aggregate_per_expression->insert(has_aggregate_per_expression->end(),
                                           project_expressions->size(),
                                           false);
      break;
    }
    case ParseSelectionClause::kNonStar: {
      const ParseSelectionList &selection_list =
          static_cast<const ParseSelectionList&>(parse_selection);
      for (const ParseSelectionItem &selection_item :
           selection_list.select_item_list()) {
        const ParseString *parse_alias = selection_item.alias();
        const ParseExpression *parse_project_expression =
            selection_item.expression();
        ExpressionResolutionInfo expr_resolution_info(
            name_resolver,
            query_aggregation_info,
            nullptr /* select_list_info */);
        const E::ScalarPtr project_scalar =
            resolveExpression(*parse_project_expression,
                              nullptr,  // No Type hint.
                              &expr_resolution_info);

        // If the resolved expression is a named expression,
        // we reuse its ID. Because we resolve an aggregate function
        // call to an Alias, we need to maintain the ID in the
        // SELECT-list expression so that we can later correctly
        // identify the SELECT-list expression during the aggregate validation.
        E::ExprId project_expression_id;
        E::NamedExpressionPtr project_named_expression;
        if (E::SomeNamedExpression::MatchesWithConditionalCast(project_scalar,
                                                               &project_named_expression)) {
          project_expression_id = project_named_expression->id();
        } else {
          project_expression_id = context_->nextExprId();
        }

        if (parse_alias != nullptr) {
          project_named_expression = E::Alias::Create(project_expression_id,
                                                      project_scalar,
                                                      parse_alias->value(),
                                                      parse_alias->value(),
                                                      select_name);
        } else {
          // Create an Alias if the expression is not a named
          // expression.
          if (project_named_expression == nullptr) {
            const std::string generated_name = selection_item.generateName();
            project_named_expression = E::Alias::Create(project_expression_id,
                                                        project_scalar,
                                                        "" /* attribute_name */,
                                                        generated_name,
                                                        select_name);
          } else if (project_named_expression->attribute_alias().at(0) == '$') {
            // Rename the expression with an internal alias name so that the output
            // is more human readable.
            project_named_expression = E::Alias::Create(project_named_expression->id(),
                                                        project_named_expression,
                                                        project_named_expression->attribute_name(),
                                                        selection_item.generateName(),
                                                        select_name);
          }
        }
        project_expressions->push_back(project_named_expression);
        has_aggregate_per_expression->push_back(
            expr_resolution_info.hasAggregate());
      }
      break;
    }
  }
}

E::ScalarPtr Resolver::resolveExpression(
    const ParseExpression &parse_expression,
    const Type *type_hint,
    ExpressionResolutionInfo *expression_resolution_info) {
  switch (parse_expression.getExpressionType()) {
    case ParseExpression::kAttribute: {
      const ParseAttribute &parse_attribute_scalar =
          static_cast<const ParseAttribute&>(parse_expression);
      if (parse_attribute_scalar.rel_name() == nullptr &&
          expression_resolution_info->select_list_info != nullptr) {
        // Check if it is an alias reference to a SELECT-list expression.
        const std::map<std::string, int> &alias_map =
            expression_resolution_info->select_list_info->getAliasMap();
        const std::string lower_alias =
            ToLower(parse_attribute_scalar.attr_name()->value());
        std::map<std::string, int>::const_iterator found_it =
            alias_map.find(lower_alias);
        if (found_it != alias_map.end()) {
          if (found_it->second == -1) {
            THROW_SQL_ERROR_AT(&parse_expression)
                << "Alias " << parse_attribute_scalar.attr_name()->value()
                << " is ambiguous in the SELECT list";
          }
          DCHECK_GE(found_it->second, 0);
          DCHECK_LT(
              found_it->second,
              static_cast<int>(expression_resolution_info->select_list_info->select_list_expressions.size()));

          if (expression_resolution_info->select_list_info
                  ->has_aggregate_per_expression[found_it->second]) {
            if (!expression_resolution_info->not_allow_aggregate_here.empty()) {
              THROW_SQL_ERROR_AT(&parse_expression)
                  << "SELECT-list column "
                  << parse_attribute_scalar.attr_name()->value()
                  << " contains an aggregate function, which is not allowed in "
                  << expression_resolution_info->not_allow_aggregate_here;
            }
            expression_resolution_info->parse_aggregate_expression =
                &parse_expression;
          }

          const E::NamedExpressionPtr select_list_expression =
              expression_resolution_info->select_list_info->select_list_expressions[found_it->second];
          if (E::SomeAttributeReference::Matches(select_list_expression)) {
            return select_list_expression;
          } else {
            // Do not precompute constant expressions.
            if (select_list_expression->isConstant()) {
              // De-alias the expression.
              E::AliasPtr select_list_alias_expression;
              CHECK(E::SomeAlias::MatchesWithConditionalCast(select_list_expression,
                                                             &select_list_alias_expression));
              return std::static_pointer_cast<const E::Scalar>(
                  select_list_alias_expression->expression());
            }

            expression_resolution_info->select_list_info->is_referenced[found_it->second] = true;
            return E::ToRef(select_list_expression);
          }
        }
      }
      return expression_resolution_info->name_resolver.lookup(
          parse_attribute_scalar.attr_name(),
          parse_attribute_scalar.rel_name());
    }
    case ParseExpression::kBinaryExpression: {
      const ParseBinaryExpression &parse_binary_scalar =
          static_cast<const ParseBinaryExpression&>(parse_expression);

      std::pair<const Type*, const Type*> argument_type_hints
          = parse_binary_scalar.op().pushDownTypeHint(type_hint);

      ExpressionResolutionInfo left_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr left_argument = resolveExpression(
          *parse_binary_scalar.left_operand(),
           argument_type_hints.first,
           &left_resolution_info);

      ExpressionResolutionInfo right_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr right_argument = resolveExpression(
          *parse_binary_scalar.right_operand(),
          argument_type_hints.second,
          &right_resolution_info);

      if (left_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            left_resolution_info.parse_aggregate_expression;
      } else if (right_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            right_resolution_info.parse_aggregate_expression;
      }

      // Check if either argument is a NULL literal of an unknown type.
      const bool left_is_nulltype = (left_argument->getValueType().getTypeID() == kNullType);
      const bool right_is_nulltype = (right_argument->getValueType().getTypeID() == kNullType);

      // If either argument is a NULL of unknown type, we try to resolve the
      // type of this BinaryExpression as follows:
      //
      //     1. If there is only one possible result type for the expression
      //        based on what is known about its argument types, then the
      //        result is a NULL of that type.
      //     2. Otherwise, if there is a type hint for the BinaryExpression's
      //        result, and if it is a plausible result type based on what we
      //        know about argument types, then the result is a NULL of the
      //        hint type.
      //     3. Otherwise, check if the BinaryExpression can plausibly be
      //        applied to the known argument types at all. If so, then the
      //        result is a NULL of unknown type (i.e. NullType).
      //     4. If all of the above steps fail, then the BinaryExpression is
      //        not possibly applicable to the given arguments.
      //
      // NOTE(chasseur): Step #3 above does not completely capture knowledge
      // about the result type of a BinaryExpression with one or more unknown
      // arguments. For instance, DivideBinaryOperation can never return a
      // DateTime or any string type, so even if we do not know its specific
      // return type, we do know that there are some restrictions on what it
      // may be. However, NullType is implicitly convertable to ANY Type, so
      // such restrictions could be violated if a parent node in the expression
      // tree converts a value of NullType to something that it shouldn't be.
      if (left_is_nulltype || right_is_nulltype) {
        const Type *fixed_result_type
            = parse_binary_scalar.op().resultTypeForPartialArgumentTypes(
                left_is_nulltype ? nullptr : &(left_argument->getValueType()),
                right_is_nulltype ? nullptr : &(right_argument->getValueType()));
        if (fixed_result_type != nullptr) {
          return E::ScalarLiteral::Create(fixed_result_type->makeNullValue(),
                                          *fixed_result_type);
        }

        if (type_hint != nullptr) {
          const Type &nullable_type_hint = type_hint->getNullableVersion();
          if (parse_binary_scalar.op().partialTypeSignatureIsPlausible(
                  &nullable_type_hint,
                  left_is_nulltype ? nullptr : &(left_argument->getValueType()),
                  right_is_nulltype ? nullptr : &(right_argument->getValueType()))) {
            return E::ScalarLiteral::Create(nullable_type_hint.makeNullValue(),
                                            nullable_type_hint);
          }
        }

        if (parse_binary_scalar.op().partialTypeSignatureIsPlausible(
                nullptr,
                left_is_nulltype ? nullptr : &(left_argument->getValueType()),
                right_is_nulltype ? nullptr : &(right_argument->getValueType()))) {
          const Type &null_type = TypeFactory::GetType(kNullType, true);
          return E::ScalarLiteral::Create(null_type.makeNullValue(),
                                          null_type);
        }

        // If nothing above worked, fall through to canApplyToTypes() below,
        // which should fail.
      }

      if (!parse_binary_scalar.op().canApplyToTypes(left_argument->getValueType(),
                                                    right_argument->getValueType())) {
        THROW_SQL_ERROR_AT(&parse_binary_scalar)
            << "Can not apply binary operation \"" << parse_binary_scalar.op().getName()
            << "\" to arguments of types " << left_argument->getValueType().getName()
            << " and " << right_argument->getValueType().getName();
      }

      return E::BinaryExpression::Create(parse_binary_scalar.op(),
                                         left_argument,
                                         right_argument);
    }
    case ParseExpression::kScalarLiteral: {
      const ParseScalarLiteral &parse_literal_scalar =
          static_cast<const ParseScalarLiteral&>(parse_expression);
      const Type *concrete_type = nullptr;
      TypedValue concrete = parse_literal_scalar.literal_value()
          ->concretize(type_hint, &concrete_type);
      return E::ScalarLiteral::Create(std::move(concrete), *concrete_type);
    }
    case ParseExpression::kUnaryExpression: {
      const ParseUnaryExpression &parse_unary_expr =
          static_cast<const ParseUnaryExpression&>(parse_expression);

      E::ScalarPtr argument = resolveExpression(
          *parse_unary_expr.operand(),
          parse_unary_expr.op().pushDownTypeHint(type_hint),
          expression_resolution_info);

      // If the argument is a NULL of unknown Type, try to resolve result Type
      // of this UnaryExpression as follows:
      //
      //     1. If the UnaryExpression can only return one type, then the
      //        result is a NULL of that type.
      //     2. If there is a type hint for the UnaryExpression's result, and
      //        it is possible for the UnaryExpression to return the hinted
      //        type, then the result is a NULL of that type.
      //     3. Otherwise, the result is a NULL of unknown type (i.e.
      //        NullType).
      //
      // NOTE(chasseur): As with binary expressions above, step #3 does not
      // always completely capture information about what types the NULL result
      // can take on, since NullType is implicitly convertable to any Type.
      if (argument->getValueType().getTypeID() == kNullType) {
        const Type *fixed_result_type = parse_unary_expr.op().fixedNullableResultType();
        if (fixed_result_type != nullptr) {
          return E::ScalarLiteral::Create(fixed_result_type->makeNullValue(),
                                          *fixed_result_type);
        }

        if (type_hint != nullptr) {
          const Type &nullable_type_hint = type_hint->getNullableVersion();
          if (parse_unary_expr.op().resultTypeIsPlausible(nullable_type_hint)) {
            return E::ScalarLiteral::Create(nullable_type_hint.makeNullValue(),
                                            nullable_type_hint);
          }
        }

        const Type &null_type = TypeFactory::GetType(kNullType, true);
        return E::ScalarLiteral::Create(null_type.makeNullValue(),
                                        null_type);
      }

      if (!parse_unary_expr.op().canApplyToType(argument->getValueType())) {
        THROW_SQL_ERROR_AT(&parse_unary_expr)
            << "Can not apply unary operation \"" << parse_unary_expr.op().getName()
            << "\" to argument of type " << argument->getValueType().getName();
      }

      return E::UnaryExpression::Create(parse_unary_expr.op(), argument);
    }
    case ParseExpression::kFunctionCall: {
      return resolveFunctionCall(
          static_cast<const ParseFunctionCall&>(parse_expression),
          expression_resolution_info);
    }
    case ParseExpression::kSubqueryExpression: {
      THROW_SQL_ERROR_AT(&parse_expression)
          << "Subquery expression in a non-FROM clause is not supported yet";
    }
    default:
      LOG(FATAL) << "Unknown scalar type: "
                 << parse_expression.getExpressionType();
  }
}

// TODO(chasseur): For now this only handles resolving aggregate functions. In
// the future it should be extended to resolve scalar functions as well.
E::ScalarPtr Resolver::resolveFunctionCall(
    const ParseFunctionCall &parse_function_call,
    ExpressionResolutionInfo *expression_resolution_info) {
  std::string function_name = ToLower(parse_function_call.name()->value());

  // First check for the special case COUNT(*).
  bool count_star = false;
  if (parse_function_call.star() != nullptr) {
    if (function_name != "count") {
      THROW_SQL_ERROR_AT(parse_function_call.star())
          << "Only COUNT can have star (*) as an argument";
    }
    count_star = true;
  }

  std::vector<E::ScalarPtr> resolved_arguments;
  const PtrList<ParseExpression> *unresolved_arguments =
      parse_function_call.arguments();
  // The first aggregate function in the arguments.
  const ParseTreeNode *first_aggregate_function = nullptr;
  if (unresolved_arguments != nullptr) {
    for (const ParseExpression &unresolved_argument : *unresolved_arguments) {
      ExpressionResolutionInfo expr_resolution_info(
          *expression_resolution_info);
      resolved_arguments.push_back(
          resolveExpression(unresolved_argument,
                            nullptr,  // No Type hint.
                            &expr_resolution_info));
      if (expr_resolution_info.hasAggregate() &&
          first_aggregate_function == nullptr) {
        first_aggregate_function =
            expr_resolution_info.parse_aggregate_expression;
      }
    }
  }

  if (count_star && !resolved_arguments.empty()) {
    THROW_SQL_ERROR_AT(&parse_function_call)
        << "COUNT aggregate has both star (*) and non-star arguments.";
  }

  // Try to look up the AggregateFunction by name using
  // AggregateFunctionFactory.
  const ::quickstep::AggregateFunction *aggregate
      = AggregateFunctionFactory::GetByName(function_name);
  if (aggregate == nullptr) {
    THROW_SQL_ERROR_AT(&parse_function_call)
        << "Unrecognized function name \""
        << parse_function_call.name()->value()
        << "\"";
  }

  // Make sure aggregates are allowed in this context.
  if (!expression_resolution_info->not_allow_aggregate_here.empty()) {
    THROW_SQL_ERROR_AT(&parse_function_call)
        << "Aggregate function not allowed in "
        << expression_resolution_info->not_allow_aggregate_here;
  }
  if (first_aggregate_function != nullptr) {
    THROW_SQL_ERROR_AT(first_aggregate_function)
        << "Aggregation of Aggregates are not allowed";
  }

  // Make sure a naked COUNT() with no arguments wasn't specified.
  if ((aggregate->getAggregationID() == AggregationID::kCount)
      && (resolved_arguments.empty())
      && (!count_star)) {
    THROW_SQL_ERROR_AT(&parse_function_call)
        << "COUNT aggregate requires an argument (either scalar or star (*))";
  }

  // Resolve each of the Scalar arguments to the aggregate.
  std::vector<const Type*> argument_types;
  for (const E::ScalarPtr &argument : resolved_arguments) {
    argument_types.emplace_back(&argument->getValueType());
  }

  // Make sure that the aggregate can apply to the specified argument(s).
  if (!aggregate->canApplyToTypes(argument_types)) {
    THROW_SQL_ERROR_AT(&parse_function_call)
        << "Aggregate function " << aggregate->getName()
        << " can not apply to the given argument(s).";
  }

  // Create the optimizer representation of the resolved aggregate and an alias
  // for it to appear in the output relation.
  const E::AggregateFunctionPtr aggregate_function
      = E::AggregateFunction::Create(*aggregate,
                                     resolved_arguments,
                                     expression_resolution_info->query_aggregation_info->has_group_by);
  const std::string internal_alias = GenerateAggregateAttributeAlias(
      expression_resolution_info->query_aggregation_info->aggregate_expressions.size());
  const E::AliasPtr aggregate_alias = E::Alias::Create(context_->nextExprId(),
                                                       aggregate_function,
                                                       "" /* attribute_name */,
                                                       internal_alias,
                                                       "$aggregate" /* relation_name */);
  expression_resolution_info->query_aggregation_info->aggregate_expressions.emplace_back(aggregate_alias);
  expression_resolution_info->parse_aggregate_expression = &parse_function_call;
  return E::ToRef(aggregate_alias);
}

std::vector<E::PredicatePtr> Resolver::resolvePredicates(
    const PtrList<ParsePredicate> &parse_predicates,
    ExpressionResolutionInfo *expression_resolution_info) {
  std::vector<E::PredicatePtr> resolved_predicates;
  for (const ParsePredicate &parse_predicate : parse_predicates) {
    ExpressionResolutionInfo child_expression_resolution_info(
        *expression_resolution_info);
    resolved_predicates.push_back(
        resolvePredicate(parse_predicate, &child_expression_resolution_info));
    if (child_expression_resolution_info.hasAggregate() &&
        expression_resolution_info->parse_aggregate_expression == nullptr) {
      expression_resolution_info->parse_aggregate_expression =
          child_expression_resolution_info.parse_aggregate_expression;
    }
  }
  return resolved_predicates;
}

void Resolver::checkComparisonCanApplyTo(
    const ParseTreeNode *location,
    const Comparison &op,
    const expressions::ScalarPtr &left_operand,
    const expressions::ScalarPtr &right_operand) const {
  if (!op.canCompareTypes(left_operand->getValueType(), right_operand->getValueType())) {
     THROW_SQL_ERROR_AT(location)
         << "Comparison operation " << op.getName()
         << " cannot be applied between "
         << left_operand->getValueType().getName() << " and "
         << right_operand->getValueType().getName();
  }
}

E::PredicatePtr Resolver::resolvePredicate(
    const ParsePredicate &parse_predicate,
    ExpressionResolutionInfo *expression_resolution_info) {
  switch (parse_predicate.getParsePredicateType()) {
    case ParsePredicate::kBetween: {
      const ParsePredicateBetween &between_predicate =
          static_cast<const ParsePredicateBetween&>(parse_predicate);
      ExpressionResolutionInfo lower_bound_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr lower_bound_operand =
          resolveExpression(*between_predicate.lower_bound_operand(),
                            nullptr,  // No Type hint
                            &lower_bound_resolution_info);
      ExpressionResolutionInfo check_expr_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr check_operand = resolveExpression(
          *between_predicate.check_operand(),
          nullptr,  // No Type hint.
          &check_expr_resolution_info);
      ExpressionResolutionInfo upper_bound_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr upper_bound_operand =
          resolveExpression(*between_predicate.upper_bound_operand(),
                            nullptr,  // No Type hint.
                            &upper_bound_resolution_info);

      if (lower_bound_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            lower_bound_resolution_info.parse_aggregate_expression;
      } else if (check_expr_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            check_expr_resolution_info.parse_aggregate_expression;
      } else if (upper_bound_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            upper_bound_resolution_info.parse_aggregate_expression;
      }

      const Comparison &less_or_equal =
          ComparisonFactory::GetComparison(ComparisonID::kLessOrEqual);

      // If any of the arguments are NULL of unknown type, check that the
      // less-or-equal comparison is applicable and assume it is false.
      //
      // TODO(chasseur): Once we have proper 3-valued logic, we should assume
      // unknown in this case, not false.
      const bool check_operand_nulltype = check_operand->getValueType().getTypeID() == kNullType;
      const bool lower_bound_nulltype = lower_bound_operand->getValueType().getTypeID() == kNullType;
      const bool upper_bound_nulltype = upper_bound_operand->getValueType().getTypeID() == kNullType;

      E::PredicatePtr lower_bound_compare;
      if (lower_bound_nulltype || check_operand_nulltype) {
        if (less_or_equal.canComparePartialTypes(
                lower_bound_nulltype ? nullptr : &(lower_bound_operand->getValueType()),
                check_operand_nulltype ? nullptr : &(check_operand->getValueType()))) {
          lower_bound_compare = E::PredicateLiteral::Create(false);
        }
      }
      if (!lower_bound_compare) {
        checkComparisonCanApplyTo(between_predicate.lower_bound_operand(),
                                  less_or_equal,
                                  lower_bound_operand,
                                  check_operand);
        lower_bound_compare = E::ComparisonExpression::Create(less_or_equal,
                                                              lower_bound_operand,
                                                              check_operand);
      }

      E::PredicatePtr upper_bound_compare;
      if (check_operand_nulltype || upper_bound_compare) {
        if (less_or_equal.canComparePartialTypes(
                check_operand_nulltype ? nullptr : &(check_operand->getValueType()),
                upper_bound_nulltype ? nullptr : &(upper_bound_operand->getValueType()))) {
          upper_bound_compare = E::PredicateLiteral::Create(false);
        }
      }
      if (!upper_bound_compare) {
        checkComparisonCanApplyTo(between_predicate.upper_bound_operand(),
                                  less_or_equal,
                                  check_operand,
                                  upper_bound_operand);
        upper_bound_compare = E::ComparisonExpression::Create(less_or_equal,
                                                              check_operand,
                                                              upper_bound_operand);
      }

      return E::LogicalAnd::Create(
          {E::ComparisonExpression::Create(less_or_equal,
                                           lower_bound_operand,
                                           check_operand),
           E::ComparisonExpression::Create(less_or_equal,
                                           check_operand,
                                           upper_bound_operand)});
    }
    case ParsePredicate::kComparison: {
      const ParsePredicateComparison &comparison_predicate =
          static_cast<const ParsePredicateComparison&>(parse_predicate);

      ExpressionResolutionInfo left_expr_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr left_operand = resolveExpression(
          *comparison_predicate.left_operand(),
          nullptr,  // No Type hint
          &left_expr_resolution_info);

      ExpressionResolutionInfo right_expr_resolution_info(
          *expression_resolution_info);
      E::ScalarPtr right_operand = resolveExpression(
          *comparison_predicate.right_operand(),
          nullptr,  // No Type hint
          &right_expr_resolution_info);

      if (left_expr_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            left_expr_resolution_info.parse_aggregate_expression;
      } else if (right_expr_resolution_info.hasAggregate()) {
        expression_resolution_info->parse_aggregate_expression =
            right_expr_resolution_info.parse_aggregate_expression;
      }

      // If either side of the comparison is NULL, just check that the
      // comparison is applicable and assume it is false.
      //
      // TODO(chasseur): Once we have proper 3-valued logic, we should assume
      // unknown in this case, not false.
      const bool left_operand_nulltype = left_operand->getValueType().getTypeID() == kNullType;
      const bool right_operand_nulltype = right_operand->getValueType().getTypeID() == kNullType;
      if (left_operand_nulltype || right_operand_nulltype) {
        if (comparison_predicate.op().canComparePartialTypes(
                left_operand_nulltype ? nullptr : &(left_operand->getValueType()),
                right_operand_nulltype ? nullptr : &(right_operand->getValueType()))) {
          return E::PredicateLiteral::Create(false);
        }
      }

      checkComparisonCanApplyTo(&comparison_predicate,
                                comparison_predicate.op(),
                                left_operand,
                                right_operand);

      return E::ComparisonExpression::Create(comparison_predicate.op(),
                                             left_operand,
                                             right_operand);
    }
    case ParsePredicate::kNegation: {
      const ParsePredicateNegation &negation_predicate =
          static_cast<const ParsePredicateNegation&>(parse_predicate);
      const E::PredicatePtr operand = resolvePredicate(
          *negation_predicate.operand(), expression_resolution_info);
      return E::LogicalNot::Create(operand);
    }
    case ParsePredicate::kConjunction: {
      return E::LogicalAnd::Create(
          resolvePredicates(static_cast<const ParsePredicateWithList&>(parse_predicate).operands(),
                            expression_resolution_info));
    }
    case ParsePredicate::kDisjunction: {
      return E::LogicalOr::Create(
          resolvePredicates(static_cast<const ParsePredicateWithList&>(parse_predicate).operands(),
                            expression_resolution_info));
    }
    default:
      LOG(FATAL) << "Unknown predicate: " << parse_predicate.toString();
  }
}

void Resolver::rewriteIfOrdinalReference(
    const ParseTreeNode *location,
    const ExpressionResolutionInfo &expr_resolution_info,
    SelectListInfo *select_list_info, E::ScalarPtr *expression) {
  E::ScalarLiteralPtr literal;
  if (E::SomeScalarLiteral::MatchesWithConditionalCast(*expression, &literal) &&
      literal->getValueType().getTypeID() == kInt &&
      !literal->value().isNull()) {
    int position = literal->value().getLiteral<int>();
    if (position < 1 ||
        position > static_cast<int>(
                       select_list_info->select_list_expressions.size())) {
      THROW_SQL_ERROR_AT(location)
          << "SELECT-list position " << std::to_string(position)
          << " is out of range [1, "
          << std::to_string(select_list_info->select_list_expressions.size())
          << "]";
    }

    --position;  // Convert to 0-based.

    if (!expr_resolution_info.not_allow_aggregate_here.empty() &&
        select_list_info->has_aggregate_per_expression[position]) {
      THROW_SQL_ERROR_AT(location)
          << "SELECT-list position " << std::to_string(position)
          << " contains an aggregate function, which is not allowed in "
          << expr_resolution_info.not_allow_aggregate_here;
    }

    if (E::SomeAttributeReference::Matches(
            select_list_info->select_list_expressions[position])) {
      *expression = select_list_info->select_list_expressions[position];
      return;
    }

    // Do not precompute constant expressions.
    if (select_list_info->select_list_expressions[position]->isConstant()) {
      // De-alias the expression.
      E::AliasPtr select_list_alias_expression;
      CHECK(E::SomeAlias::MatchesWithConditionalCast(select_list_info->select_list_expressions[position],
                                                     &select_list_alias_expression));
      CHECK(E::SomeScalar::MatchesWithConditionalCast(select_list_alias_expression->expression(),
                                                      expression));
      return;
    }

    *expression = E::ToRef(select_list_info->select_list_expressions[position]);
    select_list_info->is_referenced[position] = true;
  }
}

std::string Resolver::GenerateAggregateAttributeAlias(int index) {
  return std::string("$aggregate").append(std::to_string(index));
}

std::string Resolver::GenerateGroupingAttributeAlias(int index) {
  return std::string("$groupby").append(std::to_string(index));
}

std::string Resolver::GenerateOrderingAttributeAlias(int index) {
  return std::string("$orderby").append(std::to_string(index));
}

}  // namespace resolver
}  // namespace optimizer
}  // namespace quickstep
