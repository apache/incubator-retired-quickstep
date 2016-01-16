// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/leveldb_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<LevelDBMessageBus>::CreateBus() {
  return new LevelDBMessageBus("_test_leveldb", true);
}

typedef ::testing::Types<LevelDBMessageBus> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(LevelDB, MessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDB,
                              SeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDB, ReconnectMessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDB,
                              ReconnectActiveStateMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(LevelDB,
                              ReconnectSeparateDeletionMessageBusTest,
                              TestTypes);

}  // namespace tmb
