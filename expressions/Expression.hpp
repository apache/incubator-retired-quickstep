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

#ifndef QUICKSTEP_EXPRESSIONS_EXPRESSION_HPP_
#define QUICKSTEP_EXPRESSIONS_EXPRESSION_HPP_

#include "utility/Macros.hpp"
#include "utility/TreeStringSerializable.hpp"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Base class for all expressions (scalars and predicates) in the backend.
 */
class Expression : public TreeStringSerializable<const Expression*> {
 public:
  /**
   * @brief Virtual destructor.
   **/
  virtual ~Expression() {}

 protected:
  Expression() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Expression);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_EXPRESSION_HPP_
