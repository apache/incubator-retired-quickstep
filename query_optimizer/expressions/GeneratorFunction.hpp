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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_GENERATOR_FUNCTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_GENERATOR_FUNCTION_HPP_

#include <memory>
#include <unordered_map>

#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeFactory.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

enum class GeneratorFunctionID {
  kGenerateSeries
};

/**
 * @brief Abstract representation of a generator function.
 **/
class GeneratorFunction {
  /**
   * @brief Get the ID of this generator function
   * @return The GeneratorFunctionID of this GeneratorFunction.
   **/
  virtual GeneratorFunctionID getGeneratorFunctionID() const = 0;

  /**
   * @brief Get the human-readable name of this GeneratorFunction.
   *
   * @return The human-readable name of this GeneratorFunction.
   **/
  virtual std::string getName() const = 0;

 protected:
  GeneratorFunction() {
  }

 private:

  DISALLOW_COPY_AND_ASSIGN(GeneratorFunction);
};


class GenerateSeries : public GeneratorFunction {
 public:
  GenerateSeries(const Type *type,
                 const TypedValue &start,
                 const TypedValue &end,
                 const TypedValue &step)
      : type_(type),
        start_(start),
        end_(end),
        step_(step) {
  }
 
  GeneratorFunctionID getGeneratorFunctionID() const override {
    return GeneratorFunctionID::kGenerateSeries;
  }
 
  std::string getName() const override {
    return "generate_series";
  }
 
 private:
  const Type *type_;
  const TypedValue start_;
  const TypedValue end_;
  const TypedValue step_;

  DISALLOW_COPY_AND_ASSIGN(GenerateSeries);
};

/**
 * @brief Abstract representation of the descriptor of a generator function.
 **/
class GeneratorFunctionDescriptor {
 public:
  /**
   * @brief Concretize a GeneratorFunction with the type information of
   *        its arguments
   *
   * @param argument A list of zero or more TypedValue (in order) as
   *        arguments to this function.
   * @return The concretized GeneratorFunction, or NULL if the function cannot
   *         be concretized with the given arguments.
   **/
  virtual GeneratorFunction *concretize(
      const std::vector<TypedValue> &arguments) const = 0;

 protected:
  GeneratorFunctionDescriptor() {
  }
 
 private:
  DISALLOW_COPY_AND_ASSIGN(GeneratorFunctionDescriptor);
};

/**
 * @brief GenerateSeries
 */
class GenerateSeriesDescriptor : GeneratorFunctionDescriptor {
 public:
  static const GenerateSeriesDescriptor& Instance() {
    static GenerateSeriesDescriptor instance;
    return instance;
  }

  GeneratorFunction *concretize(const std::vector<TypedValue> &arguments) const override {
    int arg_size = arguments.size();
    if (arg_size == 2 || arg_size == 3) {
      const Type *unified_type = &TypeFactory::GetType(arguments[0].getTypeID());
      for (int i = 1; i < arg_size; i++) {
        unified_type =
            TypeFactory::GetUnifyingType(TypeFactory::GetType(arguments[i].getTypeID()),
                                         *unified_type);
        if (unified_type == nullptr) {
          return nullptr;
        }
      }
      return concretizeWithType(arguments, unified_type);
    }
    return nullptr;
  }

 protected:
  GenerateSeriesDescriptor() : GeneratorFunctionDescriptor() {
  }

 private:
   /**
   * @brief Concretize the GenerateSeries function with all arguments coerced
   *        to the specified type
   *
   * @param arguments A list of two or three TypedValue arguments to this
   *        function.
   *
   * @return The concretized GeneratorFunction with the coerced arguments.
   */
  GenerateSeries *concretizeWithType(const std::vector<TypedValue> &arguments,
                                     const Type* type) const {
    std::vector<const Type&> arg_types;
    for (auto const& arg : arguments) {
      arg_types.emplace_back(TypeFactory::GetType(arguments[i].getTypeID()))
    }
   
    TypedValue start = type->coerceValue(arguments[0], arg_types[0]);
    TypedValue end = type->coerceValue(arguments[1], arg_types[1]);
    TypedValue step =
        arguments.size() > 2 ? type->coerceValue(arguments[2], arg_types[2])
                             : type->coerceValue(TypedValue(1), TypeFactory::GetType(TypeID::kInt));
    return new GenerateSeries(type, start, end, step);
  }

  DISALLOW_COPY_AND_ASSIGN(GenerateSeriesDescriptor);
};


/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_GENERATOR_FUNCTION_HPP_ */
