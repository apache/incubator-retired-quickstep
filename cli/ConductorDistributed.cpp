/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

/* A standalone command-line interface to Distributed QuickStep Conductor */

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/ForemanDistributed.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "utility/SqlError.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc/grpc.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/native_net_client_message_bus.h"
#include "tmb/tagged_message.h"

using std::free;
using std::malloc;
using std::move;
using std::size_t;
using std::string;
using std::unique_ptr;
using std::unordered_map;

using quickstep::QueryExecutionUtil;
using quickstep::QueryProcessor;
using quickstep::kDistributedCliRegistrationResponseMessage;
using quickstep::kDistributedCliRegistrationMessage;
using quickstep::kQueryExecutionErrorMessage;
using quickstep::kQueryExecutionSuccessMessage;
using quickstep::kSqlQueryMessage;

using tmb::AnnotatedMessage;
using tmb::TaggedMessage;
using tmb::client_id;

namespace quickstep {

#ifdef QUICKSTEP_OS_WINDOWS
static constexpr char kPathSeparator = '\\';
static constexpr char kDefaultStoragePath[] = "qsstor\\";
#else
static constexpr char kPathSeparator = '/';
static constexpr char kDefaultStoragePath[] = "qsstor/";
#endif

DEFINE_string(tmb_server_ip, "127.0.0.1", "IP Address of the TMB Server.");

static bool ValidateTmbServerPort(const char *flagname,
                                 std::int32_t value) {
  if (value > 0 && value < 65536) {
    return true;
  } else {
    std::fprintf(stderr, "--%s must be between 1 and 65535 (inclusive)\n", flagname);
    return false;
  }
}
DEFINE_int32(tmb_server_port, 4575, "Port of the TMB Server.");
static const bool tmb_server_port_dummy
    = gflags::RegisterFlagValidator(&FLAGS_tmb_server_port, &ValidateTmbServerPort);

DEFINE_string(storage_path, kDefaultStoragePath,
              "Filesystem path for quickstep database storage.");

}  // namespace quickstep

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  grpc_init();

  quickstep::SqlParserWrapper parser_wrapper;

  // Setup the paths used by StorageManager.
  string fixed_storage_path(quickstep::FLAGS_storage_path);
  if (!fixed_storage_path.empty()
      && (fixed_storage_path.back() != quickstep::kPathSeparator)) {
    fixed_storage_path.push_back(quickstep::kPathSeparator);
  }

  string catalog_path(fixed_storage_path);
  catalog_path.append("catalog.pb.bin");

  // Setup QueryProcessor, including CatalogDatabase and StorageManager.
  std::unique_ptr<QueryProcessor> query_processor;
  try {
    query_processor.reset(new QueryProcessor(catalog_path, fixed_storage_path));
  } catch (const std::exception &e) {
    LOG(FATAL) << "FATAL ERROR DURING STARTUP: " << e.what();
  } catch (...) {
    LOG(FATAL) << "NON-STANDARD EXCEPTION DURING STARTUP";
  }

  tmb::NativeNetClientMessageBus bus;
  bus.AddServer(quickstep::FLAGS_tmb_server_ip.c_str(), quickstep::FLAGS_tmb_server_port);
  bus.Initialize();
  bus.ResetBus();

  const client_id conductor_id = bus.Connect();
  LOG(INFO) << "Conductor TMB Client ID: " << conductor_id;

  bus.RegisterClientAsReceiver(conductor_id, kDistributedCliRegistrationMessage);
  bus.RegisterClientAsSender(conductor_id, kDistributedCliRegistrationResponseMessage);

  bus.RegisterClientAsReceiver(conductor_id, kSqlQueryMessage);
  bus.RegisterClientAsSender(conductor_id, kQueryExecutionSuccessMessage);
  bus.RegisterClientAsSender(conductor_id, kQueryExecutionErrorMessage);

  quickstep::ForemanDistributed foreman(&bus, query_processor->getDefaultDatabase());

  for (;;) {
    AnnotatedMessage annotated_message(bus.Receive(conductor_id, 0, true));
    const TaggedMessage &tagged_message = annotated_message.tagged_message;
    const client_id sender = annotated_message.sender;

    LOG(INFO) << "Conductor received typed '" << tagged_message.message_type()
              << "' message from client " << sender;
    switch (tagged_message.message_type()) {
      case kDistributedCliRegistrationMessage: {
        quickstep::serialization::EmptyMessage proto;

        const size_t proto_length = proto.ByteSize();
        char *proto_bytes = static_cast<char*>(malloc(proto_length));
        CHECK(proto.SerializeToArray(proto_bytes, proto_length));

        TaggedMessage message(static_cast<const void*>(proto_bytes),
                              proto_length,
                              kDistributedCliRegistrationResponseMessage);
        free(proto_bytes);

        LOG(INFO) << "Conductor sent DistributedCliRegistrationResponseMessage (typed '"
                  << kDistributedCliRegistrationResponseMessage
                  << "') to Distributed CLI " << sender;
        QueryExecutionUtil::SendTMBMessage(&bus,
                                           conductor_id,
                                           sender,
                                           move(message));
        break;
      }
      case kSqlQueryMessage: {
        quickstep::serialization::SqlQueryMessage proto;
        CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

        LOG(INFO) << "Conductor received the following SQL query: " << proto.sql_query();

        string *command_string = new string(move(proto.sql_query()));

        parser_wrapper.feedNextBuffer(command_string);
        quickstep::ParseResult parse_result = parser_wrapper.getNextStatement();

        unique_ptr<quickstep::QueryHandle> query_handle;
        try {
          query_handle.reset(query_processor->generateQueryHandle(*parse_result.parsed_statement));
        } catch (const quickstep::SqlError &sql_error) {
          // Set the query execution status along with the error message.
          quickstep::serialization::QueryExecutionErrorMessage proto;
          proto.set_error_message(sql_error.formatMessage(*command_string));

          const size_t proto_length = proto.ByteSize();
          char *proto_bytes = static_cast<char*>(malloc(proto_length));
          CHECK(proto.SerializeToArray(proto_bytes, proto_length));

          TaggedMessage message(static_cast<const void*>(proto_bytes),
                                proto_length,
                                kQueryExecutionErrorMessage);
          free(proto_bytes);

          LOG(INFO) << "Conductor (on behalf of Optimizer) sent QueryExecutionErrorMessage (typed '"
                    << kQueryExecutionErrorMessage
                    << "') to Distributed CLI " << sender;
          QueryExecutionUtil::SendTMBMessage(&bus,
                                             conductor_id,
                                             sender,
                                             move(message));
          break;
        }

        try {
          foreman.setQueryHandle(query_handle.get());
          foreman.start();
          foreman.join();

          quickstep::serialization::QueryExecutionSuccessMessage proto;

          const quickstep::CatalogRelation *query_result_relation = query_handle->getQueryResultRelation();
          if (query_result_relation) {
            proto.mutable_query_result_relation()->MergeFrom(query_result_relation->getProto());
          }

          const size_t proto_length = proto.ByteSize();
          char *proto_bytes = static_cast<char*>(malloc(proto_length));
          CHECK(proto.SerializeToArray(proto_bytes, proto_length));

          TaggedMessage message(static_cast<const void*>(proto_bytes),
                                proto_length,
                                kQueryExecutionSuccessMessage);
          free(proto_bytes);

          LOG(INFO) << "Conductor sent QueryExecutionSuccessMessage (typed '" << kQueryExecutionSuccessMessage
                    << "') to Distributed CLI " << sender;
          QueryExecutionUtil::SendTMBMessage(&bus,
                                             conductor_id,
                                             sender,
                                             move(message));

          if (query_result_relation) {
            const quickstep::relation_id query_result_relation_id = query_result_relation->getID();
            DCHECK(query_processor->getDefaultDatabase()->hasRelationWithId(query_result_relation_id));
            query_processor->getDefaultDatabase()->dropRelationById(query_result_relation_id);
          }

          query_processor->saveCatalog();
        } catch (const std::exception &e) {
          string error_message("QUERY EXECUTION ERROR: ");
          error_message.append(e.what());
          error_message.push_back('\n');

          quickstep::serialization::QueryExecutionErrorMessage proto;
          proto.set_error_message(error_message);

          const size_t proto_length = proto.ByteSize();
          char *proto_bytes = static_cast<char*>(malloc(proto_length));
          CHECK(proto.SerializeToArray(proto_bytes, proto_length));

          TaggedMessage message(static_cast<const void*>(proto_bytes),
                                proto_length,
                                kQueryExecutionErrorMessage);
          free(proto_bytes);

          LOG(INFO) << "Conductor (on behalf of Executor) sent QueryExecutionErrorMessage (typed '"
                    << kQueryExecutionErrorMessage
                    << "') to Distributed CLI " << sender;
          QueryExecutionUtil::SendTMBMessage(&bus,
                                             conductor_id,
                                             sender,
                                             move(message));
        }
        break;
      }
      default: {
        LOG(FATAL) << "Unknown TMB message type";
      }
    }
  }

  return 0;
}
