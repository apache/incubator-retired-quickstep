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

#ifndef QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HPP_
#define QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HPP_

#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

class GeneratorFunctionInvalidArguments : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param message The error message.
   **/
  explicit GeneratorFunctionInvalidArguments(const std::string &message)
      : message_(message) {
  }

  ~GeneratorFunctionInvalidArguments() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Abstract representation of a generator function.
 **/
class GeneratorFunction {
 public:
   
  /**
   * @brief Concretize a GeneratorFunction with its constant arguments
   *
   * @param argument A list of zero or more TypedValue (in order) as
   *        arguments to this function.
   * @return The concretized GeneratorFunction, or NULL if the function cannot
   *         be concretized with the given arguments.
   **/
  virtual GeneratorFunctionHandlePtr concretize(
      const std::vector<const TypedValue> &arguments) const = 0;

 protected:
  GeneratorFunction() {
  }
 
 private:
  DISALLOW_COPY_AND_ASSIGN(GeneratorFunction);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HPP_ */
