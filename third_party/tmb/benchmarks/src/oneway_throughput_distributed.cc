// Copyright (c) 2014-2015, Quickstep Technologies LLC.
// Copyright (c) 2015, Pivotal Software, Inc.
// All Rights Reserved.

#include <iostream>
#include <memory>
#include <utility>
#include <vector>

#include "gflags/gflags.h"
#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"
#include "tmbbench/bus_setup.h"
#include "tmbbench/messages.h"
#include "tmbbench/receiver_thread.h"
#include "tmbbench/sender_thread.h"

int main(int argc, char *argv[]) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  if (argc != 1) {
    std::cerr << "Unrecognized command-line arguments.\n";
    return 1;
  }

  std::unique_ptr<tmb::MessageBus> message_bus(
      tmbbench::SetupBusAllInOneDistributed());
  if (!message_bus) {
    return 1;
  }

  tmb::client_id control_thread_id = message_bus->Connect();
  message_bus->RegisterClientAsReceiver(control_thread_id, 2);
  message_bus->RegisterClientAsReceiver(control_thread_id, 3);
  message_bus->RegisterClientAsSender(control_thread_id, 3);
  message_bus->RegisterClientAsSender(control_thread_id, 4);
  std::cout << "Controller online and ready...\n";
  for (;;) {
    tmb::AnnotatedMessage control_message = message_bus->Receive(
        control_thread_id, 0 , true);
    tmb::client_id coordinator_id = control_message.sender;
    if (control_message.tagged_message.message_type() != 2) {
      std::cerr << "ERROR: Unexpected control message type.\n";
      return 1;
    }

    tmb::Address coordinator_address;
    coordinator_address.AddRecipient(coordinator_id);

    tmb::MessageStyle style;

    tmbbench::RunDescription run_desc
        = *static_cast<const tmbbench::RunDescription*>(
            control_message.tagged_message.message());
    std::cout << "Spinning up " << run_desc.num_senders << " senders and "
              << run_desc.num_receivers << " receivers on this node.\n";

    std::vector<std::unique_ptr<tmbbench::SenderThread>> sender_threads;
    std::vector<std::unique_ptr<tmbbench::ReceiverThreadBase>>
        receiver_threads;
    std::vector<tmb::client_id> receiver_ids;
    for (int i = 0; i < run_desc.num_receivers; ++i) {
      if (run_desc.delete_immediately) {
        receiver_threads.emplace_back(
            new tmbbench::ReceiverThread<true>(message_bus.get(),
                                               std::vector<int>()));
      } else {
        receiver_threads.emplace_back(
            new tmbbench::ReceiverThread<false>(message_bus.get(),
                                                std::vector<int>()));
      }
      receiver_ids.push_back(receiver_threads.back()->GetClientID());
    }
    for (int i = 0; i < run_desc.num_senders; ++i) {
      sender_threads.emplace_back(new tmbbench::SenderThread(
          message_bus.get(),
          run_desc.message_bytes,
          std::vector<int>()));
    }

    std::cout << "Sending receiver IDs to coordinator... ";
    tmb::TaggedMessage receivers_msg(
        receiver_ids.data(),
        receiver_ids.size() * sizeof(tmb::client_id),
        3);
    tmb::MessageBus::SendStatus status = message_bus->Send(
        control_thread_id,
        coordinator_address,
        style,
        std::move(receivers_msg));
    if (status == tmb::MessageBus::SendStatus::kOK) {
      std::cout << "OK\n";
    } else {
      std::cerr << "ERROR\n";
      return 1;
    }

    control_message = message_bus->Receive(control_thread_id, 0, true);
    if (control_message.sender != coordinator_id) {
      std::cerr << "ERROR: Coordinator message from unexpected client\n";
      return 1;
    }
    if (control_message.tagged_message.message_type() != 3) {
      std::cerr << "ERROR: Unexpected control message type.\n";
      return 1;
    }

    std::cout << "Setting up sender threads with global receiver list.\n";
    receiver_ids.assign(
        static_cast<const tmb::client_id*>(
            control_message.tagged_message.message()),
        reinterpret_cast<const tmb::client_id*>(
            static_cast<const char*>(control_message.tagged_message.message())
                + control_message.tagged_message.message_bytes()));
    for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
         : sender_threads) {
      sender_thread->SetReceiverIDs(receiver_ids);
    }

    std::cout << "Starting local threads.\n";
    for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
         : sender_threads) {
      sender_thread->Start();
    }
    for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
         : receiver_threads) {
      receiver_thread->Start();
    }

    for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
         : sender_threads) {
      sender_thread->Join();
    }
    for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
         : receiver_threads) {
      receiver_thread->Join();
    }

    std::cout << "All local threads finished\n";
    tmbbench::ThroughputResult result;
    result.send_throughput = 0.0;
    result.receive_throughput = 0.0;
    for (const std::unique_ptr<tmbbench::SenderThread> &sender_thread
         : sender_threads) {
      result.send_throughput += sender_thread->GetThroughput();
    }
    for (const std::unique_ptr<tmbbench::ReceiverThreadBase> &receiver_thread
         : receiver_threads) {
      result.receive_throughput += receiver_thread->GetThroughput();
    }
    std::cout << "Locally measured throughput: Send: "
              << result.send_throughput << " messages/s, Receive: "
              << result.receive_throughput << " messages/s\n";

    std::cout << "Sending results to coordinator... ";
    tmb::TaggedMessage result_msg(&result, sizeof(result), 4);
    status = message_bus->Send(control_thread_id,
                               coordinator_address,
                               style,
                               std::move(result_msg));
    if (status == tmb::MessageBus::SendStatus::kOK) {
      std::cout << "OK\n";
    } else {
      std::cerr << "ERROR\n";
      return 1;
    }
    std::cout << "Ready for next experiment...\n";
  }

  return 0;
}
