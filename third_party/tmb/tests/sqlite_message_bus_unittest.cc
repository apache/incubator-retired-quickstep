// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

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
