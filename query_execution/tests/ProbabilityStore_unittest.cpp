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

#include <cstddef>
#include <vector>

#include "gtest/gtest.h"

#include "query_execution/ProbabilityStore.hpp"

namespace quickstep {

TEST(ProbabilityStoreTest, CountTest) {
  ProbabilityStore store;
  EXPECT_EQ(0u, store.getNumObjects());
  const std::size_t kProperty = 0;
  store.addProbability(kProperty, 0.5);
  EXPECT_EQ(1u, store.getNumObjects());
  store.removeObject(kProperty);
  EXPECT_EQ(0u, store.getNumObjects());

  std::vector<std::size_t> objects {3, 5, 7, 9};
  std::vector<float> probabilities {0.2, 0.3, 0.4, 0.1};
  store.addProbabilities(objects, probabilities);

  EXPECT_EQ(objects.size(), store.getNumObjects());
}

TEST(ProbabilityStoreTest, IndividualProbabilityTest) {
  ProbabilityStore store;
  std::vector<std::size_t> objects {3, 5, 7, 9};
  std::vector<float> probabilities {0.2, 0.3, 0.4, 0.1};
  store.addProbabilities(objects, probabilities);

  for (std::size_t object_num = 0; object_num < objects.size(); ++object_num) {
    EXPECT_EQ(probabilities[object_num],
              store.getIndividualProbability(objects[object_num]));
  }
}

TEST(ProbabilityStoreTest, PickRandomPropertyTest) {
  ProbabilityStore store;
  std::vector<std::size_t> objects {3, 5, 7, 9};
  std::vector<float> probabilities {0.2, 0.3, 0.4, 0.1};
  store.addProbabilities(objects, probabilities);

  const std::size_t kNumTrials = 10;
  while (!objects.empty()) {
    for (std::size_t trial_num = 0; trial_num < kNumTrials; ++trial_num) {
      const std::size_t picked_property = store.pickRandomProperty();
      const auto it = std::find(objects.begin(), objects.end(), picked_property);
      EXPECT_TRUE(it != objects.end());
    }
    const std::size_t property_to_be_removed = objects.back();
    store.removeObject(property_to_be_removed);
    objects.pop_back();
    EXPECT_EQ(objects.size(), store.getNumObjects());
  }
}

}  // namespace quickstep
