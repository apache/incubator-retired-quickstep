/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_CLI_PRINT_TO_SCREEN_HPP_
#define QUICKSTEP_CLI_PRINT_TO_SCREEN_HPP_

#include <cstdio>
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

 private:
  // Undefined default constructor. Class is all-static and should not be
  // instantiated.
  PrintToScreen();

  static void printHBar(const std::vector<int> &column_widths,
                        FILE *out);
  static void printTuple(const TupleStorageSubBlock &tuple_store,
                         const tuple_id tid,
                         const std::vector<int> &column_widths,
                         FILE *out);

  DISALLOW_COPY_AND_ASSIGN(PrintToScreen);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_PRINT_TO_SCREEN_HPP_
