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
#include "tmb/leveldb_message_bus.h"
#include "tmb/memory_mirror_message_bus.h"

namespace tmb {

template <>
MessageBus* MessageBusTest<MemoryMirrorMessageBus<false>>::CreateBus() {
  LevelDBMessageBus *internal_bus = new LevelDBMessageBus("_test_leveldb");
  internal_bus->Initialize();
  return new MemoryMirrorMessageBus<false>(internal_bus);
}

template <>
MessageBus* MessageBusTest<MemoryMirrorMessageBus<true>>::CreateBus() {
  LevelDBMessageBus *internal_bus = new LevelDBMessageBus("_test_leveldb");
  internal_bus->Initialize();
  return new MemoryMirrorMessageBus<true>(internal_bus);
}

typedef ::testing::Types<MemoryMirrorMessageBus<false>,
                         MemoryMirrorMessageBus<true>> TestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithLevelDB,
                              MessageBusTest,
                              TestTypes);
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithLevelDB,
                              ReconnectMessageBusTest,
                              TestTypes);

typedef ::testing::Types<MemoryMirrorMessageBus<true>> DeletionTestTypes;
INSTANTIATE_TYPED_TEST_CASE_P(MemoryMirrorWithLevelDB,
                              SeparateDeletionMessageBusTest,
                              DeletionTestTypes);

}  // namespace tmb
