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

#include "query_optimizer/tests/OptimizerTest.hpp"

#include <string>
#include <vector>

#include "catalog/Catalog.hpp"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/PhysicalGenerator.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/BinaryExpression.hpp"
#include "query_optimizer/expressions/ComparisonExpression.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/ScalarLiteral.hpp"
#include "query_optimizer/logical/Filter.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Cast.hpp"

namespace quickstep {
namespace optimizer {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

OptimizerTest::OptimizerTest()
    : catalog_(new Catalog),
      catalog_database_(
          new CatalogDatabase(catalog_.get(), "TestDatabase" /* name */, 0)),
      optimizer_context_(new OptimizerContext(0 /* query_id */,
                                              catalog_database_.get(),
                                              nullptr /* storage_manager */)),
      physical_generator_(new PhysicalGenerator()) {}

E::AliasPtr OptimizerTest::createAlias(const E::ExpressionPtr &expression,
                                       const std::string &attribute_name,
                                       const std::string &relation_name) {
  return E::Alias::Create(optimizer_context_->nextExprId(),
                          expression,
                          attribute_name,
                          relation_name);
}

L::TableReferencePtr OptimizerTest::createTableReference(
    const std::string &relation_name) {
  return L::TableReference::Create(catalog_relation_0_.get(),
                                   relation_name,
                                   optimizer_context_.get());
}

void OptimizerTest::SetUp() {
  setupTypes();
  setupTables();
  setupExpressions();
  setupLogical();
  setupPhysical();
  setupLogicalToPhysicalMap();
}

void OptimizerTest::setupTypes() {
  nullable_int_type_ = &TypeFactory::GetType(kInt, true);
  nonnullable_int_type_ = &TypeFactory::GetType(kInt, false);
}

void OptimizerTest::setupTables() {
  catalog_relation_0_.reset(
      new CatalogRelation(catalog_database_.get(),
                          "table_0" /* name */,
                          0 /* id */,
                          false /* temporary */));
  catalog_relation_1_.reset(
      new CatalogRelation(catalog_database_.get(),
                          "table_1" /* name */,
                          1 /* id */,
                          false /* temporary */));

  catalog_relation_0_->addAttribute(new CatalogAttribute(
      catalog_relation_0_.get(),
      "table_0_attribute_0" /* attribute_name */,
      *nullable_int_type_,
      0 /* attribute_id */,
      "table_0_attribute_0" /* display_name */));
  catalog_relation_0_->addAttribute(new CatalogAttribute(
      catalog_relation_0_.get(),
      "table_0_attribute_1" /* attribute_name */,
      *nonnullable_int_type_,
      1 /* attribute_id */,
      "table_0_attribute_1" /* display_name */));
  catalog_relation_0_->addAttribute(new CatalogAttribute(
      catalog_relation_0_.get(),
      "table_0_attribute_2" /* attribute_name */,
      *nullable_int_type_,
      2 /* attribute_id */,
      "table_0_attribute_2" /* display_name */));

  logical_table_reference_0_ = L::TableReference::Create(catalog_relation_0_.get(),
                                                         "table_0" /* relation_alias */,
                                                         optimizer_context_.get());

  catalog_relation_1_->addAttribute(new CatalogAttribute(
      catalog_relation_1_.get(),
      "table_1_attribute_0" /* attribute_name */,
      *nullable_int_type_,
      0 /* attribute_id */,
      "table_1_attribute_0" /* display_name */));
  catalog_relation_1_->addAttribute(new CatalogAttribute(
      catalog_relation_1_.get(),
      "table_1_attribute_1" /* attribute_name */,
      *nonnullable_int_type_,
      1 /* attribute_id */,
      "table_1_attribute_1" /* display_name */));

  logical_table_reference_1_ = L::TableReference::Create(catalog_relation_1_.get(),
                                                         "table_1" /* relation_alias */,
                                                         optimizer_context_.get());

  const std::vector<E::AttributeReferencePtr> relation_0_attribute_references =
      logical_table_reference_0_->attribute_list();
  relation_attribute_reference_0_0_ = relation_0_attribute_references[0];
  relation_attribute_reference_0_1_ = relation_0_attribute_references[1];
  relation_attribute_reference_0_2_ = relation_0_attribute_references[2];

  const std::vector<E::AttributeReferencePtr> relation_1_attribute_references =
      logical_table_reference_1_->attribute_list();
  relation_attribute_reference_1_0_ = relation_1_attribute_references[0];
  relation_attribute_reference_1_1_ = relation_1_attribute_references[1];
}

void OptimizerTest::setupExpressions() {
  literal_0_ = E::ScalarLiteral::Create(TypedValue(0),
                                        TypeFactory::GetType(kInt, false /* nullable */));
  literal_1_ = E::ScalarLiteral::Create(TypedValue(1),
                                        TypeFactory::GetType(kInt, false /* nullable */));
  alias_0_ = createAlias(literal_0_,
                         "literal_0" /* attribute_name */,
                         "dummy_table" /* relation_name */);
  alias_1_ = createAlias(literal_1_,
                         "literal_1" /* attribute_name */,
                         "dummy_table" /* relation_name */);
  attribute_reference_0_ = E::ToRef(alias_0_);
  attribute_reference_1_ = E::ToRef(alias_1_);

  filter_predicate_0_ = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kNotEqual),
      relation_attribute_reference_0_0_,
      relation_attribute_reference_0_2_);
  filter_predicate_1_ = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      relation_attribute_reference_1_0_,
      literal_0_);
  hash_join_predicate_0_1_ = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kEqual),
      relation_attribute_reference_0_0_,
      relation_attribute_reference_1_0_);
  non_hash_join_predicate_0_1_ = E::ComparisonExpression::Create(
      ComparisonFactory::GetComparison(ComparisonID::kGreater),
      relation_attribute_reference_0_0_,
      relation_attribute_reference_1_0_);
  add_literal_0_ = E::BinaryExpression::Create(
      BinaryOperationFactory::GetBinaryOperation(BinaryOperationID::kAdd),
      relation_attribute_reference_0_0_,
      literal_0_);
  alias_add_literal_0_ = E::Alias::Create(
      optimizer_context()->nextExprId(),
      add_literal_0_,
      "add_literal_0" /* attribute_name */,
      "add_literal_0" /* attribute_alias */,
      "" /* relation_name */);
}

