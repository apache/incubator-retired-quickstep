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

# This CMakeLists file allows building protobufs as part of the regular CMake
# build process instead of using autotools (on UNIX) or Visual Studio projects
# (on Windows). Note that the beta releases of protobufs 3.0 already use CMake
# to manage builds for Visual Studio (and the CMake files may work on other
# platforms). This file accomplishes the same thing for protobufs 2.6.1.

set(protobuf_source_dir "../protobuf")
set(protobuf_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

# Quickstep is normally compiled with -Wall, but we disable some warnings ONLY
# when compiling protobuf that would otherwise produce a flurry of warnings
# during the build that have nothing to do with actual Quickstep code.
include(CheckCXXCompilerFlag)
set(PROTOBUF_WARNING_FLAGS)

CHECK_CXX_COMPILER_FLAG("-Wno-unused-local-typedefs" COMPILER_HAS_WNO_UNUSED_LOCAL_TYPEDEFS)
if (COMPILER_HAS_WNO_UNUSED_LOCAL_TYPEDEFS)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-unused-local-typedefs")
endif()

CHECK_CXX_COMPILER_FLAG("-Wno-maybe-uninitialized" COMPILER_HAS_WNO_MAYBE_UNINITIALIZED)
if (COMPILER_HAS_WNO_MAYBE_UNINITIALIZED)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-maybe-uninitialized")
endif()

CHECK_CXX_COMPILER_FLAG("-Wno-nested-anon-types" COMPILER_HAS_WNO_NESTED_ANON_TYPES)
if (COMPILER_HAS_WNO_NESTED_ANON_TYPES)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-nested-anon-types")
endif()

CHECK_CXX_COMPILER_FLAG("-Wno-unused-function" COMPILER_HAS_WNO_UNUSED_FUNCTION)
if (COMPILER_HAS_WNO_UNUSED_FUNCTION)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-unused-function")
endif()

CHECK_CXX_COMPILER_FLAG("-Wno-pedantic" COMPILER_HAS_WNO_PEDANTIC)
if (COMPILER_HAS_WNO_PEDANTIC)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-pedantic")
endif()

CHECK_CXX_COMPILER_FLAG("-Wno-return-type" COMPILER_HAS_WNO_RETURN_TYPE)
if (COMPILER_HAS_WNO_RETURN_TYPE)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-return-type")
endif()

CHECK_CXX_COMPILER_FLAG("-Wno-sign-compare" COMPILER_HAS_WNO_SIGN_COMPARE)
if (COMPILER_HAS_WNO_SIGN_COMPARE)
  set(PROTOBUF_WARNING_FLAGS "${PROTOBUF_WARNING_FLAGS} -Wno-sign-compare")
endif()


add_definitions(-DGOOGLE_PROTOBUF_CMAKE_BUILD)

find_package(Threads REQUIRED)
if (CMAKE_USE_PTHREADS_INIT)
  set(HAVE_PTHREAD 1)
  add_definitions(-DHAVE_PTHREAD)
endif (CMAKE_USE_PTHREADS_INIT)

if (MSVC)
  # For MSVC, just copy the premade config.h
  file(COPY ${protobuf_source_dir}/vsprojects/config.h DESTINATION .)

  # The premade config.h is set up to use non-standard extensions that are
  # now deprecated. This allows them to continue being used.
  add_definitions(-D_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS)
else()
  # Everywhere else, do actual platform checks and make config.h from template.
  include(CheckIncludeFileCXX)
  check_include_file_cxx(dlfcn.h HAVE_DLFCN_H)
  check_include_file_cxx(fcntl.h HAVE_FCNTL_H)
  check_include_file_cxx(inttypes.h HAVE_INTTYPES_H)
  check_include_file_cxx(limits.h HAVE_LIMITS_H)
  check_include_file_cxx(memory.h HAVE_MEMORY_H)
  check_include_file_cxx(stdint.h HAVE_STDINT_H)
  check_include_file_cxx(stdlib.h HAVE_STDLIB_H)
  check_include_file_cxx(strings.h HAVE_STRINGS_H)
  check_include_file_cxx(string.h HAVE_STRING_H)
  check_include_file_cxx(sys/stat.h HAVE_SYS_STAT_H)
  check_include_file_cxx(sys/types.h HAVE_SYS_TYPES_H)
  check_include_file_cxx(unistd.h HAVE_UNISTD_H)

  include(CheckFunctionExists)
  check_function_exists(ftruncate HAVE_FTRUNCATE)
  check_function_exists(memset HAVE_MEMSET)
  check_function_exists(mkdir HAVE_MKDIR)
  check_function_exists(strchr HAVE_STRCHR)
  check_function_exists(strerror HAVE_STRERROR)
  check_function_exists(strtol HAVE_STRTOL)

  configure_file ("${CMAKE_CURRENT_SOURCE_DIR}/config_cmake.h.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/config.h")
endif()

include_directories(${protobuf_BINARY_DIR}
                    ${protobuf_source_dir}/src)

if (MSVC)
  # Add the "lib" prefix for generated .lib outputs.
  set(LIB_PREFIX lib)
else (MSVC)
  # When building with "make", "lib" prefix will be added automatically by
  # the build tool.
  set(LIB_PREFIX)
endif (MSVC)

# Library sources taken from src/Makefile.am in protobuf source tree.
set(libprotobuf_lite_files
    ${protobuf_source_dir}/src/google/protobuf/stubs/atomicops_internals_x86_gcc.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/common.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/once.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/hash.h
    ${protobuf_source_dir}/src/google/protobuf/stubs/map_util.h
    ${protobuf_source_dir}/src/google/protobuf/stubs/shared_ptr.h
    ${protobuf_source_dir}/src/google/protobuf/stubs/stringprintf.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/stringprintf.h
    ${protobuf_source_dir}/src/google/protobuf/extension_set.cc
    ${protobuf_source_dir}/src/google/protobuf/generated_message_util.cc
    ${protobuf_source_dir}/src/google/protobuf/message_lite.cc
    ${protobuf_source_dir}/src/google/protobuf/repeated_field.cc
    ${protobuf_source_dir}/src/google/protobuf/wire_format_lite.cc
    ${protobuf_source_dir}/src/google/protobuf/io/coded_stream.cc
    ${protobuf_source_dir}/src/google/protobuf/io/coded_stream_inl.h
    ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream.cc
    ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl_lite.cc)
add_library(libprotobuf-lite ${libprotobuf_lite_files})
target_link_libraries(libprotobuf-lite ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(libprotobuf-lite PUBLIC ${protobuf_source_dir}/src)
set_target_properties(libprotobuf-lite PROPERTIES
    COMPILE_DEFINITIONS LIBPROTOBUF_EXPORTS
    OUTPUT_NAME ${LIB_PREFIX}protobuf-lite)
if (PROTOBUF_WARNING_FLAGS)
  set_target_properties(libprotobuf-lite PROPERTIES
                        COMPILE_FLAGS ${PROTOBUF_WARNING_FLAGS})
endif()

set(libprotobuf_files
    ${libprotobuf_lite_files}
    ${protobuf_source_dir}/src/google/protobuf/stubs/strutil.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/strutil.h
    ${protobuf_source_dir}/src/google/protobuf/stubs/substitute.cc
    ${protobuf_source_dir}/src/google/protobuf/stubs/substitute.h
    ${protobuf_source_dir}/src/google/protobuf/stubs/structurally_valid.cc
    ${protobuf_source_dir}/src/google/protobuf/descriptor.cc
    ${protobuf_source_dir}/src/google/protobuf/descriptor.pb.cc
    ${protobuf_source_dir}/src/google/protobuf/descriptor_database.cc
    ${protobuf_source_dir}/src/google/protobuf/dynamic_message.cc
    ${protobuf_source_dir}/src/google/protobuf/extension_set_heavy.cc
    ${protobuf_source_dir}/src/google/protobuf/generated_message_reflection.cc
    ${protobuf_source_dir}/src/google/protobuf/message.cc
    ${protobuf_source_dir}/src/google/protobuf/reflection_ops.cc
    ${protobuf_source_dir}/src/google/protobuf/service.cc
    ${protobuf_source_dir}/src/google/protobuf/text_format.cc
    ${protobuf_source_dir}/src/google/protobuf/unknown_field_set.cc
    ${protobuf_source_dir}/src/google/protobuf/wire_format.cc
    ${protobuf_source_dir}/src/google/protobuf/io/gzip_stream.cc
    ${protobuf_source_dir}/src/google/protobuf/io/printer.cc
    ${protobuf_source_dir}/src/google/protobuf/io/strtod.cc
    ${protobuf_source_dir}/src/google/protobuf/io/tokenizer.cc
    ${protobuf_source_dir}/src/google/protobuf/io/zero_copy_stream_impl.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/importer.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/parser.cc)
add_library(libprotobuf ${libprotobuf_files})
target_link_libraries(libprotobuf ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(libprotobuf PUBLIC ${protobuf_source_dir}/src)
set_target_properties(libprotobuf PROPERTIES
    COMPILE_DEFINITIONS LIBPROTOBUF_EXPORTS
    OUTPUT_NAME ${LIB_PREFIX}protobuf)
if (PROTOBUF_WARNING_FLAGS)
  set_target_properties(libprotobuf PROPERTIES
                        COMPILE_FLAGS ${PROTOBUF_WARNING_FLAGS})
endif()

set(libprotoc_files
    ${protobuf_source_dir}/src/google/protobuf/compiler/code_generator.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/command_line_interface.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/plugin.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/plugin.pb.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/subprocess.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/subprocess.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/zip_writer.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/zip_writer.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_enum.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_enum.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_enum_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_enum_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_extension.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_extension.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_file.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_file.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_generator.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_helpers.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_helpers.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_message.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_message.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_message_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_message_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_options.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_primitive_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_primitive_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_service.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_service.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_string_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/cpp/cpp_string_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_context.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_context.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_enum.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_enum.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_enum_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_enum_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_extension.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_extension.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_file.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_file.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_generator.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_generator_factory.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_generator_factory.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_helpers.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_helpers.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_lazy_message_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_lazy_message_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_message.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_message.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_message_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_message_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_name_resolver.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_name_resolver.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_primitive_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_primitive_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_shared_code_generator.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_shared_code_generator.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_service.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_service.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_string_field.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_string_field.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_doc_comment.cc
    ${protobuf_source_dir}/src/google/protobuf/compiler/java/java_doc_comment.h
    ${protobuf_source_dir}/src/google/protobuf/compiler/python/python_generator.cc)
add_library(libprotoc ${libprotoc_files})
target_link_libraries(libprotoc libprotobuf)
set_target_properties(libprotoc PROPERTIES
    COMPILE_DEFINITIONS LIBPROTOC_EXPORTS
    OUTPUT_NAME ${LIB_PREFIX}protoc)
if (PROTOBUF_WARNING_FLAGS)
  set_target_properties(libprotoc PROPERTIES
                        COMPILE_FLAGS ${PROTOBUF_WARNING_FLAGS})
endif()

# protoc compiler executable
set(protoc_files
    ${protobuf_source_dir}/src/google/protobuf/compiler/main.cc)
add_executable(protoc ${protoc_files})
target_link_libraries(protoc libprotobuf libprotoc)
