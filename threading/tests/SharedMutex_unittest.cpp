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

#include <atomic>
#include <chrono>
#include <thread>  // NOLINT(build/c++11)

#include "gtest/gtest.h"

#include "threading/Mutex.hpp"
#include "threading/SharedMutex.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "threading/Thread.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

template <typename SharedMutexT>
struct SharedDataItem {
  typedef MutexLockImpl<SharedMutexT, true> ExclusiveLockT;
  typedef SharedMutexSharedLockImpl<SharedMutexT, true> SharedLockT;

  int payload;
  SharedMutexT mutex;
  std::atomic<int> active_readers;
};

template <typename ItemT>
class ModifierThread : public Thread {
 public:
  ModifierThread(ItemT *item, const int set_value)
      : item_(item),
        set_value_(set_value) {
  }

 protected:
  void run() override {
    typename ItemT::ExclusiveLockT lock(item_->mutex);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    item_->payload = set_value_;
  }

 private:
  ItemT *item_;
  const int set_value_;

  DISALLOW_COPY_AND_ASSIGN(ModifierThread);
};

template <typename ItemT>
class AccessorThread : public Thread {
 public:
  explicit AccessorThread(ItemT *item)
      : item_(item) {
  }

  int getReadValue() const {
    return read_value_;
  }

  bool sawOtherThreadAfterLocking() const {
    return other_threads_seen_after_lock_ > 0;
  }

 protected:
  void run() override {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    typename ItemT::SharedLockT lock(item_->mutex);
    other_threads_seen_after_lock_ = item_->active_readers.fetch_add(1);

    read_value_ = item_->payload;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    item_->active_readers.fetch_sub(1);
  }

 private:
  ItemT *item_;
  int read_value_;
  int other_threads_seen_after_lock_;

  DISALLOW_COPY_AND_ASSIGN(AccessorThread);
};

TEST(MutexTest, SharedMutexBasicTest) {
  SharedDataItem<SharedMutex> item;
  item.payload = 0;
  item.active_readers.store(0);

  ModifierThread<SharedDataItem<SharedMutex>> modifier(&item, 42);
  AccessorThread<SharedDataItem<SharedMutex>> accessor_a(&item);
  AccessorThread<SharedDataItem<SharedMutex>> accessor_b(&item);

  modifier.start();
  accessor_a.start();
  accessor_b.start();

  modifier.join();
  accessor_a.join();
  accessor_b.join();

  EXPECT_EQ(42, accessor_a.getReadValue());
  EXPECT_EQ(42, accessor_b.getReadValue());
  EXPECT_TRUE(accessor_a.sawOtherThreadAfterLocking()
              || accessor_b.sawOtherThreadAfterLocking());
}

TEST(MutexTest, SpinSharedMutexBasicTest) {
  SharedDataItem<SpinSharedMutex<false>> item;
  item.payload = 0;
  item.active_readers.store(0);

  ModifierThread<SharedDataItem<SpinSharedMutex<false>>> modifier(&item, 42);
  AccessorThread<SharedDataItem<SpinSharedMutex<false>>> accessor_a(&item);
  AccessorThread<SharedDataItem<SpinSharedMutex<false>>> accessor_b(&item);

  modifier.start();
  accessor_a.start();
  accessor_b.start();

  modifier.join();
  accessor_a.join();
  accessor_b.join();

  EXPECT_EQ(42, accessor_a.getReadValue());
  EXPECT_EQ(42, accessor_b.getReadValue());
  EXPECT_TRUE(accessor_a.sawOtherThreadAfterLocking()
              || accessor_b.sawOtherThreadAfterLocking());
}

TEST(MutexTest, SpinSharedMutexWithYieldBasicTest) {
  SharedDataItem<SpinSharedMutex<true>> item;
  item.payload = 0;
  item.active_readers.store(0);

  ModifierThread<SharedDataItem<SpinSharedMutex<true>>> modifier(&item, 42);
  AccessorThread<SharedDataItem<SpinSharedMutex<true>>> accessor_a(&item);
  AccessorThread<SharedDataItem<SpinSharedMutex<true>>> accessor_b(&item);

  modifier.start();
  accessor_a.start();
  accessor_b.start();

  modifier.join();
  accessor_a.join();
  accessor_b.join();

  EXPECT_EQ(42, accessor_a.getReadValue());
  EXPECT_EQ(42, accessor_b.getReadValue());
  EXPECT_TRUE(accessor_a.sawOtherThreadAfterLocking()
              || accessor_b.sawOtherThreadAfterLocking());
}

}  // namespace quickstep
