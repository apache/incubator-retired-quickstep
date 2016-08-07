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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_QUERY_PLAN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_QUERY_PLAN_HPP_

#include <unordered_map>
#include <utility>

#include "relational_operators/RelationalOperator.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief A plan to execute a query.
 **/
class QueryPlan {
 public:
  typedef DAG<RelationalOperator, bool>::size_type_nodes DAGNodeIndex;

  /**
   * @brief Constructor.
   */
  QueryPlan() {}

  /**
   * @brief Create a relational operator node in the plan, and set its node
   *        index.
   *
   * @param relational_operator The relational operator to be added to the plan.
   * @return The node index in the exeuciton plan.
   */
  inline DAGNodeIndex addRelationalOperator(RelationalOperator *relational_operator) {
    const DAGNodeIndex node_index = dag_operators_.createNode(relational_operator);
    relational_operator->setOperatorIndex(node_index);
    return node_index;
  }

  /**
   * @brief Creates a link from \p producer_operator_index to \p consumer_operator_index
   *        in the DAG.
   *
   * @param consumer_operator_index The index of the consumer operator.
   * @param producer_operator_index The index of the producer operator.
   * @param is_pipeline_breaker True if the result from the producer cannot be
   *                            pipelined to the consumer, otherwise false.
   */
  inline void addDirectDependency(DAGNodeIndex consumer_operator_index,
                                  DAGNodeIndex producer_operator_index,
                                  bool is_pipeline_breaker) {
    dag_operators_.createLink(producer_operator_index, consumer_operator_index, is_pipeline_breaker);
  }

  /**
   * @brief Creates dependencies for a DropTable operator with index
   *        \p drop_operator_index. If \p producer_operator_index
   *        has any dependent, creates a link from \p drop_operator_index
   *        to each dependent of \p producer_operator_index in the DAG;
   *        otherwise, create a direct link between \p drop_operator_index
   *        and \p producer_operator_index.
   *
   * @param dependent_operator_index The index of the DropTable operator node.
   * @param dependency_operator_index The index of the operator node that outputs
   *                                  the relation to be dropped by the operator
   *                                  at \p dependent_operator_index.
   */
  inline void addDependenciesForDropOperator(DAGNodeIndex drop_operator_index,
                                             DAGNodeIndex producer_operator_index) {
    const std::unordered_map<DAGNodeIndex, bool> &direct_dependents =
        dag_operators_.getDependents(producer_operator_index);
    if (!direct_dependents.empty()) {
      for (const std::pair<const DAGNodeIndex, bool> &direct_dependent : direct_dependents) {
        dag_operators_.createLink(direct_dependent.first,
                                  drop_operator_index,
                                  true /* is_pipeline_breaker */);
      }
    } else {
      dag_operators_.createLink(producer_operator_index,
                                drop_operator_index,
                                true /* is_pipeline_breaker */);
    }
  }

  /**
   * @return The DAG query plan.
   */
  const DAG<RelationalOperator, bool>& getQueryPlanDAG() const {
    return dag_operators_;
  }

  /**
   * @return The DAG query plan.
   */
  DAG<RelationalOperator, bool>* getQueryPlanDAGMutable() {
    return &dag_operators_;
  }

 private:
  // A false value of the second template argument indicates that the
  // link between two RelationalOperators is not a pipeline breaker.
  // while a true value indicates that the link is a pipeline
  // breaker. Streamed data won't move through pipeline breaker links.
  DAG<RelationalOperator, bool> dag_operators_;

  DISALLOW_COPY_AND_ASSIGN(QueryPlan);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_PLAN_HPP_
