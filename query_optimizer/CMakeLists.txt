# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# Copyright (c) 2015, Pivotal Software, Inc.
# All rights reserved.
# See file CREDITS.txt for details.

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
                      glog
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_expressions_Expressions_proto
                      quickstep_expressions_aggregation_AggregateFunction
                      quickstep_expressions_aggregation_AggregateFunction_proto
                      quickstep_expressions_predicate_Predicate
                      quickstep_expressions_scalar_Scalar
                      quickstep_expressions_scalar_ScalarAttribute
                      quickstep_queryexecution_QueryContext
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_costmodel_SimpleCostModel
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
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_CopyFrom
                      quickstep_queryoptimizer_physical_CreateTable
                      quickstep_queryoptimizer_physical_DeleteTuples
                      quickstep_queryoptimizer_physical_DropTable
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_InsertTuple
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_PatternMatcher
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_Sort
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_queryoptimizer_physical_UpdateTable
                      quickstep_relationaloperators_AggregationOperator
                      quickstep_relationaloperators_BuildHashOperator
                      quickstep_relationaloperators_CreateTableOperator
                      quickstep_relationaloperators_DeleteOperator
                      quickstep_relationaloperators_DestroyHashOperator
                      quickstep_relationaloperators_DropTableOperator
                      quickstep_relationaloperators_FinalizeAggregationOperator
                      quickstep_relationaloperators_HashJoinOperator
                      quickstep_relationaloperators_InsertOperator
                      quickstep_relationaloperators_NestedLoopsJoinOperator
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_relationaloperators_SaveBlocksOperator
                      quickstep_relationaloperators_SelectOperator
                      quickstep_relationaloperators_SortMergeRunOperator
                      quickstep_relationaloperators_SortRunGenerationOperator
                      quickstep_relationaloperators_TextScanOperator
                      quickstep_relationaloperators_UpdateOperator
                      quickstep_storage_AggregationOperationState_proto
                      quickstep_storage_HashTableFactory
                      quickstep_storage_HashTable_proto
                      quickstep_storage_InsertDestination_proto
                      quickstep_storage_StorageBlockLayout
                      quickstep_storage_StorageBlockLayout_proto
                      quickstep_types_Type
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_types_containers_Tuple_proto
                      quickstep_utility_Macros
                      quickstep_utility_SqlError)
target_link_libraries(quickstep_queryoptimizer_LogicalGenerator
                      glog
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_resolver_Resolver
                      quickstep_queryoptimizer_rules_CollapseProject
                      quickstep_queryoptimizer_rules_GenerateJoins
                      quickstep_queryoptimizer_rules_PushDownFilter
                      quickstep_queryoptimizer_rules_Rule
                      quickstep_queryoptimizer_Validator
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
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryoptimizer_OptimizerContext
                      quickstep_catalog_CatalogTypedefs
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryoptimizer_OptimizerTree
                      quickstep_utility_TreeStringSerializable
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_LogicalToPhysicalMapper
                      quickstep_queryoptimizer_logical_Logical
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_rules_PruneColumns
                      quickstep_queryoptimizer_strategy_Aggregate
                      quickstep_queryoptimizer_strategy_Join
                      quickstep_queryoptimizer_strategy_OneToOne
                      quickstep_queryoptimizer_strategy_Selection
                      quickstep_queryoptimizer_strategy_Strategy
                      quickstep_queryoptimizer_Validator
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_QueryHandle
                      quickstep_queryexecution_QueryContext_proto
                      quickstep_queryoptimizer_QueryPlan
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_QueryPlan
                      quickstep_relationaloperators_RelationalOperator
                      quickstep_utility_DAG
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_QueryProcessor
                      quickstep_catalog_Catalog
                      quickstep_catalog_CatalogDatabase
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_Catalog_proto
                      quickstep_parser_ParseStatement
                      quickstep_queryoptimizer_Optimizer
                      quickstep_queryoptimizer_QueryHandle
                      quickstep_storage_StorageManager
                      quickstep_utility_Macros
                      tmb)
target_link_libraries(quickstep_queryoptimizer_Validator
                      glog
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil)

# Module all-in-one library:
add_library(quickstep_queryoptimizer ../empty_src.cpp QueryOptimizerModule.hpp)
target_link_libraries(quickstep_queryoptimizer
                      quickstep_queryoptimizer_ExecutionGenerator
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
                      quickstep_queryoptimizer_rules)
