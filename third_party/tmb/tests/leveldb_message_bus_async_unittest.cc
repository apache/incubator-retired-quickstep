// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
