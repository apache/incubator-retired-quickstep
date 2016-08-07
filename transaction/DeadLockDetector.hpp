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

#ifndef QUICKSTEP_TRANSACTION_DEAD_LOCK_DETECTOR_HPP_
#define QUICKSTEP_TRANSACTION_DEAD_LOCK_DETECTOR_HPP_

#include <atomic>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

#include "threading/Thread.hpp"
#include "transaction/DirectedGraph.hpp"
#include "transaction/Transaction.hpp"

namespace quickstep {
namespace transaction {

class LockTable;

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Notification mechanism between LockManager and DeadLockDetector.
 **/
enum class DeadLockDetectorStatus {
  kNotReady = 0,
  kDone,
  kQuit,
};

/**
 * @brief Class for deadlock detection on wait-for graph.
 **/
class DeadLockDetector : public Thread {
 public:
  typedef std::unordered_map<transaction_id, DirectedGraph::node_id>
      transaction_id_node_map;

  /**
   * @brief Constructor for DeadLockDetector.
   *
   * @param lock_table Pointer to lock table, which this class gets the
   *        necessary information.
   * @param status Pointer to status object which will act as a message
   *        passing algorithm between LockManager.
   * @param victims Message passing buffer betwen DeadLockDetector and
   *        LockManager.
   **/
  DeadLockDetector(LockTable *lock_table,
                   std::atomic<DeadLockDetectorStatus> *status,
                   std::vector<DirectedGraph::node_id> *victims);

  void run() override;

  /**
   * @brief Adds pending information based on a resource conflict.
   *
   * @param pending Id of the transaction that waits for the resource lock.
   * @param owner Id of the transaction that owns the resource lock.
   */
  void addPendingInfo(const transaction_id pending,
                      const transaction_id owner);

  /**
   * @brief Deletes pending information on a resource.
   *
   * @param pending Id of the transaction that waits for the resource lock.
   * @param owner Id of the transaction that owns the resource lock.
   *
   * @warning This method is not implemented yet.
   */
  void deletePendingInfo(const transaction_id pending,
                         const transaction_id owner);

  /**
   * @brief Check whether first transaction waits for the latter.
   *
   * @param pending Id of the transaction which will be checked whether
   *        it waits for the other.
   * @param owner Id of the transaction which will be checked whether
   *        it is waited by the first.
   *
   * @warning This method is not implemented yet.
   */
  bool isDependent(const transaction_id pending,
                   const transaction_id owner) const;

  /**
   * @brief Gives the ids of transactions that wait for the owner transaction.
   *
   * @param owner Id of the transaction whose the penders will be returned.
   * @return Vector of transaction ids that wait for owner.
   */
  std::vector<transaction_id> getAllDependents(const transaction_id owner) const;

  /**
   * @brief Gives the ids of transaction that the pending transaction waits for.
   * @warning This method is not implemented yet.

   * @param pending Id of the transaction that is pending.
   * @return Vector of transaction ids which the pending transaction waits for.
   */
  std::vector<transaction_id> getAllDependees(transaction_id pending);

  /**
   * @brief Gives the list of victims whose the terminations will end the cycle.
   *
   * @return Vector of victim transaction ids.
   */
  std::vector<transaction_id> getAllVictims();

 private:
  static constexpr std::int64_t kSleepDurationInSeconds = 5;

  DirectedGraph::node_id getNodeId(const transaction_id tid);

  DirectedGraph::node_id addNode(const transaction_id tid);

  // Owned pointer to wait-for graph.
  std::unique_ptr<DirectedGraph> wait_for_graph_;

  // Mapping from TransactioId to NodeId in graph.
  std::unique_ptr<transaction_id_node_map> tid_node_mapping_;

  // Pointer to lock table. Dependence edges will be created
  // by the information got from lock table.
  LockTable *lock_table_;

  std::atomic<DeadLockDetectorStatus> *status_;
  std::vector<DirectedGraph::node_id> *victims_;
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_DEAD_LOCK_DETECTOR_HPP_
