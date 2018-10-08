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

#ifndef TESTS_MESSAGE_BUS_UNITTEST_COMMON_H_
#define TESTS_MESSAGE_BUS_UNITTEST_COMMON_H_

#include <algorithm>
#include <atomic>
#include <bitset>
#include <chrono>  // NOLINT(build/c++11)
#include <cstddef>
#include <cstring>
#include <limits>
#include <memory>
#include <thread>  // NOLINT(build/c++11)
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "gtest/gtest.h"

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

namespace tmb {

constexpr int kNumSampleMessages = 64;
constexpr std::size_t kNumClients = 64;
constexpr std::size_t kNumThreads = 16;

TaggedMessage MakeTaggedSimpleIntMessage(int int_payload,
                                         message_type_id type = 0) {
  return TaggedMessage(&int_payload, sizeof(int_payload), type);
}

// Object-oriented wrapper for std::thread.
class Thread {
 public:
  Thread() {
  }

  virtual ~Thread() {}

  void Start() {
    internal_thread_ = std::thread(Thread::ExecuteRunMethodOfThread, this);
  }

  void Join() {
    internal_thread_.join();
  }

 protected:
  static void ExecuteRunMethodOfThread(Thread* thread) {
    thread->Run();
  }

  virtual void Run() = 0;

 private:
  std::thread internal_thread_;

  // Disallow copy and assign:
  Thread(const Thread &orig) = delete;
  Thread& operator=(const Thread &rhs) = delete;
};

template <typename MessageBusT>
class MessageBusTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    message_bus_.reset(CreateBus());
    ASSERT_TRUE(message_bus_->Initialize());
    message_bus_->ResetBus();
  }

  MessageBus* CreateBus() {
    return new MessageBusT;
  }

  std::unique_ptr<MessageBus> message_bus_;
};

TYPED_TEST_CASE_P(MessageBusTest);

// An alias used for test cases which require support for deleting
// messages separately from receiving them.
template <typename MessageBusT> using SeparateDeletionMessageBusTest
    = MessageBusTest<MessageBusT>;
TYPED_TEST_CASE_P(SeparateDeletionMessageBusTest);

// Alias for test cases which require the ability to reconnect to a persistent
// message bus.
template <typename MessageBusT> using ReconnectMessageBusTest
    = MessageBusTest<MessageBusT>;
TYPED_TEST_CASE_P(ReconnectMessageBusTest);

// Alias for test cases which require the ability to reconnect to a persistent
// message bus, and for which "active" state objects (e.g. CancellationTokens
// and AnnotatedMessages) remain valid across different instances of the
// MessageBus.
template <typename MessageBusT> using ReconnectActiveStateMessageBusTest
    = MessageBusTest<MessageBusT>;
TYPED_TEST_CASE_P(ReconnectActiveStateMessageBusTest);

// Alias for test cases which require both separate-deletion support and active
// state objects that remain valid across different instances of the
// MessageBus.
template <typename MessageBusT> using ReconnectSeparateDeletionMessageBusTest
    = MessageBusTest<MessageBusT>;
TYPED_TEST_CASE_P(ReconnectSeparateDeletionMessageBusTest);

// Alias for test cases which require the ability for multiple instances of a
// MessageBus class to present interfaces to the same underlying bus.
template <typename MessageBusT> using MultiInstanceMessageBusTest
    = MessageBusTest<MessageBusT>;
TYPED_TEST_CASE_P(MultiInstanceMessageBusTest);


TYPED_TEST_P(MessageBusTest, ConnectTest) {
  std::unordered_set<client_id> assigned_ids;
  client_id highest_id = std::numeric_limits<client_id>::min();

  // Connect a bunch of clients and make sure their IDs don't collide.
  for (std::size_t client_idx = 0; client_idx < kNumClients; ++client_idx) {
    client_id current_client = this->message_bus_->Connect();
    if (current_client > highest_id) {
      highest_id = current_client;
    }
    EXPECT_EQ(assigned_ids.end(), assigned_ids.find(current_client));
    EXPECT_TRUE(assigned_ids.insert(current_client).second);
  }

  // Disconnect half of the clients.
  ASSERT_EQ(kNumClients, assigned_ids.size());
  std::unordered_set<client_id>::const_iterator client_it
      = assigned_ids.begin();
  for (std::size_t client_idx = 0;
       client_idx < kNumClients / 2;
       ++client_idx) {
    ASSERT_NE(client_it, assigned_ids.end());
    EXPECT_TRUE(this->message_bus_->Disconnect(*client_it));
    ++client_it;
  }

  // Attempting to disconnect the same clients again should fail.
  client_it = assigned_ids.begin();
  for (std::size_t client_idx = 0;
       client_idx < kNumClients / 2;
       ++client_idx) {
    ASSERT_NE(client_it, assigned_ids.end());
    EXPECT_FALSE(this->message_bus_->Disconnect(*client_it));
    ++client_it;
  }

  // Connect some more new clients. IDs should not be reused.
  for (std::size_t client_idx = 0;
       client_idx < kNumClients / 2;
       ++client_idx) {
    client_id current_client = this->message_bus_->Connect();
    if (current_client > highest_id) {
      highest_id = current_client;
    }
    EXPECT_EQ(assigned_ids.end(), assigned_ids.find(current_client));
    EXPECT_TRUE(assigned_ids.insert(current_client).second);
  }

  // Can not disconnect a client which was never connected.
  EXPECT_FALSE(this->message_bus_->Disconnect(highest_id + 1));
}


class ConnectorThread : public Thread {
 public:
  ConnectorThread(MessageBus *message_bus_ptr, std::size_t num_clients)
      : message_bus_ptr_(message_bus_ptr),
        num_clients_(num_clients) {
  }

  ~ConnectorThread() override {};

  const std::unordered_set<client_id>& assigned_ids() const {
    return assigned_ids_;
  }

  client_id last_connected_id() const {
    return last_connected_id_;
  }

 protected:
  void Run() override {
    for (std::size_t client_idx = 0; client_idx < num_clients_; ++client_idx) {
      client_id current_client = message_bus_ptr_->Connect();
      EXPECT_EQ(assigned_ids_.end(), assigned_ids_.find(current_client));
      EXPECT_TRUE(assigned_ids_.insert(current_client).second);
    }

    // Disconnect half of the clients.
    ASSERT_EQ(num_clients_, assigned_ids_.size());
    std::unordered_set<client_id>::const_iterator client_it
        = assigned_ids_.begin();
    for (std::size_t client_idx = 0;
         client_idx < num_clients_ / 2;
         ++client_idx) {
      ASSERT_NE(client_it, assigned_ids_.end());
      EXPECT_TRUE(message_bus_ptr_->Disconnect(*client_it));
      ++client_it;
    }

    // Attempting to disconnect the same clients again should fail.
    client_it = assigned_ids_.begin();
    for (std::size_t client_idx = 0;
         client_idx < num_clients_ / 2;
         ++client_idx) {
      ASSERT_NE(client_it, assigned_ids_.end());
      EXPECT_FALSE(message_bus_ptr_->Disconnect(*client_it));
      ++client_it;
    }

    // Connect some more new clients. IDs should not be reused.
    for (std::size_t client_idx = 0;
         client_idx < num_clients_ / 2;
         ++client_idx) {
      last_connected_id_ = message_bus_ptr_->Connect();
      EXPECT_EQ(assigned_ids_.end(), assigned_ids_.find(last_connected_id_));
      EXPECT_TRUE(assigned_ids_.insert(last_connected_id_).second);
    }
  }

 private:
  MessageBus *message_bus_ptr_;
  const std::size_t num_clients_;
  std::unordered_set<client_id> assigned_ids_;
  client_id last_connected_id_;
};

