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
#include "tmb/leveldb_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<LevelDBMessageBus>::CreateBus() {
  return new LevelDBMessageBus("_test_leveldb", false);
}

typedef ::testing::Types<LevelDBMessageBus> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(LevelDBAsync,
                              MessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDBAsync,
                              SeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDBAsync,
                              ReconnectMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDBAsync,
                              ReconnectActiveStateMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDBAsync,
                              ReconnectSeparateDeletionMessageBusTest,
                              TestTypes);

}  // namespace tmb
