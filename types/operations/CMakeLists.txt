#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015 Pivotal Software, Inc.
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

add_subdirectory(binary_operations)
add_subdirectory(comparisons)
add_subdirectory(unary_operations)

QS_PROTOBUF_GENERATE_CPP(types_operations_Operation_proto_srcs
                         types_operations_Operation_proto_hdrs
                         Operation.proto)

# Declare micro-libs:
add_library(quickstep_types_operations_Operation Operation.cpp Operation.hpp)
add_library(quickstep_types_operations_Operation_proto ${types_operations_Operation_proto_srcs})

# Link dependencies:
target_link_libraries(quickstep_types_operations_Operation
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_operations_Operation_proto
                      quickstep_types_Type_proto
                      ${PROTOBUF_LIBRARY})

# Module all-in-one library:
add_library(quickstep_types_operations ../../empty_src.cpp)
target_link_libraries(quickstep_types_operations
                      quickstep_types_operations_Operation
                      quickstep_types_operations_Operation_proto
                      quickstep_types_operations_binaryoperations
                      quickstep_types_operations_comparisons
                      quickstep_types_operations_unaryoperations)