TYPED_TEST_P(MessageBusTest, ThreadedConnectTest) {
  // Set up multiple concurrent threads.
  std::vector<std::unique_ptr<ConnectorThread>> threads;
  for (std::size_t thread_idx = 0; thread_idx < kNumThreads; ++thread_idx) {
    threads.emplace_back(new ConnectorThread(this->message_bus_.get(),
                                             kNumClients));
  }

  // Start the threads, then wait for all of them to finish.
  for (auto &thread : threads) {
    thread->Start();
  }
  for (auto &thread : threads) {
    thread->Join();
  }

  // Make sure that none of the threads got duplicate ids.
  std::unordered_set<client_id> global_assigned_ids;
  client_id highest_id = std::numeric_limits<client_id>::min();
  for (const auto &thread : threads) {
    for (const auto &client : thread->assigned_ids()) {
      EXPECT_EQ(global_assigned_ids.end(), global_assigned_ids.find(client));
      EXPECT_TRUE(global_assigned_ids.insert(client).second);
      if (client > highest_id) {
        highest_id = client;
      }
    }
  }
  EXPECT_EQ(kNumThreads * (kNumClients + kNumClients / 2),
            global_assigned_ids.size());

  // Can not disconnect a client which was never connected.
  EXPECT_FALSE(this->message_bus_->Disconnect(highest_id + 1));

  // Disconnect some clients that were first connected in other threads.
  for (const auto &thread : threads) {
    EXPECT_TRUE(this->message_bus_->Disconnect(thread->last_connected_id()));
  }
}


TYPED_TEST_P(MessageBusTest, RegisterAndSendTest) {
  client_id sender_id = this->message_bus_->Connect();

  Address addr;
  MessageStyle style;

  // Attempt to send a message which we are not registered to send.
  EXPECT_EQ(MessageBus::SendStatus::kSenderNotRegisteredForMessageType,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Attempt to send from a non-connected client.
  EXPECT_EQ(MessageBus::SendStatus::kSenderNotConnected,
            this->message_bus_->Send(sender_id + 1,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Register client as a sender.
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));
  // Registering the same client twice for the same message type should fail.
  EXPECT_FALSE(this->message_bus_->RegisterClientAsSender(sender_id, 0));
  // Registering for a different message type is OK.
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 1));
  // Can't register a client which is not connected.
  EXPECT_FALSE(this->message_bus_->RegisterClientAsSender(sender_id + 1, 0));

  // Try sending with an empty Address.
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Connect a receiver.
  client_id receiver_id = this->message_bus_->Connect();

  // Can't send to a receiver which is not registered for the message type.
  addr.AddRecipient(receiver_id);
  EXPECT_EQ(MessageBus::SendStatus::kReceiverNotRegisteredForMessageType,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Register the receiver.
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));
  // Registering twice should fail.
  EXPECT_FALSE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));
  // Registering for a different message type is OK.
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 2));
  // Can't register a client which is not connected.
  EXPECT_FALSE(
      this->message_bus_->RegisterClientAsReceiver(receiver_id + sender_id + 1,
                                                   0));

  // Sending directly should be OK now.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Still can't send a message type that the receiver is not registered for.
  EXPECT_EQ(MessageBus::SendStatus::kReceiverNotRegisteredForMessageType,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42, 1)));

  // Send to all possible recipients instead of an explicitly specified one.
  addr.All(true);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Send-to-all with no possible recipients.
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42, 1)));

  // Send-to-all without first registering as a sender for the message type.
  EXPECT_EQ(MessageBus::SendStatus::kSenderNotRegisteredForMessageType,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42, 2)));

  // Disconnect receiver.
  EXPECT_TRUE(this->message_bus_->Disconnect(receiver_id));

  addr.All(false);
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));
  addr.All(true);
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Now register sender for message type '2', which the now-disconnected
  // receiver was previously registered for.
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 2));
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42, 2)));

  // Connect a second receiver.
  client_id other_receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(other_receiver_id,
                                                           0));

  addr.All(false);
  addr.AddRecipient(other_receiver_id);
  // 'addr' now contains 2 explicit recipients, only one of which is connected.

  // OK to send as long as at least 1 receiver is still connected.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Also check that send-to-all still works as expected.
  addr.All(true);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Now disconnect the second receiver.
  EXPECT_TRUE(this->message_bus_->Disconnect(other_receiver_id));

  addr.All(false);
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));
  addr.All(true);
  EXPECT_EQ(MessageBus::SendStatus::kNoReceivers,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Disconnect the sender and try to send.
  EXPECT_TRUE(this->message_bus_->Disconnect(sender_id));
  EXPECT_EQ(MessageBus::SendStatus::kSenderNotConnected,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Trying to register a disconnected client to send or receive other types
  // should also fail.
  EXPECT_FALSE(this->message_bus_->RegisterClientAsSender(sender_id, 3));
  EXPECT_FALSE(this->message_bus_->RegisterClientAsReceiver(sender_id, 3));
}


TYPED_TEST_P(MessageBusTest, SimpleSendAndReceiveTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  for (int value = 0; value < kNumSampleMessages; ++value) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(value)));
  }

  // Check that each of the sent messages was received.
  std::bitset<kNumSampleMessages> check_set;
  AnnotatedMessage received_msg;
  while (this->message_bus_->ReceiveIfAvailable(receiver_id, &received_msg)) {
    EXPECT_EQ(sender_id, received_msg.sender);
    EXPECT_EQ(0u, received_msg.tagged_message.message_type());
    ASSERT_EQ(sizeof(int), received_msg.tagged_message.message_bytes());
    int payload
        = *static_cast<const int*>(received_msg.tagged_message.message());
    ASSERT_GE(payload, 0);
    ASSERT_LT(payload, kNumSampleMessages);
    check_set.set(payload, true);

    this->message_bus_->DeleteMessage(receiver_id, received_msg);
  }
  EXPECT_TRUE(check_set.all());
}


// Test sending of messages too large to be represented in-line in a
// TaggedMessage.
TYPED_TEST_P(MessageBusTest, LargeMessageSimpleSendAndReceiveTest) {
  const char message1[]
      = "They say there is no hope\n"
        "They say no U.F.O.s\n"
        "Why is no head held high?\n"
        "Maybe you'll see them fly.\n";

  const char message2[]
      = "Well ... I'm driving in a black on black in black Porsche 924\n"
        "Tempting fate for a little bit more\n"
        "My head is filled with Techno beat, Metro Times, Face magazine\n"
        "Shadows out of time and space\n"
        "TIME/SPACE/TRANSMAT\n";

  const char message3[]
      = "For those who know it's time to leave the house\n"
        "and go back to the field\n"
        "Find your strength in the sound and make your transition\n";


  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  TaggedMessage msg(message1, std::strlen(message1) + 1, 0);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     std::move(msg)));

  msg = TaggedMessage(message2, std::strlen(message2) + 1, 0);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     std::move(msg)));

  msg = TaggedMessage(message3, std::strlen(message3) + 1, 0);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     std::move(msg)));

  std::vector<AnnotatedMessage> received_messages;
  EXPECT_EQ(3u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_id,
                                                        &received_messages));
  ASSERT_EQ(3u, received_messages.size());

  EXPECT_EQ(sender_id, received_messages[0].sender);
  EXPECT_EQ(0u, received_messages[0].tagged_message.message_type());
  EXPECT_EQ(std::strlen(message1) + 1,
            received_messages[0].tagged_message.message_bytes());
  EXPECT_STREQ(message1,
               static_cast<const char*>(
                  received_messages[0].tagged_message.message()));

  EXPECT_EQ(sender_id, received_messages[1].sender);
  EXPECT_EQ(0u, received_messages[1].tagged_message.message_type());
  EXPECT_EQ(std::strlen(message2) + 1,
            received_messages[1].tagged_message.message_bytes());
  EXPECT_STREQ(message2,
               static_cast<const char*>(
                  received_messages[1].tagged_message.message()));

  EXPECT_EQ(sender_id, received_messages[2].sender);
  EXPECT_EQ(0u, received_messages[2].tagged_message.message_type());
  EXPECT_EQ(std::strlen(message3) + 1,
            received_messages[2].tagged_message.message_bytes());
  EXPECT_STREQ(message3,
               static_cast<const char*>(
                  received_messages[2].tagged_message.message()));
}


