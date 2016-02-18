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

#ifndef QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_FACTORY_HPP_
#define QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_FACTORY_HPP_

#include <string>

#include "expressions/table_generator/GeneratorFunction.hpp"
#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief All-static factory class that provides access to the various concrete
 *        implementations of GeneratorFunction.
 **/
class GeneratorFunctionFactory {
 public:
  /**
   * @brief Get a particular GeneratorFunction by its name.
   *
   * @param name The name of the desired GeneratorFunction.
   * @return A pointer to the GeneratorFunction specified by name, or NULL if
   *         name does not match any known GeneratorFunction.
   **/
  static const GeneratorFunction *GetByName(const std::string &name);
 
  /**
   * @brief Reconstruct a particular GeneratorFunctionHandle by its proto.
   *
   * @param proto A serialized protocol buffer representation of the
   *        GeneratorFunctionHandle.
   */
  static GeneratorFunctionHandlePtr ReconstructFromProto(
      const serialization::GeneratorFunctionHandle &proto);

 private:
  // Class is all-static and can not be instantiated.
  GeneratorFunctionFactory();

  DISALLOW_COPY_AND_ASSIGN(GeneratorFunctionFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_FACTORY_HPP_
