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

# Although flex option nounistd works fine for the generated cpp file, we need
# to make sure this is also set for the header.
set_property(
  DIRECTORY
  APPEND PROPERTY COMPILE_DEFINITIONS YY_NO_UNISTD_H
)

find_package(BISON)
if (BISON_FOUND)
  if (BISON_VERSION VERSION_GREATER 2.4 OR BISON_VERSION VERSION_EQUAL 2.4)
    set(RUN_BISON TRUE)
  else()
    set(RUN_BISON FALSE)
    message(WARNING "You are using a version of bison which is too old to build quickstep's "
                    "parser. A preprocessed copy of the parser sources will be used.")
  endif()
else()
  set(RUN_BISON FALSE)
  message(WARNING "Unable to find bison. A preprocessed copy of the parser sources will be used.")
endif()

find_package(FLEX)
if (FLEX_FOUND)
  # TODO(chasseur): This might not be the exact version to test for.
  if (FLEX_VERSION VERSION_GREATER 2.5.38)
    set(RUN_FLEX TRUE)
  else()
    set(RUN_FLEX FALSE)
    message(WARNING "You are using a version of flex which is too old to build quickstep's lexer. "
                    "A preprocessed copy of the lexer sources will be used.")
  endif()
else()
  set(RUN_FLEX FALSE)
  message(WARNING "Unable to find flex. A preprocessed copy of the lexer sources will be used.")
endif()


if (RUN_BISON)
  BISON_TARGET(SqlParser SqlParser.ypp ${CMAKE_CURRENT_BINARY_DIR}/SqlParser_gen.cpp)
else()
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/preprocessed/SqlParser_gen.hpp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/preprocessed/SqlParser_gen.cpp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  set(BISON_SqlParser_OUTPUTS
      ${CMAKE_CURRENT_BINARY_DIR}/SqlParser_gen.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/SqlParser_gen.hpp)
endif()

if (RUN_FLEX)
  FILE(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.hpp" LEXER_HEADER_FILE)
  FLEX_TARGET(SqlLexer SqlLexer.lpp ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.cpp
              COMPILE_FLAGS "-i -I --header-file=${LEXER_HEADER_FILE}")
  if (RUN_BISON)
    ADD_FLEX_BISON_DEPENDENCY(SqlLexer SqlParser)
  endif()
else()
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/preprocessed/SqlLexer_gen.hpp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/preprocessed/SqlLexer_gen.cpp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  set(FLEX_SqlLexer_OUTPUTS
      ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.cpp
      ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.hpp)
  if (RUN_BISON)
    set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.cpp
        PROPERTIES OBJECT_DEPENDS ${BISON_SqlParser_OUTPUT_HEADER})
  endif()
endif()

# Declare micro-libs:
add_library(quickstep_parser_ParseAssignment ../empty_src.cpp ParseAssignment.hpp)
add_library(quickstep_parser_ParseAttributeDefinition ParseAttributeDefinition.cpp ParseAttributeDefinition.hpp)
add_library(quickstep_parser_ParseBasicExpressions ParseBasicExpressions.cpp ParseBasicExpressions.hpp)
add_library(quickstep_parser_ParseBlockProperties ParseBlockProperties.cpp ParseBlockProperties.hpp)
add_library(quickstep_parser_ParseCommand ParseCommand.cpp ParseCommand.hpp)
add_library(quickstep_parser_ParseExpression ../empty_src.cpp ParseExpression.hpp)
add_library(quickstep_parser_ParseGeneratorTableReference ParseGeneratorTableReference.cpp ParseGeneratorTableReference.hpp)
add_library(quickstep_parser_ParseGroupBy ParseGroupBy.cpp ParseGroupBy.hpp)
add_library(quickstep_parser_ParseHaving ParseHaving.cpp ParseHaving.hpp)
add_library(quickstep_parser_ParseLimit ParseLimit.cpp ParseLimit.hpp)
add_library(quickstep_parser_ParseLiteralValue ParseLiteralValue.cpp ParseLiteralValue.hpp)
add_library(quickstep_parser_ParseOrderBy ParseOrderBy.cpp ParseOrderBy.hpp)
add_library(quickstep_parser_ParsePredicate ParsePredicate.cpp ParsePredicate.hpp)
add_library(quickstep_parser_ParserUtil ParserUtil.cpp ParserUtil.hpp)
add_library(quickstep_parser_ParseSelect ../empty_src.cpp ParseSelect.hpp)
add_library(quickstep_parser_ParseSelectionClause ParseSelectionClause.cpp ParseSelectionClause.hpp)
add_library(quickstep_parser_ParseSimpleTableReference ParseSimpleTableReference.cpp ParseSimpleTableReference.hpp)
add_library(quickstep_parser_ParseStatement ../empty_src.cpp ParseStatement.hpp)
add_library(quickstep_parser_ParseString ParseString.cpp ParseString.hpp)
add_library(quickstep_parser_ParseKeyValue ../empty_src.cpp ParseKeyValue.hpp)
add_library(quickstep_parser_ParseSubqueryExpression ParseSubqueryExpression.cpp ParseSubqueryExpression.hpp)
add_library(quickstep_parser_ParseSubqueryTableReference ParseSubqueryTableReference.cpp ParseSubqueryTableReference.hpp)
add_library(quickstep_parser_ParseTableReference ParseTableReference.cpp ParseTableReference.hpp)
add_library(quickstep_parser_ParseTreeNode ../empty_src.cpp ParseTreeNode.hpp)
add_library(quickstep_parser_SqlParserWrapper SqlParserWrapper.cpp SqlParserWrapper.hpp)
add_library(quickstep_parser_SqlParser ${BISON_SqlParser_OUTPUTS})
add_library(quickstep_parser_SqlLexer ${FLEX_SqlLexer_OUTPUTS})

