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

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "query_execution/ForemanSingleNode.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "query_optimizer/tests/TestDatabaseLoader.hpp"
#include "storage/BlockWire.pb.h"
#include "storage/DataProviderThread.hpp"
#include "storage/StorageConstants.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc++/security/server_credentials.h"
#include "grpc++/server.h"
#include "grpc++/server_builder.h"
#include "grpc/grpc.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"
#include "tmb/internal/net_service_impl.h"
#include "tmb/native_net_client_message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

class DataProviderThreadTest : public ::testing::Test {
 protected:
  static const char kStoragePath[];

  ~DataProviderThreadTest() {
    data_provider_thread_->join();
  }

  virtual void SetUp() {
    quickstep::ClientIDMap::Instance();

    bus_.Initialize();

    client_id_ = bus_.Connect();

    bus_.RegisterClientAsSender(client_id_, kPoisonMessage);
    bus_.RegisterClientAsSender(client_id_, kBlockRequestMessage);
    bus_.RegisterClientAsSender(client_id_, kFinishReadingRelationMessage);
    bus_.RegisterClientAsReceiver(client_id_, kBlockResponseMessage);
    bus_.RegisterClientAsReceiver(client_id_, kDataPipelineMessage);
    bus_.RegisterClientAsSender(client_id_, kAdmitRequestMessage);
    bus_.RegisterClientAsSender(client_id_, kPoisonMessage);
    bus_.RegisterClientAsReceiver(client_id_, kWorkloadCompletionMessage);
    bus_.RegisterClientAsSender(client_id_, kCatalogRelationNewBlockMessage);

    std::string catalog_path(kStoragePath);
    catalog_path.append(kCatalogFilename);

    std::ofstream catalog_file(catalog_path.c_str());
    Catalog catalog;
    catalog.addDatabase(new CatalogDatabase(nullptr, "default"));
    catalog.getProto().SerializeToOstream(&catalog_file);
    catalog_file.close();

    query_processor_.reset(new QueryProcessor(std::move(catalog_path)));

    database_loader_.reset(new optimizer::TestDatabaseLoader(kStoragePath));
    database_loader_->createTestRelation(false);
    database_loader_->loadTestRelation();

    data_provider_thread_.reset(new DataProviderThread(database_loader_->catalog_database(),
                                                       query_processor_.get(),
                                                       database_loader_->storage_manager(),
                                                       &bus_));

    worker_.reset(new Worker(0, &bus_));
    worker_directory_.reset(
        new WorkerDirectory(1, {worker_->getBusClientID()}, {-1}));

    foreman_.reset(
        new ForemanSingleNode(client_id_,
                              worker_directory_.get(),
                              &bus_,
                              query_processor_->getDefaultDatabase(),
                              database_loader_->storage_manager()));

    worker_->start();
    foreman_->start();
    data_provider_thread_->start();
  }

  virtual void TearDown() {
    QueryExecutionUtil::BroadcastPoisonMessage(client_id_, &bus_);
    worker_->join();
    foreman_->join();
    database_loader_.reset(nullptr);
  }

  std::unique_ptr<optimizer::TestDatabaseLoader> database_loader_;

  std::unique_ptr<DataProviderThread> data_provider_thread_;

  tmb::client_id client_id_;

  tmb::PureMemoryMessageBus<true> bus_;

  std::unique_ptr<Worker> worker_;
  std::unique_ptr<WorkerDirectory> worker_directory_;
  std::unique_ptr<ForemanSingleNode> foreman_;

 private:
  std::unique_ptr<QueryProcessor> query_processor_;
};

const char DataProviderThreadTest::kStoragePath[] = "./data_provider_thread_test_data/";

TEST_F(DataProviderThreadTest, BlockAccessTest) {
  // Get the relation ID and block ID.
  const relation_id rid = database_loader_->test_relation()->getID();
  const std::vector<block_id> relation_block_ids = database_loader_->test_relation()->getBlocksSnapshot();
  for (block_id b : relation_block_ids) {
    BlockRequest proto;
    proto.set_block_id(b);
    proto.set_relation_id(rid);
    const int proto_length = proto.ByteSize();
    char *proto_bytes = static_cast<char*>(malloc(proto_length));
    CHECK(proto.SerializeToArray(proto_bytes, proto_length));

    TaggedMessage message(static_cast<const void*>(proto_bytes),
                          proto_length,
                          kBlockRequestMessage);
    free(proto_bytes);
    QueryExecutionUtil::SendTMBMessage(
        &bus_, client_id_, data_provider_thread_->getTMBClientID(), std::move(message));

    const AnnotatedMessage annotated_message = bus_.Receive(client_id_, 0, true);
    ASSERT_EQ(kBlockResponseMessage, annotated_message.tagged_message.message_type());
  }
}

// NOTE(harshad)- We could write a test case for kFinishReadingRelationMessage,
// but there may be race conditions involved in checking whether the relation
// has been dropped or not. An alternative could be to send an acknowledgement
// once the relation has been dropped. This alternative does not seem a high
// priority right now.
}   // namespace quickstep
