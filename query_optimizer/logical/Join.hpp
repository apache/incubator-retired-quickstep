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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_JOIN_HPP_

#include <memory>
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class Join;
typedef std::shared_ptr<const Join> JoinPtr;

/**
 * @brief Base class for logical join operator that combines tuples from two or
 *        more relations based on join predicates.
 */
class Join : public Logical {
 public:
  /**
   * @brief Destructor.
   */
  ~Join() override {}

 protected:
  /**
   * @brief Constructor.
   */
  Join() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(Join);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_JOIN_HPP_ */
