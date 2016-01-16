/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/Operation.hpp"

namespace quickstep {

const char *Operation::kOperationSuperTypeNames[] = {
  "Comparison",
  "UnaryOperation",
  "BinaryOperation"
};

}  // namespace quickstep
