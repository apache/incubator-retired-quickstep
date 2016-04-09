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

#ifndef QUICKSTEP_QUERY_EXECUTION_ADMIT_REQUEST_MESSAGE_HPP_
#define QUICKSTEP_QUERY_EXECUTION_ADMIT_REQUEST_MESSAGE_HPP_

#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

class QueryHandle;

/** \addtogroup QueryExecution
 *  @{
 */

/**
 * @brief A message requesting a query or queries to be admitted to the system.
 **/
class AdmitRequestMessage {
 public:
  /**
   * @brief Constructor.
   *
   * @param query_handles The handles of the queries requesting to be admitted
   *        to the system.
   **/
  explicit AdmitRequestMessage(const std::vector<QueryHandle*> &query_handles)
      : query_handles_(query_handles) {}

  /**
   * @brief Constructor for requesting single query admission.
   *
   * @param query_handle The handle of the query requesting to be admitted.
   **/
  explicit AdmitRequestMessage(QueryHandle *query_handle) {
    query_handles_.push_back(query_handle);
  }

  /**
   * @brief Get the query handles from this message.
   **/
  const std::vector<QueryHandle*>& getQueryHandles() const {
    return query_handles_;
  }

 private:
  std::vector<QueryHandle*> query_handles_;

  DISALLOW_COPY_AND_ASSIGN(AdmitRequestMessage);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_ADMIT_REQUEST_MESSAGE_HPP_
