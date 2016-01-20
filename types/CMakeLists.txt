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

include(CheckTypeSize)
set(CMAKE_EXTRA_INCLUDE_FILES "time.h")
CHECK_TYPE_SIZE("time_t" SIZEOF_TIME_T)
if(SIZEOF_TIME_T LESS 8)
  message(
      WARNING
      "time_t is not 64 bits, so printing of years will only be accurate near the epoch")
endif()

add_subdirectory(containers)
add_subdirectory(operations)
add_subdirectory(port)

QS_PROTOBUF_GENERATE_CPP(types_TypedValue_proto_srcs types_TypedValue_proto_hdrs TypedValue.proto)
QS_PROTOBUF_GENERATE_CPP(types_Type_proto_srcs types_Type_proto_hdrs Type.proto)

# Declare micro-libs:
add_library(quickstep_types_CharType CharType.cpp CharType.hpp)
add_library(quickstep_types_DateOperatorOverloads ../empty_src.cpp DateOperatorOverloads.hpp)
add_library(quickstep_types_DatetimeIntervalType DatetimeIntervalType.cpp DatetimeIntervalType.hpp)
add_library(quickstep_types_DatetimeLit ../empty_src.cpp DatetimeLit.hpp)
add_library(quickstep_types_DatetimeType DatetimeType.cpp DatetimeType.hpp)
add_library(quickstep_types_DoubleType DoubleType.cpp DoubleType.hpp)
add_library(quickstep_types_FloatType FloatType.cpp FloatType.hpp)
add_library(quickstep_types_IntType IntType.cpp IntType.hpp)
add_library(quickstep_types_IntervalLit ../empty_src.cpp IntervalLit.hpp)
add_library(quickstep_types_IntervalParser IntervalParser.cpp IntervalParser.hpp)
add_library(quickstep_types_LongType LongType.cpp LongType.hpp)
add_library(quickstep_types_NullCoercibilityCheckMacro ../empty_src.cpp NullCoercibilityCheckMacro.hpp)
add_library(quickstep_types_NullType ../empty_src.cpp NullType.hpp)
add_library(quickstep_types_NumericSuperType ../empty_src.cpp NumericSuperType.hpp)
add_library(quickstep_types_NumericTypeUnifier ../empty_src.cpp NumericTypeUnifier.hpp)
add_library(quickstep_types_Type Type.cpp Type.hpp)
add_library(quickstep_types_TypeErrors ../empty_src.cpp TypeErrors.hpp)
add_library(quickstep_types_TypeFactory TypeFactory.cpp TypeFactory.hpp)
add_library(quickstep_types_TypeID TypeID.cpp TypeID.hpp)
add_library(quickstep_types_Type_proto ${types_Type_proto_srcs})
add_library(quickstep_types_TypedValue TypedValue.cpp TypedValue.hpp)
add_library(quickstep_types_TypedValue_proto ${types_TypedValue_proto_srcs})
add_library(quickstep_types_VarCharType VarCharType.cpp VarCharType.hpp)
add_library(quickstep_types_YearMonthIntervalType YearMonthIntervalType.cpp YearMonthIntervalType.hpp)

# Link dependencies:
target_link_libraries(quickstep_types_CharType
                      glog
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue
                      quickstep_types_port_strnlen
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap)
target_link_libraries(quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_port_gmtime_r
                      quickstep_types_port_timegm)
target_link_libraries(quickstep_types_DatetimeIntervalType
                      glog
                      quickstep_types_IntervalLit
                      quickstep_types_IntervalParser
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_CheckSnprintf
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_DatetimeLit
                      quickstep_types_port_gmtime_r)
target_link_libraries(quickstep_types_DatetimeType
                      glog
                      quickstep_types_DatetimeLit
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_port_gmtime_r
                      quickstep_types_port_timegm
                      quickstep_utility_CheckSnprintf
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_DoubleType
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_NumericSuperType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_FloatType
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_NumericSuperType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_IntType
                      glog
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_NumericSuperType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_IntervalLit
                      glog
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_IntervalParser
                      quickstep_types_IntervalLit
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_LongType
                      glog
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_NumericSuperType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_EqualsAnyConstant
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID)
target_link_libraries(quickstep_types_NullType
                      glog
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_NumericSuperType
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_Type
                      glog
                      quickstep_types_Type_proto
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_TypeFactory
                      glog
                      quickstep_types_CharType
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DatetimeType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_Type_proto
                      quickstep_types_VarCharType
                      quickstep_types_YearMonthIntervalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_TypedValue
                      farmhash
                      glog
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_TypeID
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue_proto
                      quickstep_types_port_strnlen
                      quickstep_utility_Macros)
target_link_libraries(quickstep_types_TypedValue_proto
                      quickstep_types_Type_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_types_Type_proto
                      ${PROTOBUF_LIBRARY})
target_link_libraries(quickstep_types_VarCharType
                      glog
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_Type_proto
                      quickstep_types_TypedValue
                      quickstep_types_port_strnlen
                      quickstep_utility_Macros
                      quickstep_utility_PtrMap)
