# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

# Declare micro-libs:
add_library(quickstep_queryoptimizer_physical_Aggregate Aggregate.cpp Aggregate.hpp)
add_library(quickstep_queryoptimizer_physical_BinaryJoin BinaryJoin.cpp BinaryJoin.hpp)
add_library(quickstep_queryoptimizer_physical_CopyFrom CopyFrom.cpp CopyFrom.hpp)
add_library(quickstep_queryoptimizer_physical_CreateIndex CreateIndex.cpp CreateIndex.hpp)
add_library(quickstep_queryoptimizer_physical_CreateTable CreateTable.cpp CreateTable.hpp)
add_library(quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
            CrossReferenceCoalesceAggregate.cpp
            CrossReferenceCoalesceAggregate.hpp)
add_library(quickstep_queryoptimizer_physical_DeleteTuples DeleteTuples.cpp DeleteTuples.hpp)
add_library(quickstep_queryoptimizer_physical_DropTable DropTable.cpp DropTable.hpp)
add_library(quickstep_queryoptimizer_physical_FilterJoin FilterJoin.cpp FilterJoin.hpp)
add_library(quickstep_queryoptimizer_physical_HashJoin HashJoin.cpp HashJoin.hpp)
add_library(quickstep_queryoptimizer_physical_InsertSelection InsertSelection.cpp InsertSelection.hpp)
add_library(quickstep_queryoptimizer_physical_InsertTuple InsertTuple.cpp InsertTuple.hpp)
add_library(quickstep_queryoptimizer_physical_Join ../../empty_src.cpp Join.hpp)
add_library(quickstep_queryoptimizer_physical_LIPFilterConfiguration ../../empty_src.cpp LIPFilterConfiguration.hpp)
add_library(quickstep_queryoptimizer_physical_NestedLoopsJoin NestedLoopsJoin.cpp NestedLoopsJoin.hpp)
add_library(quickstep_queryoptimizer_physical_PartitionSchemeHeader PartitionSchemeHeader.cpp PartitionSchemeHeader.hpp)
add_library(quickstep_queryoptimizer_physical_PatternMatcher ../../empty_src.cpp PatternMatcher.hpp)
add_library(quickstep_queryoptimizer_physical_Physical ../../empty_src.cpp Physical.hpp)
add_library(quickstep_queryoptimizer_physical_PhysicalType ../../empty_src.cpp PhysicalType.hpp)
add_library(quickstep_queryoptimizer_physical_Sample Sample.cpp Sample.hpp)
add_library(quickstep_queryoptimizer_physical_Selection Selection.cpp Selection.hpp)
add_library(quickstep_queryoptimizer_physical_SharedSubplanReference
            SharedSubplanReference.cpp
            SharedSubplanReference.hpp)
add_library(quickstep_queryoptimizer_physical_Sort Sort.cpp Sort.hpp)
add_library(quickstep_queryoptimizer_physical_TableGenerator ../../empty_src.cpp TableGenerator.hpp)
add_library(quickstep_queryoptimizer_physical_TableReference TableReference.cpp TableReference.hpp)
add_library(quickstep_queryoptimizer_physical_TopLevelPlan TopLevelPlan.cpp TopLevelPlan.hpp)
add_library(quickstep_queryoptimizer_physical_UnionAll ../../empty_src.cpp UnionAll.hpp)
add_library(quickstep_queryoptimizer_physical_UpdateTable UpdateTable.cpp UpdateTable.hpp)
add_library(quickstep_queryoptimizer_physical_WindowAggregate WindowAggregate.cpp WindowAggregate.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_BinaryJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Join
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_CopyFrom
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_CreateIndex
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_CreateTable
                      glog
                      quickstep_catalog_Catalog_proto
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_DeleteTuples
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_DropTable
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_FilterJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_BinaryJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_HashJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_BinaryJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_InsertSelection
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_InsertTuple
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_Join
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter)
target_link_libraries(quickstep_queryoptimizer_physical_NestedLoopsJoin
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_BinaryJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_PartitionSchemeHeader
                      glog
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_PhysicalType)
target_link_libraries(quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_Sample
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_Selection
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_SharedSubplanReference
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_Sort
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_expressions_tablegenerator_GeneratorFunctionHandle
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_TableReference
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_TopLevelPlan
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_UnionAll
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_UpdateTable
                      glog
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_physical_WindowAggregate
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Cast
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_physical ../../empty_src.cpp OptimizerPhysicalModule.hpp)
target_link_libraries(quickstep_queryoptimizer_physical
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_BinaryJoin
                      quickstep_queryoptimizer_physical_CopyFrom
                      quickstep_queryoptimizer_physical_CreateIndex
                      quickstep_queryoptimizer_physical_CreateTable
                      quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
                      quickstep_queryoptimizer_physical_DeleteTuples
                      quickstep_queryoptimizer_physical_DropTable
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_InsertSelection
                      quickstep_queryoptimizer_physical_InsertTuple
                      quickstep_queryoptimizer_physical_Join
                      quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_PartitionSchemeHeader
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Sample
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_Sort
                      quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_physical_UnionAll
                      quickstep_queryoptimizer_physical_UpdateTable
                      quickstep_queryoptimizer_physical_WindowAggregate)
