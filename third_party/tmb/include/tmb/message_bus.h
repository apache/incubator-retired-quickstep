// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_MESSAGE_BUS_H_
#define TMB_MESSAGE_BUS_H_

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

#include "tmb/id_typedefs.h"
#include "tmb/priority.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/container_pusher.h"
#include "tmb/internal/iterator_adapter.h"

namespace tmb {

class Address;
class CancellationToken;
class MessageStyle;

namespace internal { class NetMessageRemovalInterface; }

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief A bus which allows clients to connect and send messages to eachother.
 *        This class defines the core interface for all MessageBus
 *        implementations.
 **/
class MessageBus {
 public:
  /**
   * @brief Indicates the result of a call to Send().
   **/
  enum class SendStatus {
    kOK = 0,              // Message successfully sent. This means that at
                          // least one recipient which was connected at the
                          // time the message was sent will have the message
                          // queued for delivery, and it will eventually be
                          // delivered if the recipient does not disconnect
                          // and the message timeout (if any) does not expire.
                          // Note that kOK will still be returned if some of
                          // the specified recipients of a message have already
                          // disconnected at the time the message is sent, so
                          // long as at least one recipient is still connected.

    kNoReceivers,         // The set of receivers specified by the Address is
                          // empty. This may be returned if all of the
                          // specified recipients have already disconnected at
                          // the time the message is sent.

    kSenderNotConnected,  // Error: the sender is not connected to this bus.

    kSenderNotRegisteredForMessageType,   // Error: the sender is not
                                          // registered as a sender for the
                                          // specified message type.

    kReceiverNotRegisteredForMessageType  // Error: an explicitly-specified
                                          // recipient has not been registered
                                          // as a receiver for the specified
                                          // message type.
  };

  MessageBus() {
  }

  virtual ~MessageBus() {
  }

  /**
   * @brief For MessageBus implementations which talk to remote servers
   *        (currently VoltDBMessageBus and ZookeeperMessageBus), this method
   *        adds a server to the set of remote servers which we will connect
   *        to. All servers specified should be in the same cluster. Has no
   *        effect for other MessageBus implementations.
   * @warning This must be called at least once BEFORE calling Initialize()
   *          below to set up a VoltDBMessageBus or ZookeeperMessageBus.
   *          Calling this method after Initialize() has no effect.
   * @warning Unlike other methods, this is not necessarily threadsafe.
   *
   * @param hostname The hostname or IP address of the server to connect to.
   * @param port The TCP/IP port number to connect to on the server.
   **/
  virtual void AddServer(const std::string &hostname,
                         const std::uint16_t port) {
  }

  /**
   * @brief Complete initialization of this MessageBus object and make it
   *        usable. Depending on the particular MessageBus implementation,
   *        calling this method may open database files, open connections to
   *        remote servers, etc.
   * @warning This method MUST be called successfully before using any of the
   *          other methods below.
   * @warning This method is not necessarily threadsafe, and should be called
   *          only once.
   *
   * @return true on success, false if some error occured.
   **/
  virtual bool Initialize() = 0;

  /**
   * @brief Reset the message bus and prepare the underlying storage, wiping
   *        out any existing state.
   * @note Some MessageBus implementations, currently SQLiteMessageBus and
   *       ZookeeperMessageBus, must have this method called once before using
   *       the bus in order to set up certain persistent data structures for
   *       the bus.
   * @warning This is NOT threadsafe, and must be called only once, AFTER
   *          Initialize() but before any calls to the methods below.
   **/
  virtual void ResetBus() = 0;

  /**
   * @brief Connect a client (an entity which can send and/or receive messages)
   *        to this bus.
   *
   * @return An ID which uniquely identifies the client among all the clients
   *         of this MessageBus.
   **/
  virtual client_id Connect() = 0;

  /**
   * @brief Disconnect a client from this bus, so that it may no longer send
   *        or receive messages.
   * @note If there are messages pending for the client, they will not be
   *       delivered.
   *
   * @param client The ID of the client, originally provided by Connect(), to
   *        disconnect from this bus.
   * @return true on success, false if some error is encountered (e.g.
   *         attempting to disconnect a client which is not actually
   *         connected).
   **/
  virtual bool Disconnect(const client_id client) = 0;