// Similar to SimpleSendAndReceiveTest above, but uses the blocking Receive()
// call and interleaves sending with receiving.
TYPED_TEST_P(MessageBusTest, InterleavedBlockingSimpleSendAndReceiveTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  for (int value = 0; value < kNumSampleMessages; ++value) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(value)));

    AnnotatedMessage received_msg;
    // Alternate between deleting immediately and in a seperate call.
    if (value & 0x1) {
      received_msg = this->message_bus_->Receive(receiver_id, 0, true);
    } else {
      received_msg = this->message_bus_->Receive(receiver_id, 0, false);
    }

    EXPECT_EQ(sender_id, received_msg.sender);
    EXPECT_EQ(0u, received_msg.tagged_message.message_type());
    ASSERT_EQ(sizeof(int), received_msg.tagged_message.message_bytes());
    int payload
        = *static_cast<const int*>(received_msg.tagged_message.message());
    EXPECT_EQ(value, payload);

    if (!(value & 0x1)) {
      this->message_bus_->DeleteMessage(receiver_id, received_msg);
    }
  }

  // Send and receive an extra message that is not deleted (this checks that
  // garbage collection of leftover watch contexts works properly in the
  // Zookeeper implementation).
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));
  AnnotatedMessage final_msg
      = this->message_bus_->Receive(receiver_id, 0, false);
  EXPECT_EQ(sender_id, final_msg.sender);
  EXPECT_EQ(0u, final_msg.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), final_msg.tagged_message.message_bytes());
  int payload = *static_cast<const int*>(final_msg.tagged_message.message());
  EXPECT_EQ(42, payload);
}


class SimpleReceiverThread : public Thread {
 public:
  SimpleReceiverThread(MessageBus *message_bus_ptr,
                       const client_id sender_id)
      : message_bus_ptr_(message_bus_ptr),
        sender_id_(sender_id),
        receiver_id_(sender_id) {
  }

  ~SimpleReceiverThread() override {};

  // Spins until the receiver id is actually available.
  client_id GetReceiverID() const {
    client_id receiver_id = receiver_id_.load();
    while (receiver_id == sender_id_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      receiver_id = receiver_id_.load();
    }
    return receiver_id;
  }

  bool AllMessagesReceived() const {
    return received_set_.all();
  }

 protected:
  void Run() override {
    const client_id receiver_id = message_bus_ptr_->Connect();
    EXPECT_TRUE(message_bus_ptr_->RegisterClientAsReceiver(receiver_id, 0));
    receiver_id_.store(receiver_id);
    for (std::size_t message_num = 0;
         message_num < kNumSampleMessages;
         ++message_num) {
      AnnotatedMessage received_msg = message_bus_ptr_->Receive(receiver_id);
      EXPECT_EQ(sender_id_, received_msg.sender);
      EXPECT_EQ(0u, received_msg.tagged_message.message_type());
      ASSERT_EQ(sizeof(int), received_msg.tagged_message.message_bytes());
      int payload
          = *static_cast<const int*>(received_msg.tagged_message.message());
      ASSERT_GE(payload, 0);
      ASSERT_LT(payload, kNumSampleMessages);
      received_set_.set(payload, true);

      message_bus_ptr_->DeleteMessage(receiver_id, received_msg);
    }
    message_bus_ptr_->Disconnect(receiver_id);
  }

 private:
  MessageBus *message_bus_ptr_;
  const client_id sender_id_;
  std::atomic<client_id> receiver_id_;
  std::bitset<kNumSampleMessages> received_set_;
};

TYPED_TEST_P(MessageBusTest, ThreadedSimpleSendAndReceiveTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  SimpleReceiverThread receiver_thread(this->message_bus_.get(), sender_id);
  receiver_thread.Start();
  client_id receiver_id = receiver_thread.GetReceiverID();

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  for (int value = 0; value < kNumSampleMessages; ++value) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(value)));
    // Yield every 16 messages.
    if ((value & 0xF) == 0) {
      std::this_thread::yield();
    }
  }

  receiver_thread.Join();
  EXPECT_TRUE(receiver_thread.AllMessagesReceived());
}


TYPED_TEST_P(MessageBusTest, ThreadedMultipleReceiversTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  // Create and start receiver threads.
  std::vector<std::unique_ptr<SimpleReceiverThread>> threads;
  for (std::size_t thread_idx = 0; thread_idx < kNumThreads; ++thread_idx) {
    threads.emplace_back(new SimpleReceiverThread(this->message_bus_.get(),
                                                  sender_id));
  }
  for (auto &thread : threads) {
    thread->Start();
  }

  Address addr;
  for (auto &thread : threads) {
    addr.AddRecipient(thread->GetReceiverID());
  }
  MessageStyle style;
  style.Broadcast(true);

  for (int value = 0; value < kNumSampleMessages; ++value) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(value)));
    // Yield every 16 messages.
    if ((value & 0xF) == 0) {
      std::this_thread::yield();
    }
  }

  for (auto &thread : threads) {
    thread->Join();
  }
  for (auto &thread : threads) {
    EXPECT_TRUE(thread->AllMessagesReceived());
  }
}


class SimpleSenderThread : public Thread {
 public:
  SimpleSenderThread(MessageBus *message_bus_ptr,
                     const client_id receiver_id)
      : message_bus_ptr_(message_bus_ptr),
        receiver_id_(receiver_id),
        sender_id_(receiver_id) {
  }

  ~SimpleSenderThread() override {}

  // Spins until the sender id is actually available.
  client_id GetSenderID() const {
    client_id sender_id = sender_id_.load();
    while (sender_id == receiver_id_) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      sender_id = sender_id_.load();
    }
    return sender_id;
  }

 protected:
  void Run() override {
    const client_id sender_id = message_bus_ptr_->Connect();
    EXPECT_TRUE(message_bus_ptr_->RegisterClientAsSender(sender_id, 0));
    sender_id_.store(sender_id);

    Address addr;
    addr.AddRecipient(receiver_id_);
    MessageStyle style;
    for (std::size_t message_num = 0;
         message_num < kNumSampleMessages;
         ++message_num) {
      EXPECT_EQ(
          MessageBus::SendStatus::kOK,
          message_bus_ptr_->Send(sender_id,
                                 addr,
                                 style,
                                 MakeTaggedSimpleIntMessage(message_num)));
    }
    message_bus_ptr_->Disconnect(sender_id);
  }

 private:
  MessageBus *message_bus_ptr_;
  const client_id receiver_id_;
  std::atomic<client_id> sender_id_;
};

