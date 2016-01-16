// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "tmb/internal/zookeeper_transaction_batch.h"

#include <cassert>
#include <string>
#include <utility>
#include <vector>

#include "zookeeper/zookeeper.h"

namespace tmb {
namespace internal {

void ZookeeperTransactionBatch::CreateEmptyNode(const std::string &path,
                                                const struct ACL_vector *acl,
                                                int flags) {
  strings_.push_back(path);
  ops_.push_back(zoo_op());

  zoo_create_op_init(&(ops_.back()),
                     strings_.back().c_str(),
                     nullptr,
                     -1,
                     acl,
                     flags,
                     nullptr,
                     0);

  delete_only_ = false;
}

void ZookeeperTransactionBatch::CreateEmptyNode(
    std::string &&path,  // NOLINT(whitespace/operators)
    const struct ACL_vector *acl,
    int flags) {
  strings_.push_back(std::move(path));
  ops_.push_back(zoo_op());

  zoo_create_op_init(&(ops_.back()),
                     strings_.back().c_str(),
                     nullptr,
                     -1,
                     acl,
                     flags,
                     nullptr,
                     0);

  delete_only_ = false;
}

void ZookeeperTransactionBatch::CreateNodeWithExternalPayload(
    const std::string &path,
    const void *data,
    const int data_size,
    const struct ACL_vector *acl,
    int flags) {
  strings_.push_back(path);
  ops_.push_back(zoo_op());

  zoo_create_op_init(&(ops_.back()),
                     strings_.back().c_str(),
                     reinterpret_cast<const char*>(data),
                     data_size,
                     acl,
                     flags,
                     nullptr,
                     0);

  delete_only_ = false;
}

void ZookeeperTransactionBatch::CreateNodeWithExternalPayload(
    std::string &&path,  // NOLINT(whitespace/operators)
    const void *data,
    const int data_size,
    const struct ACL_vector *acl,
    int flags) {
  strings_.push_back(std::move(path));
  ops_.push_back(zoo_op());

  zoo_create_op_init(&(ops_.back()),
                     strings_.back().c_str(),
                     reinterpret_cast<const char*>(data),
                     data_size,
                     acl,
                     flags,
                     nullptr,
                     0);

  delete_only_ = false;
}

void ZookeeperTransactionBatch::Delete(const std::string &path, int version) {
  strings_.push_back(path);
  ops_.push_back(zoo_op());

  zoo_delete_op_init(&(ops_.back()), strings_.back().c_str(), version);
}

void ZookeeperTransactionBatch::Delete(
    std::string &&path,  // NOLINT(whitespace/operators)
    int version) {
  strings_.push_back(std::move(path));
  ops_.push_back(zoo_op());

  zoo_delete_op_init(&(ops_.back()), strings_.back().c_str(), version);
}

void ZookeeperTransactionBatch::DeleteWithExternalPath(const std::string &path,
                                                       int version) {
  ops_.push_back(zoo_op());
  zoo_delete_op_init(&(ops_.back()), path.c_str(), version);
}

void ZookeeperTransactionBatch::Set(const std::string &path,
                                    const void *data,
                                    const int data_size,
                                    int version) {
  strings_.push_back(path);
  ops_.push_back(zoo_op());

  zoo_set_op_init(&(ops_.back()),
                  strings_.back().c_str(),
                  reinterpret_cast<const char*>(data),
                  data_size,
                  version,
                  nullptr);

  delete_only_ = false;
}

void ZookeeperTransactionBatch::Set(
    std::string &&path,  // NOLINT(whitespace/operators)
    const void *data,
    const int data_size,
    int version) {
  strings_.push_back(std::move(path));
  ops_.push_back(zoo_op());

  zoo_set_op_init(&(ops_.back()),
                  strings_.back().c_str(),
                  reinterpret_cast<const char*>(data),
                  data_size,
                  version,
                  nullptr);

  delete_only_ = false;
}

int ZookeeperTransactionBatch::Commit(zhandle_t *zookeeper_handle) {
  if (ops_.empty()) {
    return ZOK;
  }

  results_.resize(ops_.size());

  return zoo_multi(zookeeper_handle,
                   ops_.size(),
                   ops_.data(),
                   results_.data());
}

void ZookeeperTransactionBatch::RemoveNonexistentNodesFromDeleteBatch(
    zhandle_t *zookeeper_handle) {
  assert(delete_only_);

  std::vector<zoo_op> verified_ops;
  struct Stat stat_buffer;
  for (const zoo_op &delete_op : ops_) {
    int rc = zoo_exists(zookeeper_handle,
                        delete_op.delete_op.path,
                        0,
                        &stat_buffer);
    if (rc == ZOK) {
      verified_ops.push_back(delete_op);
    } else {
      assert(rc == ZNONODE);
    }
  }

  ops_.swap(verified_ops);
}

}  // namespace internal
}  // namespace tmb
