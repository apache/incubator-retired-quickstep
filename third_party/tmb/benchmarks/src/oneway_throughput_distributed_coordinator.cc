//   Copyright 2014-2015 Quickstep Technologies LLC.
//   Copyright 2015 Pivotal Software, Inc.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include <chrono>  // NOLINT(build/c++11)
#include <iostream>
#include <memory>
#include <thread>  // NOLINT(build/c++11)
#include <utility>
#include <vector>

#include "gflags/gflags.h"
#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"
#include "tmbbench/bus_setup.h"
#include "tmbbench/messages.h"

// Command-line flags.
DEFINE_uint64(test_duration, 20,
              "Test duration in seconds. May run for longer if receiver "
              "threads have trouble keeping up with send throughput.");
DEFINE_uint64(processes, 0,
              "Number of processes running oneway_throughput_distributed");
DEFINE_uint64(sender_threads, 1,
              "Number of sender threads per process to use. Must be at least "
              "1.");
DEFINE_uint64(receiver_threads, 1,
              "Number of receiver threads per process to use. Must be at "
              "least 1.");
DEFINE_uint64(message_bytes, 8,
              "Size of messages exchanged via the TMB in bytes. Must be at "
              "least 8.");
DEFINE_bool(delete_messages_immediately, false,
            "Whether to delete messages immediately as they are received, or "
            "to delete separately afterwards.");
DEFINE_bool(verbose, false,
            "Whether to enable verbose logging of experiments.");

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (argc != 1) {
    std::cerr << "Unrecognized command-line arguments.\n";
    return 1;
  }

  if (FLAGS_message_bytes < 8) {
    std::cerr << "message_bytes must be at least 8.\n";
    return 1;
  }

  if (FLAGS_processes == 0) {
    std::cerr << "processes must be at least 1.\n";
    return 1;
  }

  std::unique_ptr<tmb::MessageBus> message_bus(
      tmbbench::SetupBusAllInOneDistributed());
  if (!message_bus) {
    return 1;
  }

  tmb::client_id coordinator_id = message_bus->Connect();
  message_bus->RegisterClientAsSender(coordinator_id, 1);
  message_bus->RegisterClientAsSender(coordinator_id, 2);
  message_bus->RegisterClientAsSender(coordinator_id, 3);
  message_bus->RegisterClientAsReceiver(coordinator_id, 3);
  message_bus->RegisterClientAsReceiver(coordinator_id, 4);
  if (FLAGS_verbose) {
    std::cout << "Coordinator connected and registered.\n";
  }

  tmb::Address address_all;
  address_all.All(true);

  tmb::MessageStyle broadcast_style;
  broadcast_style.Broadcast(true);

  tmbbench::RunDescription run_desc;
  run_desc.num_senders = FLAGS_sender_threads;
  run_desc.num_receivers = FLAGS_receiver_threads;
  run_desc.message_bytes = FLAGS_message_bytes;
  run_desc.delete_immediately = FLAGS_delete_messages_immediately;
  tmb::TaggedMessage run_desc_msg(&run_desc, sizeof(run_desc), 2);

  if (FLAGS_verbose) {
    std::cout << "Sending setup message to other processes... ";
  }
  tmb::MessageBus::SendStatus status = message_bus->Send(
      coordinator_id,
      address_all,
      broadcast_style,
      std::move(run_desc_msg));
  if (status == tmb::MessageBus::SendStatus::kOK) {
    if (FLAGS_verbose) {
      std::cout << "OK\n";
    }
  } else {
    std::cerr << "ERROR sending setup message\n";
    return 1;
  }

  std::vector<tmb::AnnotatedMessage> responses;
  while (responses.size() < FLAGS_processes) {
    message_bus->ReceiveBatch(coordinator_id, &responses, 0, 0, true);
  }
  if (FLAGS_verbose) {
    std::cout << "Received replies from other processes.\n";
  }
  tmb::Address controllers_address;
  std::vector<tmb::client_id> global_receiver_list;
  for (const tmb::AnnotatedMessage &response : responses) {
    if (response.tagged_message.message_type() != 3) {
      std::cerr << "ERROR: unexpected response type\n";
      return 1;
    }
    controllers_address.AddRecipient(response.sender);
    global_receiver_list.insert(
        global_receiver_list.end(),
        static_cast<const tmb::client_id*>(
            response.tagged_message.message()),
        reinterpret_cast<const tmb::client_id*>(
            static_cast<const char*>(response.tagged_message.message())
                + response.tagged_message.message_bytes()));
  }
  if (FLAGS_verbose) {
    std::cout << "Constructed list of " << global_receiver_list.size()
              << " total receivers\n";
  }

  tmb::TaggedMessage receiver_list_msg(
      global_receiver_list.data(),
      global_receiver_list.size() * sizeof(tmb::client_id),
      3);
  if (FLAGS_verbose) {
    std::cout << "Sending receiver list to other processes... ";
  }
  status = message_bus->Send(coordinator_id,
                             controllers_address,
                             broadcast_style,
                             std::move(receiver_list_msg));
  if (status == tmb::MessageBus::SendStatus::kOK) {
    if (FLAGS_verbose) {
      std::cout << "OK\n";
    }
  } else {
    std::cerr << "ERROR sending receiver list\n";
    return 1;
  }

  if (FLAGS_verbose) {
    std::cout << "Sleeping for " << FLAGS_test_duration << " seconds\n";
  }
  std::this_thread::sleep_for(std::chrono::seconds(FLAGS_test_duration));

  tmb::TaggedMessage poison(&tmbbench::kPoisonMessage,
                            sizeof(tmbbench::kPoisonMessage),
                            1);
  status = message_bus->Send(coordinator_id,
                             address_all,
                             broadcast_style,
                             std::move(poison),
                             tmb::kDefaultPriority + 1);
  if (FLAGS_verbose) {
    std::cout << "Sent poison message to all test threads.\n"
              << "Awaiting responses from other processes...\n";
  }

  responses.clear();
  while (responses.size() < FLAGS_processes) {
    message_bus->ReceiveBatch(coordinator_id, &responses, 0, 0, true);
  }

  message_bus->Disconnect(coordinator_id);

  double total_send_throughput = 0.0;
  double total_receive_throughput = 0.0;
  for (const tmb::AnnotatedMessage &response : responses) {
    if (response.tagged_message.message_type() != 4) {
      std::cerr << "ERROR: unexpected response type\n";
      return 1;
    }

    const tmbbench::ThroughputResult *result
        = static_cast<const tmbbench::ThroughputResult*>(
            response.tagged_message.message());
    total_send_throughput += result->send_throughput;
    total_receive_throughput += result->receive_throughput;
  }

  std::cout << "Send throughput:    " << total_send_throughput
            << " messages/s\n";
  std::cout << "Receive throughput: " << total_receive_throughput
            << " messages/s\n";

  return 0;
}
