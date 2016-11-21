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
add_library(quickstep_viz_rules_Grouping ../../empty_src.cpp Grouping.hpp)
add_library(quickstep_viz_rules_OneDimensionOneMeasure
            ../../empty_src.cpp
            OneDimensionOneMeasure.hpp)
add_library(quickstep_viz_rules_TwoDimensionsOneMeasure
            ../../empty_src.cpp
            TwoDimensionsOneMeasure.hpp)
add_library(quickstep_viz_rules_VizRule ../../empty_src.cpp VizRule.hpp)

# Link dependencies:
target_link_libraries(quickstep_viz_rules_Grouping
                      quickstep_catalog_CatalogAttribute
                      quickstep_catalog_CatalogRelation
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_rules_OneDimensionOneMeasure
                      quickstep_viz_rules_TwoDimensionsOneMeasure)
target_link_libraries(quickstep_viz_rules_OneDimensionOneMeasure
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_VizContext
                      quickstep_viz_VizObject
                      quickstep_viz_configs_BarChart
                      quickstep_viz_configs_PieChart
                      quickstep_viz_configs_TimeSeries)
target_link_libraries(quickstep_viz_rules_TwoDimensionsOneMeasure
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_viz_VizAnalyzer
                      quickstep_viz_VizContext
                      quickstep_viz_VizObject
                      quickstep_viz_configs_StackedAreaTimeSeries
                      quickstep_viz_configs_TimeSeries)
target_link_libraries(quickstep_viz_rules_VizRule
                      quickstep_utility_Macros
                      quickstep_viz_VizContext
                      quickstep_viz_configs_VizConfig)