# Link dependencies:
target_link_libraries(quickstep_parser_ParseAssignment
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseAttributeDefinition
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_types_Type
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseBasicExpressions
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseBlockProperties
                      glog
                      quickstep_parser_ParseKeyValue
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_parser_ParseCommand
                      quickstep_parser_ParseStatement
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_StringUtil)
target_link_libraries(quickstep_parser_ParseExpression
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseGeneratorTableReference
                      quickstep_parser_ParseBasicExpressions
                      quickstep_parser_ParseTableReference
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseGroupBy
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseHaving
                      quickstep_parser_ParsePredicate
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseLimit
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseLiteralValue
                      glog
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_types_DatetimeIntervalType
                      quickstep_types_DoubleType
                      quickstep_types_IntType
                      quickstep_types_LongType
                      quickstep_types_NullType
                      quickstep_types_Type
                      quickstep_types_TypedValue
                      quickstep_types_VarCharType
                      quickstep_types_YearMonthIntervalType
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseOrderBy
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParsePredicate
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseTreeNode
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseSelect
                      glog
                      quickstep_parser_ParseGroupBy
                      quickstep_parser_ParseHaving
                      quickstep_parser_ParseLimit
                      quickstep_parser_ParseOrderBy
                      quickstep_parser_ParsePredicate
                      quickstep_parser_ParseSelectionClause
                      quickstep_parser_ParseTableReference
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseSelectionClause
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseSimpleTableReference
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTableReference
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseStatement
                      glog
                      quickstep_parser_ParseAssignment
                      quickstep_parser_ParseAttributeDefinition
                      quickstep_parser_ParseBasicExpressions
                      quickstep_parser_ParseBlockProperties
                      quickstep_parser_ParseKeyValue
                      quickstep_parser_ParsePredicate
                      quickstep_parser_ParseSelect
                      quickstep_parser_ParseString
                      quickstep_parser_ParseSubqueryTableReference
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseKeyValue
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseSubqueryExpression
                      glog
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseSelect
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseSubqueryTableReference
                      quickstep_parser_ParseSubqueryExpression
                      quickstep_parser_ParseTableReference
                      quickstep_utility_Macros)
target_link_libraries(quickstep_parser_ParseTableReference
                      quickstep_parser_ParseString
                      quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_PtrList)
target_link_libraries(quickstep_parser_ParseTreeNode
                      quickstep_utility_Macros
                      quickstep_utility_TreeStringSerializable)
target_link_libraries(quickstep_parser_ParserUtil
                      quickstep_utility_SqlError)
