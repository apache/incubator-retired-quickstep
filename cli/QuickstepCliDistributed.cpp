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

/* A standalone command-line interface to distributed QuickStep */

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogRelation.hpp"
#include "cli/CliConfig.h"  // For QUICKSTEP_USE_LINENOISE.
#include "cli/LineReader.hpp"

#ifdef QUICKSTEP_USE_LINENOISE
#include "cli/LineReaderLineNoise.hpp"
typedef quickstep::LineReaderLineNoise LineReaderImpl;
#else
#include "cli/LineReaderDumb.hpp"
typedef quickstep::LineReaderDumb LineReaderImpl;
#endif

#include "cli/PrintToScreen.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "storage/StorageConfig.h"  // For QUICKSTEP_HAVE_FILE_MANAGER_HDFS.

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
#include "storage/FileManagerHdfs.hpp"
#endif

#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"
#include "grpc/grpc.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/native_net_client_message_bus.h"
#include "tmb/tagged_message.h"

using std::fprintf;
using std::free;
using std::malloc;
using std::move;
using std::printf;
using std::size_t;
using std::string;

using quickstep::ParseResult;
using quickstep::QueryExecutionUtil;
using quickstep::block_id;
using quickstep::kDistributedCliRegistrationResponseMessage;
using quickstep::kDistributedCliRegistrationMessage;
using quickstep::kQueryExecutionErrorMessage;
using quickstep::kQueryExecutionSuccessMessage;
using quickstep::kSqlQueryMessage;

using tmb::Address;
using tmb::AnnotatedMessage;
using tmb::MessageBus;
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

DEFINE_string(storage_path, kDefaultStoragePath,
              "Filesystem path for quickstep database storage.");

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

}  // namespace quickstep

