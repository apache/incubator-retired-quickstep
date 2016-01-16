// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/pure_memory_message_bus.h"

namespace tmb {

typedef ::testing::Types<PureMemoryMessageBus<false>,
                         PureMemoryMessageBus<true>> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PureMemory, MessageBusTest, TestTypes);

typedef ::testing::Types<PureMemoryMessageBus<true>> DeletionTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(PureMemory,
                              SeparateDeletionMessageBusTest,
                              DeletionTestTypes);

}  // namespace tmb
