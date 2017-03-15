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

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::string;
using std::unique_ptr;
using std::unordered_set;
using std::vector;

using tmb::AnnotatedMessage;
using tmb::MessageBus;
using tmb::TaggedMessage;

namespace quickstep {

class BlockLocatorTest : public ::testing::Test {
 protected:
  static const char kStoragePath[];
  static const char kDomainNetworkAddress[];

  ~BlockLocatorTest() {
    locator_->join();
  }

  virtual void SetUp() {
    bus_.Initialize();

    locator_.reset(new BlockLocator(&bus_));
    locator_->start();

    worker_client_id_ = bus_.Connect();

    bus_.RegisterClientAsSender(worker_client_id_, kBlockDomainRegistrationMessage);
    bus_.RegisterClientAsReceiver(worker_client_id_, kBlockDomainRegistrationResponseMessage);

    bus_.RegisterClientAsSender(worker_client_id_, kPoisonMessage);

    block_domain_ =
        block_locator::getBlockDomain(kDomainNetworkAddress, worker_client_id_, &locator_client_id_, &bus_);
    DCHECK_EQ(locator_->getBusClientID(), locator_client_id_);

    storage_manager_.reset(
        new StorageManager(kStoragePath, block_domain_, locator_client_id_, &bus_));
  }

  virtual void TearDown() {
    storage_manager_.reset();

    TaggedMessage message(kPoisonMessage);

    LOG(INFO) << "Worker with Client " << worker_client_id_ << " sent PoisonMessage to BlockLocator with Client "
              << locator_client_id_;
    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_,
                                           worker_client_id_,
                                           locator_client_id_,
                                           move(message)));
  }

  void checkLoaded(const block_id block) {
    unordered_set<block_id_domain> domains;
    do {
      domains = locator_->getBlockDomains(block);
    } while (domains.empty());

    EXPECT_EQ(1u, domains.size());
    EXPECT_EQ(1u, domains.count(block_domain_));
  }

  void checkEvicted(const block_id block) {
    unordered_set<block_id_domain> domains;
    do {
      domains = locator_->getBlockDomains(block);
    } while (!domains.empty());
  }

  tmb::client_id worker_client_id_;

  block_id_domain block_domain_;
  unique_ptr<StorageManager> storage_manager_;

 private:
  MessageBusImpl bus_;

  unique_ptr<BlockLocator> locator_;
  tmb::client_id locator_client_id_;
};

const char BlockLocatorTest::kStoragePath[] = "./block_locator_test_data/";
const char BlockLocatorTest::kDomainNetworkAddress[] = "ip:port";

TEST_F(BlockLocatorTest, BlockTest) {
  CatalogRelation relation(nullptr, "rel");
  relation.addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));

  const block_id block =
      storage_manager_->createBlock(relation, relation.getDefaultStorageBlockLayout());
  checkLoaded(block);

  const string peer_domain_network_address =
      storage_manager_->getPeerDomainNetworkAddress(BlockIdUtil::Domain(block));
  EXPECT_STREQ(kDomainNetworkAddress, peer_domain_network_address.data());

  ASSERT_TRUE(storage_manager_->saveBlockOrBlob(block));
  storage_manager_->evictBlockOrBlob(block);
  checkEvicted(block);

  {
    const BlockReference block_ref = storage_manager_->getBlock(block, relation);
  }
  checkLoaded(block);

  storage_manager_->deleteBlockOrBlobFile(block);
  checkEvicted(block);
}

TEST_F(BlockLocatorTest, BlobTest) {
  const block_id blob = storage_manager_->createBlob(kDefaultBlockSizeInSlots);
  checkLoaded(blob);

  const string peer_domain_network_address =
      storage_manager_->getPeerDomainNetworkAddress(BlockIdUtil::Domain(blob));
  EXPECT_STREQ(kDomainNetworkAddress, peer_domain_network_address.data());

  ASSERT_TRUE(storage_manager_->saveBlockOrBlob(blob));
  storage_manager_->evictBlockOrBlob(blob);
  checkEvicted(blob);

  {
    const BlobReference blob_ref = storage_manager_->getBlob(blob);
  }
  checkLoaded(blob);

  storage_manager_->deleteBlockOrBlobFile(blob);
  checkEvicted(blob);
}

}  // namespace quickstep

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  // Honor FLAGS_buffer_pool_slots in StorageManager.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
