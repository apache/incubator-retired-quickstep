/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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

#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class GeneratorFunctionHandle;

/** \addtogroup Expressions
 *  @{
 */

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
 * GeneratorFunctionHandle to implement the logics. Also add a line in
 * GeneratorFunctionFactory's constructor to make the new function registered.
 *
 * See GenerateSeries and GenerateSeriesHandle as an example of how to implement
 * new generator funcitons.
 **/
class GeneratorFunction {
 public:
  /**
   * @brief Get the name of this generator function. The name should be unique
   * as it is used to register this function into the generator function pool.
   *
   * @return The name of this generator function.
   **/
  virtual const std::string getName() const = 0;

  /**
   * @brief Get the usage syntax of this generator function in text form. The
   *        syntax information will be displayed when the end users ask for help
   *        on the usage of this function.
   *
   * @return The usage syntax of this generator function in text form.
   **/
  virtual const std::string getSyntax() const = 0;

  /**
   * @brief Create an GeneratorFunctionHandle.
   *
   * @param arguments A list of zero or more constant arguments to this
   *        generator funciton.
   * @exception GeneratorFunctionInvalidArguments The arguments to this
   *            generator function are invalid.
   * @return A new GeneratorFunctionHandle object that is used to do the actual
   *         table generation. Caller is responsible for deleting the returned
   *         object.
   **/
  virtual GeneratorFunctionHandle *createHandle(
      const std::vector<TypedValue> &arguments) const = 0;

 protected:
  GeneratorFunction() {
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(GeneratorFunction);
};


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

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HPP_ */
