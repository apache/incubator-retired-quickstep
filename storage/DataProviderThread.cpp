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

#include "storage/DataProviderThread.hpp"

#include <cstddef>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "cli/DropRelation.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/BlockWire.pb.h"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadUtil.hpp"

#include "glog/logging.h"

#include "tmb/tagged_message.h"

namespace quickstep {

void DataProviderThread::run() {
  if (cpu_id_ >= 0) {
    ThreadUtil::BindToCPU(cpu_id_);
  }
  for (;;) {
    const AnnotatedMessage annotated_message =
        bus_->Receive(dpt_client_id_, 0, true);
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    const message_type_id message_type = tagged_message.message_type();
    switch (message_type) {
      case kPoisonMessage: {
        return;
      }
      case kBlockRequestMessage: {
        BlockRequest request_proto;
        CHECK(request_proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));
        // extract block ID from the message.
        BlockResponse proto;
        storage_manager_->sendBlockContents(request_proto.block_id(), &proto);
        if (proto.is_valid()) {
          // send response back to the client.
          const int proto_length = proto.ByteSize();
          char *proto_bytes = static_cast<char*>(malloc(proto_length));
          CHECK(proto.SerializeToArray(proto_bytes, proto_length));

          TaggedMessage message(static_cast<const void*>(proto_bytes),
                                proto_length,
                                kBlockResponseMessage);
          free(proto_bytes);
          QueryExecutionUtil::SendTMBMessage(
              bus_, dpt_client_id_, annotated_message.sender, std::move(message));
        }
        break;
      }
      case kFinishReadingRelationMessage: {
        FinishReadingRelationMessage request_proto;
        DropRelation::Drop(*database_->getRelationById(request_proto.relation_id()),
                           database_,
                           storage_manager_);
        // TODO(harshad) - Check if the call below is thread safe?
        query_processor_->saveCatalog();
        break;
      }
    }
  }
}

}  // namespace quickstep
