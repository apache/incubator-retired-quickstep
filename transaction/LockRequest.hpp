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

#ifndef QUICKSTEP_TRANSACTION_LOCK_REQUEST_HPP_
#define QUICKSTEP_TRANSACTION_LOCK_REQUEST_HPP_

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"
#include "transaction/Transaction.hpp"

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Enum class for representing request types.
 **/
enum class RequestType {
  kAcquireLock = 0,
  kReleaseLocks,
};

/**
 * @brief Class for encapsulate lock request put into
 *        the buffers.
 **/
class LockRequest {
 public:
  /**
   * @brief Constructor for LockRequest.
   *
   * @param tid Id of the transaction that requests the lock.
   * @param rid Id of the resource that is requested.
   * @param access_mode Access mode of the request.
   * @param type Type of the request.
   */
  LockRequest(const transaction_id tid,
              const ResourceId &rid,
              const AccessMode access_mode,
              const RequestType request_type)
      : tid_(tid),
        rid_(rid),
        access_mode_(access_mode),
        request_type_(request_type) {
  }

  /**
   * @brief Getter for transaction id.
   *
   * @return Transaction id of the request.
   **/
  inline transaction_id getTransactionId() const {
    return tid_;
  }

  /**
   * @brief Getter for resource id.
   *
   * @return Resource id of the request.
   **/
  inline const ResourceId& getResourceId() const {
    return rid_;
  }

  /**
   * @brief Getter for access mode.
   *
   * @return Access mode of the request.
   **/
  inline AccessMode getAccessMode() const {
    return access_mode_;
  }

  /**
   * @brief Getter for request type.
   *
   * @return Type of the request.
   **/
  inline RequestType getRequestType() const {
    return request_type_;
  }

 private:
  transaction_id tid_;
  ResourceId rid_;
  AccessMode access_mode_;
  RequestType request_type_;
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif
