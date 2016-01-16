// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
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
DEFINE_uint64(num_hw_threads, 0,
              "Number of hardware threads (vCPUs) available to the OS.");
DEFINE_uint64(num_cpu_sockets, 0,
              "Number of physical CPU sockets.");
DEFINE_string(affinity_mode, "",
              "NUMA affinity mode. Options are none, thread, or full. none "
              "means that threads will run on any CPU core and send messages "
              "to any other thread. thread means that threads will be "
              "affinitized so that they only run on one socket, but will "
              "send messages to any other thread (including those on other "
              "sockets). full means that threads will be affinitized to a "
              "single socket and only communicate with their peers on the "
              "same socket.");
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

  if (FLAGS_num_hw_threads == 0) {
    std::cerr << "num_hw_threads must be at least 1.\n";
    return 1;
  }

  if (FLAGS_num_cpu_sockets == 0) {
    std::cerr << "num_cpu_sockets must be at least 1.\n";
    return 1;
  }

  if (FLAGS_num_hw_threads % FLAGS_num_cpu_sockets != 0) {
    std::cerr << "WARNING: num_cpu_sockets does not evenly divide "
              << "num_hw_threads.\n";
  }

  std::vector<std::vector<int>> affinity_sets(FLAGS_num_cpu_sockets);
  if (FLAGS_affinity_mode == "none") {
    // Do nothing.
  } else if ((FLAGS_affinity_mode == "thread")
             || (FLAGS_affinity_mode == "full")) {
    std::size_t threads_per_socket = FLAGS_num_hw_threads
                                     / FLAGS_num_cpu_sockets;
    for (std::size_t thread_idx = 0;
         thread_idx < threads_per_socket;
         ++thread_idx) {
      for (std::size_t socket_idx = 0;
           socket_idx < FLAGS_num_cpu_sockets;
           ++socket_idx) {
        affinity_sets[socket_idx].push_back(
            socket_idx + thread_idx * FLAGS_num_cpu_sockets);
      }
    }
  } else {
    std::cerr << "affinity_mode must be one either none, thread, or full.\n";
    return 1;
  }

  std::unique_ptr<tmb::MessageBus> message_bus(
      tmbbench::SetupBusAllInOne(FLAGS_delete_messages_immediately));
  if (!message_bus) {
    return 1;
  }

  tmb::client_id control_thread_id = message_bus->Connect();
  message_bus->RegisterClientAsSender(control_thread_id, 1);

  std::vector<std::unique_ptr<tmbbench::SenderThread>> sender_threads;
  std::vector<std::vector<std::unique_ptr<tmbbench::ReceiverThreadBase>>>
      receiver_threads;

  if (FLAGS_affinity_mode == "none") {
    receiver_threads.resize(1);
    for (std::size_t receiver_idx = 0;
         receiver_idx < FLAGS_receiver_threads;
         ++receiver_idx) {
      if (FLAGS_delete_messages_immediately) {
        receiver_threads[0].emplace_back(
            new tmbbench::ReceiverThread<true>(message_bus.get(),
                                               affinity_sets[0]));
      } else {
        receiver_threads[0].emplace_back(
            new tmbbench::ReceiverThread<false>(message_bus.get(),
                                                affinity_sets[0]));
      }
    }

    for (std::size_t sender_idx = 0;
         sender_idx < FLAGS_sender_threads;
         ++sender_idx) {
      sender_threads.emplace_back(new tmbbench::SenderThread(
          message_bus.get(),
          FLAGS_message_bytes,
          affinity_sets[0]));
      for (const auto &receiver_thread : receiver_threads[0]) {
        sender_threads.back()->AddReceiverID(receiver_thread->GetClientID());
      }
    }
  } else if (FLAGS_affinity_mode == "thread") {
    receiver_threads.resize(1);
    for (std::size_t receiver_idx = 0;
         receiver_idx < FLAGS_receiver_threads;
         ++receiver_idx) {
      const std::vector<int> &socket_affinity
          = affinity_sets[receiver_idx % affinity_sets.size()];
      if (FLAGS_delete_messages_immediately) {
        receiver_threads[0].emplace_back(
            new tmbbench::ReceiverThread<true>(message_bus.get(),
                                               socket_affinity));
      } else {
        receiver_threads[0].emplace_back(
            new tmbbench::ReceiverThread<false>(message_bus.get(),
                                                socket_affinity));
      }
    }

    for (std::size_t sender_idx = 0;
         sender_idx < FLAGS_sender_threads;
         ++sender_idx) {
      const std::vector<int> &socket_affinity
          = affinity_sets[sender_idx % affinity_sets.size()];
      sender_threads.emplace_back(new tmbbench::SenderThread(
          message_bus.get(),
          FLAGS_message_bytes,
          socket_affinity));
      for (const auto &receiver_thread : receiver_threads[0]) {
        sender_threads.back()->AddReceiverID(receiver_thread->GetClientID());
      }
    }
  } else {
    receiver_threads.resize(FLAGS_num_cpu_sockets);
    for (std::size_t receiver_idx = 0;
         receiver_idx < FLAGS_receiver_threads;
         ++receiver_idx) {
      std::size_t socket_num = receiver_idx % FLAGS_num_cpu_sockets;
      if (FLAGS_delete_messages_immediately) {
        receiver_threads[socket_num].emplace_back(
            new tmbbench::ReceiverThread<true>(message_bus.get(),
                                               affinity_sets[socket_num]));
      } else {
        receiver_threads[socket_num].emplace_back(
            new tmbbench::ReceiverThread<false>(message_bus.get(),
                                                affinity_sets[socket_num]));
      }
    }

    for (std::size_t sender_idx = 0;
         sender_idx < FLAGS_sender_threads;
         ++sender_idx) {
      const std::vector<int> &socket_affinity
          = affinity_sets[sender_idx % affinity_sets.size()];
      sender_threads.emplace_back(new tmbbench::SenderThread(
          message_bus.get(),
          FLAGS_message_bytes,
          socket_affinity));
      for (const auto &receiver_thread
           : receiver_threads[sender_idx % receiver_threads.size()]) {
        sender_threads.back()->AddReceiverID(receiver_thread->GetClientID());
      }
    }
  }

  for (const auto &sender_thread : sender_threads) {
    sender_thread->Start();
  }
  for (const auto &receiver_thread_set : receiver_threads) {
    for (const auto &receiver_thread : receiver_thread_set) {
      receiver_thread->Start();
    }
  }

  std::this_thread::sleep_for(std::chrono::seconds(FLAGS_test_duration));

  tmb::MessageStyle style;
  for (const auto &sender_thread : sender_threads) {
    tmb::Address address;
    address.AddRecipient(sender_thread->GetClientID());
    tmb::TaggedMessage message(&tmbbench::kPoisonMessage,
                               sizeof(tmbbench::kPoisonMessage),
                               1);
    message_bus->Send(control_thread_id, address, style, std::move(message));
  }
  for (const auto &receiver_thread_set : receiver_threads) {
    for (const auto &receiver_thread : receiver_thread_set) {
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
  }

  for (const auto &sender_thread : sender_threads) {
    sender_thread->Join();
  }
  for (const auto &receiver_thread_set : receiver_threads) {
    for (const auto &receiver_thread : receiver_thread_set) {
      receiver_thread->Join();
    }
  }

  double total_send_throughput = 0;
  for (const auto &sender_thread : sender_threads) {
    total_send_throughput += sender_thread->GetThroughput();
  }
  double total_receive_throughput = 0;
  for (const auto &receiver_thread_set : receiver_threads) {
    for (const auto &receiver_thread : receiver_thread_set) {
      total_receive_throughput += receiver_thread->GetThroughput();
    }
  }

  std::cout << "Send throughput:    " << total_send_throughput
            << " messages/s\n";
  std::cout << "Receive throughput: " << total_receive_throughput
            << " messages/s\n";

  return 0;
}
