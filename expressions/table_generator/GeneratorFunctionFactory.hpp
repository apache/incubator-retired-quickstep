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

#ifndef QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_FACTORY_HPP_
#define QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_FACTORY_HPP_

#include <map>
#include <string>

#include "utility/Macros.hpp"

namespace quickstep {

class GeneratorFunction;
class GeneratorFunctionHandle;

namespace serialization { class GeneratorFunctionHandle; }

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Singleton factory class that provides access to the various concrete
 *        implementations of GeneratorFunction.
 *
 * @note Generator functions are used for generating tables. A generator
 *       function takes a list of constant arguments at query compile time,
 *       and populates a table at query run time. See the documentation in
 *       GeneratorFunction.hpp for a detailed description and for how to
 *       implement a new generator function.
 **/
class GeneratorFunctionFactory {
 public:
  /**
   * @brief Singleton instance of the GeneratorFunctionFactory class.
   * @return A const reference to the singleton instance of the
   *         GeneratorFunctionFactory class.
   */
  static const GeneratorFunctionFactory& Instance();

  /**
   * @brief Get a particular GeneratorFunction by its name.
   *
   * @param name The name of the desired GeneratorFunction.
   * @return A pointer to the GeneratorFunction specified by name, or NULL if
   *         name does not match any known GeneratorFunction.
   **/
  const GeneratorFunction* getByName(const std::string &name) const;

  /**
   * @brief Reconstruct a particular GeneratorFunctionHandle by its protobuf
   *        message.
   *
   * @param proto A serialized protocol buffer representation of the
   *        GeneratorFunctionHandle.
   * @return A new GeneratorFunctionHandle object constructed from the protobuf
   *         message. Caller is responsible for deleting the returned object.
   */
  GeneratorFunctionHandle* reconstructFromProto(
      const serialization::GeneratorFunctionHandle &proto) const;

 private:
  /**
   * @brief Constructor. All the implemented generator functions should be
   *        registered into GeneratorFunctionFactory inside the constructor.
   */
  GeneratorFunctionFactory();

  std::map<std::string, const GeneratorFunction *> func_map_;

  DISALLOW_COPY_AND_ASSIGN(GeneratorFunctionFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_FACTORY_HPP_
