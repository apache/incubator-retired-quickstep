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

    bus_.RegisterClientAsSender(worker_client_id_, kLocateBlockMessage);
    bus_.RegisterClientAsReceiver(worker_client_id_, kLocateBlockResponseMessage);

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

  vector<block_id_domain> getPeerDomains(const block_id block) {
    serialization::BlockMessage proto;
    proto.set_block_id(block);

    const int proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kLocateBlockMessage);
    free(proto_bytes);

    LOG(INFO) << "Worker wth Client " << worker_client_id_ << " sent LocateBlockMessage to BlockLocator";
    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_,
                                           worker_client_id_,
                                           locator_client_id_,
                                           move(message)));

    const AnnotatedMessage annotated_message(bus_.Receive(worker_client_id_, 0, true));
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    CHECK_EQ(kLocateBlockResponseMessage, tagged_message.message_type());
    LOG(INFO) << "Worker (id '" << worker_client_id_
              << "') received LocateBlockResponseMessage from BlockLocator";

    serialization::LocateBlockResponseMessage response_proto;
    CHECK(response_proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

    vector<block_id_domain> domains;
    for (int i = 0; i < response_proto.block_domains_size(); ++i) {
      domains.push_back(response_proto.block_domains(i));
    }

    return domains;
  }

  void checkLoaded(const block_id block) {
    const vector<string> peer_domain_network_addresses = storage_manager_->getPeerDomainNetworkAddresses(block);
    EXPECT_EQ(1u, peer_domain_network_addresses.size());
    EXPECT_STREQ(kDomainNetworkAddress, peer_domain_network_addresses[0].data());

    const vector<block_id_domain> domains = getPeerDomains(block);
    EXPECT_EQ(1u, domains.size());
    EXPECT_EQ(block_domain_, domains[0]);
  }

  void checkEvicted(const block_id block) {
    const vector<string> peer_domain_network_addresses = storage_manager_->getPeerDomainNetworkAddresses(block);
    EXPECT_TRUE(peer_domain_network_addresses.empty());

    const vector<block_id_domain> domains = getPeerDomains(block);
    EXPECT_TRUE(domains.empty());
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
