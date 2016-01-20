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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RESOLVER_RESOLVER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RESOLVER_RESOLVER_HPP_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
class Comparison;
class ParseExpression;
class ParseFunctionCall;
class ParseOrderBy;
class ParsePredicate;
class ParseSelect;
class ParseSelectionClause;
class ParseSimpleTableReference;
class ParseStatement;
class ParseStatementCopyFrom;
class ParseStatementCreateTable;
class ParseStatementDelete;
class ParseStatementDropTable;
class ParseStatementInsert;
class ParseStatementSelect;
class ParseStatementUpdate;
class ParseString;
class ParseTableReference;
class ParseTableReferenceSignature;
class ParseTreeNode;
template <class T>
class PtrList;
class Type;

}  // namespace quickstep

namespace quickstep {
namespace optimizer {

class OptimizerContext;

namespace resolver {

class NameResolver;

/** \addtogroup QueryResolver
 *  @{
 */

/**
 * @brief Performs semantic analysis and validation on a parse tree for a SELECT
 *        statement and outputs an initial logical plan.
 */
class Resolver {
 public:
  /**
   * @brief Constructor. Does not take ownership of \p context.
   *
   * @param context The context of this query.
   */
  explicit Resolver(OptimizerContext *context)
      : context_(context) {}

  /**
   * @brief Validates the query is semantically correct and converts the parse
   *        tree to an initial logical plan.
   *
   * @param parse_query
   * @return The converted logical node.
   */
  logical::LogicalPtr resolve(const ParseStatement &parse_query);

 private:
  /**
   * @brief Expression-scoped info that contains both constant and non-constant
   *        info (e.g. the name resolver, whether an aggregate is allowed)
   *        during resolving an expression.
   */
  struct ExpressionResolutionInfo;

  /**
   * @brief Query-scoped info that contains the aggregate expressions in a query
   *        block.
   */
  struct QueryAggregationInfo;

  /**
   * @brief Query-scoped info that contains select-list expressions
   *        and whether an expression is referenced by an
   *        ordinal or alias reference.
   */
  struct SelectListInfo;

  /**
   * @brief Contains resolution info for subqueries in the WITH clause.
   */
  struct WithQueriesInfo {
    /**
     * @brief The plans for WITH queries.
     */
    std::vector<logical::LogicalPtr> with_query_plans;

    /**
     * @brief The query name (the name of the table derived by the query) to
     *        the position of the plan in <with_query_plans>.
     */
    std::unordered_map<std::string, int> with_query_name_to_vector_position;

    /**
     * @brief Indexes of WITH queries that are not used.
     */
    std::unordered_set<int> unreferenced_query_indexes;
  };

  /**
   * @brief Resolves a SELECT (sub)query and returns a logical plan.
   *
   * @param select_statement The SELECT parse tree.
   * @param select_name The name for the SELECT query.
   * @return A logical plan for the SELECT query.
   */
  logical::LogicalPtr resolveSelect(
      const ParseSelect &select_statement,
      const std::string &select_name);

  /**
   * @brief Resolves a CREATE TABLE query and returns a logical plan.
   *
   * @param create_table_statement The CREATE TABLE parse tree.
   * @return A logical plan for the CREATE TABLE query.
   */
  logical::LogicalPtr resolveCreateTable(
      const ParseStatementCreateTable &create_table_statement);

  /**
   * @brief Resolves a DELETE query and returns a logical plan.
   *
   * @param delete_statement The DELETE parse tree.
   * @return A logical plan for the DELETE query.
   */
  logical::LogicalPtr resolveDelete(
      const ParseStatementDelete &delete_statement);

  /**
   * @brief Resolves a DROP TABLE query and returns a logical plan.
   *
   * @param drop_table_statement The DROP TABLE parse tree.
   * @return A logical plan for the DROP TABLE query.
   */
  logical::LogicalPtr resolveDropTable(
      const ParseStatementDropTable &drop_table_statement);