target_link_libraries(quickstep_parser_SqlLexer
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParserUtil
                      quickstep_parser_ParseString
                      quickstep_parser_SqlParser
                      quickstep_utility_PtrList
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_parser_SqlParser
                      quickstep_parser_ParseAssignment
                      quickstep_parser_ParseAttributeDefinition
                      quickstep_parser_ParseBasicExpressions
                      quickstep_parser_ParseBlockProperties
                      quickstep_parser_ParseCommand
                      quickstep_parser_ParseGeneratorTableReference
                      quickstep_parser_ParseGroupBy
                      quickstep_parser_ParseHaving
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseKeyValue
                      quickstep_parser_ParseLimit
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParseOrderBy
                      quickstep_parser_ParsePredicate
                      quickstep_parser_ParseSelect
                      quickstep_parser_ParseSelectionClause
                      quickstep_parser_ParseSimpleTableReference
                      quickstep_parser_ParseStatement
                      quickstep_parser_ParseString
                      quickstep_parser_ParseSubqueryExpression
                      quickstep_parser_ParseSubqueryTableReference
                      quickstep_parser_ParseTableReference
                      quickstep_parser_ParserUtil
                      quickstep_storage_StorageBlockInfo
                      quickstep_types_Type
                      quickstep_types_TypeFactory
                      quickstep_types_TypeID
                      quickstep_types_operations_binaryoperations_BinaryOperation
                      quickstep_types_operations_binaryoperations_BinaryOperationFactory
                      quickstep_types_operations_binaryoperations_BinaryOperationID
                      quickstep_types_operations_comparisons_Comparison
                      quickstep_types_operations_comparisons_ComparisonFactory
                      quickstep_types_operations_comparisons_ComparisonID
                      quickstep_types_operations_unaryoperations_UnaryOperation
                      quickstep_types_operations_unaryoperations_UnaryOperationFactory
                      quickstep_types_operations_unaryoperations_UnaryOperationID
                      quickstep_utility_PtrList
                      quickstep_utility_PtrVector)
target_link_libraries(quickstep_parser_SqlParserWrapper
                      glog
                      quickstep_parser_ParseStatement
                      quickstep_parser_SqlLexer
                      quickstep_parser_SqlParser
                      quickstep_utility_Macros
                      quickstep_utility_SqlError)

# Dependencies on generated sources.
set_property(SOURCE SqlParserWrapper.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/SqlParser_gen.hpp)
# The dependency is on the header, but this picks up the generated sources
# from the CMake environment.
set_property(SOURCE SqlParserWrapper.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.cpp)
set_property(SOURCE ${CMAKE_CURRENT_BINARY_DIR}/SqlParser_gen.cpp APPEND PROPERTY OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/SqlLexer_gen.cpp)

# Lexers generated by some versions of Flex use the C "register" keyword. This
# supresses warnings about that for the lexer ONLY.
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-Wno-deprecated-register" COMPILER_HAS_WNO_DEPRECATED_REGISTER)
if (COMPILER_HAS_WNO_DEPRECATED_REGISTER)
  set_target_properties(quickstep_parser_SqlLexer PROPERTIES COMPILE_FLAGS "-Wno-deprecated-register")
endif()

add_subdirectory(tests)

# Module all-in-one library:
add_library(quickstep_parser ../empty_src.cpp ParserModule.hpp)
target_link_libraries(quickstep_parser
                      quickstep_parser_ParseAssignment
                      quickstep_parser_ParseAttributeDefinition
                      quickstep_parser_ParseBasicExpressions
                      quickstep_parser_ParseBlockProperties
                      quickstep_parser_ParseCommand
                      quickstep_parser_ParseExpression
                      quickstep_parser_ParseGeneratorTableReference
                      quickstep_parser_ParseGroupBy
                      quickstep_parser_ParseHaving
                      quickstep_parser_ParseLimit
                      quickstep_parser_ParseLiteralValue
                      quickstep_parser_ParseOrderBy
                      quickstep_parser_ParsePredicate
                      quickstep_parser_ParserUtil
                      quickstep_parser_ParseSelect
                      quickstep_parser_ParseSelectionClause
                      quickstep_parser_ParseSimpleTableReference
                      quickstep_parser_ParseStatement
                      quickstep_parser_ParseString
                      quickstep_parser_ParseKeyValue
                      quickstep_parser_ParseSubqueryExpression
                      quickstep_parser_ParseSubqueryTableReference
                      quickstep_parser_ParseTableReference
                      quickstep_parser_ParseTreeNode
                      quickstep_parser_SqlLexer
                      quickstep_parser_SqlParser
                      quickstep_parser_SqlParserWrapper)
