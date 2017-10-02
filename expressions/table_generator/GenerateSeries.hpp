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

#ifndef QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATE_SERIES_HPP_
#define QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATE_SERIES_HPP_

#include <string>
#include <vector>

#include "expressions/table_generator/GenerateSeriesHandle.hpp"
#include "expressions/table_generator/GeneratorFunction.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/GreaterComparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class GeneratorFunctionHandle;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief GeneratorFunction that generates a series of values, from a start
 *        value to a stop value with a step size.
 */
class GenerateSeries : public GeneratorFunction {
 public:
  /**
   * @brief Singleton instance of the GenerateSeries class.
   * @return A const reference to the singleton instance of the GenerateSeries
   *         class.
   */
  static const GenerateSeries& Instance() {
    static GenerateSeries instance;
    return instance;
  }

  const std::string getName() const override {
    return "generate_series";
  }

  const std::string getSyntax() const override {
    return getName() + "(<start>, <end>[, <step>])";
  }

  GeneratorFunctionHandle* createHandle(
      const std::vector<TypedValue> &arguments) const override {
    // Checks arguments and create the function handle for generate_series.

    // Arguments should have the pattern (start, end) or (start, end, step).
    int arg_size = arguments.size();
    if (arg_size != 2 && arg_size != 3) {
      throw GeneratorFunctionInvalidArguments("Invalid number of arguments");
    }

    std::vector<const Type*> arg_types;
    for (const TypedValue &arg : arguments) {
      if (TypeFactory::TypeRequiresLengthParameter(arg.getTypeID())) {
        throw GeneratorFunctionInvalidArguments("Invalid argument types");
      }
      arg_types.emplace_back(&TypeFactory::GetType(arg.getTypeID()));
    }

    // Get the unified type of all arguments.
    const Type *unified_type = arg_types[0];
    for (int i = 1; i < arg_size && unified_type != nullptr; ++i) {
      unified_type =
          TypeFactory::GetUnifyingType(*arg_types[i],
                                       *unified_type);
    }

    // Check if the unified type if applicable, then create the handle.
    if (unified_type != nullptr) {
      TypeID tid = unified_type->getTypeID();
      if (tid == TypeID::kInt
          || tid == TypeID::kLong
          || tid == TypeID::kFloat
          || tid == TypeID::kDouble) {
        return concretizeWithType(arg_types, arguments, *unified_type);
      }
    }
    throw GeneratorFunctionInvalidArguments("Invalid argument types");
    return nullptr;
  }

 protected:
  GenerateSeries() : GeneratorFunction() {
  }

 private:
  GeneratorFunctionHandle* concretizeWithType(
      const std::vector<const Type*> &arg_types,
      const std::vector<TypedValue> &args,
      const Type &type) const {
    DCHECK(args.size() == 2 || args.size() == 3);

    // Coerce all arguments to the unified type.
    TypedValue start = type.coerceTypedValue(args[0], *arg_types[0]);
    TypedValue end = type.coerceTypedValue(args[1], *arg_types[1]);
    TypedValue step = args.size() > 2
        ? type.coerceTypedValue(args[2], *arg_types[2])
        : type.coerceTypedValue(TypedValue(1), TypeFactory::GetType(TypeID::kInt));

    // Check that step is not 0, and (end - start) / step is positive
    const GreaterComparison &gt_comparator = GreaterComparison::Instance();
    bool start_gt_end = gt_comparator.compareTypedValuesChecked(start, type, end, type);
    bool step_gt_0 = gt_comparator.compareTypedValuesChecked(
        step, type, TypedValue(0), TypeFactory::GetType(TypeID::kInt));
    bool step_lt_0 = gt_comparator.compareTypedValuesChecked(
        TypedValue(0), TypeFactory::GetType(TypeID::kInt), step, type);
    if ((!start_gt_end && step_lt_0) || (start_gt_end && step_gt_0)) {
      throw GeneratorFunctionInvalidArguments("Invalid step width");
    }

    return new GenerateSeriesHandle(getName(), args, type, start, end, step);
  }

  DISALLOW_COPY_AND_ASSIGN(GenerateSeries);
};


/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATE_SERIES_HPP_ */
