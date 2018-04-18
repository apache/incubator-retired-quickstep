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

if (ENABLE_DISTRIBUTED)
  set(QUICKSTEP_DISTRIBUTED TRUE)
endif()

configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/QueryOptimizerConfig.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/QueryOptimizerConfig.h"
)

set_gflags_lib_name ()

add_subdirectory(cost_model)
add_subdirectory(expressions)
add_subdirectory(logical)
add_subdirectory(physical)
add_subdirectory(resolver)
add_subdirectory(rules)
add_subdirectory(strategy)
add_subdirectory(tests)

# Declare micro-libs:
add_library(quickstep_queryoptimizer_ExecutionGenerator ExecutionGenerator.cpp ExecutionGenerator.hpp)
add_library(quickstep_queryoptimizer_LIPFilterGenerator LIPFilterGenerator.cpp LIPFilterGenerator.hpp)
add_library(quickstep_queryoptimizer_LogicalGenerator LogicalGenerator.cpp LogicalGenerator.hpp)
add_library(quickstep_queryoptimizer_LogicalToPhysicalMapper
            ../empty_src.cpp
            LogicalToPhysicalMapper.hpp)
add_library(quickstep_queryoptimizer_Optimizer Optimizer.cpp Optimizer.hpp)
add_library(quickstep_queryoptimizer_OptimizerContext OptimizerContext.cpp OptimizerContext.hpp)
add_library(quickstep_queryoptimizer_OptimizerTree ../empty_src.cpp OptimizerTree.hpp)
add_library(quickstep_queryoptimizer_PhysicalGenerator PhysicalGenerator.cpp PhysicalGenerator.hpp)
add_library(quickstep_queryoptimizer_QueryHandle ../empty_src.cpp QueryHandle.hpp)
add_library(quickstep_queryoptimizer_QueryPlan ../empty_src.cpp QueryPlan.hpp)
add_library(quickstep_queryoptimizer_QueryProcessor QueryProcessor.cpp QueryProcessor.hpp)
add_library(quickstep_queryoptimizer_Validator ../empty_src.cpp Validator.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_ExecutionGenerator
                      ${GFLAGS_LIB_NAME}
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_PartitionScheme
                      quickstep_catalog_PartitionSchemeHeader
                      quickstep_cli_Flags
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_expressions_aggregation_AggregateFunction_proto
                      quickstep_expressions_aggregation_AggregationID
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_expressions_windowaggregation_WindowAggregateFunction
                      quickstep_expressions_windowaggregation_WindowAggregateFunction_proto
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryoptimizer_LIPFilterGenerator
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_costmodel_SimpleCostModel
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      quickstep_queryoptimizer_expressions_AggregateFunction
                      quickstep_queryoptimizer_expressions_Alias
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_NamedExpression
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_expressions_Scalar
                      quickstep_queryoptimizer_expressions_ScalarLiteral
                      quickstep_queryoptimizer_expressions_WindowAggregateFunction
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_CopyFrom
                      quickstep_queryoptimizer_physical_CopyTo
                      quickstep_queryoptimizer_physical_CreateIndex
                      quickstep_queryoptimizer_physical_CreateTable
                      quickstep_queryoptimizer_physical_CrossReferenceCoalesceAggregate
                      quickstep_queryoptimizer_physical_DeleteTuples
                      quickstep_queryoptimizer_physical_DropTable
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_InsertSelection
                      quickstep_queryoptimizer_physical_InsertTuple
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
                      quickstep_queryoptimizer_physical_WindowAggregate
                      quickstep_relationaloperators_AggregationOperator
                      quickstep_relationaloperators_BuildAggregationExistenceMapOperator
                      quickstep_relationaloperators_BuildHashOperator
                      quickstep_relationaloperators_BuildLIPFilterOperator
                      quickstep_relationaloperators_CreateIndexOperator
                      quickstep_relationaloperators_CreateTableOperator
                      quickstep_relationaloperators_DeleteOperator
                      quickstep_relationaloperators_DestroyAggregationStateOperator
                      quickstep_relationaloperators_DestroyHashOperator
                      quickstep_relationaloperators_DropTableOperator
                      quickstep_relationaloperators_FinalizeAggregationOperator
                      quickstep_relationaloperators_HashJoinOperator
                      quickstep_relationaloperators_InitializeAggregationOperator
                      quickstep_relationaloperators_InsertOperator
                      quickstep_relationaloperators_NestedLoopsJoinOperator
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_relationaloperators_SampleOperator
                      quickstep_relationaloperators_SaveBlocksOperator
                      quickstep_relationaloperators_SelectOperator
                      quickstep_relationaloperators_SortMergeRunOperator
                      quickstep_relationaloperators_SortRunGenerationOperator
                      quickstep_relationaloperators_TableExportOperator
                      quickstep_relationaloperators_TableGeneratorOperator
                      quickstep_relationaloperators_TextScanOperator
                      quickstep_relationaloperators_UnionAllOperator
                      quickstep_relationaloperators_UpdateOperator
                      quickstep_relationaloperators_WindowAggregationOperator
                      quickstep_storage_AggregationOperationState_proto
                      quickstep_storage_HashTableFactory
                      quickstep_storage_HashTable_proto
                      quickstep_storage_InsertDestination_proto
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_storage_StorageConstants
                      quickstep_storage_SubBlockTypeRegistry
                      quickstep_types_Type
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_types_containers_Tuple_proto
                      quickstep_utility_BarrieredReadWriteConcurrentBitVector
                      quickstep_utility_Macros
                      quickstep_utility_SqlError)
