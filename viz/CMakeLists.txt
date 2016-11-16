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

add_subdirectory(configs)
add_subdirectory(rules)

# Declare micro-libs:
add_library(quickstep_viz_VizAnalyzer VizAnalyzer.cpp VizAnalyzer.hpp)
add_library(quickstep_viz_VizContext VizContext.cpp VizContext.hpp)
add_library(quickstep_viz_VizEngine VizEngine.cpp VizEngine.hpp)
add_library(quickstep_viz_VizHelper VizHelper.cpp VizHelper.hpp)
add_library(quickstep_viz_VizObject VizObject.cpp VizObject.hpp)

# Link dependencies:
target_link_libraries(quickstep_viz_VizAnalyzer
                      quickstep_catalog_CatalogTypedefs
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryoptimizer_expressions_AttributeReference
                      quickstep_queryoptimizer_expressions_ExpressionUtil
                      quickstep_queryoptimizer_physical_Aggregate
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_queryoptimizer_physical_PhysicalType
                      quickstep_utility_Macros
                      quickstep_viz_VizObject)
target_link_libraries(quickstep_viz_VizContext
                      quickstep_utility_Macros
                      quickstep_viz_VizObject)
target_link_libraries(quickstep_viz_VizEngine
                      quickstep_utility_ExecutionDAGVisualizer
                      quickstep_utility_Macros
                      quickstep_utility_PlanVisualizer
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_VizContext
                      quickstep_viz_configs_DumpRelation
                      quickstep_viz_configs_VizConfig
                      quickstep_viz_rules_VizRule
                      quickstep_viz_rules_Grouping)
target_link_libraries(quickstep_viz_VizHelper
                      quickstep_queryexecution_QueryExecutionTypedefs
                      quickstep_queryoptimizer_LogicalGenerator
                      quickstep_queryoptimizer_OptimizerContext
                      quickstep_queryoptimizer_PhysicalGenerator
                      quickstep_queryoptimizer_physical_Physical
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_VizContext
                      quickstep_viz_VizEngine
                      quickstep_utility_Macros)
target_link_libraries(quickstep_viz_VizObject
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil
                      quickstep_utility_TreeStringSerializable)
