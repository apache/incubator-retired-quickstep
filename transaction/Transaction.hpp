/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_TRANSACTION_TRANSACTION_HPP_
#define QUICKSTEP_TRANSACTION_TRANSACTION_HPP_

#include <cstdint>

namespace quickstep {

namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Type that represents id of a transaction.
 **/
using TransactionId = std::uint64_t;

/**
 * @brief Enum class for transaction internal status.
 **/
enum class TransactionStatus {
  kRUNNING = 0,
  kABORTED,
  kCOMMITTED
};

/**
 * @brief Transaction
 **/
class Transaction {
 public:
  /**
   * @brief Constructor for Transaction.
   *
   * @param tid Id of this transaction.
   **/
  explicit Transaction(TransactionId tid)
    : tid_(tid) {
  }

  /**
   * @brief Getter for transaction id.
   *
   * @return Transaction id of this.
   **/
  TransactionId getTransactionId() const;

  /**
   * @brief Setter for transaction status.
   *
   * @param status New status of the transaction.
   **/
  void setStatus(TransactionStatus status);

  TransactionStatus getStatus() const;

  /**
   * @brief Equality operator for Transaction class.
   *
   * @param other Other transaction to be compared with.
   * @return True if other tid and this tid are equal, false otherwise.
   **/
  bool operator==(const Transaction &other) const;

  /**
   * @brief Helper hasher class for transaction.
   *
   **/
  struct TransactionHasher {
    std::size_t operator()(const Transaction &transaction) const;
  };

 private:
  TransactionId tid_;
  TransactionStatus status_;
  // std::unique_ptr<QueryPlan> query_plan_;
  // std::unique_ptr<TransactionThread> thread_;
};

/** @} */

}  // namespace transaction

}  // namespace quickstep

#endif
