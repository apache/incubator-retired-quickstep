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
add_library(quickstep_types_operations_utility_CastUtil CastUtil.cpp CastUtil.hpp)
add_library(quickstep_types_operations_utility_OperationSynthesizeUtil
            ../../../empty_src.cpp
            OperationSynthesizeUtil.hpp)

# Link dependencies:
target_link_libraries(quickstep_types_operations_utility_CastUtil
                      quickstep_types_TypeID
                      quickstep_types_TypeRegistrar
                      quickstep_types_operations_unaryoperations_CastFunctorOverloads
                      quickstep_utility_Macros
                      quickstep_utility_meta_Common
                      quickstep_utility_meta_TypeList)
target_link_libraries(quickstep_types_operations_utility_OperationSynthesizeUtil
                      quickstep_catalog_CatalogTypedefs
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_containers_ColumnVector
                      quickstep_utility_Macros
                      quickstep_utility_meta_TypeList)

# Module all-in-one library:
add_library(quickstep_types_operations_utility ../../../empty_src.cpp)
target_link_libraries(quickstep_types_operations_utility
                      quickstep_types_operations_utility_CastUtil
                      quickstep_types_operations_utility_OperationSynthesizeUtil)
