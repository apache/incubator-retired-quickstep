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

#include "transaction/Transaction.hpp"

#include <functional>

namespace quickstep {

namespace transaction {

TransactionId Transaction::getTransactionId() const {
  return tid_;
}

void Transaction::setStatus(TransactionStatus status) {
  status_ = status;
}

TransactionStatus Transaction::getStatus() const {
  return status_;
}

bool Transaction::operator==(const Transaction &other) const {
  return tid_ == other.tid_;
}

std::size_t Transaction::TransactionHasher::operator()(const Transaction &transaction) const {
  return std::hash<TransactionId>()(transaction.tid_);
}

}  // namespace transaction

}  // namespace quickstep