  /**
   * @brief Resolves a INSERT query and returns a logical plan.
   *
   * @param insert_statement The INSERT parse tree.
   * @return A logical plan for the INSERT query.
   */
  logical::LogicalPtr resolveInsert(
      const ParseStatementInsert &insert_statement);

  /**
   * @brief Resolves a COPY FROM query and returns a logical plan.
   *
   * @param copy_from_statement The COPY FROM parse tree.
   * @return A logical plan for the COPY FROM query.
   */
  logical::LogicalPtr resolveCopyFrom(
      const ParseStatementCopyFrom &copy_from_statement);

  /**
   * @brief Resolves a UPDATE query and returns a logical plan.
   *
   * @param update_statement The UPDATE parse tree.
   * @return A logical plan for the UPDATE query.
   */
  logical::LogicalPtr resolveUpdate(
      const ParseStatementUpdate &update_statement);

  /**
   * @brief Resolves SELECT-list expressions and returns the resolved
   *        expressions in \p project_expressions.
   * @warning \p project_expressions will be cleared first.
   *
   * @param parse_selection The SELECT parse tree.
   * @param select_name The name of the SELECT query. It is used to populate the
   *                    relation name in the expressions.
   * @param name_resolver NameResolver to resolve the relation/attribute names.
   * @param query_aggregation_info Passed down to each expression to collects
   *                               aggregate expressions.
   * @param project_expressions Converted SELECT-list expressions.
   * @param has_aggregate_per_expression For each SELECT-list expression,
   *                                     indicates whether it contains
   *                                     an aggregate.
   */
  void resolveSelectClause(
      const ParseSelectionClause &parse_selection,
      const std::string &select_name,
      const NameResolver &name_resolver,
      QueryAggregationInfo *query_aggregation_info,
      std::vector<expressions::NamedExpressionPtr> *project_expressions,
      std::vector<bool> *has_aggregate_per_expression);

  /**
   * @brief Resolves FROM clause and converts it to a logical plan. If the FROM
   *        clause has multiple items, the returned plan is a MultiwayCartisianJoin
   *        on all logical subplans for FROM-list items.
   *
   * @param from_list The FROM-list items.
   * @param name_resolver The resolver to resolve names.
   * @return A logical plan for the FROM clause.
   */
  logical::LogicalPtr resolveFromClause(
      const PtrList<ParseTableReference> &from_list,
      NameResolver *name_resolver);

  /**
   * @brief Resolves a table reference item, which can be a base table,
   *        a subquery table, or a joined table defined by a join chain.
   *
   * @param table_reference The parse table reference to be resolved.
   * @param name_resolver The name resolver to be updated with new tables
   *                      visible from this table reference item.
   * @return The logical plan to derived the table.
   */
  logical::LogicalPtr resolveTableReference(const ParseTableReference &table_reference,
                                            NameResolver *name_resolver);

  /**
   * @brief Resolves a table reference by name, and converts it to a logical
   *        plan. The table is not added into the NameResolver.
   *
   * @param table_name The parse node for the table name string.
   * @param reference_alias The alias of the table reference.
   * @return A logical plan for the table reference.
   */
  logical::LogicalPtr resolveSimpleTableReference(
      const ParseString &table_name, const ParseString *reference_alias);

  /**
   * @brief Renames the output columns from \p logical_plan based on the table signature
   *        by wrapping it with a Project.
   *
   * @param logical The logical node whose output columns are
   * @param table_signature A table reference signature that specifies column names.
   * @return A logical plan with a wrapper Project.
   */
  logical::LogicalPtr RenameOutputColumns(
      const logical::LogicalPtr &logical_plan,
      const ParseTableReferenceSignature &table_signature);

