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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_UNNEST_SUBQUERIES_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_UNNEST_SUBQUERIES_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "query_optimizer/rules/BottomUpRule.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {

class OptimizerContext;

namespace expressions {

class Exists;
class InTableQuery;

}

struct CorrelatedQueryInfo;

/**
 * @brief Rule that removes subquery expressions including IN with a subquery,
 *        Exists and scalar subquery expressions.
 */
class UnnestSubqueries : public Rule<logical::Logical> {
 public:
  explicit UnnestSubqueries(OptimizerContext *context)
      : context_(context) {}

  std::string getName() const override { return "UnnestSubqueries"; }

  logical::LogicalPtr apply(const logical::LogicalPtr &input) override;

 private:
  OptimizerContext *context_;

  DISALLOW_COPY_AND_ASSIGN(UnnestSubqueries);
};

class UnnestSubqueriesForNonRootLogical : public Rule<logical::Logical> {
 public:
  UnnestSubqueriesForNonRootLogical(
      const bool scalar_query,
      const expressions::UnorderedNamedExpressionSet &visiable_attrs_from_outer_query,
      OptimizerContext *context,
      std::unordered_map<expressions::ExprId, logical::LogicalPtr> *uncorrelated_subqueries,
      std::vector<expressions::AttributeReferencePtr> *probe_join_attributes,
      std::vector<expressions::AttributeReferencePtr> *build_join_attributes,
      std::vector<expressions::PredicatePtr> *correlated_non_hash_join_predicates)
      : scalar_query_(scalar_query),
        context_(context),
        visiable_attrs_from_outer_query_(visiable_attrs_from_outer_query),
        uncorrelated_subqueries_(uncorrelated_subqueries),
        probe_join_attributes_(probe_join_attributes),
        build_join_attributes_(build_join_attributes),
        correlated_non_hash_join_predicates_(correlated_non_hash_join_predicates) {}

  std::string getName() const override { return "UnnestScalarSubuqeryForLogical"; }

  logical::LogicalPtr apply(const logical::LogicalPtr &input) override {
    expressions::UnorderedAttributeSet inner_attributes;
    std::vector<expressions::AttributeReferencePtr> probe_join_attributes;
    std::vector<expressions::AttributeReferencePtr> build_join_attributes;
    std::vector<expressions::PredicatePtr> non_hash_join_predicates;

    const logical::LogicalPtr output =
        applyInternal(input,
                      &inner_attributes,
                      &probe_join_attributes,
                      &build_join_attributes,
                      &non_hash_join_predicates);

    probe_join_attributes_->insert(probe_join_attributes_->end(),
                                   probe_join_attributes.begin(),
                                   probe_join_attributes.end());
    build_join_attributes_->insert(build_join_attributes_->end(),
                                   build_join_attributes.begin(),
                                   build_join_attributes.end());
    correlated_non_hash_join_predicates_->insert(correlated_non_hash_join_predicates_->end(),
                                                 non_hash_join_predicates.begin(),
                                                 non_hash_join_predicates.end());

    return output;
  }

 private:
  logical::LogicalPtr applyInternal(const logical::LogicalPtr &input,
                                    expressions::UnorderedAttributeSet *inner_attributes,
                                    std::vector<expressions::AttributeReferencePtr> *probe_join_attributes,
                                    std::vector<expressions::AttributeReferencePtr> *build_join_attributes,
                                    std::vector<expressions::PredicatePtr> *non_hash_join_predicates);

  logical::LogicalPtr applyToNode(
      const logical::LogicalPtr &input,
      expressions::UnorderedAttributeSet *inner_attributes,
      std::vector<expressions::AttributeReferencePtr> *probe_join_attributes,
      std::vector<expressions::AttributeReferencePtr> *build_join_attributes,
      std::vector<expressions::PredicatePtr> *non_hash_join_predicates);

  // For each input expression, call UnnestSubqueriesForExpession to
  // eliminate any subquery expressions in it and add a join if necessary.
  logical::LogicalPtr eliminateNestedScalarQueries(const logical::LogicalPtr &node);