TYPED_TEST_P(MessageBusTest, ThreadedMultipleSendersTest) {
  // Receive messages in the main thread.
  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  // Create and start sender threads.
  std::vector<std::unique_ptr<SimpleSenderThread>> threads;
  for (std::size_t thread_idx = 0; thread_idx < kNumThreads; ++thread_idx) {
    threads.emplace_back(new SimpleSenderThread(this->message_bus_.get(),
                                                receiver_id));
  }
  for (auto &thread : threads) {
    thread->Start();
  }

  // Set up data structure to check that all expected messages are received.
  std::unordered_map<client_id, std::bitset<kNumSampleMessages>> received_sets;
  for (auto &thread : threads) {
    received_sets.emplace(thread->GetSenderID(),
                          std::bitset<kNumSampleMessages>());
  }

  // Receive all the expected messages.
  for (std::size_t received_num = 0;
       received_num < kNumThreads * kNumSampleMessages;
       ++received_num) {
    AnnotatedMessage received_message
        = this->message_bus_->Receive(receiver_id);
    EXPECT_EQ(0u, received_message.tagged_message.message_type());
    ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
    int payload
        = *static_cast<const int*>(received_message.tagged_message.message());
    ASSERT_GE(payload, 0);
    ASSERT_LT(payload, kNumSampleMessages);

    ASSERT_NE(received_sets.find(received_message.sender),
              received_sets.end());
    EXPECT_FALSE(received_sets[received_message.sender][payload]);
    received_sets[received_message.sender].set(payload, true);

    this->message_bus_->DeleteMessage(receiver_id, received_message);
  }

  // No more messages should be recieved after the above loop.
  for (auto &thread : threads) {
    thread->Join();
  }
  AnnotatedMessage received_message;
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                      &received_message));

  // Check that all expected messages have been received.
  for (const auto &received_pair : received_sets) {
    EXPECT_TRUE(received_pair.second.all());
  }
}


TYPED_TEST_P(MessageBusTest, ReceiveBatchTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  std::vector<AnnotatedMessage> received_messages;

  // Initial attempt to receive should give 0 messages.
  EXPECT_EQ(0u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_id,
                                                        &received_messages));
  EXPECT_EQ(0u, received_messages.size());

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  // Send a few messages.
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(i)));
  }

  // Receive a batch of limited size.
  EXPECT_EQ(8u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_id,
                                                        &received_messages,
                                                        0,
                                                        8));
  EXPECT_EQ(8u, received_messages.size());
  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(static_cast<int>(i), payload);
  }

  this->message_bus_->DeleteMessages(receiver_id,
                                     received_messages.begin(),
                                     received_messages.end());
  received_messages.clear();

  // Receive a single message.
  AnnotatedMessage single_message;
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &single_message));
  EXPECT_EQ(sender_id, single_message.sender);
  EXPECT_EQ(0u, single_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), single_message.tagged_message.message_bytes());
  EXPECT_EQ(8,
            *static_cast<const int*>(single_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, single_message);

  // Receive a batch via the blocking interface.
  EXPECT_EQ(4u,
            this->message_bus_->ReceiveBatch(receiver_id,
                                             &received_messages,
                                             0,
                                             4));
  EXPECT_EQ(4u, received_messages.size());
  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(static_cast<int>(i + 9), payload);
  }
  this->message_bus_->DeleteMessages(receiver_id,
                                     received_messages.begin(),
                                     received_messages.end());

  // Append messages to a container that already has some in it.
  EXPECT_EQ(3u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_id,
                                                        &received_messages));
  EXPECT_EQ(7u, received_messages.size());
  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(static_cast<int>(i + 9), payload);
  }

  // Double-deletion of the first four elements is a no-op.
  this->message_bus_->DeleteMessages(receiver_id,
                                     received_messages.begin(),
                                     received_messages.end());
}


TYPED_TEST_P(MessageBusTest, PriorityTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  // Fill up a vector with all possible priority levels, then shuffle the
  // order.
  std::vector<Priority> priorities;
  for (Priority priority = 0; priority <= kMaxAsyncPriority; ++priority) {
    priorities.push_back(priority);
  }
  priorities.push_back(kSyncResponsePriority);
  std::random_shuffle(priorities.begin(), priorities.end());

  // Send messages at each priority level in random order.
  for (const Priority &priority : priorities) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(priority),
                                       priority));
  }

  // Receiver should get messages in descending order of priority.
  Priority expected_priority = kSyncResponsePriority;
  AnnotatedMessage received_msg;
  while (this->message_bus_->ReceiveIfAvailable(receiver_id, &received_msg)) {
    EXPECT_EQ(sender_id, received_msg.sender);
    EXPECT_EQ(0u, received_msg.tagged_message.message_type());
    ASSERT_EQ(sizeof(int), received_msg.tagged_message.message_bytes());
    int payload
        = *static_cast<const int*>(received_msg.tagged_message.message());
    EXPECT_EQ(expected_priority, payload);
    this->message_bus_->DeleteMessage(receiver_id, received_msg);
    if (expected_priority > 0) {
      --expected_priority;
    } else {
      // Should be finished, now.
      EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                          &received_msg));
    }
  }
  EXPECT_EQ(0, expected_priority);
}


TYPED_TEST_P(MessageBusTest, TimeoutTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);

  MessageStyle style;

  // Send a message that expires right away.
  style.Timeout(std::chrono::high_resolution_clock::now());
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(0)));

  // Send a message that expires in 2000 seconds.
  style.Timeout(std::chrono::high_resolution_clock::now()
                + std::chrono::seconds(2000));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(1)));

  // Send a message that expires in 1000 seconds.
  style.Timeout(std::chrono::high_resolution_clock::now()
                + std::chrono::seconds(1000));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(2)));

  // Sleep briefly.
  std::this_thread::sleep_for(std::chrono::seconds(2));

  AnnotatedMessage received_message;

  // Should receive the message with the earlier deadline first.
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(2, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(1, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  // The expired message should never be received.
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                      &received_message));
}


