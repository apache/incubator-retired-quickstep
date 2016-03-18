#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015-2016 Pivotal Software, Inc.
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
add_library(quickstep_queryoptimizer_logical_Aggregate Aggregate.cpp Aggregate.hpp)
add_library(quickstep_queryoptimizer_logical_BinaryJoin BinaryJoin.cpp BinaryJoin.hpp)
add_library(quickstep_queryoptimizer_logical_CopyFrom CopyFrom.cpp CopyFrom.hpp)
add_library(quickstep_queryoptimizer_logical_CreateIndex CreateIndex.cpp CreateIndex.hpp)
add_library(quickstep_queryoptimizer_logical_CreateTable CreateTable.cpp CreateTable.hpp)
add_library(quickstep_queryoptimizer_logical_DeleteTuples DeleteTuples.cpp DeleteTuples.hpp)
add_library(quickstep_queryoptimizer_logical_DropTable DropTable.cpp DropTable.hpp)
add_library(quickstep_queryoptimizer_logical_Filter Filter.cpp Filter.hpp)
add_library(quickstep_queryoptimizer_logical_HashJoin ../../empty_src.cpp HashJoin.hpp)
add_library(quickstep_queryoptimizer_logical_InsertTuple InsertTuple.cpp InsertTuple.hpp)
add_library(quickstep_queryoptimizer_logical_Join ../../empty_src.cpp Join.hpp)
add_library(quickstep_queryoptimizer_logical_Logical ../../empty_src.cpp Logical.hpp)
add_library(quickstep_queryoptimizer_logical_LogicalType ../../empty_src.cpp LogicalType.hpp)
add_library(quickstep_queryoptimizer_logical_MultiwayCartesianJoin MultiwayCartesianJoin.cpp MultiwayCartesianJoin.hpp)
add_library(quickstep_queryoptimizer_logical_NestedLoopsJoin ../../empty_src.cpp NestedLoopsJoin.hpp)
add_library(quickstep_queryoptimizer_logical_PatternMatcher ../../empty_src.cpp PatternMatcher.hpp)
add_library(quickstep_queryoptimizer_logical_Project Project.cpp Project.hpp)
add_library(quickstep_queryoptimizer_logical_Sample Sample.cpp Sample.hpp)
add_library(quickstep_queryoptimizer_logical_SharedSubplanReference
            SharedSubplanReference.cpp
            SharedSubplanReference.hpp)
add_library(quickstep_queryoptimizer_logical_Sort Sort.cpp Sort.hpp)
add_library(quickstep_queryoptimizer_logical_TableReference TableReference.cpp TableReference.hpp)
add_library(quickstep_queryoptimizer_logical_TableGenerator ../../empty_src.cpp TableGenerator.hpp)
add_library(quickstep_queryoptimizer_logical_TopLevelPlan TopLevelPlan.cpp TopLevelPlan.hpp)
add_library(quickstep_queryoptimizer_logical_UpdateTable UpdateTable.cpp UpdateTable.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_logical_Aggregate
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_BinaryJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Join
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_CopyFrom
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_queryoptimizer_logical_CreateIndex
                      glog
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_CreateTable
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_DeleteTuples
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_DropTable
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_Filter
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_HashJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_BinaryJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_InsertTuple
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_Join
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_MultiwayCartesianJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Join
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_NestedLoopsJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_PredicateLiteral
                      quickstep_queryoptimizer_logical_BinaryJoin
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_logical_LogicalType)
target_link_libraries(quickstep_queryoptimizer_logical_Project
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_Sample
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)                      
target_link_libraries(quickstep_queryoptimizer_logical_SharedSubplanReference
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_Sort
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_TableGenerator
                      quickstep_expressions_tablegenerator_GeneratorFunctionHandle
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_TableReference
                      glog
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_TopLevelPlan
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_logical_UpdateTable
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_logical ../../empty_src.cpp OptimizerLogicalModule.hpp)
target_link_libraries(quickstep_queryoptimizer_logical
                      quickstep_queryoptimizer_logical_Aggregate
                      quickstep_queryoptimizer_logical_BinaryJoin
                      quickstep_queryoptimizer_logical_CopyFrom
                      quickstep_queryoptimizer_logical_CreateIndex
                      quickstep_queryoptimizer_logical_CreateTable
                      quickstep_queryoptimizer_logical_DeleteTuples
                      quickstep_queryoptimizer_logical_DropTable
                      quickstep_queryoptimizer_logical_Filter
                      quickstep_queryoptimizer_logical_HashJoin
                      quickstep_queryoptimizer_logical_InsertTuple
                      quickstep_queryoptimizer_logical_Join
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_logical_LogicalType
                      quickstep_queryoptimizer_logical_MultiwayCartesianJoin
                      quickstep_queryoptimizer_logical_NestedLoopsJoin
                      quickstep_queryoptimizer_logical_PatternMatcher
                      quickstep_queryoptimizer_logical_Project
                      quickstep_queryoptimizer_logical_Sample
                      quickstep_queryoptimizer_logical_SharedSubplanReference
                      quickstep_queryoptimizer_logical_Sort
                      quickstep_queryoptimizer_logical_TableGenerator
                      quickstep_queryoptimizer_logical_TableReference
                      quickstep_queryoptimizer_logical_TopLevelPlan
                      quickstep_queryoptimizer_logical_UpdateTable)