int main(int argc, char* argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  grpc_init();

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  if (quickstep::FLAGS_use_hdfs) {
    LOG(INFO) << "Using HDFS as the default persistent storage, with namenode at "
              << quickstep::FLAGS_hdfs_namenode_host << ":"
              << quickstep::FLAGS_hdfs_namenode_port << " and block replication factor "
              << quickstep::FLAGS_hdfs_num_replications << "\n";
  }
#endif

  // Setup the paths used by StorageManager.
  string fixed_storage_path(quickstep::FLAGS_storage_path);
  if (!fixed_storage_path.empty()
      && (fixed_storage_path.back() != quickstep::kPathSeparator)) {
    fixed_storage_path.push_back(quickstep::kPathSeparator);
  }

  quickstep::StorageManager storage_manager(fixed_storage_path);

  tmb::NativeNetClientMessageBus bus;
  bus.AddServer(quickstep::FLAGS_tmb_server_ip.c_str(), quickstep::FLAGS_tmb_server_port);
  bus.Initialize();

  const client_id cli_id = bus.Connect();
  LOG(INFO) << "CLI TMB Client ID: " << cli_id;

  bus.RegisterClientAsSender(cli_id, kDistributedCliRegistrationMessage);
  bus.RegisterClientAsReceiver(cli_id, kDistributedCliRegistrationResponseMessage);

  LOG(INFO) << "Distributed CLI sent DistributedCliRegistrationMessage (typed '"
            << kDistributedCliRegistrationMessage
            << "') to all";

  Address all_addresses;
  all_addresses.All(true);

  // NOTE(zuyu): The singleton Conductor would need one copy of the message.
  tmb::MessageStyle style;

  quickstep::serialization::EmptyMessage proto;

  const size_t proto_length = proto.ByteSize();
  char *proto_bytes = static_cast<char*>(malloc(proto_length));
  CHECK(proto.SerializeToArray(proto_bytes, proto_length));

  TaggedMessage cli_reg_message(static_cast<const void*>(proto_bytes),
                                proto_length,
                                kDistributedCliRegistrationMessage);
  free(proto_bytes);

  MessageBus::SendStatus send_status =
      bus.Send(cli_id, all_addresses, style, move(cli_reg_message));
  DCHECK(send_status == MessageBus::SendStatus::kOK);

  // Wait for Conductor to response.
  AnnotatedMessage cli_reg_response_message(bus.Receive(cli_id, 0, true));
  DCHECK_EQ(kDistributedCliRegistrationResponseMessage,
            cli_reg_response_message.tagged_message.message_type());
  const client_id conductor_id = cli_reg_response_message.sender;

  LOG(INFO) << "Distributed CLI received typed '" << kDistributedCliRegistrationResponseMessage
            << "' message from client " << conductor_id;

  // Prepare for executing a query.
  bus.RegisterClientAsSender(cli_id, kSqlQueryMessage);
  bus.RegisterClientAsReceiver(cli_id, kQueryExecutionSuccessMessage);
  bus.RegisterClientAsReceiver(cli_id, kQueryExecutionErrorMessage);

  LineReaderImpl line_reader("distributed_quickstep> ",
                             "                  ...> ");
  quickstep::SqlParserWrapper parser_wrapper;
  std::chrono::time_point<std::chrono::steady_clock> start, end;

  for (;;) {
    string *command_string = new string();
    *command_string = line_reader.getNextCommand();
    if (command_string->size() == 0) {
      delete command_string;
      break;
    }

    parser_wrapper.feedNextBuffer(command_string);

    bool quitting = false;
    for (;;) {
      ParseResult result = parser_wrapper.getNextStatement();
      if (result.condition == ParseResult::kSuccess) {
        if (result.parsed_statement->getStatementType() == quickstep::ParseStatement::kQuit) {
          quitting = true;
          break;
        }

        LOG(INFO) << "Distributed CLI sent SqlQueryMessage (typed '" << kSqlQueryMessage
                  << "') to Foreman";
        quickstep::serialization::SqlQueryMessage proto;
        proto.set_sql_query(*command_string);

        const size_t proto_length = proto.ByteSize();
        char *proto_bytes = static_cast<char*>(malloc(proto_length));
        CHECK(proto.SerializeToArray(proto_bytes, proto_length));

        TaggedMessage sql_query_message(static_cast<const void*>(proto_bytes),
                                        proto_length,
                                        kSqlQueryMessage);
        free(proto_bytes);

        QueryExecutionUtil::SendTMBMessage(&bus,
                                           cli_id,
                                           conductor_id,
                                           move(sql_query_message));

        start = std::chrono::steady_clock::now();

        AnnotatedMessage annotated_message(bus.Receive(cli_id, 0, true));
        const TaggedMessage &tagged_message = annotated_message.tagged_message;
        LOG(INFO) << "Distributed CLI received typed '" << tagged_message.message_type()
                  << "' message from client " << annotated_message.sender;
        switch (tagged_message.message_type()) {
          case kQueryExecutionSuccessMessage: {
            end = std::chrono::steady_clock::now();

            quickstep::serialization::QueryExecutionSuccessMessage proto;
            CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

            if (proto.has_query_result_relation()) {
              quickstep::CatalogRelation query_result_relation(proto.query_result_relation());

              quickstep::PrintToScreen::PrintRelation(query_result_relation,
                                                      &storage_manager,
                                                      stdout);

              const std::vector<block_id> blocks(query_result_relation.getBlocksSnapshot());
              for (const block_id block : blocks) {
                storage_manager.deleteBlockOrBlobFile(block);
              }
            }

            printf("Execution time: %g seconds\nQuery Complete\n", std::chrono::duration<double>(end - start).count());
            break;
          }
          case kQueryExecutionErrorMessage: {
            quickstep::serialization::QueryExecutionErrorMessage proto;
            CHECK(proto.ParseFromArray(tagged_message.message(), tagged_message.message_bytes()));

            fprintf(stderr, "%s", proto.error_message().c_str());
            break;
          }
          default: {
            LOG(ERROR) << "Unknown TMB message type";
          }
        }
      } else {
        if (result.condition == ParseResult::kError) {
          fprintf(stderr, "%s", result.error_message.c_str());
        }
        break;
      }
    }

    if (quitting) {
      break;
    }
  }

  bus.Disconnect(cli_id);

  return 0;
}
