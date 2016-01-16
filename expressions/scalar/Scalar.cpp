/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "expressions/scalar/Scalar.hpp"

#include "utility/Macros.hpp"

namespace quickstep {

const char *Scalar::kScalarDataSourceNames[] = {
  "Literal",
  "Attribute",
  "UnaryExpression",
  "BinaryExpression",
  "SimpleCase"
};

const TypedValue& Scalar::getStaticValue() const {
  FATAL_ERROR("Called getStaticValue() on a Scalar which does not have a static value");
}

}  // namespace quickstep
