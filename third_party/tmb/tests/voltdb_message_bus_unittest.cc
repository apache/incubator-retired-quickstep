// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/voltdb_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<VoltDBMessageBus>::CreateBus() {
  VoltDBMessageBus *bus = new VoltDBMessageBus();
  bus->AddServer("127.0.0.1", 21212);
  return bus;
}

typedef ::testing::Types<VoltDBMessageBus> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(VoltDB, MessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(VoltDB,
                              SeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(VoltDB, ReconnectMessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(VoltDB,
                              ReconnectActiveStateMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(VoltDB,
                              ReconnectSeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(VoltDB,
                              MultiInstanceMessageBusTest,
                              TestTypes);

}  // namespace tmb
