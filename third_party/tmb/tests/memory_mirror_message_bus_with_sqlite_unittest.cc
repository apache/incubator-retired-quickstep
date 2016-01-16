// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/memory_mirror_message_bus.h"
#include "tmb/sqlite_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<MemoryMirrorMessageBus<false>>::CreateBus() {
  SQLiteMessageBus *internal_bus = new SQLiteMessageBus("_test.db");
  internal_bus->Initialize();
  return new MemoryMirrorMessageBus<false>(internal_bus);
}

template <>
MessageBus* MessageBusTest<MemoryMirrorMessageBus<true>>::CreateBus() {
  SQLiteMessageBus *internal_bus = new SQLiteMessageBus("_test.db");
  internal_bus->Initialize();
  return new MemoryMirrorMessageBus<true>(internal_bus);
}

typedef ::testing::Types<MemoryMirrorMessageBus<false>,
                         MemoryMirrorMessageBus<true>> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithSQLite,
                              MessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithSQLite,
                              ReconnectMessageBusTest,
                              TestTypes);

typedef ::testing::Types<MemoryMirrorMessageBus<true>> DeletionTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithSQLite,
                              SeparateDeletionMessageBusTest,
                              DeletionTestTypes);

}  // namespace tmb
