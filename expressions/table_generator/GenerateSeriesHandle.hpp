/**
 *   Copyright 2016 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATE_SERIES_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATE_SERIES_HANDLE_HPP_

#include <string>
#include <vector>

#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Handle for the instantiated GenerateSeries function.
 */
class GenerateSeriesHandle : public GeneratorFunctionHandle {
 public:
  /**
   * @brief Constructor
   *
   * @param func_name The registered name of the GenerateSeries function.
   * @param orig_args The original constant arguments to this function
   * @param type The unified type for the arguments.
   * @param start The start value. Its type should equal unified_type.
   * @param end The end value. Its type should equal unified_type.
   * @param step The step size. Its type should equal unified_type.
   */
  GenerateSeriesHandle(const std::string &func_name,
                       const std::vector<TypedValue> &orig_args,
                       const Type &unified_type,
                       const TypedValue &start,
                       const TypedValue &end,
                       const TypedValue &step)
      : GeneratorFunctionHandle(func_name, orig_args),
        type_(unified_type),
        start_(start),
        end_(end),
        step_(step) {
  }

  int getNumberOfOutputColumns() const override {
    return 1;
  }

  std::string getOutputColumnName(int index) const override {
    if (index > 0) {
      LOG(FATAL) << "generate_series function has only 1 output column";
    }
    // Use the function name as the column name.
    return getName();
  }

  const Type &getOutputColumnType(int index) const override {
    if (index > 0) {
      LOG(FATAL) << "generate_series function has only 1 output column";
    }
    return type_;
  }

  void populateColumns(ColumnVectorsValueAccessor *results) const override {
    DCHECK(results != nullptr);

    // Populate the output column.
    ColumnVector *result_vec;
    switch (type_.getTypeID()) {
      case TypeID::kInt: {
        result_vec = generateColumn<int>();
        break;
      }
      case TypeID::kLong: {
        result_vec = generateColumn<std::int64_t>();
        break;
      }
      case TypeID::kFloat: {
        result_vec = generateColumn<float>();
        break;
      }
      case TypeID::kDouble: {
        result_vec = generateColumn<double>();
        break;
      }
      default:
        LOG(FATAL) << "GenerateSeries cannot handle arguments with type "
                   << type_.getName();
    }
    results->addColumn(result_vec);
  }

 private:
  template <typename T>
  ColumnVector *generateColumn() const {
    T start = start_.getLiteral<T>();
    T end = end_.getLiteral<T>();
    T step = step_.getLiteral<T>();

    DCHECK_NE(step, static_cast<T>(0));
    std::size_t length = static_cast<std::size_t>((end - start) / step) + 1;
    DCHECK_GE(length, static_cast<std::size_t>(0));

    NativeColumnVector *result_vec = new NativeColumnVector(type_, length);
    T value = start;
    for (std::size_t i = 0; i < length; i++) {
      result_vec->appendUntypedValue(&value);
      value += step;
    }
    return result_vec;
  }

  const Type &type_;
  const TypedValue start_;
  const TypedValue end_;
  const TypedValue step_;

  DISALLOW_COPY_AND_ASSIGN(GenerateSeriesHandle);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATE_SERIES_HANDLE_HPP_ */
