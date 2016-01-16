// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/memory_mirror_message_bus.h"
#include "tmb/voltdb_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<MemoryMirrorMessageBus<false>>::CreateBus() {
  VoltDBMessageBus *internal_bus = new VoltDBMessageBus();
  internal_bus->AddServer("127.0.0.1", 21212);
  internal_bus->Initialize();
  return new MemoryMirrorMessageBus<false>(internal_bus);
}

template <>
MessageBus* MessageBusTest<MemoryMirrorMessageBus<true>>::CreateBus() {
  VoltDBMessageBus *internal_bus = new VoltDBMessageBus();
  internal_bus->AddServer("127.0.0.1", 21212);
  internal_bus->Initialize();
  return new MemoryMirrorMessageBus<true>(internal_bus);
}

typedef ::testing::Types<MemoryMirrorMessageBus<false>,
                         MemoryMirrorMessageBus<true>> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithVoltDB,
                              MessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithVoltDB,
                              ReconnectMessageBusTest,
                              TestTypes);

typedef ::testing::Types<MemoryMirrorMessageBus<true>> DeletionTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithVoltDB,
                              SeparateDeletionMessageBusTest,
                              DeletionTestTypes);

}  // namespace tmb
