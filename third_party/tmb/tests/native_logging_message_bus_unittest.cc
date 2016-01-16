// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/native_logging_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<NativeLoggingMessageBus<false>>::CreateBus() {
  return new NativeLoggingMessageBus<false>("_test_log.tmb", true);
}

template <>
MessageBus* MessageBusTest<NativeLoggingMessageBus<true>>::CreateBus() {
  return new NativeLoggingMessageBus<true>("_test_log.tmb", true);
}

typedef ::testing::Types<NativeLoggingMessageBus<false>,
                         NativeLoggingMessageBus<true>> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(NativeLogging,
                              MessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeLogging,
                              ReconnectMessageBusTest,
                              TestTypes);

typedef ::testing::Types<NativeLoggingMessageBus<true>> DeletionTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(NativeLogging,
                              SeparateDeletionMessageBusTest,
                              DeletionTestTypes);

}  // namespace tmb
