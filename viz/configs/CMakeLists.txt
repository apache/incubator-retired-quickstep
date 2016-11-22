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
add_library(quickstep_viz_configs_BarChart ../../empty_src.cpp BarChart.hpp)
add_library(quickstep_viz_configs_DumpRelation ../../empty_src.cpp DumpRelation.hpp)
add_library(quickstep_viz_configs_HeatMap ../../empty_src.cpp HeatMap.hpp)
add_library(quickstep_viz_configs_LineChart ../../empty_src.cpp LineChart.hpp)
add_library(quickstep_viz_configs_PieChart ../../empty_src.cpp PieChart.hpp)
add_library(quickstep_viz_configs_StackedAreaTimeSeries ../../empty_src.cpp StackedAreaTimeSeries.hpp)
add_library(quickstep_viz_configs_TimeSeries ../../empty_src.cpp TimeSeries.hpp)
add_library(quickstep_viz_configs_VizConfig VizConfig.cpp VizConfig.hpp)

# Link dependencies:
target_link_libraries(quickstep_viz_configs_BarChart
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_DumpRelation
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_HeatMap
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_LineChart
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_PieChart
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_StackedAreaTimeSeries
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_TimeSeries
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
target_link_libraries(quickstep_viz_configs_VizConfig
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogRelationSchema
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageManager
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_utility_Macros
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_VizContext
                      quickstep_viz_VizObject)
