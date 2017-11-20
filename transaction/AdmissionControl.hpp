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

#ifndef QUICKSTEP_TRANSACTION_ADMISSION_CONTROL_HPP_
#define QUICKSTEP_TRANSACTION_ADMISSION_CONTROL_HPP_

#include "utility/Macros.hpp"
#include "transaction/TransactionTable.hpp"

namespace quickstep {
namespace transaction {

class AdmissionControl {
 public:
  /**
   * @brief Constructor
   * @param transaction_table A lookup table that stores information about
   *        all the running and waiting transactions.
   */
  AdmissionControl(TransactionTable *transaction_table) {}

  virtual ~AdmissionControl() {}

  /**
   * @brief Admit a transaction to the system.
   *
   * @note Check the transaction's compatibility with the other running
   *       transactions. If it is compatible, let it run, otherwise put
   *       the transaction in the waiting list.
   *
   * @note Accesses to the transaction_table may need protection.
   *
   * @param tid The ID of the given transaction.
   * @param resource_requests A vector of pairs such that each pair has a
   *        resource ID and its requested access mode.
   * @return True if the transaction can be admitted, false if it has to wait.
   */
  virtual bool admitTransaction(const transaction_id tid,
                                const std::vector<std::pair<ResourceId, AccessMode>> &resource_requests) {
    return false;
  }

  /**
   * @brief Attempt to admit a waiting transaction.
   *
   * @note Check the transaction's compatibility with the other running
   *       transactions. If it is compatible, let it run, otherwise put
   *       the transaction in the waiting list.
   *
   * @note Accesses to the transaction_table may need protection.
   *
   * @param tid The ID of the given transaction.
   * @return True if the transaction can be admitted, false if the
   *         transaction has to wait.
   */
  virtual bool admitWaitingTransaction(const transaction_id tid) {
    return false;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(AdmissionControl);
};

}  // namespace transaction
}  // namespace quickstep

#endif  //QUICKSTEP_TRANSACTION_ADMISSION_CONTROL_HPP_