target_link_libraries(quickstep_queryoptimizer_LIPFilterGenerator
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_FilterJoin
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_LIPFilterConfiguration
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_types_Type
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter
                      quickstep_utility_lipfilter_LIPFilter_proto)
target_link_libraries(quickstep_queryoptimizer_LogicalGenerator
                      glog
                      quickstep_parser_ParseStatement
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_Validator
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_resolver_Resolver
                      quickstep_queryoptimizer_rules_CollapseProject
                      quickstep_queryoptimizer_rules_GenerateJoins
                      quickstep_queryoptimizer_rules_PushDownFilter
                      quickstep_queryoptimizer_rules_PushDownSemiAntiJoin
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_rules_UnnestSubqueries
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_Optimizer
                      quickstep_queryoptimizer_ExecutionGenerator
                      quickstep_queryoptimizer_LogicalGenerator
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_resolver_Resolver
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_OptimizerTree
                      glog
                      quickstep_catalog_Catalog_proto
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_utility_Macros
                      quickstep_utility_TreeStringSerializable)
target_link_libraries(quickstep_queryoptimizer_PhysicalGenerator
                      ${GFLAGS_LIB_NAME}
                      glog
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_Validator
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_rules_AttachLIPFilters
                      quickstep_queryoptimizer_rules_CollapseSelection
                      quickstep_queryoptimizer_rules_EliminateEmptyNode
                      quickstep_queryoptimizer_rules_ExtractCommonSubexpression
                      quickstep_queryoptimizer_rules_FuseAggregateJoin
                      quickstep_queryoptimizer_rules_FuseHashSelect
                      quickstep_queryoptimizer_rules_InjectJoinFilters
                      quickstep_queryoptimizer_rules_Partition
                      quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_queryoptimizer_rules_PushDownLowCostDisjunctivePredicate
                      quickstep_queryoptimizer_rules_ReduceGroupByAttributes
                      quickstep_queryoptimizer_rules_ReorderColumns
                      quickstep_queryoptimizer_rules_ReuseAggregateExpressions
                      quickstep_queryoptimizer_rules_StarSchemaHashJoinOrderOptimization
                      quickstep_queryoptimizer_rules_SwapProbeBuild
                      quickstep_queryoptimizer_strategy_Aggregate
                      quickstep_queryoptimizer_strategy_Join
                      quickstep_queryoptimizer_strategy_OneToOne
                      quickstep_queryoptimizer_strategy_Selection
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_utility_Macros
                      quickstep_utility_PlanVisualizer)
target_link_libraries(quickstep_queryoptimizer_QueryHandle
                      quickstep_catalog_CatalogTypedefs
                      quickstep_catalog_Catalog_proto
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryoptimizer_QueryPlan
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_utility_DAG
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_QueryProcessor
                      quickstep_catalog_Catalog
                      quickstep_catalog_Catalog_proto
                      quickstep_queryoptimizer_Optimizer
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_Validator
                      glog
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil)

if (ENABLE_DISTRIBUTED)
  target_link_libraries(quickstep_queryoptimizer_ExecutionGenerator
                        quickstep_catalog_Catalog_proto)
endif()

# Module all-in-one library:
add_library(quickstep_queryoptimizer ../empty_src.cpp QueryOptimizerModule.hpp)
target_link_libraries(quickstep_queryoptimizer
                      quickstep_queryoptimizer_ExecutionGenerator
                      quickstep_queryoptimizer_LIPFilterGenerator
                      quickstep_queryoptimizer_LogicalGenerator
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_Optimizer
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_OptimizerTree
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_QueryProcessor
                      quickstep_queryoptimizer_Validator
                      quickstep_queryoptimizer_costmodel
                      quickstep_queryoptimizer_expressions
                      quickstep_queryoptimizer_logical
                      quickstep_queryoptimizer_physical
                      quickstep_queryoptimizer_resolver
                      quickstep_queryoptimizer_rules
                      quickstep_queryoptimizer_strategy)
