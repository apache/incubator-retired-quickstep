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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_RULES_RULE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_RULES_RULE_HPP_

#include <memory>
#include <string>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {

/** \addtogroup OptimizerRules
 *  @{
 */

#define LOG_APPLYING_RULE(input, output)                                   \
  do {                                                                     \
    DVLOG(6) << "Applied rule " << this->getName() << " on "               \
             << input->getShortString() << "\n" << output->toString();     \
  } while (false)

#define LOG_IGNORING_RULE(input)                                           \
  do {                                                                     \
    DVLOG(6) << "Ignored rule " << this->getName() << " on "               \
             << input->getShortString() << "\n" << input->toString();      \
  } while (false)

/**
 * @brief Base class for rules.
 *        A rule may be used to transform a tree or validate a tree without doing
 *        any transformation.
 */
template <class TreeType>
class Rule {
 public:
  /**
   * @brief Empty constructor.
   */
  Rule() {}

  /**
   * @brief Virtual destructor.
   */
  virtual ~Rule() {}

  /**
   * @brief Gets the rule name.
   *
   * @return The name of the rule.
   */
  virtual std::string getName() const = 0;

  /**
   * @brief Applies the rule to \p input
   *
   * @param input The input tree.
   * @return The output tree after the rule is applied.
   */
  virtual std::shared_ptr<const TreeType> apply(const std::shared_ptr<const TreeType> &input) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(Rule);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_RULES_RULE_HPP_ */