TYPED_TEST_P(MessageBusTest, SendOrderTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);

  MessageStyle style;

  // Send a few messages at default priority with no timeout.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(0)));

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(1)));

  // Messages at higher priority.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(2),
                                     kDefaultPriority + 1));

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(3),
                                     kDefaultPriority + 1));

  // Messages with a timeout.
  MessageStyle timeout_style;
  timeout_style.Timeout(std::chrono::high_resolution_clock::now()
                        + std::chrono::seconds(1000));

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     timeout_style,
                                     MakeTaggedSimpleIntMessage(4)));

  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     timeout_style,
                                     MakeTaggedSimpleIntMessage(5)));

  // Another one of each set of messages.
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(6)));

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(7),
                                     kDefaultPriority + 1));

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     timeout_style,
                                     MakeTaggedSimpleIntMessage(8)));

  // Check that messages are received in expected order. Ordering is determined
  // by priority, then deadline, then send time.
  AnnotatedMessage received_message;

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(2, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(3, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(7, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(4, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(5, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(8, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(0, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(1, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_message));
  EXPECT_EQ(sender_id, received_message.sender);
  EXPECT_EQ(0u, received_message.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
  EXPECT_EQ(6, *static_cast<const int*>(
                   received_message.tagged_message.message()));
  this->message_bus_->DeleteMessage(receiver_id, received_message);

  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                      &received_message));
}


TYPED_TEST_P(MessageBusTest, BroadcastTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  // A receiver which gets every message.
  client_id receiver_id_1 = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id_1, 0));

  // A receiver which gets every even-numbered message.
  client_id receiver_id_2 = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id_2, 0));

  // A receiver which gets every third message.
  client_id receiver_id_3 = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id_3, 0));

  // A receiver which gets every fifth message.
  client_id receiver_id_5 = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id_5, 0));

  // Send out broadcast messages.
  MessageStyle style;
  style.Broadcast(true);
  for (int message_num = 0;
       message_num < kNumSampleMessages;
       ++message_num) {
    Address addr;
    addr.AddRecipient(receiver_id_1);
    if (message_num % 2 == 0) {
      addr.AddRecipient(receiver_id_2);
    }
    if (message_num % 3 == 0) {
      addr.AddRecipient(receiver_id_3);
    }
    if (message_num % 5 == 0) {
      addr.AddRecipient(receiver_id_5);
    }

    EXPECT_EQ(
        MessageBus::SendStatus::kOK,
        this->message_bus_->Send(sender_id,
                                 addr,
                                 style,
                                 MakeTaggedSimpleIntMessage(message_num)));
  }

  AnnotatedMessage received_message;

  std::bitset<kNumSampleMessages> received_set_1;
  for (int received_message_num = 0;
       received_message_num < kNumSampleMessages;
       ++received_message_num) {
    EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id_1,
                                                       &received_message));
    EXPECT_EQ(sender_id, received_message.sender);
    EXPECT_EQ(0u, received_message.tagged_message.message_type());

    ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_message.tagged_message.message());
    ASSERT_GE(payload, 0);
    ASSERT_LT(payload, kNumSampleMessages);
    received_set_1.set(payload, true);

    this->message_bus_->DeleteMessage(receiver_id_1, received_message);
  }
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id_1,
                                                      &received_message));
  EXPECT_TRUE(received_set_1.all());

  std::bitset<(kNumSampleMessages + 1) / 2> received_set_2;
  for (int received_message_num = 0;
       received_message_num < (kNumSampleMessages + 1) / 2;
       ++received_message_num) {
    EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id_2,
                                                       &received_message));
    EXPECT_EQ(sender_id, received_message.sender);
    EXPECT_EQ(0u, received_message.tagged_message.message_type());

    ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_message.tagged_message.message());
    ASSERT_GE(payload, 0);
    ASSERT_LT(payload, kNumSampleMessages);
    ASSERT_EQ(0, payload % 2);
    received_set_2.set(payload / 2, true);

    this->message_bus_->DeleteMessage(receiver_id_2, received_message);
  }
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id_2,
                                                      &received_message));
  EXPECT_TRUE(received_set_2.all());

  std::bitset<(kNumSampleMessages + 2) / 3> received_set_3;
  for (int received_message_num = 0;
       received_message_num < (kNumSampleMessages + 2) / 3;
       ++received_message_num) {
    EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id_3,
                                                       &received_message));
    EXPECT_EQ(sender_id, received_message.sender);
    EXPECT_EQ(0u, received_message.tagged_message.message_type());

    ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_message.tagged_message.message());
    ASSERT_GE(payload, 0);
    ASSERT_LT(payload, kNumSampleMessages);
    ASSERT_EQ(0, payload % 3);
    received_set_3.set(payload / 3, true);

    this->message_bus_->DeleteMessage(receiver_id_3, received_message);
  }
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id_3,
                                                      &received_message));
  EXPECT_TRUE(received_set_3.all());

  std::bitset<(kNumSampleMessages + 4) / 5> received_set_5;
  for (int received_message_num = 0;
       received_message_num < (kNumSampleMessages + 4) / 5;
       ++received_message_num) {
    EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id_5,
                                                       &received_message));
    EXPECT_EQ(sender_id, received_message.sender);
    EXPECT_EQ(0u, received_message.tagged_message.message_type());

    ASSERT_EQ(sizeof(int), received_message.tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_message.tagged_message.message());
    ASSERT_GE(payload, 0);
    ASSERT_LT(payload, kNumSampleMessages);
    ASSERT_EQ(0, payload % 5);
    received_set_5.set(payload / 5, true);

    this->message_bus_->DeleteMessage(receiver_id_5, received_message);
  }
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id_5,
                                                      &received_message));
  EXPECT_TRUE(received_set_5.all());
}


TYPED_TEST_P(MessageBusTest, CancelTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_a_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_a_id, 0));

  client_id receiver_b_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_b_id, 0));

  Address addr;
  MessageStyle style;

  // Send 3 messages, then cancel the middle one.
  addr.AddRecipient(receiver_a_id);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(0),
                                     kDefaultPriority,
                                     nullptr));
  CancellationToken token;
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(1),
                                     kDefaultPriority,
                                     &token));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(2),
                                     kDefaultPriority,
                                     nullptr));

  this->message_bus_->CancelMessage(sender_id, token);

  // Receiver should not get the cancelled message.
  std::vector<AnnotatedMessage> received_messages;
  EXPECT_EQ(2u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages));
  ASSERT_EQ(2u, received_messages.size());

  EXPECT_EQ(sender_id, received_messages.front().sender);
  EXPECT_EQ(0u, received_messages.front().tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_messages.front().tagged_message.message_bytes());
  int payload = *static_cast<const int*>(
      received_messages.front().tagged_message.message());
  EXPECT_EQ(0, payload);

  EXPECT_EQ(sender_id, received_messages.back().sender);
  EXPECT_EQ(0u, received_messages.back().tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_messages.back().tagged_message.message_bytes());
  payload = *static_cast<const int*>(
      received_messages.back().tagged_message.message());
  EXPECT_EQ(2, payload);

  this->message_bus_->DeleteMessages(receiver_a_id,
                                     received_messages.begin(),
                                     received_messages.end());
  received_messages.clear();

  // Send a broadcast message and have a receiver cancel it for its peer.
  addr.AddRecipient(receiver_b_id);
  style.Broadcast(true);

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(3),
                                     kDefaultPriority,
                                     &token));
  EXPECT_EQ(1u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages));
  ASSERT_EQ(1u, received_messages.size());
  EXPECT_EQ(sender_id, received_messages.front().sender);
  EXPECT_EQ(0u, received_messages.front().tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_messages.front().tagged_message.message_bytes());
  payload = *static_cast<const int*>(
      received_messages.front().tagged_message.message());
  EXPECT_EQ(3, payload);

  this->message_bus_->CancelMessage(receiver_a_id, received_messages.front());

  // Other receiver should not get the message.
  AnnotatedMessage message_buf;
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_b_id,
                                                      &message_buf));
  // Cancelling a message also implicitly deletes it for a client which already
  // received it.
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_a_id,
                                                      &message_buf));

  // Attempting to cancel a non-cancellable message does nothing.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(4),
                                     kDefaultPriority,
                                     nullptr));
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_a_id,
                                                     &message_buf));
  EXPECT_EQ(sender_id, message_buf.sender);
  EXPECT_EQ(0u, message_buf.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), message_buf.tagged_message.message_bytes());
  payload = *static_cast<const int*>(message_buf.tagged_message.message());
  EXPECT_EQ(4, payload);
  this->message_bus_->DeleteMessage(receiver_a_id, message_buf);

  this->message_bus_->CancelMessage(receiver_a_id, message_buf);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_b_id,
                                                     &message_buf));
  EXPECT_EQ(sender_id, message_buf.sender);
  EXPECT_EQ(0u, message_buf.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), message_buf.tagged_message.message_bytes());
  payload = *static_cast<const int*>(message_buf.tagged_message.message());
  EXPECT_EQ(4, payload);
  this->message_bus_->DeleteMessage(receiver_b_id, message_buf);

  // Try cancelling a message after it has already been received and deleted.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(5),
                                     kDefaultPriority,
                                     &token));

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_a_id,
                                                     &message_buf));
  EXPECT_EQ(sender_id, message_buf.sender);
  EXPECT_EQ(0u, message_buf.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), message_buf.tagged_message.message_bytes());
  payload = *static_cast<const int*>(message_buf.tagged_message.message());
  EXPECT_EQ(5, payload);
  this->message_bus_->DeleteMessage(receiver_a_id, message_buf);

  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_b_id,
                                                     &message_buf));
  EXPECT_EQ(sender_id, message_buf.sender);
  EXPECT_EQ(0u, message_buf.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), message_buf.tagged_message.message_bytes());
  payload = *static_cast<const int*>(message_buf.tagged_message.message());
  EXPECT_EQ(5, payload);
  this->message_bus_->DeleteMessage(receiver_b_id, message_buf);

  this->message_bus_->CancelMessage(sender_id, token);

  // Test batch-cancellation. Send 2 cancellable messages with a
  // non-cancellable one in the middle.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(6),
                                     kDefaultPriority,
                                     &token));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(7),
                                     kDefaultPriority,
                                     nullptr));
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(8),
                                     kDefaultPriority,
                                     &token));

  received_messages.clear();
  EXPECT_EQ(3u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages));
  this->message_bus_->CancelMessages(receiver_a_id,
                                     received_messages.begin(),
                                     received_messages.end());

  // Other receiver should only get the non-cancellable message.
  received_messages.clear();
  EXPECT_EQ(1u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_b_id,
                                                        &received_messages));
  ASSERT_EQ(1u, received_messages.size());
  EXPECT_EQ(sender_id, received_messages.front().sender);
  EXPECT_EQ(0u, received_messages.front().tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_messages.front().tagged_message.message_bytes());
  payload = *static_cast<const int*>(
      received_messages.front().tagged_message.message());
  EXPECT_EQ(7, payload);
}