target_link_libraries(quickstep_types_YearMonthIntervalType
                      quickstep_types_IntervalLit
                      quickstep_types_IntervalParser
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_Type
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_utility_CheckSnprintf
                      quickstep_utility_Macros)

# Module all-in-one library:
add_library(quickstep_types ../empty_src.cpp TypesModule.hpp)
target_link_libraries(quickstep_types
                      quickstep_types_CharType
                      quickstep_types_DateOperatorOverloads
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DatetimeLit
                      quickstep_types_DatetimeType
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_IntervalLit
                      quickstep_types_IntervalParser
                      quickstep_types_LongType
                      quickstep_types_NullCoercibilityCheckMacro
                      quickstep_types_NullType
                      quickstep_types_NumericSuperType
                      quickstep_types_NumericTypeUnifier
                      quickstep_types_Type
                      quickstep_types_Type_proto
                      quickstep_types_TypeErrors
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_types_VarCharType
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_containers
                      quickstep_types_operations
                      quickstep_types_port)

# Tests:
add_library(quickstep_types_tests_TypeTest_common
            ../empty_src.cpp
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/TypeTest_common.hpp")
target_link_libraries(quickstep_types_tests_TypeTest_common
                      gtest
                      quickstep_types_DoubleType
                      quickstep_types_FloatType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue)

add_executable(CharType_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/CharType_unittest.cpp")
target_link_libraries(CharType_unittest
                      gtest
                      gtest_main
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(CharType_unittest CharType_unittest)

add_executable(DatetimeIntervalType_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/DatetimeIntervalType_unittest.cpp")
target_link_libraries(DatetimeIntervalType_unittest
                      gtest
                      gtest_main
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_IntervalLit
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(DatetimeIntervalType_unittest DatetimeIntervalType_unittest)

add_executable(DatetimeType_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/DatetimeType_unittest.cpp")
target_link_libraries(DatetimeType_unittest
                      gtest
                      gtest_main
                      quickstep_types_DatetimeLit
                      quickstep_types_DatetimeType
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(DatetimeType_unittest DatetimeType_unittest)

add_executable(DoubleType_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/DoubleType_unittest.cpp")
target_link_libraries(DoubleType_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_types_DoubleType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(DoubleType_unittest DoubleType_unittest)

add_executable(FloatType_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/FloatType_unittest.cpp")
target_link_libraries(FloatType_unittest
                      glog
                      gtest
                      gtest_main
                      quickstep_types_FloatType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(FloatType_unittest FloatType_unittest)

add_executable(IntType_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/IntType_unittest.cpp")
target_link_libraries(IntType_unittest
                      gtest
                      gtest_main
                      quickstep_types_IntType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(IntType_unittest IntType_unittest)

add_executable(LongType_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/LongType_unittest.cpp")
target_link_libraries(LongType_unittest
                      gtest
                      gtest_main
                      quickstep_types_LongType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(LongType_unittest LongType_unittest)

add_executable(Type_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/Type_unittest.cpp")
target_link_libraries(Type_unittest
                      gtest
                      gtest_main
                      quickstep_types_CharType
                      quickstep_types_Type
                      quickstep_types_Type_proto
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_VarCharType
                      quickstep_utility_Macros)
add_test(Type_unittest Type_unittest)

add_executable(TypedValue_unittest "${CMAKE_CURRENT_SOURCE_DIR}/tests/TypedValue_unittest.cpp")
target_link_libraries(TypedValue_unittest
                      gtest
                      gtest_main
                      quickstep_types_DatetimeLit
                      quickstep_types_IntervalLit
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_TypedValue_proto
                      quickstep_utility_Macros
                      quickstep_utility_ScopedBuffer)
add_test(TypedValue_unittest TypedValue_unittest)

# We intentionally test self-move of TypedValues in the unittest. This disables
# compiler warnings about self-moves in the test ONLY.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-Wno-self-move" COMPILER_HAS_WNO_SELF_MOVE)
if (COMPILER_HAS_WNO_SELF_MOVE)
  set_target_properties(TypedValue_unittest PROPERTIES COMPILE_FLAGS "-Wno-self-move")
endif()

add_executable(VarCharType_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/VarCharType_unittest.cpp")
target_link_libraries(VarCharType_unittest
                      gtest
                      gtest_main
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(VarCharType_unittest VarCharType_unittest)

add_executable(YearMonthIntervalType_unittest
               "${CMAKE_CURRENT_SOURCE_DIR}/tests/YearMonthIntervalType_unittest.cpp")
target_link_libraries(YearMonthIntervalType_unittest
                      gtest
                      gtest_main
                      quickstep_types_IntervalLit
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_TypedValue
                      quickstep_types_YearMonthIntervalType
                      quickstep_types_tests_TypeTest_common
                      quickstep_utility_MemStream)
add_test(YearMonthIntervalType_unittest YearMonthIntervalType_unittest)
