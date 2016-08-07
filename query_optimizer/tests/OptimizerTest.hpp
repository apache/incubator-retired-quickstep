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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_OPTIMIZER_TEST_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_OPTIMIZER_TEST_HPP_

#include <memory>
#include <string>

#include "catalog/Catalog.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/rules/Rule.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest.h"

namespace quickstep {

class Type;

namespace optimizer {

/** \addtogroup QueryRules
 *  @{
 */

/**
 * @brief Base class for optimizer tests (rules, and logical, physical and
 * execution generator).
 */
class OptimizerTest : public ::testing::Test {
 protected:
  /**
   * @brief Constructor.
   */
  OptimizerTest();

  /**
   * @brief Virtual destructor.
   */
  ~OptimizerTest() override {}

  /**
   * @brief Initialize common fields used in tests.
   */
  void SetUp() override;

  /**
   * @return The optimizer context.
   */
  OptimizerContext* optimizer_context() { return optimizer_context_.get(); }

  /**
   * @return The physical generator.
   */
  PhysicalGenerator* physical_generator() { return physical_generator_.get(); }

  /**
   * @brief Helper method to create an Alias.
   *
   * @param expression The expression to be named.
   * @param attribute_name The name to be given to the expression.
   * @param relation_name The relation name.
   * @return An Alias.
   */
  expressions::AliasPtr createAlias(
      const expressions::ExpressionPtr &expression,
      const std::string &attribute_name,
      const std::string &relation_name);

  /**
   * @brief Helper method to create a TableReference on <catalog_relation_0_>
   *
   * @param relation_name The name of the table.
   */
  logical::TableReferencePtr createTableReference(const std::string &relation_name);

  const Type *nullable_int_type_ = nullptr;
  const Type *nonnullable_int_type_ = nullptr;

  expressions::ScalarLiteralPtr literal_0_;
  expressions::ScalarLiteralPtr literal_1_;
  // Aliases of the previous two literals.
  expressions::AliasPtr alias_0_;
  expressions::AliasPtr alias_1_;
  // References to the previous two aliases.
  expressions::AttributeReferencePtr attribute_reference_0_;
  expressions::AttributeReferencePtr attribute_reference_1_;

  // AttributeReferences in catalog_relation_0_.
  expressions::AttributeReferencePtr relation_attribute_reference_0_0_;
  expressions::AttributeReferencePtr relation_attribute_reference_0_1_;
  expressions::AttributeReferencePtr relation_attribute_reference_0_2_;

  // AttributeReferences in catalog_relation_1_.
  expressions::AttributeReferencePtr relation_attribute_reference_1_0_;
  expressions::AttributeReferencePtr relation_attribute_reference_1_1_;

  std::unique_ptr<Catalog> catalog_;
  std::unique_ptr<CatalogDatabase> catalog_database_;
  std::unique_ptr<CatalogRelation> catalog_relation_0_;
  std::unique_ptr<CatalogRelation> catalog_relation_1_;

  logical::TableReferencePtr logical_table_reference_0_;
  logical::TableReferencePtr logical_table_reference_1_;

  // relation_attribute_reference_0_0_ != relation_attribute_reference_0_2_
  expressions::PredicatePtr filter_predicate_0_;
  // relation_attribute_reference_1_0_ > literal_0_
  expressions::PredicatePtr filter_predicate_1_;
  // relation_attribute_reference_0_0_ = relation_attribute_reference_1_0_
  expressions::PredicatePtr hash_join_predicate_0_1_;
  // relation_attribute_reference_0_0_ > relation_attribute_reference_1_0_
  expressions::PredicatePtr non_hash_join_predicate_0_1_;

  // relation_attribute_reference_0_0_ + literal_0_
  expressions::ScalarPtr add_literal_0_;
  expressions::AliasPtr alias_add_literal_0_;

  // Project expressions: alias_0_, alias_add_literal_0_,
  // relation_attribute_reference_0_1_
  logical::ProjectPtr logical_project_0_;
  // Project expressions: alias_1_, relation_attribute_reference_1_0_,
  // relation_attribute_reference_1_1_
  logical::ProjectPtr logical_project_1_;
  // Filter predicate: filter_predicate_0_
  logical::FilterPtr logical_filter_0_;
  // Filter predicate: filter_predicate_1_
  logical::FilterPtr logical_filter_1_;

  // logical_project_0_ on top of logical_filter_0_
  logical::ProjectPtr logical_project_on_filter_0_;
  // logical_project_1_ on top of filter_predicate_1_
  logical::ProjectPtr logical_project_on_filter_1_;

  physical::TableReferencePtr physical_table_reference_0_;
  physical::TableReferencePtr physical_table_reference_1_;
  physical::SelectionPtr physical_project_0_;
  physical::SelectionPtr physical_project_1_;
  physical::SelectionPtr physical_filter_0_;
  physical::SelectionPtr physical_filter_1_;
  physical::SelectionPtr physical_project_on_filter_0_;
  physical::SelectionPtr physical_project_on_filter_1_;

 private:
  void setupTypes();
  void setupExpressions();
  void setupTables();
  void setupLogical();
  void setupPhysical();
  void setupLogicalToPhysicalMap();

  std::unique_ptr<OptimizerContext> optimizer_context_;
  std::unique_ptr<PhysicalGenerator> physical_generator_;

  DISALLOW_COPY_AND_ASSIGN(OptimizerTest);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_TESTS_OPTIMIZER_TEST_HPP_ */
