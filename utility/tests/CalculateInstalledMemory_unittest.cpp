/**
 *   Copyright 2016 Pivotal Software, Inc.
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
#include <cstdint>
#include <limits>

#include "gtest/gtest.h"
#include "utility/CalculateInstalledMemory.hpp"

namespace quickstep {
namespace utility {
namespace system {

TEST(CalculateInstalledMemoryTest, BasicCheck) {
  std::uint64_t installed_memory_in_bytes;

  EXPECT_EQ(true, calculateTotalMemoryInBytes(&installed_memory_in_bytes));
  EXPECT_GT(installed_memory_in_bytes, 0u);
}

}  // namespace system
}  // namespace utility
}  // namespace quickstep
