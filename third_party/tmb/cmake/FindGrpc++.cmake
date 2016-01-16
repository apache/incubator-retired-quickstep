# Module to find grpc core library and c++ bindings (grpc++).

# Based on a similar function from FindProtobuf.cmake. The variables SRCS and
# HDRS in the calling environment will be set to the generated C++ source and
# header files, respectively. Files will be generated in the build directory
# under OUTPUT_PATH.
function(GRPC_GENERATE_CPP SRCS HDRS OUTPUT_PATH)
  if(NOT ARGN)
    message(SEND_ERROR "Error: GRPC_GENERATE_CPP() called without any proto files")
    return()
  endif()

  file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}")

  set(${SRCS})
  set(${HDRS})
  foreach(FIL ${ARGN})
    get_filename_component(ABS_FIL ${FIL} ABSOLUTE)
    get_filename_component(FIL_WE ${FIL} NAME_WE)
    get_filename_component(FIL_PATH ${ABS_FIL} PATH)

    list(APPEND ${SRCS} "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.pb.cc"
                        "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.grpc.pb.cc")
    list(APPEND ${HDRS} "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.pb.h"
                        "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.grpc.pb.h")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.pb.cc"
             "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.grpc.pb.cc"
             "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.pb.h"
             "${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH}/${FIL_WE}.grpc.pb.h"
      COMMAND  ${PROTOBUF3_PROTOC_EXECUTABLE}
      ARGS -I ${FIL_PATH} --cpp_out=${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH} --grpc_out=${CMAKE_CURRENT_BINARY_DIR}/${OUTPUT_PATH} --plugin=protoc-gen-grpc=${GRPCPLUSPLUS_PROTOC_PLUGIN} ${ABS_FIL}
      DEPENDS ${ABS_FIL}
      COMMENT "Running C++ protocol buffer compiler and grpc++ plugin on ${FIL}"
      VERBATIM)
  endforeach()

  set_source_files_properties(${${SRCS}} ${${HDRS}} PROPERTIES GENERATED TRUE)
  set(${SRCS} ${${SRCS}} PARENT_SCOPE)
  set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

# grpc++ needs proto3.
find_package(Protobuf3 REQUIRED)

# Include path for C core library.
find_path(GRPC_INCLUDE_DIR grpc/grpc.h)

# Include path for C++ bindings.
find_path(GRPCPLUSPLUS_INCLUDE_DIR
          grpc++/server.h
          HINTS ${GRPC_INCLUDE_DIR})

# Core library.
find_library(GRPC_LIBRARY
             NAMES libgrpc grpc)
get_filename_component(GRPC_LIB_DIR ${GRPC_LIBRARY} PATH)

# Portable run time library (must also link against this).
find_library(GPR_LIBRARY
             NAMES libgpr gpr
             HINTS ${GRPC_LIB_DIR})

# C++ bindings library.
find_library(GRPCPLUSPLUS_LIBRARY
             NAMES libgrpc++ grpc++
             HINTS ${GRPC_LIB_DIR})

# Plugin for protoc.
find_program(GRPCPLUSPLUS_PROTOC_PLUGIN
             NAMES grpc_cpp_plugin
             HINTS ${GRPC_LIB_DIR}/../bin)


set(GRPCPLUSPLUS_LIBRARIES ${GRPCPLUSPLUS_LIBRARY}
                           ${GRPC_LIBRARY}
                           ${GPR_LIBRARY}
                           ${PROTOBUF3_LIBRARIES})
set(GRPCPLUSPLUS_INCLUDE_DIRS ${GRPCPLUSPLUS_INCLUDE_DIR}
                              ${GRPC_INCLUDE_DIR}
                              ${PROTOBUF3_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Grpc++ DEFAULT_MSG
                                  GRPCPLUSPLUS_LIBRARY
                                  GRPC_LIBRARY
                                  GPR_LIBRARY
                                  GRPC_INCLUDE_DIR
                                  GRPCPLUSPLUS_INCLUDE_DIR)

mark_as_advanced(GRPC_INCLUDE_DIR
                 GRPCPLUSPLUS_INCLUDE_DIR
                 GRPC_LIBRARY
                 GPR_LIBRARY
                 GRPCPLUSPLUS_LIBRARY
                 GRPCPLUSPLUS_PROTOC_PLUGIN)
