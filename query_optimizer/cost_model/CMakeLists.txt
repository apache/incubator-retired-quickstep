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
add_library(quickstep_queryoptimizer_costmodel_CostModel ../../empty_src.cpp CostModel.hpp)
add_library(quickstep_queryoptimizer_costmodel_SimpleCostModel SimpleCostModel.cpp SimpleCostModel.hpp)
add_library(quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
            StarSchemaSimpleCostModel.cpp
            StarSchemaSimpleCostModel.hpp)

# Link dependencies:
target_link_libraries(quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_costmodel_SimpleCostModel
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_utility_Macros)
target_link_libraries(quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel
                      glog
                      quickstep_catalog_CatalogRelation
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ComparisonExpression
                      quickstep_queryoptimizer_expressions_ExprId
                      quickstep_queryoptimizer_expressions_ExpressionType
                      quickstep_queryoptimizer_expressions_LogicalAnd
                      quickstep_queryoptimizer_expressions_LogicalOr
                      quickstep_queryoptimizer_expressions_PatternMatcher
                      quickstep_queryoptimizer_expressions_Predicate
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_HashJoin
                      quickstep_queryoptimizer_physical_NestedLoopsJoin
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_queryoptimizer_physical_Selection
                      quickstep_queryoptimizer_physical_SharedSubplanReference
                      quickstep_queryoptimizer_physical_Sort
                      quickstep_queryoptimizer_physical_TableGenerator
                      quickstep_queryoptimizer_physical_TableReference
                      quickstep_queryoptimizer_physical_TopLevelPlan
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_queryoptimizer_costmodel ../../empty_src.cpp CostModelModule.hpp)
target_link_libraries(quickstep_queryoptimizer_costmodel
                      quickstep_queryoptimizer_costmodel_CostModel
                      quickstep_queryoptimizer_costmodel_SimpleCostModel
                      quickstep_queryoptimizer_costmodel_StarSchemaSimpleCostModel)
