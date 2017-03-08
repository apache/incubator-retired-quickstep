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

#ifndef QUICKSTEP_QUERY_EXECUTION_PIPELINE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_PIPELINE_HPP_

#include <algorithm>
#include <cstddef>
#include <vector>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A class that abstracts a pipeline of relational operators in a query
 *        plan DAG.
 **/
class Pipeline {
 public:
  /**
   * @brief Constructor.
   *
   * @param operator_ids The IDs of the operator belonging to the pipeline.
   **/
  explicit Pipeline(const std::vector<std::size_t> &operator_ids)
      : operators_(operator_ids) {}

  /**
   * @brief Constructor for a single node pipeline.
   *
   * @param operator_id The ID of the operator belonging to the pipeline.
   **/
  explicit Pipeline(const std::size_t operator_id) {
    operators_.emplace_back(operator_id);
  }

  /**
   * @brief Get the IDs of the operators belonging to the pipeline.
   **/
  const std::vector<std::size_t>& getOperatorIDs() const {
    return operators_;
  }

  /**
   * @brief Add an operator to the pipeline.
   *
   * @param operator_id The ID of the operator.
   **/
  void addOperatorToPipeline(const std::size_t operator_id) {
    DCHECK(!hasOperator(operator_id));
    operators_.emplace_back(operator_id);
  }

  /**
   * @brief Check if the given operator belongs to the pipeline.
   **/
  bool hasOperator(const std::size_t operator_id) const {
    return std::find(operators_.begin(), operators_.end(), operator_id) !=
           operators_.end();
  }

  /**
   * @brief Get the size of the pipeline.
   **/
  std::size_t size() const {
    return operators_.size();
  }

 private:
  std::vector<std::size_t> operators_;

  DISALLOW_COPY_AND_ASSIGN(Pipeline);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_PIPELINE_HPP_
