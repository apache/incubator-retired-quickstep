/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "types/operations/unary_operations/UnaryOperationID.hpp"

namespace quickstep {

const char *kUnaryOperationNames[] = {
  "Negate", "Cast", "DateExtract"
};

const char *kUnaryOperationShortNames[] = {
  "-", "Cast", "DateExtract"
};

}  // namespace quickstep
