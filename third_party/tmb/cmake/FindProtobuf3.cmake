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
