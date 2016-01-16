// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#include <chrono>  // NOLINT(build/c++11)
#include <iostream>
#include <memory>
#include <string>
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
#include "tmbbench/affinity.h"
#include "tmbbench/bus_setup.h"
#include "tmbbench/messages.h"
#include "tmbbench/receiver_thread.h"
#include "tmbbench/sender_thread.h"

// Command-line flags.
DEFINE_uint64(test_duration, 20,
              "Test duration in seconds. May run for longer if receiver "
              "threads have trouble keeping up with send throughput.");
DEFINE_uint64(sender_threads, 1,
              "Number of sender threads to use. Must be at least 1.");
DEFINE_uint64(receiver_threads, 1,
              "Number of receiver threads to use. Must be at least 1.");
DEFINE_string(sender_thread_affinities, "",
              "Optional comma-seperated list of CPU core IDs to affinitize "
              "sender threads to. -1 indicates no affinity.");
DEFINE_string(receiver_thread_affinities, "",
              "Optional comma-seperated list of CPU core IDs to affinitize "
              "receiver threads to. -1 indicates no affinity.");
DEFINE_string(global_affinity_set, "",
              "As an alternative to sender_thread_affinities and "
              "receiver_thread_affinities, this option can be used to set the "
              "affinity of all threads to the specified comma-seperated list "
              "of CPU IDs.");
DEFINE_uint64(message_bytes, 8,
              "Size of messages exchanged via the TMB in bytes. Must be at "
              "least 8.");
DEFINE_bool(delete_messages_immediately, false,
            "Whether to delete messages immediately as they are received, or "
            "to delete separately afterwards.");

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

  std::unique_ptr<tmb::MessageBus> message_bus(
      tmbbench::SetupBusAllInOne(FLAGS_delete_messages_immediately));
  if (!message_bus) {
    return 1;
  }

  std::vector<int> global_affinity;
  if (!FLAGS_global_affinity_set.empty()) {
    if (!(FLAGS_sender_thread_affinities.empty()
          && FLAGS_receiver_thread_affinities.empty())) {
      std::cerr << "Can not use global_affinity_set together with "
                   "sender_thread_affinities or receiver_thread_affinities.";
      return 1;
    }
    if (!tmbbench::ParseGlobalAffinityString(FLAGS_global_affinity_set,
                                             &global_affinity)) {
      std::cerr << "Could not parse global_affinity_set.\n";
      return 1;
    }
  }

  std::vector<std::vector<int>> sender_affinities;
  if (FLAGS_sender_threads < 1) {
    std::cerr << "sender_threads must be at least 1.\n";
    return 1;
  }
  if (FLAGS_sender_thread_affinities.empty()) {
    sender_affinities.resize(FLAGS_sender_threads, global_affinity);
  } else {
    if (!tmbbench::ParseAffinityString(FLAGS_sender_thread_affinities,
                                       &sender_affinities)) {
      std::cerr << "Could not parse sender_thread_affinities.\n";
      return 1;
    }
    if (sender_affinities.size() != FLAGS_sender_threads) {
      std::cerr << "sender_thread_affinities must have the same number of "
                   "entries as the number of sender_threads.\n";
      return 1;
    }
  }

  std::vector<std::vector<int>> receiver_affinities;
  if (FLAGS_receiver_threads < 1) {
    std::cerr << "receiver_threads must be at least 1.\n";
    return 1;
  }
  if (FLAGS_receiver_thread_affinities.empty()) {
    receiver_affinities.resize(FLAGS_receiver_threads, global_affinity);
  } else {
    if (!tmbbench::ParseAffinityString(FLAGS_receiver_thread_affinities,
                                       &receiver_affinities)) {
      std::cerr << "Could not parse receiver_thread_affinities.\n";
      return 1;
    }
    if (receiver_affinities.size() != FLAGS_receiver_threads) {
      std::cerr << "receiver_thread_affinities must have the same number of "
                   "entries as the number of receiver_threads.\n";
      return 1;
    }
  }

  tmb::client_id control_thread_id = message_bus->Connect();
  message_bus->RegisterClientAsSender(control_thread_id, 1);

  std::vector<std::unique_ptr<tmbbench::SenderThread>> sender_threads;
  std::vector<std::unique_ptr<tmbbench::ReceiverThreadBase>> receiver_threads;

  for (const std::vector<int> &affinity : receiver_affinities) {
    if (FLAGS_delete_messages_immediately) {
      receiver_threads.emplace_back(
          new tmbbench::ReceiverThread<true>(message_bus.get(), affinity));
    } else {
      receiver_threads.emplace_back(
          new tmbbench::ReceiverThread<false>(message_bus.get(), affinity));
    }
  }
  for (const std::vector<int> &affinity : sender_affinities) {
    sender_threads.emplace_back(new tmbbench::SenderThread(message_bus.get(),
                                                           FLAGS_message_bytes,
                                                           affinity));
    for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
         : receiver_threads) {
      sender_threads.back()->AddReceiverID(receiver_thread->GetClientID());
    }
  }

  for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
       : sender_threads) {
    sender_thread->Start();
  }
  for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
       : receiver_threads) {
    receiver_thread->Start();
  }

  std::this_thread::sleep_for(std::chrono::seconds(FLAGS_test_duration));

  tmb::MessageStyle style;
  for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
       : sender_threads) {
    tmb::Address address;
    address.AddRecipient(sender_thread->GetClientID());
    tmb::TaggedMessage message(&tmbbench::kPoisonMessage,
                               sizeof(tmbbench::kPoisonMessage),
                               1);
    message_bus->Send(control_thread_id, address, style, std::move(message));
  }
  for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
       : receiver_threads) {
    tmb::Address address;
    address.AddRecipient(receiver_thread->GetClientID());
    tmb::TaggedMessage message(&tmbbench::kPoisonMessage,
                               sizeof(tmbbench::kPoisonMessage),
                               1);
    message_bus->Send(control_thread_id,
                      address,
                      style,
                      std::move(message),
                      tmb::kDefaultPriority + 1);
  }

  for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
       : sender_threads) {
    sender_thread->Join();
  }
  for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
       : receiver_threads) {
    receiver_thread->Join();
  }

  double total_send_throughput = 0;
  for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
       : sender_threads) {
    total_send_throughput += sender_thread->GetThroughput();
  }
  double total_receive_throughput = 0;
  for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
       : receiver_threads) {
    total_receive_throughput += receiver_thread->GetThroughput();
  }

  std::cout << "Send throughput:    " << total_send_throughput
            << " messages/s\n";
  std::cout << "Receive throughput: " << total_receive_throughput
            << " messages/s\n";

  return 0;
}
