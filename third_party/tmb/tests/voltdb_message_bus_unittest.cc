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