  /**
   * @brief Resolves a parse expression and converts it to a scalar expression
   *        in the query optimizer. A non-scalar parse expression is resolved
   *        to an AttributeReference to another optimizer expression.
   *
   * @param parse_expression The parse expression to be resolved.
   * @param type_hint A hint about what Type the expression is expected to be
   *        (e.g. an attribute's type in an INSERT or UPDATE query). May be
   *        NULL if there is no expectation, or the expectation is ambiguous.
   *        This is passed through to ParseLiteralValue::concretize().
   * @param expression_resolution_info Resolution info that contains the name
   *                                   resolver and info to be updated after
   *                                   resolution.
   * @return An expression in the query optimizer.
   */
  expressions::ScalarPtr resolveExpression(
      const ParseExpression &parse_expression,
      const Type *type_hint,
      ExpressionResolutionInfo *expression_resolution_info);

  /**
   * @brief Resolves a function call. For a non-scalar function, the returned
   *        expression is an AttributeReference to the actual resolved expression.
   *
   * @note This currently only handles resolving aggregate functions.
   *
   * @param parse_function_call The function call to be resolved.
   * @param expression_resolution_info Resolution info that contains the name
   *                                   resolver and info to be updated after
   *                                   resolution.
   * @return An expression in the query optimizer.
   */
  expressions::ScalarPtr resolveFunctionCall(
      const ParseFunctionCall &parse_function_call,
      ExpressionResolutionInfo *expression_resolution_info);

  /**
   * @brief Resolves a parse Predicate and converts it to a predicate in the
   *        query optimizer.
   *
   * @param parse_predicate The parse predicate to be resolved.
   * @param expression_resolution_info Resolution info that contains the name
   *                                   resolver and info to be updated after
   *                                   resolution.
   * @return A predicate in the query optimizer
   */
  expressions::PredicatePtr resolvePredicate(
      const ParsePredicate &parse_predicate,
      ExpressionResolutionInfo *expression_resolution_info);

  /**
   * @brief Convenient method to resolve a list of predicates that uses
   *        resolvePredicate as a subroutine.
   *
   * @param parse_predicates Parse predicates to be resolved.
   * @param expression_resolution_info Resolution info that contains the name
   *                                   resolver and info to be updated after
   *                                   resolution.
   * @return Resolved predicates in the query optimizer
   */
  std::vector<expressions::PredicatePtr> resolvePredicates(
      const PtrList<ParsePredicate> &parse_predicates,
      ExpressionResolutionInfo *expression_resolution_info);

  /**
   * @brief Resolves a relation name to a pointer to the corresponding
   *        CatalogRelation with the name.
   *
   * @param relation_name The parser node of the relation name.
   * @return The CatalogRelation.
   */
  const CatalogRelation *resolveRelationName(const ParseString *relation_name);

  /**
   * @brief Determines whether \p op can apply to \p left_operand and \p
   *        right_operand.
   *
   * @param location The location for the error message.
   * @param op The comparison operator.
   * @param left_operand The left operand.
   * @param right_operand The right operand.
   */
  void checkComparisonCanApplyTo(const ParseTreeNode *location,
                                 const Comparison &op,
                                 const expressions::ScalarPtr &left_operand,
                                 const expressions::ScalarPtr &right_operand) const;

  /**
   * @brief Generates an internal alias for an aggregate attribute.
   *
   * @param index The index of the aggregate attribute used for generating the
   *              name.
   * @return A string for the name.
   */
  static std::string GenerateAggregateAttributeAlias(int index);

  /**
   * @brief Generates an internal alias for a grouping attribute.
   *
   * @param index The index of the grouping attribute used for generating the
   *              name.
   * @return A string for the name.
   */
  static std::string GenerateGroupingAttributeAlias(int index);

  /**
   * @brief Generates an internal alias for an ordering attribute.
   *
   * @param index The index of an ordering attribute used for generating
   *              the name.
   * @return A string for the name.
   */
  static std::string GenerateOrderingAttributeAlias(int index);

