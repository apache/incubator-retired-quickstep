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

#include "gtest/gtest.h"
#include "utility/ScopedDeleter.hpp"

namespace quickstep {

namespace {

// Various dummy classes that set an external bool to let us know when a
// destructor is invoked.
class Base {
 public:
  explicit Base(bool *base_deleted)
      : base_deleted_(base_deleted) {
  }

  virtual ~Base() {
    *base_deleted_ = true;
  }

 private:
  bool *base_deleted_;
};

class DerivedA : public Base {
 public:
  DerivedA(bool *base_deleted, bool *derived_a_deleted)
      : Base(base_deleted),
        derived_a_deleted_(derived_a_deleted) {
  }

  virtual ~DerivedA() {
    *derived_a_deleted_ = true;
  }

 private:
  bool *derived_a_deleted_;
};

class DerivedB : public Base {
 public:
  DerivedB(bool *base_deleted, bool *derived_b_deleted)
      : Base(base_deleted),
        derived_b_deleted_(derived_b_deleted) {
  }

  virtual ~DerivedB() {
    *derived_b_deleted_ = true;
  }

 private:
  bool *derived_b_deleted_;
};

class Other {
 public:
  explicit Other(bool *other_deleted)
      : other_deleted_(other_deleted) {
  }

  ~Other() {
    *other_deleted_ = true;
  }

 private:
  bool *other_deleted_;
};

}  // namespace

TEST(ScopedDeleterTest, DeletionTest) {
  // Setup flags that record whether objects are deleted.
  bool base_1_deleted, base_2_deleted,
       derived_a_1_base_deleted, derived_a_1_derived_deleted,
       derived_a_2_base_deleted, derived_a_2_derived_deleted,
       derived_b_1_base_deleted, derived_b_1_derived_deleted,
       derived_b_2_base_deleted, derived_b_2_derived_deleted,
       other_1_deleted, other_2_deleted;
  base_1_deleted = base_2_deleted
      = derived_a_1_base_deleted = derived_a_1_derived_deleted
      = derived_a_2_base_deleted = derived_a_2_derived_deleted
      = derived_b_1_base_deleted = derived_b_1_derived_deleted
      = derived_b_2_base_deleted = derived_b_2_derived_deleted
      = other_1_deleted = other_2_deleted
      = false;

  // Create objects and throw them into the ScopedDeleter.
  ScopedDeleter deleter;

  deleter.addObject(new Base(&base_1_deleted));
  deleter.addObject(new Base(&base_2_deleted));

  // Add one instance of DerivedA as itself, and one as a Base class pointer.
  deleter.addObject(new DerivedA(&derived_a_1_base_deleted,
                                 &derived_a_1_derived_deleted));
  Base *derived_a_2 = new DerivedA(&derived_a_2_base_deleted,
                                   &derived_a_2_derived_deleted);
  deleter.addObject(derived_a_2);

  // Do the same for the other derived class.
  deleter.addObject(new DerivedB(&derived_b_1_base_deleted,
                                 &derived_b_1_derived_deleted));
  Base *derived_b_2 = new DerivedB(&derived_b_2_base_deleted,
                                   &derived_b_2_derived_deleted);
  deleter.addObject(derived_b_2);

  // Also throw in a couple of objects that aren't part of the same inheritance
  // heirarchy.
  deleter.addObject(new Other(&other_1_deleted));
  deleter.addObject(new Other(&other_2_deleted));

  // Add some heap-allocated built-in types (we don't directly check whether
  // these are deleted, but running under valgrind should report no leaks).
  deleter.addObject(new int);
  deleter.addObject(new float);

  // Have the deleter delete everything and verify that destructors were
  // invoked.
  deleter.deleteAll();

  EXPECT_TRUE(base_1_deleted);
  EXPECT_TRUE(base_2_deleted);
  EXPECT_TRUE(derived_a_1_base_deleted);
  EXPECT_TRUE(derived_a_1_derived_deleted);
  EXPECT_TRUE(derived_a_2_base_deleted);
  EXPECT_TRUE(derived_a_2_derived_deleted);
  EXPECT_TRUE(derived_b_1_base_deleted);
  EXPECT_TRUE(derived_b_1_derived_deleted);
  EXPECT_TRUE(derived_b_2_base_deleted);
  EXPECT_TRUE(derived_b_2_derived_deleted);
  EXPECT_TRUE(other_1_deleted);
  EXPECT_TRUE(other_2_deleted);

  // Should not cause double-frees:
  deleter.deleteAll();
}

}  // namespace quickstep
