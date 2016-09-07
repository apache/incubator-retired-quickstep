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

#include "utility/DisjointTreeForest.hpp"

#include <cstddef>
#include <string>
#include <vector>

#include "gtest/gtest.h"

namespace quickstep {

TEST(DisjointTreeForestTest, IntTest) {
  DisjointTreeForest<int> forest;
  for (int i = 10; i < 20; ++i) {
    forest.makeSet(i);
  }

  for (int i = 10; i < 20; i += 2) {
    EXPECT_NE(forest.find(i), forest.find(i+1));
    EXPECT_FALSE(forest.isConnected(i, i+1));

    forest.merge(i, i+1);
    EXPECT_EQ(forest.find(i), forest.find(i+1));
    EXPECT_TRUE(forest.isConnected(i, i+1));

    forest.merge(i+1, i);
    EXPECT_EQ(forest.find(i), forest.find(i+1));
    EXPECT_TRUE(forest.isConnected(i, i+1));
  }

  for (int i = 12; i < 20; i += 2) {
    EXPECT_NE(forest.find(i), forest.find(i-1));
    EXPECT_FALSE(forest.isConnected(i, i-1));
  }

  forest.merge(10, 17);
  forest.merge(11, 18);
  EXPECT_EQ(forest.find(11), forest.find(16));
  EXPECT_EQ(forest.find(10), forest.find(19));
  EXPECT_NE(forest.find(10), forest.find(12));
  EXPECT_NE(forest.find(15), forest.find(17));

  forest.merge(12, 14);
  forest.merge(15, 16);
  const std::size_t id = forest.find(10);
  for (int i = 10; i < 20; ++i) {
    EXPECT_EQ(forest.find(i), id);
  }
}

TEST(DisjointTreeForestTest, StringTest) {
  DisjointTreeForest<std::string> forest;
  const std::vector<std::string> elements = { "aaa", "bbb", "ccc", "ddd" };
  for (const std::string &element : elements) {
    forest.makeSet(element);
  }

  EXPECT_NE(forest.find("aaa"), forest.find("bbb"));
  forest.merge("aaa", "bbb");
  EXPECT_EQ(forest.find("aaa"), forest.find("bbb"));

  EXPECT_NE(forest.find("ccc"), forest.find("ddd"));
  forest.merge("ccc", "ddd");
  EXPECT_EQ(forest.find("ccc"), forest.find("ddd"));

  EXPECT_NE(forest.find("aaa"), forest.find("ccc"));
  EXPECT_NE(forest.find("aaa"), forest.find("ddd"));
  EXPECT_NE(forest.find("bbb"), forest.find("ccc"));
  EXPECT_NE(forest.find("bbb"), forest.find("ddd"));

  forest.merge("aaa", "ddd");
  for (const std::string &e1 : elements) {
    for (const std::string &e2 : elements) {
      EXPECT_EQ(forest.find(e1), forest.find(e2));
      EXPECT_TRUE(forest.isConnected(e1, e2));
    }
  }
}

}  // namespace quickstep