void OptimizerTest::setupLogical() {
  logical_project_0_ = L::Project::Create(
      logical_table_reference_0_,
      {alias_0_,
       alias_add_literal_0_,
       relation_attribute_reference_0_1_} /* project_expressions */);
  logical_project_1_ = L::Project::Create(
      logical_table_reference_1_,
      {alias_1_,
       relation_attribute_reference_1_0_,
       relation_attribute_reference_1_1_} /* project_expressions */);
  logical_filter_0_ =
      L::Filter::Create(logical_table_reference_0_, filter_predicate_0_);
  logical_filter_1_ =
      L::Filter::Create(logical_table_reference_1_, filter_predicate_1_);

  logical_project_on_filter_0_ = L::Project::Create(
      logical_filter_0_, logical_project_0_->project_expressions());
  logical_project_on_filter_1_ = L::Project::Create(
      logical_filter_1_, logical_project_1_->project_expressions());
}

void OptimizerTest::setupPhysical() {
  physical_table_reference_0_ =
      P::TableReference::Create(logical_table_reference_0_->catalog_relation(),
                                logical_table_reference_0_->relation_alias(),
                                logical_table_reference_0_->attribute_list());
  physical_table_reference_1_ =
      P::TableReference::Create(logical_table_reference_1_->catalog_relation(),
                                logical_table_reference_1_->relation_alias(),
                                logical_table_reference_1_->attribute_list());
  physical_project_0_ = P::Selection::Create(
      physical_table_reference_0_,
      logical_project_0_->project_expressions(),
      E::PredicatePtr());
  physical_project_1_ = P::Selection::Create(
      physical_table_reference_1_,
      logical_project_1_->project_expressions(),
      E::PredicatePtr());
  physical_filter_0_ = P::Selection::Create(
      physical_table_reference_0_,
      CastSharedPtrVector<E::NamedExpression>(logical_filter_0_->getOutputAttributes()),
      logical_filter_0_->filter_predicate());
  physical_filter_1_ = P::Selection::Create(
      physical_table_reference_1_,
      CastSharedPtrVector<E::NamedExpression>(logical_filter_1_->getOutputAttributes()),
      logical_filter_1_->filter_predicate());

  physical_project_on_filter_0_ = P::Selection::Create(
      physical_table_reference_0_,
      physical_project_0_->project_expressions(),
      logical_filter_0_->filter_predicate());
  physical_project_on_filter_1_ = P::Selection::Create(
      physical_table_reference_1_,
      physical_project_1_->project_expressions(),
      logical_filter_1_->filter_predicate());
}

void OptimizerTest::setupLogicalToPhysicalMap() {
  physical_generator_->setBestPhysicalForLogical(logical_table_reference_0_,
                                                 physical_table_reference_0_);
  physical_generator_->setBestPhysicalForLogical(logical_table_reference_1_,
                                                 physical_table_reference_1_);
  physical_generator_->setBestPhysicalForLogical(logical_project_0_,
                                                 physical_project_0_);
  physical_generator_->setBestPhysicalForLogical(logical_project_1_,
                                                 physical_project_1_);
  physical_generator_->setBestPhysicalForLogical(logical_filter_0_,
                                                 physical_filter_0_);
  physical_generator_->setBestPhysicalForLogical(logical_filter_1_,
                                                 physical_filter_1_);
  physical_generator_->setBestPhysicalForLogical(
      logical_project_on_filter_0_, physical_project_on_filter_0_);
  physical_generator_->setBestPhysicalForLogical(
      logical_project_on_filter_1_, physical_project_on_filter_1_);
}

}  // namespace optimizer
}  // namespace quickstep
