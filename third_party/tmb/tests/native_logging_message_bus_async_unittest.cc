/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "gtest/gtest.h"
#include "tests/message_bus_unittest_common.h"
#include "tmb/native_logging_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<NativeLoggingMessageBus<false>>::CreateBus() {
  return new NativeLoggingMessageBus<false>("_test_log.tmb", false);
}

template <>
MessageBus* MessageBusTest<NativeLoggingMessageBus<true>>::CreateBus() {
  return new NativeLoggingMessageBus<true>("_test_log.tmb", false);
}

typedef ::testing::Types<NativeLoggingMessageBus<false>,
                         NativeLoggingMessageBus<true>> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(NativeLoggingAsync,
                              MessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(NativeLoggingAsync,
                              ReconnectMessageBusTest,
                              TestTypes);

typedef ::testing::Types<NativeLoggingMessageBus<true>> DeletionTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(NativeLoggingAsync,
                              SeparateDeletionMessageBusTest,
                              DeletionTestTypes);

}  // namespace tmb
