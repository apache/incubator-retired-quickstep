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

#ifndef QUICKSTEP_TRANSACTION_TRANSACTION_COMPATIBILITY_CHECKER_HPP_
#define QUICKSTEP_TRANSACTION_TRANSACTION_COMPATIBILITY_CHECKER_HPP_

#include <utility>
#include <vector>

#include "transaction/TransactionTable.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelation;

namespace transaction {

/**
 * @brief A class that checks the compatibility of a transaction with the
 *        other running transactions.
 */
class CompatibilityChecker {
 public:
  /**
   * @brief Constructor
   * @param transaction_table A lookup table that stores information about
   *        all the running and waiting transactions.
   */
  CompatibilityChecker(TransactionTable *transaction_table) {}

  virtual ~CompatibilityChecker() {}

  /**
   * @brief Check if the given transaction is compatible with other
   *        running transactions.
   * @note The transaction table has the list of all the running or pending
   *       transactions. Lookup the table and check if the given transaction
   *       is compatible, based on the CC protocol specifications.
   * @note Accesses to the transaction_table may need protection.

   * @param tid The ID of the given transaction
   * @param resource_requests A vector of pairs such that each pair has a
   *        resource ID and its requested access mode.
   * @return True if the transaction is compatible and false if it is not.
   */
  virtual bool isTranasctionCompatible(const transaction_id tid,
                                       const std::vector<std::pair<ResourceId, AccessMode>> &resource_requests) {
    return false;
  }

  /**
   * @brief Check if the given transaction can acquire a Catalog lock on the
   *        given relation.
   * @param tid The ID of the given transaction.
   * @param relation The given relation.
   * @param access_mode The access_mode requested to lock the catalog.
   * @return True if the lock can be acquired, false otherwise.
   */
  virtual bool canAcquireCatalogLock(const transaction_id tid,
                                     const CatalogRelation &relation,
                                     const AccessMode access_mode) {
    return false;
  }

  /**
   * @brief Check if the given transaction can acquire locks on all the blocks
   *        of the given relation.
   * @param tid The ID of the given transaction.
   * @param rid The ID of the given relation.
   * @param blocks The list of the blocks of the given relation.
   * @param access_mode The access_mode requested to lock the blocks.
   * @return True if the lock can be acquired, false otherwise.
   */
  virtual bool canAcquireBlockLocks(const transaction_id tid,
                                    const relation_id rid,
                                    const std::vector<block_id> &blocks,
                                    const AccessMode access_mode) {
    return false;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(CompatibilityChecker);

};

}  // namespace transaction
}  // namespace quickstep

#endif  //QUICKSTEP_TRANSACTION_TRANSACTION_COMPATIBILITY_CHECKER_HPP_