// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/native_net_client_message_bus.h"

#include "grpc/grpc.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<NativeNetClientMessageBus>::CreateBus() {
  grpc_init();

  NativeNetClientMessageBus *bus = new NativeNetClientMessageBus();
  bus->AddServer("127.0.0.1", 4575);
  return bus;
}

typedef ::testing::Types<NativeNetClientMessageBus> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(NativeNet, MessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeNet,
                              SeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeNet, ReconnectMessageBusTest, TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeNet,
                              ReconnectActiveStateMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeNet,
                              ReconnectSeparateDeletionMessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeNet,
                              MultiInstanceMessageBusTest,
                              TestTypes);

}  // namespace tmb
