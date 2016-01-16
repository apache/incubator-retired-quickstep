# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# Copyright (c) 2015, Pivotal Software, Inc.
# All rights reserved.
# See file CREDITS.txt for details.

add_subdirectory(aggregation)
add_subdirectory(predicate)
add_subdirectory(scalar)

QS_PROTOBUF_GENERATE_CPP(expressions_Expressions_proto_srcs
                         expressions_Expressions_proto_hdrs
                         Expressions.proto)

add_library(quickstep_expressions_ExpressionFactories
            ExpressionFactories.cpp
            ExpressionFactories.hpp)
add_library(quickstep_expressions_Expressions_proto
            ${expressions_Expressions_proto_srcs})

target_link_libraries(quickstep_expressions_ExpressionFactories
                      glog
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_expressions_predicate_ConjunctionPredicate
                      quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_expressions_predicate_NegationPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_predicate_TrivialPredicates
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_scalar_ScalarBinaryExpression
                      quickstep_expressions_scalar_ScalarCaseExpression
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_expressions_scalar_ScalarUnaryExpression
                      quickstep_types_TypeFactory
                      quickstep_types_TypedValue
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_unaryoperations_UnaryOperationFactory
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_Expressions_proto
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue_proto
                      quickstep_types_operations_Operation_proto
                      ${PROTOBUF_LIBRARY})

# Module all-in-one library:
add_library(quickstep_expressions ../empty_src.cpp ExpressionsModule.hpp)
target_link_libraries(quickstep_expressions
                      quickstep_expressions_ExpressionFactories
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_aggregate
                      quickstep_expressions_predicate
                      quickstep_expressions_scalar)