TYPED_TEST_P(MessageBusTest, CountQueuedMessagesForClientTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  // Receiver's queue is initially empty.
  EXPECT_EQ(0u, this->message_bus_->CountQueuedMessagesForClient(receiver_id));

  // Now we will enqueue a few messages for the receiver.
  MessageStyle style;
  Address addr;
  addr.AddRecipient(receiver_id);
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(i)));
  }

  EXPECT_EQ(3u, this->message_bus_->CountQueuedMessagesForClient(receiver_id));

  // Receive and delete a single message.
  AnnotatedMessage received_msg;
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_msg,
                                                     0,
                                                     true));
  // 2 messages remain on the queue:
  EXPECT_EQ(2u, this->message_bus_->CountQueuedMessagesForClient(receiver_id));

  // Add and immediately cancel a message at a higher priority than the
  // existing messages.
  CancellationToken token;
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42),
                                     kDefaultPriority + 1,
                                     &token));
  this->message_bus_->CancelMessage(sender_id, token);

  // Depending on the particular implementation of this message bus, a
  // cancelled message may or may not count towards the number in the queue
  // until a call to one of the Receive methods cleans it up.
  EXPECT_TRUE(
      this->message_bus_->CountQueuedMessagesForClient(receiver_id) == 2
      || this->message_bus_->CountQueuedMessagesForClient(receiver_id) == 3);

  // Receive one more message. The cancelled message will be cleaned from the
  // queue no matter what at this point.
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_msg,
                                                     0,
                                                     true));
  EXPECT_EQ(1u, this->message_bus_->CountQueuedMessagesForClient(receiver_id));

  // Send a message that expires immediately.
  style.Timeout(std::chrono::high_resolution_clock::now());
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(121)));
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // Until we receive, an expired message will still count towards the queue's
  // total.
  EXPECT_EQ(2u, this->message_bus_->CountQueuedMessagesForClient(receiver_id));

  // Receive one more message, which should clear out the queue (including the
  // expired message).
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_msg,
                                                     0,
                                                     true));
  EXPECT_EQ(0u, this->message_bus_->CountQueuedMessagesForClient(receiver_id));
}


TYPED_TEST_P(SeparateDeletionMessageBusTest, DeleteTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_a_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_a_id, 0));

  client_id receiver_b_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_b_id, 0));

  // Enqueue several messages for a single receiver.
  MessageStyle style;
  Address addr;
  addr.AddRecipient(receiver_a_id);
  for (int i = 0; i < 16; ++i) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(sender_id,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(i)));
  }

  // Receive a batch of messages without immediately deleting them.
  std::vector<AnnotatedMessage> received_messages;
  EXPECT_EQ(16u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages,
                                                        0,
                                                        0,
                                                        false));

  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(static_cast<int>(i), payload);
  }

  // Receiving again without first deleting will give us the same messages.
  received_messages.clear();
  EXPECT_EQ(16u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages,
                                                        0,
                                                        0,
                                                        false));

  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(static_cast<int>(i), payload);
  }

  // Delete only some of the messages.
  this->message_bus_->DeleteMessages(receiver_a_id,
                                     received_messages.begin(),
                                     received_messages.begin() + 4);

  // Should receive non-deleted messages.
  received_messages.clear();
  EXPECT_EQ(12u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages,
                                                        0,
                                                        0,
                                                        false));

  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(static_cast<int>(i + 4), payload);
  }

  // Delete some messages in an order other than that in which they were
  // received.
  ASSERT_EQ(12u, received_messages.size());
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[8]);
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[4]);
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[0]);

  // Double-deletion is a silent no-op.
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[8]);
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[4]);
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[0]);

  received_messages.clear();
  EXPECT_EQ(9u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages,
                                                        0,
                                                        0,
                                                        false));

  int expected = 5;
  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(expected, payload);

    ++expected;
    if ((expected == 8) || (expected == 12)) {
      ++expected;
    }
  }

  // Send a new, higher-priority message.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(-1),
                                     kDefaultPriority + 1));

  // Delete the previous "front" message.
  ASSERT_FALSE(received_messages.empty());
  this->message_bus_->DeleteMessage(receiver_a_id, received_messages[0]);

  // Receive messages, deleting them as they are received.
  received_messages.clear();
  EXPECT_EQ(9u,
            this->message_bus_->ReceiveBatchIfAvailable(receiver_a_id,
                                                        &received_messages,
                                                        0,
                                                        0,
                                                        true));

  expected = -1;
  for (std::size_t i = 0; i < received_messages.size(); ++i) {
    EXPECT_EQ(sender_id, received_messages[i].sender);
    EXPECT_EQ(0u, received_messages[i].tagged_message.message_type());
    ASSERT_EQ(sizeof(int),
              received_messages[i].tagged_message.message_bytes());
    int payload = *static_cast<const int*>(
        received_messages[i].tagged_message.message());
    EXPECT_EQ(expected, payload);

    if (expected == -1) {
      expected = 6;
    } else {
      ++expected;
      if ((expected == 8) || (expected == 12)) {
        ++expected;
      }
    }
  }

  // Now all pending messages should be deleted.
  AnnotatedMessage message_buffer;
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_a_id,
                                                      &message_buffer));

  // Send a broadcast message to multiple receivers.
  addr.AddRecipient(receiver_b_id);
  style.Broadcast(true);
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(42)));

  // Receive and delete from one client.
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_a_id,
                                                     &message_buffer));
  EXPECT_EQ(sender_id, message_buffer.sender);
  EXPECT_EQ(0u, message_buffer.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), message_buffer.tagged_message.message_bytes());
  EXPECT_EQ(42,
            *static_cast<const int*>(message_buffer.tagged_message.message()));

  this->message_bus_->DeleteMessage(receiver_a_id, message_buffer);

  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_a_id,
                                                      &message_buffer));

  // Message should still be receivable by the other client.
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_b_id,
                                                     &message_buffer));
  EXPECT_EQ(sender_id, message_buffer.sender);
  EXPECT_EQ(0u, message_buffer.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), message_buffer.tagged_message.message_bytes());
  EXPECT_EQ(42,
            *static_cast<const int*>(message_buffer.tagged_message.message()));
}