  /**
   * @brief Register a connected client as a sender of a particular type of
   *        message, allowing it to use Send() with the specified message type.
   *
   * @param sender_id The ID the sender, originally provided by Connect().
   * @param message_type The application-specific type ID of the messages which
   *        will be sent.
   * @return true on success, false if some error occured (e.g. there is no
   *         connected client with sender_id, or the specified sender is
   *         already registered as a sender of the specified message_type).
   **/
  virtual bool RegisterClientAsSender(const client_id sender_id,
                                      const message_type_id message_type) = 0;

  /**
   * @brief Register a connected client as a receiver of a particular type of
   *        message, allowing it to start receiving messages of the specified
   *        type.
   *
   * @param receiver_id The ID of the receiver to deliver messages to.
   * @param message_type The application-specific type ID of the messages to
   *        start receiving.
   * @return true on success, false if some error occured (e.g. there is no
   *         connected client with receiver_id, or the specified receiver is
   *         already registered as a receiver of the specified message_type).
   **/
  virtual bool RegisterClientAsReceiver(
      const client_id receiver_id,
      const message_type_id message_type) = 0;

  /**
   * @brief Send a message asynchronously.
   *
   * @param sender_id The ID of the client which is sending the message.
   * @param destination_address The Address describing which recipients should
   *        receive the message.
   * @param style The style of messaging for this message.
   * @param message The message to send (will be "moved from").
   * @param priority The priority of the message.
   * @param cancellation_token If non-NULL, make this message cancellable and
   *        overwrite with a token that can be used by the sender to cancel the
   *        message. Note that clients which receive the message will also be
   *        able to cancel it, possibly preventing the message from being
   *        received by their peers.
   * @return The result of the send. See SendStatus for details.
   **/
  virtual SendStatus Send(
      const client_id sender_id,
      const Address &destination_address,
      const MessageStyle &style,
      TaggedMessage &&message,  // NOLINT(whitespace/operators)
      const Priority priority = kDefaultPriority,
      CancellationToken *cancellation_token = nullptr) = 0;

  /**
   * @brief Receive a pending asynchronous message.
   * @note This method blocks the caller until a message can be delivered. See
   *       also ReceiveIfAvailable().
   * @note It can be more efficient to receive messages in a batch rather than
   *       one-at-a-time. See also ReceiveBatch().
   *
   * @param receiver_id The ID of the receiver which calls this method to get a
   *        message.
   * @param minimum_priority The minimum priority of messages to receive.
   * @param delete_immediately If true, delete the received message from the
   *        bus immediately during this call. Otherwise, message should be
   *        explicitly deleted by calling DeleteMessage().
   * @return The next pending message for the specified receiver.
   **/
  AnnotatedMessage Receive(const client_id receiver_id,
                           const Priority minimum_priority = 0,
                           const bool delete_immediately = false) {
    AnnotatedMessage message;
    internal::SingleElementPusher pusher(&message);
    ReceiveImpl(receiver_id, minimum_priority, 1, delete_immediately, &pusher);
    return message;
  }

  /**
   * @brief Receive a batch of pending asynchronous messages.
   * @note This method blocks the caller until at least one message can be
   *       delivered. See also ReceiveBatchIfAvailable().
   *
   * @param receiver_id The ID of the receiver which calls this method to get a
   *        message.
   * @param message_container A pointer to a container with a
   *        push_back(const AnnotatedMessage&) or push_back(AnnotatedMessage&&)
   *        method (e.g. std::vector<AnnotatedMessage>,
   *        std::list<AnnotatedMessage>, or std::deque<AnnotatedMessage>) which
   *        messages will be inserted into.
   * @param minimum_priority The minimum priority of messages to receive.
   * @param max_messages The maximum number of messages to deliver in the batch
   *        (0 for no limit).
   * @param delete_immediately If true, delete the received messages from the
   *        bus immediately during this call. Otherwise, messages should be
   *        explicitly deleted later by calling DeleteMessages().
   * @return The number of messages actually received in the batch (i.e. the
   *         number of messages inserted into message_container).
   **/
  template <typename ContainerT>
  std::size_t ReceiveBatch(const client_id receiver_id,
                           ContainerT *message_container,
                           const Priority minimum_priority = 0,
                           const std::size_t max_messages = 0,
                           const bool delete_immediately = false) {
    internal::ContainerPusherImpl<ContainerT> pusher(message_container);
    return ReceiveImpl(receiver_id,
                       minimum_priority,
                       max_messages,
                       delete_immediately,
                       &pusher);
  }

