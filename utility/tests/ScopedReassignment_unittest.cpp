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

#include "utility/ScopedReassignment.hpp"

#include "utility/Macros.hpp"

#include "gtest/gtest.h"

namespace quickstep {

namespace {
const int kOldValue = 10;
const int kNewValue = 20;
}  // namespace

/**
 * @brief A test class that is movable but not copyable.
 */
class NonCopyable {
 public:
  explicit NonCopyable(const int value)
      : value_(value) {}

  explicit NonCopyable(NonCopyable &&other)
      : value_(other.value_) {}

  NonCopyable& operator=(NonCopyable &&other) {
    value_ = other.value_;
    return *this;
  }

  int value() const {
    return value_;
  }

 private:
  int value_;

  DISALLOW_COPY_AND_ASSIGN(NonCopyable);
};

/**
 * @brief A test class that is copyable but not movable.
 */
class NonMovable {
 public:
  explicit NonMovable(const int value)
      : value_(value) {}

  explicit NonMovable(const NonMovable &other)
      : value_(other.value_) {}

  NonMovable& operator=(const NonMovable &other) {
    value_ = other.value_;
    return *this;
  }

  int value() const {
    return value_;
  }

 private:
  int value_;
};

/**
 * @brief A test class that is copyable and movable.
 */
class CopyableMovable {
 public:
  explicit CopyableMovable(const int value)
      : value_(value) {}

  explicit CopyableMovable(const CopyableMovable &other)
      : copy_constructed_(true),
        value_(other.value_) {}

  explicit CopyableMovable(CopyableMovable &&other)
      : value_(other.value_) {}

  CopyableMovable& operator=(const CopyableMovable &other) {
    value_ = other.value_;
    copy_assigned_ = true;
    return *this;
  }

  CopyableMovable& operator=(CopyableMovable &&other) {
    value_ = other.value_;
    copy_assigned_ = false;
    return *this;
  }

  int value() const {
    return value_;
  }

  bool copy_constructed() const {
    return copy_constructed_;
  }

  bool copy_assigned() const {
    return copy_assigned_;
  }

 private:
  const bool copy_constructed_ = false;
  int value_;
  bool copy_assigned_ = false;
};

TEST(ScopedReassignment, NonCopyableTest) {
  NonCopyable non_copyable(kOldValue);
  {
    NonCopyable other(kNewValue);
    ScopedReassignment<NonCopyable> reassign(&non_copyable, std::move(other));
    EXPECT_EQ(kNewValue, non_copyable.value());
  }
  EXPECT_EQ(kOldValue, non_copyable.value());

  {
    ScopedReassignment<NonCopyable> reassign(&non_copyable, NonCopyable(kNewValue));
    EXPECT_EQ(kNewValue, non_copyable.value());
  }
  EXPECT_EQ(kOldValue, non_copyable.value());
}

TEST(ScopedReassignment, NonMovableTest) {
  NonMovable non_movable(kOldValue);
  {
    NonMovable other(kNewValue);
    ScopedReassignment<NonMovable> reassign(&non_movable, other);
    EXPECT_EQ(kNewValue, non_movable.value());
  }
  EXPECT_EQ(kOldValue, non_movable.value());

  {
    ScopedReassignment<NonMovable> reassign(&non_movable, NonMovable(kNewValue));
    EXPECT_EQ(kNewValue, non_movable.value());
  }
  EXPECT_EQ(kOldValue, non_movable.value());
}

TEST(ScopedReassignment, CopyableMovableTest) {
  CopyableMovable copyable_movable(kOldValue);
  {
    CopyableMovable other(kNewValue);
    ScopedReassignment<CopyableMovable> reassign(&copyable_movable, other);
    EXPECT_EQ(kNewValue, copyable_movable.value());
    EXPECT_FALSE(reassign.old_value().copy_constructed());
    EXPECT_TRUE(copyable_movable.copy_assigned());
  }
  EXPECT_EQ(kOldValue, copyable_movable.value());
  EXPECT_FALSE(copyable_movable.copy_assigned());

  {
    CopyableMovable other(kNewValue);
    ScopedReassignment<CopyableMovable> reassign(&copyable_movable, std::move(other));
    EXPECT_EQ(kNewValue, copyable_movable.value());
    EXPECT_FALSE(reassign.old_value().copy_constructed());
    EXPECT_FALSE(copyable_movable.copy_assigned());
  }
  EXPECT_EQ(kOldValue, copyable_movable.value());
  EXPECT_FALSE(copyable_movable.copy_assigned());
}

}  // namespace quickstep
