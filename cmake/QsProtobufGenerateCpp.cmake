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

# This function is partially based on a similar one from FindProtobuf.cmake,
# but it adds the ability to scan .proto files for imports and automatically
# set the proper dependencies.
#
#  QS_PROTOBUF_GENERATE_CPP(SRCS HDRS [PROTOFILES ...])
#
# - Example
#
# include(QsProtobufGenerateCpp)  NOTE: already included in /CMakeLists.txt
# QsProtobufGenerateCpp(storage_AggregationOperationState_proto_srcs
#                       storage_AggregationOperationState_proto_hdrs
#                       AggregationOperationState.proto)
# Requires CMake 2.6+
function(QS_PROTOBUF_GENERATE_CPP SRCS HDRS)
  if (NOT ARGN)
    message(SEND_ERROR "Error: QS_PROTOBUF_GENERATE_CPP() called without any proto files")
    return()
  endif ()

  set(${SRCS})
  set(${HDRS})
  foreach (FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)

    # Scan for other protos imported in the .proto file.
    set(PROTO_IMPORT_TARGETS)
    file(STRINGS ${ABS_FIL} PROTO_SRC)
    foreach (PROTO_SRC_LINE ${PROTO_SRC})
      string(REGEX MATCH "import \"(.*)[.]proto\"" TMPMATCH ${PROTO_SRC_LINE})
      if (CMAKE_MATCH_1)
        string(REPLACE "/" "_" IMPORTED_PROTO_FLAT "${CMAKE_BINARY_DIR}/${CMAKE_MATCH_1}")
        string(REPLACE ":" "_" IMPORTED_PROTO_FLAT ${IMPORTED_PROTO_FLAT})
        list(APPEND PROTO_IMPORT_TARGETS "protoc_${IMPORTED_PROTO_FLAT}")
      endif ()
    endforeach ()

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")

    # Note that this adds dependencies for the .proto file itself, as well as
    # for any proto imports and the protobuf library if we have to build it
    # ourselves.
    add_custom_command(
              OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.cc"
              "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h"
              COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
              ARGS --cpp_out ${CMAKE_BINARY_DIR} -I${PROJECT_SOURCE_DIR} ${ABS_FIL}
              DEPENDS ${ABS_FIL} ${PROTO_IMPORT_TARGETS} ${PROTOBUF_GLOBAL_DEPS}
              COMMENT "Running C++ protocol buffer compiler on ${FIL}"
              VERBATIM)

    # Make a custom global target so that we have something that protos in
    # other directories can depend on for imports.
    string(REPLACE "/" "_" PROTOC_TARGET_NAME "protoc_${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}")
    string(REPLACE ":" "_" PROTOC_TARGET_NAME ${PROTOC_TARGET_NAME})
    add_custom_target(${PROTOC_TARGET_NAME} DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/${FIL_WE}.pb.h")
  endforeach ()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()
