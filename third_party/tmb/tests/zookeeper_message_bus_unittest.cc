// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/zookeeper_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<ZookeeperMessageBus>::CreateBus() {
  ZookeeperMessageBus *bus = new ZookeeperMessageBus("/_tmb_test");
  bus->AddServer("127.0.0.1", 2181);
  return bus;
}

typedef ::testing::Types<ZookeeperMessageBus> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(Zookeeper, MessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(Zookeeper,
                              SeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(Zookeeper, ReconnectMessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(Zookeeper,
                              ReconnectActiveStateMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(Zookeeper,
                              ReconnectSeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(Zookeeper,
                              MultiInstanceMessageBusTest,
                              TestTypes);

}  // namespace tmb