TYPED_TEST_P(ReconnectMessageBusTest, ReconnectTest) {
  // Connect a couple of clients and have them send messages to eachother.
  client_id client_a = this->message_bus_->Connect();
  client_id client_b = this->message_bus_->Connect();

  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(client_a, 0));
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(client_a, 1));

  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(client_b, 1));
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(client_b, 0));

  Address addr_a;
  addr_a.AddRecipient(client_a);

  Address addr_b;
  addr_b.AddRecipient(client_b);

  MessageStyle style;

  for (int i = 0; i < kNumSampleMessages; ++i) {
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(client_a,
                                       addr_b,
                                       style,
                                       MakeTaggedSimpleIntMessage(i, 0)));
    EXPECT_EQ(MessageBus::SendStatus::kOK,
              this->message_bus_->Send(client_b,
                                       addr_a,
                                       style,
                                       MakeTaggedSimpleIntMessage(-i, 1)));
  }

  // Receive and delete the first message for each client before disconnecting.
  AnnotatedMessage received;
  received = this->message_bus_->Receive(client_a);
  EXPECT_EQ(client_b, received.sender);
  EXPECT_EQ(1u, received.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
  EXPECT_EQ(0, *static_cast<const int*>(received.tagged_message.message()));
  this->message_bus_->DeleteMessage(client_a, received);

  received = this->message_bus_->Receive(client_b);
  EXPECT_EQ(client_a, received.sender);
  EXPECT_EQ(0u, received.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
  EXPECT_EQ(0, *static_cast<const int*>(received.tagged_message.message()));
  this->message_bus_->DeleteMessage(client_b, received);

  // Destroy the MessageBus object and "reconnect" to the same persistent
  // instance.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Connect an additional client and make sure that client_ids don't collide.
  client_id client_c = this->message_bus_->Connect();
  EXPECT_NE(client_a, client_c);
  EXPECT_NE(client_b, client_c);

  // Attempting to re-register the clients for the same message types should
  // fail.
  EXPECT_FALSE(this->message_bus_->RegisterClientAsSender(client_a, 0));
  EXPECT_FALSE(this->message_bus_->RegisterClientAsReceiver(client_a, 1));

  EXPECT_FALSE(this->message_bus_->RegisterClientAsSender(client_b, 1));
  EXPECT_FALSE(this->message_bus_->RegisterClientAsReceiver(client_b, 0));

  // Send one more message from each client.
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(
                client_a,
                addr_b,
                style,
                MakeTaggedSimpleIntMessage(kNumSampleMessages, 0)));
  EXPECT_EQ(
      MessageBus::SendStatus::kOK,
      this->message_bus_->Send(
          client_b,
          addr_a,
          style,
          MakeTaggedSimpleIntMessage(-static_cast<int>(kNumSampleMessages),
                                     1)));

  // Receive and delete another message for each client.
  received = this->message_bus_->Receive(client_a);
  EXPECT_EQ(client_b, received.sender);
  EXPECT_EQ(1u, received.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
  EXPECT_EQ(-1, *static_cast<const int*>(received.tagged_message.message()));
  this->message_bus_->DeleteMessage(client_a, received);

  received = this->message_bus_->Receive(client_b);
  EXPECT_EQ(client_a, received.sender);
  EXPECT_EQ(0u, received.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
  EXPECT_EQ(1, *static_cast<const int*>(received.tagged_message.message()));
  this->message_bus_->DeleteMessage(client_b, received);

  // Disconnect and reconnect once more.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Now receive all the previously sent messages.
  for (int i = 2; i <= kNumSampleMessages; ++i) {
    received = this->message_bus_->Receive(client_a);
    EXPECT_EQ(client_b, received.sender);
    EXPECT_EQ(1u, received.tagged_message.message_type());
    ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
    EXPECT_EQ(-i, *static_cast<const int*>(received.tagged_message.message()));
    this->message_bus_->DeleteMessage(client_a, received);

    received = this->message_bus_->Receive(client_b);
    EXPECT_EQ(client_a, received.sender);
    EXPECT_EQ(0u, received.tagged_message.message_type());
    ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
    EXPECT_EQ(i, *static_cast<const int*>(received.tagged_message.message()));
    this->message_bus_->DeleteMessage(client_b, received);
  }

  // No more messages for either client remain.
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(client_a, &received));
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(client_b, &received));

  // Send a cancellable broadcast message.
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(client_c, 0));
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(client_a, 0));

  Address broadcast_addr;
  broadcast_addr.AddRecipient(client_a).AddRecipient(client_b);

  MessageStyle broadcast_style;
  broadcast_style.Broadcast(true);

  CancellationToken cancellation_token;

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(client_c,
                                     broadcast_addr,
                                     broadcast_style,
                                     MakeTaggedSimpleIntMessage(42, 0),
                                     kDefaultPriority,
                                     &cancellation_token));

  // Disconnect one of the clients.
  EXPECT_TRUE(this->message_bus_->Disconnect(client_c));

  // Reconnect.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Shouldn't be able to do anything with the disconnected client.
  EXPECT_FALSE(this->message_bus_->RegisterClientAsSender(client_c, 0));

  // Receive and cancel the broadcast message from one of the clients.
  received = this->message_bus_->Receive(client_a);
  EXPECT_EQ(client_c, received.sender);
  EXPECT_EQ(0u, received.tagged_message.message_type());
  ASSERT_EQ(sizeof(int), received.tagged_message.message_bytes());
  EXPECT_EQ(42, *static_cast<const int*>(received.tagged_message.message()));
  this->message_bus_->CancelMessage(client_a, received);

  // Reconnect.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Neither client should be able to receive cancelled message.
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(client_a, &received));
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(client_b, &received));
}


TYPED_TEST_P(ReconnectActiveStateMessageBusTest, ReconnectAndCancelTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_a_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_a_id, 0));

  client_id receiver_b_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_b_id, 0));

  Address addr;
  addr.AddRecipient(receiver_a_id).AddRecipient(receiver_b_id);

  MessageStyle style;
  style.Broadcast(true);

  CancellationToken token_0;
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(0),
                                     kDefaultPriority,
                                     &token_0));

  CancellationToken token_1;
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(1),
                                     kDefaultPriority,
                                     &token_1));

  // Destroy the MessageBus object and "reconnect" to the same persistent
  // instance.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Cancel the first message on the sender side using the reconnected bus.
  this->message_bus_->CancelMessage(sender_id, token_0);

  // Receive a message.
  AnnotatedMessage received_msg = this->message_bus_->Receive(receiver_a_id);
  EXPECT_EQ(sender_id, received_msg.sender);
  EXPECT_EQ(0u, received_msg.tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_msg.tagged_message.message_bytes());
  int payload
      = *static_cast<const int*>(received_msg.tagged_message.message());
  EXPECT_EQ(1, payload);

  // Destroy and reconnect again.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Cancel the message on the receiver side using the reconnected bus.
  this->message_bus_->CancelMessage(receiver_a_id, received_msg);

  // Both messages are cancelled, so there is nothing left to receive.
  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_b_id,
                                                      &received_msg));
}


TYPED_TEST_P(ReconnectSeparateDeletionMessageBusTest,
             ReconnectAndDeleteTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  client_id receiver_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsReceiver(receiver_id, 0));

  Address addr;
  addr.AddRecipient(receiver_id);
  MessageStyle style;

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(0)));

  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(1)));

  // Receive the first message.
  AnnotatedMessage received_msg = this->message_bus_->Receive(receiver_id);
  EXPECT_EQ(sender_id, received_msg.sender);
  EXPECT_EQ(0u, received_msg.tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_msg.tagged_message.message_bytes());
  int payload
      = *static_cast<const int*>(received_msg.tagged_message.message());
  EXPECT_EQ(0, payload);

  // Destroy the MessageBus object and "reconnect" to the same persistent
  // instance.
  this->message_bus_.reset(nullptr);
  this->message_bus_.reset(this->CreateBus());
  ASSERT_TRUE(this->message_bus_->Initialize());

  // Delete using the reconnected bus instance.
  this->message_bus_->DeleteMessage(receiver_id, received_msg);

  // Should now receive the next message.
  EXPECT_TRUE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                     &received_msg));
  EXPECT_EQ(sender_id, received_msg.sender);
  EXPECT_EQ(0u, received_msg.tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_msg.tagged_message.message_bytes());
  payload = *static_cast<const int*>(received_msg.tagged_message.message());
  EXPECT_EQ(1, payload);

  // After deleting the second message, there is nothing left to receive.
  this->message_bus_->DeleteMessage(receiver_id, received_msg);

  EXPECT_FALSE(this->message_bus_->ReceiveIfAvailable(receiver_id,
                                                      &received_msg));
}


