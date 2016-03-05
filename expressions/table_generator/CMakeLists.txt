#   Copyright 2016 Pivotal Software, Inc.
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

QS_PROTOBUF_GENERATE_CPP(expressions_tablegenerator_GeneratorFunction_proto_srcs
                         expressions_tablegenerator_GeneratorFunction_proto_hdrs
                         GeneratorFunction.proto)

# Declare micro-libs:
add_library(quickstep_expressions_tablegenerator_GenerateSeries
            ../../empty_src.cpp
            GeneratorFunction.hpp)
add_library(quickstep_expressions_tablegenerator_GenerateSeriesHandle
            ../../empty_src.cpp
            GeneratorFunctionHandle.hpp)
add_library(quickstep_expressions_tablegenerator_GeneratorFunction
            ../../empty_src.cpp
            GeneratorFunction.hpp)
add_library(quickstep_expressions_tablegenerator_GeneratorFunction_proto
            ${expressions_tablegenerator_GeneratorFunction_proto_srcs})
add_library(quickstep_expressions_tablegenerator_GeneratorFunctionFactory
            GeneratorFunctionFactory.cpp
            GeneratorFunctionFactory.hpp)
add_library(quickstep_expressions_tablegenerator_GeneratorFunctionHandle
            ../../empty_src.cpp
            GeneratorFunctionHandle.hpp)

# Link dependencies:
target_link_libraries(quickstep_expressions_tablegenerator_GenerateSeries
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_tablegenerator_GenerateSeriesHandle
                      glog
                      quickstep_expressions_tablegenerator_GeneratorFunction_proto
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_tablegenerator_GeneratorFunction
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_tablegenerator_GeneratorFunctionFactory
                      glog
                      quickstep_expressions_tablegenerator_GenerateSeries
                      quickstep_expressions_tablegenerator_GeneratorFunction
                      quickstep_expressions_tablegenerator_GeneratorFunction_proto
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_tablegenerator_GeneratorFunctionHandle
                      quickstep_expressions_tablegenerator_GeneratorFunction_proto
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_utility_Macros)
target_link_libraries(quickstep_expressions_tablegenerator_GeneratorFunction_proto
                      quickstep_types_TypedValue_proto
                      ${PROTOBUF_LIBRARY})