  // Eliminate outer attribute references in this node, add predicates with
  // outer attribute references into join attributes or non_hash_join_predicates.
  expressions::ExpressionPtr eliminateOuterAttributeReference(
      const expressions::ExpressionPtr &expression,
      expressions::UnorderedAttributeSet *inner_attributes,
      std::vector<expressions::AttributeReferencePtr> *probe_join_attributes,
      std::vector<expressions::AttributeReferencePtr> *build_join_attributes,
      std::vector<expressions::PredicatePtr> *non_hash_join_predicates);

  void validateNonOuterAttributeReference(const expressions::ExpressionPtr &expression);

  bool isCorrelatedOuterAttribute(const expressions::AttributeReferencePtr &attribute) const;

  const bool scalar_query_;
  OptimizerContext *context_;
  const expressions::UnorderedNamedExpressionSet &visiable_attrs_from_outer_query_;
  std::unordered_map<expressions::ExprId, logical::LogicalPtr> *uncorrelated_subqueries_;
  std::vector<expressions::AttributeReferencePtr> *probe_join_attributes_;
  std::vector<expressions::AttributeReferencePtr> *build_join_attributes_;
  std::vector<expressions::PredicatePtr> *correlated_non_hash_join_predicates_;

  DISALLOW_COPY_AND_ASSIGN(UnnestSubqueriesForNonRootLogical);
};

class UnnestSubqueriesForExpession : public Rule<expressions::Expression> {
 public:
  UnnestSubqueriesForExpession(
      const expressions::UnorderedNamedExpressionSet &visible_attributes_from_outer_query,
      OptimizerContext *context,
      std::unordered_map<expressions::ExprId, logical::LogicalPtr> *uncorrelated_subqueries,
      std::vector<CorrelatedQueryInfo> *correlated_query_info_vec)
      : visible_attributes_from_outer_query_(visible_attributes_from_outer_query),
        context_(context),
        uncorrelated_subqueries_(uncorrelated_subqueries),
        correlated_query_info_vec_(correlated_query_info_vec) {}

  std::string getName() const override { return "UnnestScalarSubqueryForExpession"; }

  expressions::ExpressionPtr apply(const expressions::ExpressionPtr &node) override {
    return applyInternal(true /* allow_exists_or_in */,
                         node);
  }

 private:
  const expressions::UnorderedNamedExpressionSet &visible_attributes_from_outer_query_;

  expressions::ExpressionPtr applyInternal(
      const bool allow_exists_or_in,
      const expressions::ExpressionPtr &node);

  /**
   * @brief Transform an EXIST predicate into a HashLeftSemiJoin and store the
   *        transformed results into correlated_query_info_vec_
   *
   * @param exists_predicate The EXISTS predicate to be transformed.
   */
  void transformExists(const expressions::Exists &exists_predicate);

  /**
   * @brief Transform an IN predicate into a HashLeftSemiJoin and store the
   *        transformed results into correlated_query_info_vec_
   *
   * @param in_table_query The IN predicate to be transformed.
   */
  void transformInTableQuery(const expressions::InTableQuery &in_table_query);

  OptimizerContext *context_;
  std::unordered_map<expressions::ExprId, logical::LogicalPtr> *uncorrelated_subqueries_;
  std::vector<CorrelatedQueryInfo> *correlated_query_info_vec_;

  DISALLOW_COPY_AND_ASSIGN(UnnestSubqueriesForExpession);
};

class DeOuterAttributeReference : public BottomUpRule<expressions::Expression> {
 public:
  DeOuterAttributeReference(
      const bool allow_outer_reference,
      const std::unordered_map<expressions::ExprId, logical::LogicalPtr> &uncorrelated_subqueries,
      const expressions::UnorderedNamedExpressionSet &visiable_attrs_from_outer_query)
      : allow_outer_reference_(allow_outer_reference),
        uncorrelated_subqueries_(uncorrelated_subqueries),
        visiable_attrs_from_outer_query_(visiable_attrs_from_outer_query) {}

  std::string getName() const override {
    return "DeOuterAttributeReference";
  }

 protected:
  expressions::ExpressionPtr applyToNode(const expressions::ExpressionPtr &input) override;

 private:
  const bool allow_outer_reference_;
  const std::unordered_map<expressions::ExprId, logical::LogicalPtr> &uncorrelated_subqueries_;
  const expressions::UnorderedNamedExpressionSet &visiable_attrs_from_outer_query_;

  DISALLOW_COPY_AND_ASSIGN(DeOuterAttributeReference);
};

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_UNNEST_SUBQUERIES_HPP_ */
