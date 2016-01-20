//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/sqlite_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<SQLiteMessageBus>::CreateBus() {
  return new SQLiteMessageBus("_test.db");
}

typedef ::testing::Types<SQLiteMessageBus> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(SQLite, MessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(SQLite,
                              SeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(SQLite, ReconnectMessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(SQLite,
                              ReconnectActiveStateMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(SQLite,
                              ReconnectSeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(SQLite, MultiInstanceMessageBusTest, TestTypes);

}  // namespace tmb