  /**
   * @brief Receive a pending asynchronous message if any is available when
   *        this method is called.
   * @note Unlike Receive(), this method will not block indefinitely while
   *       waiting for a message.
   * @note It can be more efficient to receive messages in a batch rather than
   *       one-at-a-time. See also ReceiveBatchIfAvailable().
   *
   * @param receiver_id The ID of the receiver which calls this method to get a
   *        message.
   * @param message_ptr A pointer to an uninitialized AnnotatedMessage which
   *        will be overwritten by the received message, if any.
   * @param minimum_priority The minimum priority of messages to receive.
   * @param delete_immediately If true, delete the received message (if any)
   *        from the bus immediately during this call. Otherwise, message
   *        should be explicitly deleted later by calling DeleteMessage().
   * @return true if a message was delivered (overwriting *message_ptr), false
   *         if there were no pending messages for the specified Receiver at
   *         the time of the call.
   **/
  bool ReceiveIfAvailable(const client_id receiver_id,
                          AnnotatedMessage *message_ptr,
                          const Priority minimum_priority = 0,
                          const bool delete_immediately = false) {
    internal::SingleElementPusher pusher(message_ptr);
    return ReceiveIfAvailableImpl(receiver_id,
                                  minimum_priority,
                                  1,
                                  delete_immediately,
                                  &pusher)
           > 0;
  }

  /**
   * @brief Receive a batch of pending asynchronous messages if any are
   *        available when this method is called.
   * @note Unlike ReceiveBatch(), this method will not block indefinitely while
   *       waiting for a message.
   *
   * @param receiver_id The ID of the receiver which calls this method to get a
   *        message.
   * @param message_container A pointer to a container with a
   *        push_back(const AnnotatedMessage&) or push_back(AnnotatedMessage&&)
   *        method (e.g. std::vector<AnnotatedMessage>,
   *        std::list<AnnotatedMessage>, or std::deque<AnnotatedMessage>) which
   *        messages will be inserted into.
   * @param minimum_priority The minimum priority of messages to receive.
   * @param max_messages The maximum number of messages to deliver in the batch
   *        (0 for no limit).
   * @param delete_immediately If true, delete the received messages (if any)
   *        from the bus immediately during this call. Otherwise, messages
   *        should be explicitly deleted later by calling DeleteMessages().
   * @return The number of messages actually received in the batch (i.e. the
   *         number of messages inserted into '*message_container').
   **/
  template <typename ContainerT>
  std::size_t ReceiveBatchIfAvailable(const client_id receiver_id,
                                      ContainerT *message_container,
                                      const Priority minimum_priority = 0,
                                      const std::size_t max_messages = 0,
                                      const bool delete_immediately = false) {
    internal::ContainerPusherImpl<ContainerT> pusher(message_container);
    return ReceiveIfAvailableImpl(receiver_id,
                                  minimum_priority,
                                  max_messages,
                                  delete_immediately,
                                  &pusher);
  }

  /**
   * @brief Delete a single message received by a call to one of the Receive
   *        methods.
   *
   * @param receiver_id The ID of the receiver which calls this method to
   *        delete a message.
   * @param message A message previously received by the client.
   **/
  void DeleteMessage(const client_id receiver_id,
                     const AnnotatedMessage &message) {
    internal::IteratorAdapterImpl<const AnnotatedMessage*>
        adapter(&message, &message + 1);
    DeleteImpl(receiver_id, &adapter);
  }

  /**
   * @brief Delete a batch of messages received by a call to one of the Receive
   *        methods.
   *
   * @param receiver_id The ID of the receiver which calls this method to
   *        delete messages.
   * @param begin An iterator to the first AnnotatedMessage to be deleted.
   * @param end An iterator to one-past-the-end of the AnnotatedMessages to
   *        be deleted.
   **/
  template <typename IteratorT>
  void DeleteMessages(const client_id receiver_id,
                      const IteratorT &begin,
                      const IteratorT &end) {
    internal::IteratorAdapterImpl<IteratorT> adapter(begin, end);
    DeleteImpl(receiver_id, &adapter);
  }

  /**
   * @brief Cancel a previously sent message.
   * @note This does NOT guarantee that clients will not receive the cancelled
   *       message, as they could have received it before it was cancelled. It
   *       will prevent the message from being received after the call is
   *       complete, however.
   *
   * @param sender_id The ID of the client which originally sent the message,
   *                  and is now cancelling it.
   * @param cancellation_token The cancellation token generated by the call to
   *        Send() which originally sent the message.
   **/
  virtual void CancelMessage(const client_id sender_id,
                             const CancellationToken &cancellation_token) = 0;

