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

# Module to find Protocol Buffers supporting proto3 syntax.

# Specifically look for map.h, which is a new header for the proto3 syntax.
find_path(PROTOBUF3_INCLUDE_DIR google/protobuf/map.h)

find_library(PROTOBUF3_LIBRARY
             NAMES protobuf libprotobuf
             HINTS ${PROTOBUF3_INCLUDE_DIR}/../lib
                   ${PROTOBUF3_INCLUDE_DIR}/../lib64
                   ${PROTOBUF3_INCLUDE_DIR}/../lib32)

find_program(PROTOBUF3_PROTOC_EXECUTABLE
             NAMES protoc
             HINTS ${PROTOBUF3_INCLUDE_DIR}/../bin)

set(PROTOBUF3_LIBRARIES ${PROTOBUF3_LIBRARY})
set(PROTOBUF3_INCLUDE_DIRS ${PROTOBUF3_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Protobuf3 DEFAULT_MSG
                                  PROTOBUF3_LIBRARY PROTOBUF3_INCLUDE_DIR)

mark_as_advanced(PROTOBUF3_INCLUDE_DIR PROTOBUF3_LIBRARY PROTOBUF3_PROTOC_EXECUTABLE)
