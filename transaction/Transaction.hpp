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
#include <functional>

namespace quickstep {

namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Type that represents id of a transaction.
 **/
using transaction_id = std::uint64_t;

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
  explicit Transaction(transaction_id tid)
    : tid_(tid) {
  }

  /**
   * @brief Getter for transaction id.
   *
   * @return Transaction id of this.
   **/
  inline transaction_id getTransactionId() const {
    return tid_;
  }

  /**
   * @brief Setter for the transaction status.
   *
   * @param status New status of the transaction.
   **/
  inline void setStatus(TransactionStatus status) {
    status_ = status_;
  }

  /**
   * @brief Getter for the transaction status.
   *
   * @return Status of the transaction.
   **/
  inline TransactionStatus getStatus() const {
    return status_;
  }

  /**
   * @brief Equality operator for Transaction class.
   *
   * @param other Other transaction to be compared with.
   * @return True if other tid and this tid are equal, false otherwise.
   **/
  inline bool operator==(const Transaction &other) const {
    return tid_ == other.tid_;
  }

  /**
   * @brief Helper hasher class for transaction.
   **/
  struct TransactionHasher {
    std::size_t operator()(const Transaction &transaction) const {
      return std::hash<transaction_id>()(transaction.tid_);
    }
  };

 private:
  // Unique id for the transaction.
  transaction_id tid_;

  // Transaction's running status.
  TransactionStatus status_;
};

/** @} */

}  // namespace transaction

}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_TRANSACTION_HPP_
