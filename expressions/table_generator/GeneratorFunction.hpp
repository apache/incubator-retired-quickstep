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

#include <string>
#include <vector>

#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Exception thrown for invalid arguments to a generator function.
 **/
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
 * @brief A class representing a particular generator function in the abstract
 *        sense.
 *
 * Generator functions are used for generating tables. A generator function
 * takes a list of constant arguments at query compile time, and populates the
 * given ColumnVectorValueAccesor at query run time.
 *
 * The GeneratorFunction class provides informational methods about the
 * applicability of a particular generator function to particular constant
 * arguments. The actual implementation of the generator functions' logic is in
 * the GeneratorFunctionHandle class hierarchy, and can be different depending
 * on the particular arguments given to the function. At query compile time,
 * a caller should first call GeneratorFunction::generateHandle() to instantiate
 * an GeneratorFunctionHandle object. The handle object provides information
 * about the concrete function, e.g. the number and types of the output columns
 * Then, at query run time, the backend also uses the handle object's methods
 * to actually generate a table.
 *
 * To add a new generator function, subclass both GeneratorFunction and
 * GeneratorFunctionHandle to implement the logics. Also modify
 * GeneratorFunctionFactory::GetByName() to make the new function registered.
 * 
 **/
class GeneratorFunction {
 public:
  /**
   * @brief Get the name of this generator function. The name should be unique
   * as it is used to register this function into the generator function pool.
   *
   * @return The name of this generator function.
   **/
  virtual std::string getName() const = 0;

  /**
   * @brief Create an GeneratorFunctionHandle.
   *
   * @param arguments A list of zero or more constant arguments to this
   *        generator funciton.
   * @exception GeneratorFunctionInvalidArguments The arguments to this
                generator function are invalid.
   * @return Reference to a GeneratorFunctionHandle object that is used to do
   *         the actual table generation.
   **/
  virtual GeneratorFunctionHandlePtr createHandle(
      const std::vector<TypedValue> &arguments) const = 0;

 protected:
  GeneratorFunction() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(GeneratorFunction);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HPP_ */