// Simulate a client-server interaction for MultiInstanceTest, with a client
// sending ints to the server and the server returning their squares.
class ClientThread : public Thread {
 public:
  ClientThread(MessageBus *message_bus,
               const int num_messages,
               const client_id server_id)
      : message_bus_ptr_(message_bus),
        num_messages_(num_messages),
        server_(server_id) {
    me_ = message_bus_ptr_->Connect();
    EXPECT_TRUE(message_bus_ptr_->RegisterClientAsSender(me_, 0));
    EXPECT_TRUE(message_bus_ptr_->RegisterClientAsReceiver(me_, 1));
  }

  ~ClientThread() override {};

 protected:
  void Run() override {
    Address addr;
    addr.AddRecipient(server_);
    MessageStyle style;
    for (int i = 0; i < num_messages_; ++i) {
      EXPECT_EQ(MessageBus::SendStatus::kOK,
                message_bus_ptr_->Send(me_,
                                       addr,
                                       style,
                                       MakeTaggedSimpleIntMessage(i, 0)));

      AnnotatedMessage response = message_bus_ptr_->Receive(me_);
      EXPECT_EQ(server_, response.sender);
      EXPECT_EQ(1u, response.tagged_message.message_type());
      ASSERT_EQ(sizeof(int), response.tagged_message.message_bytes());
      EXPECT_EQ(i * i,
                *static_cast<const int*>(response.tagged_message.message()));
      message_bus_ptr_->DeleteMessage(me_, response);
    }
  }

 private:
  MessageBus *message_bus_ptr_;
  const int num_messages_;
  client_id me_;
  client_id server_;
};

class ServerThread : public Thread {
 public:
  ServerThread(MessageBus *message_bus, const int num_messages)
      : message_bus_ptr_(message_bus),
        num_messages_(num_messages) {
    me_ = message_bus_ptr_->Connect();
    EXPECT_TRUE(message_bus_ptr_->RegisterClientAsSender(me_, 1));
    EXPECT_TRUE(message_bus_ptr_->RegisterClientAsReceiver(me_, 0));
  }

  ~ServerThread() override {};

  client_id GetID() const {
    return me_;
  }

 protected:
  void Run() override {
    MessageStyle style;
    for (int i = 0; i < num_messages_; ++i) {
      AnnotatedMessage request = message_bus_ptr_->Receive(me_);
      EXPECT_EQ(0u, request.tagged_message.message_type());
      ASSERT_EQ(sizeof(int), request.tagged_message.message_bytes());
      int request_param
          = *static_cast<const int*>(request.tagged_message.message());

      Address addr;
      addr.AddRecipient(request.sender);
      EXPECT_EQ(MessageBus::SendStatus::kOK,
                message_bus_ptr_->Send(
                    me_,
                    addr,
                    style,
                    MakeTaggedSimpleIntMessage(request_param * request_param,
                                               1)));

      message_bus_ptr_->DeleteMessage(me_, request);
    }
  }

 private:
  MessageBus *message_bus_ptr_;
  const int num_messages_;
  client_id me_;
};

TYPED_TEST_P(MultiInstanceMessageBusTest, MultiInstanceTest) {
  std::unique_ptr<MessageBus> server_bus_instance(this->CreateBus());
  ASSERT_TRUE(server_bus_instance->Initialize());
  ServerThread server(server_bus_instance.get(), 16);
  server.Start();

  ClientThread client(this->message_bus_.get(), 16, server.GetID());
  client.Start();

  server.Join();
  client.Join();
}


TYPED_TEST_P(MultiInstanceMessageBusTest, MultiInstanceCancelTest) {
  client_id sender_id = this->message_bus_->Connect();
  EXPECT_TRUE(this->message_bus_->RegisterClientAsSender(sender_id, 0));

  std::unique_ptr<MessageBus> receiver_a_bus_instance(this->CreateBus());
  ASSERT_TRUE(receiver_a_bus_instance->Initialize());
  client_id receiver_a_id = receiver_a_bus_instance->Connect();
  EXPECT_TRUE(receiver_a_bus_instance->RegisterClientAsReceiver(receiver_a_id,
                                                                0));

  std::unique_ptr<MessageBus> receiver_b_bus_instance(this->CreateBus());
  ASSERT_TRUE(receiver_b_bus_instance->Initialize());
  client_id receiver_b_id = receiver_b_bus_instance->Connect();
  EXPECT_TRUE(receiver_b_bus_instance->RegisterClientAsReceiver(receiver_b_id,
                                                                0));

  Address addr;
  addr.AddRecipient(receiver_a_id).AddRecipient(receiver_b_id);

  MessageStyle style;
  style.Broadcast(true);

  CancellationToken token_0;
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(0),
                                     kDefaultPriority,
                                     &token_0));

  CancellationToken token_1;
  EXPECT_EQ(MessageBus::SendStatus::kOK,
            this->message_bus_->Send(sender_id,
                                     addr,
                                     style,
                                     MakeTaggedSimpleIntMessage(1),
                                     kDefaultPriority,
                                     &token_1));

  // Cancel the first message on the sender side.
  this->message_bus_->CancelMessage(sender_id, token_0);

  // Receive a message.
  AnnotatedMessage received_msg = receiver_a_bus_instance->Receive(
      receiver_a_id);
  EXPECT_EQ(sender_id, received_msg.sender);
  EXPECT_EQ(0u, received_msg.tagged_message.message_type());
  ASSERT_EQ(sizeof(int),
            received_msg.tagged_message.message_bytes());
  int payload
      = *static_cast<const int*>(received_msg.tagged_message.message());
  EXPECT_EQ(1, payload);

  // Cancel the message on the receiver side.
  receiver_a_bus_instance->CancelMessage(receiver_a_id, received_msg);

  // Both messages are cancelled, so there is nothing left to receive.
  EXPECT_FALSE(receiver_b_bus_instance->ReceiveIfAvailable(receiver_b_id,
                                                           &received_msg));
}


REGISTER_TYPED_TEST_CASE_P(MessageBusTest,
                           ConnectTest, ThreadedConnectTest,
                           RegisterAndSendTest, SimpleSendAndReceiveTest,
                           LargeMessageSimpleSendAndReceiveTest,
                           InterleavedBlockingSimpleSendAndReceiveTest,
                           ThreadedSimpleSendAndReceiveTest,
                           ThreadedMultipleReceiversTest,
                           ThreadedMultipleSendersTest, ReceiveBatchTest,
                           PriorityTest, TimeoutTest, SendOrderTest,
                           BroadcastTest, CancelTest,
                           CountQueuedMessagesForClientTest);

REGISTER_TYPED_TEST_CASE_P(SeparateDeletionMessageBusTest,
                           DeleteTest);

REGISTER_TYPED_TEST_CASE_P(ReconnectMessageBusTest,
                           ReconnectTest);

REGISTER_TYPED_TEST_CASE_P(ReconnectActiveStateMessageBusTest,
                           ReconnectAndCancelTest);

REGISTER_TYPED_TEST_CASE_P(ReconnectSeparateDeletionMessageBusTest,
                           ReconnectAndDeleteTest);

REGISTER_TYPED_TEST_CASE_P(MultiInstanceMessageBusTest,
                           MultiInstanceTest,
                           MultiInstanceCancelTest);

}  // namespace tmb

#endif  // TESTS_MESSAGE_BUS_UNITTEST_COMMON_H_
