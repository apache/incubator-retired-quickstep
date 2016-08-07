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

#include <memory>
#include <string>
#include <thread>  // NOLINT(build/c++11)
#include <unordered_map>
#include <vector>

#include "gtest/gtest.h"

#include "tmb/internal/rcu.h"

namespace tmb {
namespace internal {

TEST(RCUTest, SimpleOrderingTest) {
  RCU<int> rcu(new int(0));

  // Create a reader and check that it reads the initial value properly.
  RCU<int>::ReadHandle zero_reader_a = rcu.GetReadHandle();
  EXPECT_TRUE(zero_reader_a.valid());
  EXPECT_EQ(0, *zero_reader_a);

  // Create a reader with the default constructor, then assign to it.
  RCU<int>::ReadHandle zero_reader_b;
  zero_reader_b = rcu.GetReadHandle();
  EXPECT_TRUE(zero_reader_b.valid());
  EXPECT_EQ(0, *zero_reader_b);

  // Create a writer and increment the value in the writable copy.
  RCU<int>::WriteHandle increment_writer = rcu.GetWriteHandle();
  EXPECT_TRUE(increment_writer.valid());
  EXPECT_EQ(0, *increment_writer);
  ++(*increment_writer);
  EXPECT_EQ(1, *increment_writer);

  // Readers should still refer to the original snapshot.
  EXPECT_EQ(0, *zero_reader_a);
  EXPECT_EQ(0, *zero_reader_b);

  // Commit changes. Readers should still see their original snapshot.
  increment_writer.Commit();
  EXPECT_EQ(0, *zero_reader_a);
  EXPECT_EQ(0, *zero_reader_b);

  // A new reader should see the most recent committed version.
  RCU<int>::ReadHandle one_reader_a = rcu.GetReadHandle();
  EXPECT_TRUE(one_reader_a.valid());
  EXPECT_EQ(1, *one_reader_a);

  // Re-assign an existing reader.
  zero_reader_b = one_reader_a;
  EXPECT_TRUE(zero_reader_b.valid());
  EXPECT_EQ(1, *zero_reader_b);

  // Create another writer.
  RCU<int>::WriteHandle negative_writer = rcu.GetWriteHandle();
  EXPECT_TRUE(negative_writer.valid());
  EXPECT_EQ(1, *negative_writer);
  *negative_writer = -1;

  // A new reader should still see the last value actually committed.
  RCU<int>::ReadHandle one_reader_b = rcu.GetReadHandle();
  EXPECT_TRUE(one_reader_b.valid());
  EXPECT_EQ(1, *one_reader_b);

  // Abort the changes.
  negative_writer.Abort();

  // Check that existing readers see the same snapshots.
  EXPECT_EQ(0, *zero_reader_a);
  EXPECT_EQ(1, *zero_reader_b);
  EXPECT_EQ(1, *one_reader_a);
  EXPECT_EQ(1, *one_reader_b);

  // A new reader should see the last committed value.
  RCU<int>::ReadHandle one_reader_c = rcu.GetReadHandle();
  EXPECT_TRUE(one_reader_c.valid());
  EXPECT_EQ(1, *one_reader_c);
}

TEST(RCUTest, UnorderedMapTest) {
  typedef RCU<std::unordered_map<std::string, std::string>> RcuMap;

  // A map of fortresses to the houses that hold them.
  std::unordered_map<std::string, std::string> *initial_map
      = new std::unordered_map<std::string, std::string>();
  initial_map->emplace("Winterfell", "Stark");
  initial_map->emplace("Eirie", "Arryn");
  initial_map->emplace("Riverrun", "Tully");
  initial_map->emplace("Casterly Rock", "Lannister");
  initial_map->emplace("Highgarden", "Tyrell");
  initial_map->emplace("Storm's End", "Baratheon");
  initial_map->emplace("Sunspear", "Martell");
  RcuMap rcu(initial_map);

  // Check the initial view.
  RcuMap::ReadHandle initial_reader = rcu.GetReadHandle();
  EXPECT_EQ("Stark", initial_reader->at("Winterfell"));
  EXPECT_EQ(initial_reader->end(), initial_reader->find("Pike"));

  // Add a new entry.
  RcuMap::WriteHandle update_writer = rcu.GetWriteHandle();
  update_writer->emplace("Pike", "Greyjoy");
  update_writer.Commit();

  // Original snapshot should look the same.
  EXPECT_EQ("Stark", initial_reader->at("Winterfell"));
  EXPECT_EQ(initial_reader->end(), initial_reader->find("Pike"));

  // Get a new snapshot.
  initial_reader = rcu.GetReadHandle();
  EXPECT_EQ("Stark", initial_reader->at("Winterfell"));
  EXPECT_EQ("Greyjoy", initial_reader->at("Pike"));

  // Spoiler alert.
  update_writer = rcu.GetWriteHandle();
  (*update_writer)["Winterfell"] = "Bolton";
  (*update_writer)["Riverrun"] = "Frey";
  update_writer.Commit();

  // Updates should not be visible in previous snapshot.
  EXPECT_EQ("Stark", initial_reader->at("Winterfell"));
  EXPECT_EQ("Tully", initial_reader->at("Riverrun"));

  // Check state with another new snapshot.
  RcuMap::ReadHandle final_reader = rcu.GetReadHandle();
  EXPECT_EQ("Bolton", final_reader->at("Winterfell"));
  EXPECT_EQ("Frey", final_reader->at("Riverrun"));
  EXPECT_EQ("Lannister", final_reader->at("Casterly Rock"));
}

class IncrementerThread {
 public:
  IncrementerThread(const int difference,
                    const int iterations,
                    RCU<int> *rcu)
      : difference_(difference),
        iterations_(iterations),
        rcu_(rcu) {
  }

  void Start() {
    internal_thread_ = std::thread(IncrementerThread::ExecuteRunMethodOfThread,
                                   this);
  }

  void Join() {
    internal_thread_.join();
  }

 private:
  static void ExecuteRunMethodOfThread(IncrementerThread *thread) {
    thread->Run();
  }

  void Run() {
    for (int i = 0; i < iterations_; ++i) {
      RCU<int>::WriteHandle write_handle = rcu_->GetWriteHandle();
      *write_handle += difference_;
      write_handle.Commit();
    }
  }

  std::thread internal_thread_;
  const int difference_;
  const int iterations_;
  RCU<int> *rcu_;
};

TEST(RCUTest, ThreadedUpdatesTest) {
  constexpr int kLargestBase = 32;

  RCU<int> rcu(new int(0));
  RCU<int>::ReadHandle initial_reader = rcu.GetReadHandle();

  // Here's a slow and complicated way to sum some squares.
  int sum = 0;
  std::vector<std::unique_ptr<IncrementerThread>> incrementer_threads;
  for (int base = 1; base <= kLargestBase; ++base) {
    sum += base * base;
    incrementer_threads.emplace_back(new IncrementerThread(base, base, &rcu));
  }

  for (auto &thread : incrementer_threads) {
    thread->Start();
  }
  for (auto &thread : incrementer_threads) {
    thread->Join();
  }

  EXPECT_EQ(0, *initial_reader);

  RCU<int>::ReadHandle final_reader = rcu.GetReadHandle();
  EXPECT_EQ(sum, *final_reader);
}

}  // namespace internal
}  // namespace tmb
