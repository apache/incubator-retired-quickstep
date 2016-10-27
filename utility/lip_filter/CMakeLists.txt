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

QS_PROTOBUF_GENERATE_CPP(utility_lipfilter_LIPFilter_proto_srcs
                         utility_lipfilter_LIPFilter_proto_hdrs
                         LIPFilter.proto)

# Declare micro-libs:
add_library(quickstep_utility_lipfilter_BitVectorExactFilter ../../empty_src.cpp BitVectorExactFilter.hpp)
add_library(quickstep_utility_lipfilter_LIPFilter ../../empty_src.cpp LIPFilter.hpp)
add_library(quickstep_utility_lipfilter_LIPFilterAdaptiveProber ../../empty_src.cpp LIPFilterAdaptiveProber.hpp)
add_library(quickstep_utility_lipfilter_LIPFilterBuilder ../../empty_src.cpp LIPFilterBuilder.hpp)
add_library(quickstep_utility_lipfilter_LIPFilterDeployment LIPFilterDeployment.cpp LIPFilterDeployment.hpp)
add_library(quickstep_utility_lipfilter_LIPFilterFactory LIPFilterFactory.cpp LIPFilterFactory.hpp)
add_library(quickstep_utility_lipfilter_LIPFilterUtil ../../empty_src.cpp LIPFilterUtil.hpp)
add_library(quickstep_utility_lipfilter_LIPFilter_proto
            ${utility_lipfilter_LIPFilter_proto_srcs})
add_library(quickstep_utility_lipfilter_SingleIdentityHashFilter ../../empty_src.cpp SingleIdentityHashFilter.hpp)

# Link dependencies:
target_link_libraries(quickstep_utility_lipfilter_BitVectorExactFilter
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_utility_lipfilter_LIPFilter
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_lipfilter_LIPFilter
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_lipfilter_LIPFilterAdaptiveProber
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_TupleIdSequence
                      quickstep_storage_ValueAccessor
                      quickstep_types_Type
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter)
target_link_libraries(quickstep_utility_lipfilter_LIPFilterBuilder
                      quickstep_catalog_CatalogTypedefs
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter)
target_link_libraries(quickstep_utility_lipfilter_LIPFilterDeployment
                      quickstep_catalog_CatalogTypedefs
                      quickstep_types_TypeFactory
                      quickstep_utility_Macros
                      quickstep_utility_lipfilter_LIPFilter
                      quickstep_utility_lipfilter_LIPFilterAdaptiveProber
                      quickstep_utility_lipfilter_LIPFilterBuilder
                      quickstep_utility_lipfilter_LIPFilter_proto)
target_link_libraries(quickstep_utility_lipfilter_LIPFilterFactory
                      quickstep_utility_lipfilter_BitVectorExactFilter
                      quickstep_utility_lipfilter_LIPFilter_proto
                      quickstep_utility_lipfilter_SingleIdentityHashFilter
                      quickstep_utility_Macros)
target_link_libraries(quickstep_utility_lipfilter_LIPFilterUtil
                      quickstep_queryexecution_QueryContext
                      quickstep_utility_lipfilter_LIPFilterDeployment)
target_link_libraries(quickstep_utility_lipfilter_LIPFilter_proto
                      ${PROTOBUF_LIBRARY}
                      quickstep_types_Type_proto)
target_link_libraries(quickstep_utility_lipfilter_SingleIdentityHashFilter
                      quickstep_catalog_CatalogTypedefs
                      quickstep_storage_StorageBlockInfo
                      quickstep_storage_StorageConstants
                      quickstep_storage_ValueAccessor
                      quickstep_storage_ValueAccessorUtil
                      quickstep_types_Type
                      quickstep_utility_lipfilter_LIPFilter
                      quickstep_utility_Macros)
