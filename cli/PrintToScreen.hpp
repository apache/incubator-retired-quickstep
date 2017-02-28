/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_CLI_PRINT_TO_SCREEN_HPP_
#define QUICKSTEP_CLI_PRINT_TO_SCREEN_HPP_

#include <cstdio>
#include <string>
#include <vector>

#include "storage/StorageBlockInfo.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelation;
class StorageManager;
class TupleStorageSubBlock;

/** \addtogroup CLI
 *  @{
 */

/**
 * @brief All static methods which print all of the tuples in a relation to
 *        a FILE* stream.
 **/
class PrintToScreen {
 public:
  static void PrintRelation(const CatalogRelation &relation,
                            StorageManager *storage_manager,
                            FILE *out);

  static std::string GenerateHBar(const std::vector<int> &column_widths);

  /**
   * @brief Get the total number of tuples in the given relation.
   *
   * @param relation The given relation.
   * @param storage_manager The storage manager.
   *
   * @return The total number of tuples in the relation.
   **/
  static std::size_t GetNumTuplesInRelation(const CatalogRelation &relation,
                                            StorageManager *storage_manager);

  /**
   * @brief Print the size of the output (i.e. number of rows in the relation).
   *
   * @param relation The given relation.
   * @param storage_manager The storage manager.
   * @param out The output stream.
   **/
  static void PrintOutputSize(const CatalogRelation &relation,
                              StorageManager *storage_manager,
                              FILE *out);

  /**
   * @brief Return the number of digits in a number
   *
   * @param number The input number.
   * @param out The number of digits in the input number.
   **/
  static int GetNumberOfDigits(int number);

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  PrintToScreen();

  static void printTuple(const TupleStorageSubBlock &tuple_store,
                         const tuple_id tid,
                         const std::vector<int> &column_widths,
                         FILE *out);

  DISALLOW_COPY_AND_ASSIGN(PrintToScreen);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_PRINT_TO_SCREEN_HPP_
