#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015 Pivotal Software, Inc.
#   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
#     University of Wisconsin—Madison.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

# Declare micro-libs:
add_library(quickstep_queryoptimizer_expressions_AggregateFunction AggregateFunction.cpp AggregateFunction.hpp)
add_library(quickstep_queryoptimizer_expressions_Alias Alias.cpp Alias.hpp)
add_library(quickstep_queryoptimizer_expressions_AttributeReference AttributeReference.cpp AttributeReference.hpp)
add_library(quickstep_queryoptimizer_expressions_BinaryExpression BinaryExpression.cpp BinaryExpression.hpp)
add_library(quickstep_queryoptimizer_expressions_Cast Cast.cpp Cast.hpp)
add_library(quickstep_queryoptimizer_expressions_ComparisonExpression ComparisonExpression.cpp
            ComparisonExpression.hpp)
add_library(quickstep_queryoptimizer_expressions_Exists Exists.cpp Exists.hpp)
add_library(quickstep_queryoptimizer_expressions_Expression ../../empty_src.cpp Expression.hpp)
add_library(quickstep_queryoptimizer_expressions_ExpressionType ../../empty_src.cpp ExpressionType.hpp)
add_library(quickstep_queryoptimizer_expressions_ExprId ../../empty_src.cpp ExprId.hpp)
add_library(quickstep_queryoptimizer_expressions_ExpressionUtil ExpressionUtil.cpp ExpressionUtil.hpp)
add_library(quickstep_queryoptimizer_expressions_InTableQuery InTableQuery.cpp InTableQuery.hpp)
add_library(quickstep_queryoptimizer_expressions_InValueList InValueList.cpp InValueList.hpp)
add_library(quickstep_queryoptimizer_expressions_LogicalAnd LogicalAnd.cpp LogicalAnd.hpp)
add_library(quickstep_queryoptimizer_expressions_LogicalNot LogicalNot.cpp LogicalNot.hpp)
add_library(quickstep_queryoptimizer_expressions_LogicalOr LogicalOr.cpp LogicalOr.hpp)
add_library(quickstep_queryoptimizer_expressions_NamedExpression NamedExpression.cpp NamedExpression.hpp)
add_library(quickstep_queryoptimizer_expressions_PatternMatcher ../../empty_src.cpp PatternMatcher.hpp)
add_library(quickstep_queryoptimizer_expressions_Predicate ../../empty_src.cpp Predicate.hpp)
add_library(quickstep_queryoptimizer_expressions_PredicateLiteral PredicateLiteral.cpp PredicateLiteral.hpp)
add_library(quickstep_queryoptimizer_expressions_Scalar ../../empty_src.cpp Scalar.hpp)
add_library(quickstep_queryoptimizer_expressions_ScalarLiteral ScalarLiteral.cpp ScalarLiteral.hpp)
add_library(quickstep_queryoptimizer_expressions_SearchedCase SearchedCase.cpp SearchedCase.hpp)
add_library(quickstep_queryoptimizer_expressions_SimpleCase SimpleCase.cpp SimpleCase.hpp)
add_library(quickstep_queryoptimizer_expressions_SubqueryExpression SubqueryExpression.cpp SubqueryExpression.hpp)
add_library(quickstep_queryoptimizer_expressions_UnaryExpression UnaryExpression.cpp UnaryExpression.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_expressions_AggregateFunction
                      glog
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_Type
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_Alias
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_AttributeReference
                      glog
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_BinaryExpression
                      glog
                      quickstep_expressions_scalar_ScalarBinaryExpression
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_Cast
                      glog
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarUnaryExpression
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_Type
                      quickstep_types_operations_unaryoperations_NumericCastOperation
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_ComparisonExpression
                      glog
                      quickstep_expressions_predicate_ComparisonPredicate
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_Exists
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_SubqueryExpression
                      quickstep_utility_Macros
                      glog)
target_link_libraries(quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_types_Type
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_ExpressionUtil
                      glog
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher)
target_link_libraries(quickstep_queryoptimizer_expressions_InTableQuery
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_expressions_SubqueryExpression
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_InValueList
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_expressions_predicate_Predicate
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_LogicalAnd
                      glog
                      quickstep_expressions_predicate_ConjunctionPredicate
                      quickstep_expressions_predicate_TrivialPredicates
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_PredicateLiteral
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_LogicalNot
                      glog
                      quickstep_expressions_predicate_NegationPredicate
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_LogicalOr
                      glog
                      quickstep_expressions_predicate_DisjunctionPredicate
                      quickstep_expressions_predicate_TrivialPredicates
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_PredicateLiteral
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_Type
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_ExpressionType)
target_link_libraries(quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_PredicateLiteral
                      glog
                      quickstep_expressions_predicate_TrivialPredicates
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_Scalar
                      glog
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_ScalarLiteral
                      glog
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_SearchedCase
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarCaseExpression
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_Type
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_SimpleCase
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarCaseExpression
                      quickstep_expressions_scalar_ScalarLiteral
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_Type
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_SubqueryExpression
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_expressions_UnaryExpression
                      glog
                      quickstep_expressions_scalar_ScalarUnaryExpression
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationID
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_expressions ../../empty_src.cpp OptimizerExpressionsModule.hpp)
target_link_libraries(quickstep_queryoptimizer_expressions
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_BinaryExpression
                      quickstep_queryoptimizer_expressions_Cast
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_Exists
                      quickstep_queryoptimizer_expressions_Expression
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_InTableQuery
                      quickstep_queryoptimizer_expressions_InValueList
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalNot
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_PredicateLiteral
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_expressions_SearchedCase
                      quickstep_queryoptimizer_expressions_SimpleCase
                      quickstep_queryoptimizer_expressions_SubqueryExpression
                      quickstep_queryoptimizer_expressions_UnaryExpression)
