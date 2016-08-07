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

#include <chrono>
#include <thread>  // NOLINT(build/c++11)

#include "gtest/gtest.h"

#include "threading/Mutex.hpp"
#include "threading/SpinMutex.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

template <typename MutexT>
struct SharedDataItem {
  typedef MutexLockImpl<MutexT, true> LockT;

  int payload;
  MutexT mutex;
};

struct SharedDataItemRecursive {
  typedef RecursiveMutexLock LockT;

  int payload;
  RecursiveMutex mutex;
};

template <class ItemT>
class ModifierThread : public Thread {
 public:
  ModifierThread(ItemT *item, const int set_value, const bool lock_recursively)
      : item_(item),
        set_value_(set_value),
        lock_recursively_(lock_recursively) {
  }

 protected:
  void run() override {
    typename ItemT::LockT lock(item_->mutex);
    if (lock_recursively_) {
      item_->mutex.lock();
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    item_->payload = set_value_;
    if (lock_recursively_) {
      item_->mutex.unlock();
    }
  }

 private:
  ItemT *item_;
  const int set_value_;
  const bool lock_recursively_;

  DISALLOW_COPY_AND_ASSIGN(ModifierThread);
};

template <class ItemT>
class AccessorThread : public Thread {
 public:
  AccessorThread(ItemT *item, const bool lock_recursively)
      : item_(item),
        lock_recursively_(lock_recursively) {
  }

  int getReadValue() const {
    return read_value_;
  }

 protected:
  void run() override {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    typename ItemT::LockT lock(item_->mutex);
    if (lock_recursively_) {
      item_->mutex.lock();
    }
    read_value_ = item_->payload;
    if (lock_recursively_) {
      item_->mutex.unlock();
    }
  }

 private:
  ItemT *item_;
  int read_value_;
  const bool lock_recursively_;

  DISALLOW_COPY_AND_ASSIGN(AccessorThread);
};

TEST(MutexTest, MutexBasicTest) {
  SharedDataItem<Mutex> item;
  item.payload = 0;

  ModifierThread<SharedDataItem<Mutex>> modifier(&item, 42, false);
  AccessorThread<SharedDataItem<Mutex>> accessor_a(&item, false);
  AccessorThread<SharedDataItem<Mutex>> accessor_b(&item, false);

  modifier.start();
  accessor_a.start();
  accessor_b.start();

  modifier.join();
  accessor_a.join();
  accessor_b.join();

  EXPECT_EQ(42, accessor_a.getReadValue());
  EXPECT_EQ(42, accessor_b.getReadValue());
}

TEST(MutexTest, RecursiveMutexBasicTest) {
  SharedDataItemRecursive item;
  item.payload = 0;

  ModifierThread<SharedDataItemRecursive> modifier(&item, 42, true);
  AccessorThread<SharedDataItemRecursive> accessor_a(&item, true);
  AccessorThread<SharedDataItemRecursive> accessor_b(&item, true);

  modifier.start();
  accessor_a.start();
  accessor_b.start();

  modifier.join();
  accessor_a.join();
  accessor_b.join();

  EXPECT_EQ(42, accessor_a.getReadValue());
  EXPECT_EQ(42, accessor_b.getReadValue());
}

TEST(MutexTest, SpinMutexBasicTest) {
  SharedDataItem<SpinMutex> item;
  item.payload = 0;

  ModifierThread<SharedDataItem<SpinMutex>> modifier(&item, 42, false);
  AccessorThread<SharedDataItem<SpinMutex>> accessor_a(&item, false);
  AccessorThread<SharedDataItem<SpinMutex>> accessor_b(&item, false);

  modifier.start();
  accessor_a.start();
  accessor_b.start();

  modifier.join();
  accessor_a.join();
  accessor_b.join();

  EXPECT_EQ(42, accessor_a.getReadValue());
  EXPECT_EQ(42, accessor_b.getReadValue());
}

}  // namespace quickstep
