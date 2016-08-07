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

#ifndef TMB_INTERNAL_ZOOKEEPER_TRANSACTION_BATCH_H_
#define TMB_INTERNAL_ZOOKEEPER_TRANSACTION_BATCH_H_

#include <string>
#include <vector>

#include "zookeeper/zookeeper.h"

namespace tmb {
namespace internal {

/**
 * @brief A class for managing a Zookeeper transaction, including RAII memory
 *        management.
 **/
class ZookeeperTransactionBatch {
 public:
  ZookeeperTransactionBatch()
      : delete_only_(true) {
  }

  ~ZookeeperTransactionBatch() {
  }

  inline bool Empty() const {
    return ops_.empty();
  }

  void CreateEmptyNode(const std::string &path,
                       const struct ACL_vector *acl,
                       int flags);

  void CreateEmptyNode(std::string &&path,  // NOLINT(whitespace/operators)
                       const struct ACL_vector *acl,
                       int flags);

  void CreateNodeWithExternalPayload(const std::string &path,
                                     const void *data,
                                     const int data_size,
                                     const struct ACL_vector *acl,
                                     int flags);

  void CreateNodeWithExternalPayload(
      std::string &&path,  // NOLINT(whitespace/operators)
      const void *data,
      const int data_size,
      const struct ACL_vector *acl,
      int flags);

  void Delete(const std::string &path, int version);

  void Delete(std::string &&path,  // NOLINT(whitespace/operators)
              int version);

  void DeleteWithExternalPath(const std::string &path, int version);

  void Set(const std::string &path,
           const void *data,
           const int data_size,
           int version);

  void Set(std::string &&path,  // NOLINT(whitespace/operators)
           const void *data,
           const int data_size,
           int version);

  int Commit(zhandle_t *zookeeper_handle);

  inline const std::vector<zoo_op_result>& Results() const {
    return results_;
  }

  // Only safe to call this if the only operations in this transaction are
  // deletes. Queries Zookeeper about the existence of each node to be deleted
  // and removes nodes which do not exist (i.e. were already deleted).
  void RemoveNonexistentNodesFromDeleteBatch(zhandle_t *zookeeper_handle);

 private:
  std::vector<zoo_op> ops_;
  std::vector<zoo_op_result> results_;
  std::vector<std::string> strings_;

  bool delete_only_;

  // Disallow copy and assign:
  ZookeeperTransactionBatch(const ZookeeperTransactionBatch &orig) = delete;
  ZookeeperTransactionBatch& operator=(const ZookeeperTransactionBatch &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_ZOOKEEPER_TRANSACTION_BATCH_H_
