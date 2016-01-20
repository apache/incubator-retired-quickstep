/**
 *   Copyright 2015 Pivotal Software, Inc.
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

#include <array>

#include "gtest/gtest.h"

#include "utility/EqualsAnyConstant.hpp"

namespace quickstep {

TEST(EqualsAnyConstantTest, IntTest) {
  for (int checked = 0; checked < 100; ++checked) {
    // Always false when checking against no constants.
    EXPECT_FALSE(EqualsAnyConstant(checked));

    // Check against a single constant (using the template directly and the
    // macro).
    EXPECT_EQ(checked == 42,
              (EqualsAnyConstant<int, 42>(checked)));
    EXPECT_EQ(checked == 42,
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 42));

    // Check against a few constants.
    EXPECT_EQ((checked == 2) || (checked == 3) || (checked == 5) || (checked == 7),
              (EqualsAnyConstant<int, 2, 3, 5, 7>(checked)));
    EXPECT_EQ((checked == 2) || (checked == 3) || (checked == 5) || (checked == 7),
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 2, 3, 5, 7));

    // Check against exactly 8 constants.
    EXPECT_EQ((checked == 2) || (checked == 3) || (checked == 5) || (checked == 7)
                  || (checked == 11) || (checked == 13) || (checked == 17) || (checked == 19),
              (EqualsAnyConstant<int, 2, 3, 5, 7, 11, 13, 17, 19>(checked)));
    EXPECT_EQ((checked == 2) || (checked == 3) || (checked == 5) || (checked == 7)
                  || (checked == 11) || (checked == 13) || (checked == 17) || (checked == 19),
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 2, 3, 5, 7, 11, 13, 17, 19));

    // Check against more than 8 constants (will recursively expand templates).
    EXPECT_EQ((checked == 2) || (checked == 3) || (checked == 5) || (checked == 7)
                  || (checked == 11) || (checked == 13) || (checked == 17) || (checked == 19)
                  || (checked == 23) || (checked == 29) || (checked == 31) || (checked == 37),
              (EqualsAnyConstant<int, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37>(checked)));
    EXPECT_EQ((checked == 2) || (checked == 3) || (checked == 5) || (checked == 7)
                  || (checked == 11) || (checked == 13) || (checked == 17) || (checked == 19)
                  || (checked == 23) || (checked == 29) || (checked == 31) || (checked == 37),
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37));
  }
}

namespace {

enum class TestEnum {
  kCase00, kCase01, kCase02, kCase03, kCase04, kCase05, kCase06, kCase07,
  kCase08, kCase09, kCase10, kCase11, kCase12, kCase13, kCase14, kCase15,
  kCase16, kCase17, kCase18, kCase19, kCase20, kCase21, kCase22, kCase23,
  kCase24, kCase25, kCase26, kCase27, kCase28, kCase29, kCase30, kCase31,
  kCase32, kCase33, kCase34, kCase35, kCase36, kCase37, kCase38, kCase39
};

TEST(EqualsAnyConstantTest, EnumTest) {
  std::array<TestEnum, 40> all_cases{{
      TestEnum::kCase00, TestEnum::kCase01, TestEnum::kCase02, TestEnum::kCase03,
      TestEnum::kCase04, TestEnum::kCase05, TestEnum::kCase06, TestEnum::kCase07,
      TestEnum::kCase08, TestEnum::kCase09, TestEnum::kCase10, TestEnum::kCase11,
      TestEnum::kCase12, TestEnum::kCase13, TestEnum::kCase14, TestEnum::kCase15,
      TestEnum::kCase16, TestEnum::kCase17, TestEnum::kCase18, TestEnum::kCase19,
      TestEnum::kCase20, TestEnum::kCase21, TestEnum::kCase22, TestEnum::kCase23,
      TestEnum::kCase24, TestEnum::kCase25, TestEnum::kCase26, TestEnum::kCase27,
      TestEnum::kCase28, TestEnum::kCase29, TestEnum::kCase30, TestEnum::kCase31,
      TestEnum::kCase32, TestEnum::kCase33, TestEnum::kCase34, TestEnum::kCase35,
      TestEnum::kCase36, TestEnum::kCase37, TestEnum::kCase38, TestEnum::kCase39}};

  // We intentionally use a reference type instead of the literal type here to
  // make sure that everything still works correctly with references.
  for (const TestEnum &checked : all_cases) {
    // Always false when checking against no constants.
    EXPECT_FALSE(EqualsAnyConstant(checked));

    // Check against a single constant (using the template directly and the
    // macro).
    EXPECT_EQ(checked == TestEnum::kCase32,
              (EqualsAnyConstant<TestEnum, TestEnum::kCase32>(checked)));
    EXPECT_EQ(checked == TestEnum::kCase32,
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked, TestEnum::kCase32));

    // Check against a few constants.
    EXPECT_EQ((checked == TestEnum::kCase02) || (checked == TestEnum::kCase03)
                  || (checked == TestEnum::kCase05) || (checked == TestEnum::kCase07),
              (EqualsAnyConstant<TestEnum,
                                 TestEnum::kCase02, TestEnum::kCase03,
                                 TestEnum::kCase05, TestEnum::kCase07>(checked)));
    EXPECT_EQ((checked == TestEnum::kCase02) || (checked == TestEnum::kCase03)
                  || (checked == TestEnum::kCase05) || (checked == TestEnum::kCase07),
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked,
                                            TestEnum::kCase02, TestEnum::kCase03,
                                            TestEnum::kCase05, TestEnum::kCase07));

    // Check against exactly 8 constants.
    EXPECT_EQ((checked == TestEnum::kCase02) || (checked == TestEnum::kCase03)
                  || (checked == TestEnum::kCase05) || (checked == TestEnum::kCase07)
                  || (checked == TestEnum::kCase11) || (checked == TestEnum::kCase13)
                  || (checked == TestEnum::kCase17) || (checked == TestEnum::kCase19),
              (EqualsAnyConstant<TestEnum,
                                 TestEnum::kCase02, TestEnum::kCase03,
                                 TestEnum::kCase05, TestEnum::kCase07,
                                 TestEnum::kCase11, TestEnum::kCase13,
                                 TestEnum::kCase17, TestEnum::kCase19>(checked)));
    EXPECT_EQ((checked == TestEnum::kCase02) || (checked == TestEnum::kCase03)
                  || (checked == TestEnum::kCase05) || (checked == TestEnum::kCase07)
                  || (checked == TestEnum::kCase11) || (checked == TestEnum::kCase13)
                  || (checked == TestEnum::kCase17) || (checked == TestEnum::kCase19),
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked,
                                            TestEnum::kCase02, TestEnum::kCase03,
                                            TestEnum::kCase05, TestEnum::kCase07,
                                            TestEnum::kCase11, TestEnum::kCase13,
                                            TestEnum::kCase17, TestEnum::kCase19));

    // Check against more than 8 constants (will recursively expand templates).
    EXPECT_EQ((checked == TestEnum::kCase02) || (checked == TestEnum::kCase03)
                  || (checked == TestEnum::kCase05) || (checked == TestEnum::kCase07)
                  || (checked == TestEnum::kCase11) || (checked == TestEnum::kCase13)
                  || (checked == TestEnum::kCase17) || (checked == TestEnum::kCase19)
                  || (checked == TestEnum::kCase23) || (checked == TestEnum::kCase29)
                  || (checked == TestEnum::kCase31) || (checked == TestEnum::kCase37),
              (EqualsAnyConstant<TestEnum,
                                 TestEnum::kCase02, TestEnum::kCase03,
                                 TestEnum::kCase05, TestEnum::kCase07,
                                 TestEnum::kCase11, TestEnum::kCase13,
                                 TestEnum::kCase17, TestEnum::kCase19,
                                 TestEnum::kCase23, TestEnum::kCase29,
                                 TestEnum::kCase31, TestEnum::kCase37>(checked)));
    EXPECT_EQ((checked == TestEnum::kCase02) || (checked == TestEnum::kCase03)
                  || (checked == TestEnum::kCase05) || (checked == TestEnum::kCase07)
                  || (checked == TestEnum::kCase11) || (checked == TestEnum::kCase13)
                  || (checked == TestEnum::kCase17) || (checked == TestEnum::kCase19)
                  || (checked == TestEnum::kCase23) || (checked == TestEnum::kCase29)
                  || (checked == TestEnum::kCase31) || (checked == TestEnum::kCase37),
              QUICKSTEP_EQUALS_ANY_CONSTANT(checked,
                                            TestEnum::kCase02, TestEnum::kCase03,
                                            TestEnum::kCase05, TestEnum::kCase07,
                                            TestEnum::kCase11, TestEnum::kCase13,
                                            TestEnum::kCase17, TestEnum::kCase19,
                                            TestEnum::kCase23, TestEnum::kCase29,
                                            TestEnum::kCase31, TestEnum::kCase37));
  }
}

}  // namespace

}  // namespace quickstep
