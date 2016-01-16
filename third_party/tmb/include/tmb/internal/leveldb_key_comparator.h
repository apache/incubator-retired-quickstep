// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_LEVELDB_KEY_COMPARATOR_H_
#define TMB_INTERNAL_LEVELDB_KEY_COMPARATOR_H_

#include <string>

#include "leveldb/comparator.h"

namespace tmb {
namespace internal {

/**
 * @brief A comparator which orders keys according to their actual semantic
 *        structure (see tmb/internal/leveldb_keys.h).
 **/
class LevelDBKeyComparator : public leveldb::Comparator {
 public:
  LevelDBKeyComparator() {
  }

  ~LevelDBKeyComparator() {
  }

  int Compare(const leveldb::Slice &a, const leveldb::Slice &b) const;

  const char* Name() const {
    return "tmb.keycomp0000";
  }

  void FindShortestSeparator(std::string *start,
                             const leveldb::Slice &limit) const {
  }

  void FindShortSuccessor(std::string *key) const {
  }
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_LEVELDB_KEY_COMPARATOR_H_