  /**
   * @brief Validates each SELECT-list expression to ensure that it does not
   *        reference a named expression with an ID not in \p valid_expr_id_set.
   *
   * @param parse_select The parse tree for SELECT.
   * @param select_list_expressions The resolved SELECT-list expressions.
   * @param visible_expr_id_set A set of IDs of expressions visible to the
   *                            SELECT list.
   */
  void validateSelectExpressionsForAggregation(
      const ParseSelectionClause &parse_selection,
      const std::vector<expressions::NamedExpressionPtr> &
          select_list_expressions,
      const std::unordered_set<expressions::ExprId> &visible_expr_id_set) const;

  /**
   * @brief Validates each ordering expression to ensure that it does not
   *        reference a named expression with an ID not in \p valid_expr_id_set.
   *
   * @param parse_order_by The parse tree for ORDER BY.
   * @param order_by_expressions The resolved order by expressions.
   * @param visible_expr_id_set A set of IDs of expressions visible to ORDER BY.
   */
  void validateOrderingExpressionsForAggregation(
      const ParseOrderBy &parse_order_by,
      const std::vector<expressions::ScalarPtr> &order_by_expressions,
      const std::unordered_set<expressions::ExprId> &visible_expr_id_set) const;

  /**
   * @brief Validates \p expression does not reference a name expression with an
   *        ID not in \p valid_expr_id_set.
   *
   * @param location The location where to report an error at.
   * @param expression The expression to be checked.
   * @param visible_expr_id_set A set of IDs of expressions visible to the
   *                            expression.
   */
  void validateExpressionForAggregation(
      const ParseTreeNode *location,
      const expressions::ExpressionPtr &expression,
      const std::unordered_set<expressions::ExprId> &visible_expr_id_set) const;

  /**
   * @brief Rewrites \p expression if it is an integer literal that can be used
   *        as an ordinal reference to a SELECT-list expression.
   *
   * @param location The location where to report an error at.
   * @param expr_resolution_info Used to know aggregate functions are allowed.
   * @param select_list_info The SELECT-list expressions info.
   * @param expression The expression to be checked and rewritten.
   */
  void rewriteIfOrdinalReference(
      const ParseTreeNode *location,
      const ExpressionResolutionInfo &expr_resolution_info,
      SelectListInfo *select_list_info, expressions::ScalarPtr *expression);

  /**
   * @brief Wraps \p project_plan with a Project if \p select_list_info
   *        indicates that there are SELECT-list expressions
   *        that do not contain aggregates and need to be precomputed (those
   *        that are referenced by aliases), updates those precomputed
   *        expressions in \p select_list_expressions with the references.
   *
   * @param select_list_info Select list info to determine precomputed
   *                         expressions.
   * @param select_list_expressions Select list expressions.
   * @param logical_plan The logical plan to be updated.
   */
  void appendProjectIfNeedPrecomputationBeforeAggregation(
      const SelectListInfo &select_list_info,
      std::vector<expressions::NamedExpressionPtr> *select_list_expressions,
      logical::LogicalPtr *logical_plan);

  /**
   * @brief Wraps \p project_plan with a Project if \p select_list_info
   *        indicates that there are SELECT-list expressions
   *        that contain aggregates and need to be precomputed (those that are
   *        referenced by aliases), updates those precomputed expressions in
   *        \p select_list_expressions with the references.
   *
   * @param select_list_info Select list info to determine precomputed
   *                         expressions.
   * @param select_list_expressions Select list expressions.
   * @param logical_plan The logical plan to be updated.
   */
  void appendProjectIfNeedPrecomputationAfterAggregation(
      const SelectListInfo &select_list_info,
      std::vector<expressions::NamedExpressionPtr> *select_list_expressions,
      logical::LogicalPtr *logical_plan);

  OptimizerContext *context_;
  WithQueriesInfo with_queries_info_;

  logical::LogicalPtr logical_plan_;

  DISALLOW_COPY_AND_ASSIGN(Resolver);
};

/** @} */

}  // namespace resolver
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RESOLVER_RESOLVER_HPP_ */
