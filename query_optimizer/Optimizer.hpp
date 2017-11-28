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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_HPP_

#include "query_optimizer/PhysicalGenerator.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
class ParseStatement;
class QueryHandle;

namespace optimizer {

class OptimizerContext;

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Main class of the optimizer that creates and optimizes all types of query
 *        plans.
 */
class Optimizer {
 public:
  /**
   * @brief Constructor.
   */
  Optimizer() {}

  /**
   * @brief Destructor.
   */
  ~Optimizer() {}

  /**
   * @brief Fills an execution plan as a list of operators,
   *        and the query result relation in
   *        \p query_handle for \p parse_statement.
   * @note Does not take ownership of \p query_handle.
   *
   * @param parse_statement The parse tree of the input query.
   * @param catalog_database The database that the query is executed on.
   * @param optimizer_context The optimizer context of the input query.
   * @param query_handle The generated query handle to output.
   */
  void generateQueryHandle(const ParseStatement &parse_statement,
                           CatalogDatabase *catalog_database,
                           OptimizerContext *optimizer_context,
                           QueryHandle *query_handle);

 private:
  DISALLOW_COPY_AND_ASSIGN(Optimizer);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_HPP_ */