  /**
   * @brief Cancel a previously received message, preventing other clients from
   *        receiving it in the future.
   * @note This does NOT guarantee that clients will not receive the cancelled
   *       message, as they could have received it before it was cancelled. It
   *       will prevent the message from being received after the call is
   *       complete, however.
   * @note A message can only be cancelled if the sender originally made it
   *       cancellable. Attempting to cancel a non-cancellable message is a
   *       silent no-op.
   *
   * @param receiver_id The ID of the client which received the message and is
   *        now cancelling it.
   * @param received_message The previously received message which is now being
   *        cancelled.
   **/
  void CancelMessage(const client_id receiver_id,
                     const AnnotatedMessage &received_message) {
    internal::IteratorAdapterImpl<const AnnotatedMessage*>
        adapter(&received_message, &received_message + 1);
    CancelImpl(receiver_id, &adapter);
  }

  /**
   * @brief Cancel a batch of previously received messages, preventing other
   *        clients from receiving them in the future.
   * @note This does NOT guarantee that clients will not receive a cancelled
   *       message, as they could have received it before it was cancelled. It
   *       will prevent messages from being received after the call is
   *       complete, however.
   * @note A message can only be cancelled if the sender originally made it
   *       cancellable. Attempting to cancel a non-cancellable message is a
   *       silent no-op.
   *
   * @param receiver_id The ID of the client which received the messages and is
   *        now cancelling them.
   * @param begin An iterator to the first AnnotatedMessage to be cancelled.
   * @param end An iterator to one-past-the-end of the AnnotatedMessages to
   *        be cancelled.
   **/
  template <typename IteratorT>
  void CancelMessages(const client_id receiver_id,
                      const IteratorT &begin,
                      const IteratorT &end) {
    internal::IteratorAdapterImpl<IteratorT> adapter(begin, end);
    CancelImpl(receiver_id, &adapter);
  }

  /**
   * @brief Count the number of pending messages queued for a particular
   *        client.
   * @note This method may count messages that are already expired or
   *       cancelled, and therefore will never be delivered to the specified
   *       receiver.
   *
   * @param receiver_id The ID of the client to count pending queued messages
   *        for.
   * @return The number of messages queued for the specified client.
   **/
  virtual std::size_t CountQueuedMessagesForClient(
      const client_id receiver_id) = 0;

  /**
   * @brief Determine if GetNetMessageRemovalInterface() can be called for this
   *        MessageBus.
   * @warning This is intended only for internal use by TMB network servers.
   *
   * @return true if GetNetMessageRemovalInterface() can be called safely.
   **/
  virtual bool SupportsNetMessageRemovalInterface() const {
    return false;
  }

  /**
   * @brief Return a pointer to this MessageBus cast to
   *        NetMessageRemovalInterface.
   * @warning This is intended only for internal use by TMB network servers,
   *          and can only be called if SupportsNetMessageRemovalInterface()
   *          returns true.
   *
   * @return A pointer to this MessageBus cast to NetMessageRemovalInterface.
   **/
  virtual internal::NetMessageRemovalInterface*
      GetNetMessageRemovalInterface();

 protected:
  // Default implementation does active polling. Implementations should
  // override this if they can do something smarter.
  virtual std::size_t ReceiveImpl(const client_id receiver_id,
                                  const Priority minimum_priority,
                                  const std::size_t max_messages,
                                  const bool delete_immediately,
                                  internal::ContainerPusher *pusher);

  virtual std::size_t ReceiveIfAvailableImpl(
      const client_id receiver_id,
      const Priority minimum_priority,
      const std::size_t max_messages,
      const bool delete_immediately,
      internal::ContainerPusher *pusher) = 0;

  virtual void DeleteImpl(
      const client_id receiver_id,
      internal::IteratorAdapter<const AnnotatedMessage> *adapter) = 0;

  virtual void CancelImpl(
      const client_id receiver_id,
      internal::IteratorAdapter<const AnnotatedMessage> *adapter) = 0;

 private:
  // The number of milliseconds to sleep between calls to
  // ReceiveIfAvailableImpl() in the default active-polling implementation of
  // ReceiveImpl().
  static const unsigned int kReceivePollIntervalMS = 100;

  // Disallow copy and assign:
  MessageBus(const MessageBus &orig) = delete;
  MessageBus& operator=(const MessageBus &rhs) = delete;
};

/** @} */

}  // namespace tmb

#endif  // TMB_MESSAGE_BUS_H_
