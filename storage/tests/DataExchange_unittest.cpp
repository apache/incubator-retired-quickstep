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
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/BlockLocator.hpp"
#include "query_execution/BlockLocatorUtil.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "storage/CountedReference.hpp"
#include "storage/DataExchangerAsync.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"

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

using tmb::MessageBus;
using tmb::TaggedMessage;

namespace quickstep {

class DataExchangeTest : public ::testing::Test {
 protected:
  static const char kStoragePath[];
  static const char kCheckedDomainNetworkAddress[];

  ~DataExchangeTest() {
    data_exchanger_expected_.join();
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

    storage_manager_expected_.reset(new StorageManager(
        kStoragePath,
        block_locator::getBlockDomain(
            data_exchanger_expected_.network_address(), worker_client_id_, &locator_client_id_, &bus_),
        locator_client_id_,
        &bus_));
    DCHECK_EQ(locator_->getBusClientID(), locator_client_id_);

    data_exchanger_expected_.set_storage_manager(storage_manager_expected_.get());
    data_exchanger_expected_.start();

    storage_manager_checked_.reset(new StorageManager(
        kStoragePath,
        block_locator::getBlockDomain(
            kCheckedDomainNetworkAddress, worker_client_id_, &locator_client_id_, &bus_),
        locator_client_id_,
        &bus_));
    DCHECK_EQ(locator_->getBusClientID(), locator_client_id_);
  }

  virtual void TearDown() {
    storage_manager_checked_.reset();

    data_exchanger_expected_.shutdown();
    storage_manager_expected_.reset();

    TaggedMessage message(kPoisonMessage);

    LOG(INFO) << "Worker (id '" << worker_client_id_
              << "') sent PoisonMessage (typed '" << kPoisonMessage
              << "') to BlockLocator (id '" << locator_client_id_ << "')";
    CHECK(MessageBus::SendStatus::kOK ==
        QueryExecutionUtil::SendTMBMessage(&bus_,
                                           worker_client_id_,
                                           locator_client_id_,
                                           move(message)));
  }

  unique_ptr<StorageManager> storage_manager_expected_, storage_manager_checked_;

 private:
  MessageBusImpl bus_;

  unique_ptr<BlockLocator> locator_;
  tmb::client_id locator_client_id_;

  tmb::client_id worker_client_id_;

  DataExchangerAsync data_exchanger_expected_;
};

const char DataExchangeTest::kStoragePath[] = "./data_exchange_test_data/";
const char DataExchangeTest::kCheckedDomainNetworkAddress[] = "ip:port";

TEST_F(DataExchangeTest, BlockPull) {
  CatalogRelation relation(nullptr, "rel");
  const attribute_id attr_id =
      relation.addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));

  const block_id block =
      storage_manager_expected_->createBlock(relation, relation.getDefaultStorageBlockLayout());

  {
    MutableBlockReference block_expected = storage_manager_expected_->getBlockMutable(block, relation);

    // Insert a tuple.
    const int value_expected = -1;
    {
      vector<TypedValue> attrs(1, TypedValue(value_expected));
      const Tuple tuple(move(attrs));

      EXPECT_TRUE(block_expected->insertTuple(tuple));
    }

    const BlockReference block_checked = storage_manager_checked_->getBlock(block, relation);
    EXPECT_FALSE(block_checked->isBlob());

    const TupleStorageSubBlock &tuple_store_checked = block_checked->getTupleStorageSubBlock();

    EXPECT_EQ(1, tuple_store_checked.numTuples());
    EXPECT_EQ(value_expected, tuple_store_checked.getAttributeValueTyped(0 /* tuple_id */, attr_id).getLiteral<int>());
  }

  storage_manager_checked_->deleteBlockOrBlobFile(block);
  storage_manager_expected_->deleteBlockOrBlobFile(block);
}

TEST_F(DataExchangeTest, BlobPull) {
  const block_id blob = storage_manager_expected_->createBlob(kDefaultBlockSizeInSlots);
  {
    const BlobReference blob_expected = storage_manager_expected_->getBlob(blob);
    const BlobReference blob_checked  =  storage_manager_checked_->getBlob(blob);
    EXPECT_TRUE(blob_checked->isBlob());
    EXPECT_EQ(blob, blob_checked->getID());
  }

  storage_manager_checked_->deleteBlockOrBlobFile(blob);
  storage_manager_expected_->deleteBlockOrBlobFile(blob);
}

}  // namespace quickstep

int main(int argc, char **argv) {
  google::InitGoogleLogging(argv[0]);
  // Honor FLAGS_buffer_pool_slots in StorageManager.
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
