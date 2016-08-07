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
