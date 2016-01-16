// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

// TODO(chasseur): Better error handling in place of asserts.

#include "tmb/sqlite_message_bus.h"

#include <sqlite3.h>
#include <cassert>
#include <chrono>  // NOLINT(build/c++11)
#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "tmb/address.h"
#include "tmb/cancellation_token.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

#include "tmb/internal/container_pusher.h"
#include "tmb/internal/iterator_adapter.h"
#include "tmb/internal/memory_mirror_delete_batch.h"
#include "tmb/internal/persistent_bus_state_interface.h"
#include "tmb/internal/queued_message.h"
#include "tmb/internal/sqlite_connection.h"
#include "tmb/internal/sqlite_connection_pool.h"


#define RETRY_WHILE_BUSY(return_code, ...)  \
  do {                                      \
    return_code = __VA_ARGS__;              \
  } while ((return_code & 0xFF) == SQLITE_BUSY);

namespace tmb {

namespace {

class SQLiteMemoryMirrorDeleteBatch
    : public internal::MemoryMirrorDeleteBatch {
 public:
  explicit SQLiteMemoryMirrorDeleteBatch(const client_id receiver_id)
      : internal::MemoryMirrorDeleteBatch(receiver_id) {
  }

  ~SQLiteMemoryMirrorDeleteBatch() {
  }

  void AddMessage(const internal::QueuedMessage &message) override {
    message_ids_.push_back(message.GetMessageID());
  }

 private:
  std::vector<std::int64_t> message_ids_;

  friend class tmb::SQLiteMessageBus;
};

}  // namespace

client_id SQLiteMessageBus::Connect() {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->insert_client_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->insert_client_stmt());

  sqlite3_int64 rowid = sqlite3_last_insert_rowid(conn->connection());

  assert(rowid != 0);
  assert(rowid == static_cast<client_id>(rowid));

  return rowid;
}

bool SQLiteMessageBus::Disconnect(const client_id client) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  rc = sqlite3_bind_int64(conn->disconnect_client_stmt(), 1, client);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->disconnect_client_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->disconnect_client_stmt());

  int rows_modified = sqlite3_changes(conn->connection());
  if (rows_modified == 0) {
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->commit_xact_stmt());

    return false;
  }
  assert(rows_modified == 1);

  rc = sqlite3_bind_int64(conn->delete_sender_entries_for_client_stmt(),
                          1,
                          client);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(
      rc,
      sqlite3_step(conn->delete_sender_entries_for_client_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->delete_sender_entries_for_client_stmt());

  rc = sqlite3_bind_int64(conn->delete_receiver_entries_for_client_stmt(),
                          1,
                          client);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(
      rc,
      sqlite3_step(conn->delete_receiver_entries_for_client_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->delete_receiver_entries_for_client_stmt());

  rc = sqlite3_bind_int64(conn->delete_queued_messages_for_client_stmt(),
                          1,
                          client);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(
      rc,
      sqlite3_step(conn->delete_queued_messages_for_client_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->delete_queued_messages_for_client_stmt());

  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());

  return true;
}

bool SQLiteMessageBus::RegisterClientAsSender(
    const client_id sender_id,
    const message_type_id message_type) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  // Check that client is connected.
  rc = sqlite3_bind_int64(conn->check_client_connected_stmt(), 1, sender_id);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_client_connected_stmt()));
  assert(rc == SQLITE_ROW);
  bool client_connected
      = (sqlite3_column_int(conn->check_client_connected_stmt(), 0) > 0);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_client_connected_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->check_client_connected_stmt());

  if (!client_connected) {
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->commit_xact_stmt());

    return false;
  }

  // Insert entry in sendable table.
  rc = sqlite3_bind_int64(conn->register_sender_stmt(), 1, sender_id);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->register_sender_stmt(), 2, message_type);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->register_sender_stmt()));
  bool retval = (rc == SQLITE_DONE);
  sqlite3_reset(conn->register_sender_stmt());

  // Commit.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());

  return retval;
}

bool SQLiteMessageBus::RegisterClientAsReceiver(
    const client_id receiver_id,
    const message_type_id message_type) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  // Check that client is connected.
  rc = sqlite3_bind_int64(conn->check_client_connected_stmt(), 1, receiver_id);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_client_connected_stmt()));
  assert(rc == SQLITE_ROW);
  bool client_connected
      = (sqlite3_column_int(conn->check_client_connected_stmt(), 0) > 0);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_client_connected_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->check_client_connected_stmt());

  if (!client_connected) {
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->commit_xact_stmt());

    return false;
  }

  // Insert entry in receivable table.
  rc = sqlite3_bind_int64(conn->register_receiver_stmt(), 1, receiver_id);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->register_receiver_stmt(), 2, message_type);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->register_receiver_stmt()));
  bool retval = (rc == SQLITE_DONE);
  sqlite3_reset(conn->register_receiver_stmt());

  // Commit.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());

  return retval;
}

MessageBus::SendStatus SQLiteMessageBus::Send(
    const client_id sender_id,
    const Address &destination_address,
    const MessageStyle &style,
    TaggedMessage &&message,  // NOLINT(whitespace/operators)
    const Priority priority,
    CancellationToken *cancellation_token) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  // Start transaction. Use an immediate transaction in anticipation of
  // writing. This assumes success (the expected case) and eliminates cases
  // where we would need to rollback because a read transaction could not be
  // upgraded to a write transaction.
  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  // Check if sender is connected.
  rc = sqlite3_bind_int64(conn->check_client_connected_stmt(), 1, sender_id);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_client_connected_stmt()));
  assert(rc == SQLITE_ROW);
  bool sender_connected
      = (sqlite3_column_int(conn->check_client_connected_stmt(), 0) > 0);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_client_connected_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->check_client_connected_stmt());

  if (!sender_connected) {
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->commit_xact_stmt());

    return SendStatus::kSenderNotConnected;
  }

  // Check if sender is registered for the given message type.
  rc = sqlite3_bind_int64(conn->check_sender_registered_stmt(), 1, sender_id);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int64(conn->check_sender_registered_stmt(),
                          2,
                          message.message_type());
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_sender_registered_stmt()));
  assert(rc == SQLITE_ROW);
  bool sender_registered
      = (sqlite3_column_int(conn->check_sender_registered_stmt(), 0) > 0);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_sender_registered_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->check_sender_registered_stmt());

  if (!sender_registered) {
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->commit_xact_stmt());

    return SendStatus::kSenderNotRegisteredForMessageType;
  }

  if (destination_address.send_to_all_) {
    // Check if there are any connected receivers capable of receiving this
    // message.
    rc = sqlite3_bind_int64(conn->check_any_receiver_stmt(),
                            1,
                            message.message_type());
    assert(rc == SQLITE_OK);
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_any_receiver_stmt()));
    assert(rc == SQLITE_ROW);
    int receiver_exists = sqlite3_column_int(conn->check_any_receiver_stmt(),
                                             0);
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->check_any_receiver_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->check_any_receiver_stmt());

    if (receiver_exists == 0) {
      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
      assert(rc == SQLITE_DONE);
      sqlite3_reset(conn->commit_xact_stmt());

      return SendStatus::kNoReceivers;
    }
  } else {
    // Check that each explicitly-specified recipient is registered as a
    // receiver for the message's type.
    sqlite3_int64 num_receivers = 0;
    std::vector<client_id>::const_iterator receiver_it
        = destination_address.explicit_recipients_.begin();
    while (receiver_it != destination_address.explicit_recipients_.end()) {
      receiver_it = internal::SQLiteConnection::BindIntegersToSetStatement(
          receiver_it,
          destination_address.explicit_recipients_.end(),
          conn->count_clients_in_set_stmt(),
          1);

      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->count_clients_in_set_stmt()));
      assert(rc == SQLITE_ROW);
      num_receivers
          += sqlite3_column_int64(conn->count_clients_in_set_stmt(), 0);
      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->count_clients_in_set_stmt()));
      assert(rc == SQLITE_DONE);
      sqlite3_reset(conn->count_clients_in_set_stmt());
    }

    // Count the number of specified recievers that are connected.
    if (num_receivers == 0) {
      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
      assert(rc == SQLITE_DONE);
      sqlite3_reset(conn->commit_xact_stmt());

      return SendStatus::kNoReceivers;
    }

    // Count the number of receivers that are registered for the given message
    // type and verify that it matches the number of connected receivers.
    sqlite3_int64 num_registered_receivers = 0;
    rc = sqlite3_bind_int64(conn->count_receivers_in_set_stmt(),
                            1,
                            message.message_type());
    receiver_it = destination_address.explicit_recipients_.begin();
    while (receiver_it != destination_address.explicit_recipients_.end()) {
      receiver_it = internal::SQLiteConnection::BindIntegersToSetStatement(
          receiver_it,
          destination_address.explicit_recipients_.end(),
          conn->count_receivers_in_set_stmt(),
          2);

      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->count_receivers_in_set_stmt()));
      assert(rc == SQLITE_ROW);
      num_registered_receivers
          += sqlite3_column_int64(conn->count_receivers_in_set_stmt(), 0);
      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->count_receivers_in_set_stmt()));
      assert(rc == SQLITE_DONE);
      sqlite3_reset(conn->count_receivers_in_set_stmt());
    }

    assert(num_registered_receivers <= num_receivers);
    if (num_registered_receivers < num_receivers) {
      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
      assert(rc == SQLITE_DONE);
      sqlite3_reset(conn->commit_xact_stmt());

      return SendStatus::kReceiverNotRegisteredForMessageType;
    }
  }

  // Actually insert the message.
  rc = sqlite3_bind_int64(conn->insert_message_stmt(), 1, sender_id);
  assert(rc == SQLITE_OK);
  if (style.expiration_time_.time_since_epoch().count() == 0) {
    rc = sqlite3_bind_null(conn->insert_message_stmt(), 2);
  } else {
    rc = sqlite3_bind_int64(
        conn->insert_message_stmt(),
        2,
        std::chrono::duration_cast<std::chrono::seconds>(
            style.expiration_time_.time_since_epoch()).count());
  }
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->insert_message_stmt(), 3, priority);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int64(conn->insert_message_stmt(),
                          4,
                          message.message_type());
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->insert_message_stmt(),
                        5,
                        cancellation_token == nullptr ? 0 : 1);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_blob(conn->insert_message_stmt(),
                         6,
                         message.message(),
                         message.message_bytes(),
                         SQLITE_STATIC);
  assert(rc == SQLITE_OK);

  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->insert_message_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_int64 message_id = sqlite3_last_insert_rowid(conn->connection());
  sqlite3_reset(conn->insert_message_stmt());

  // Queue the message for delivery.
  sqlite3_int64 queued_rows = 0;
  if (destination_address.send_to_all_) {
    sqlite3_stmt *queue_stmt = nullptr;
    if (style.broadcast_) {
      queue_stmt = conn->queue_message_for_all_receivers_stmt();
    } else {
      queue_stmt = conn->queue_message_for_any_receiver_stmt();
    }
    rc = sqlite3_bind_int64(queue_stmt, 1, message_id);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_int(queue_stmt, 2, priority);
    assert(rc == SQLITE_OK);
    if (style.expiration_time_.time_since_epoch().count() == 0) {
      rc = sqlite3_bind_null(queue_stmt, 3);
      assert(rc == SQLITE_OK);
      rc = sqlite3_bind_int(queue_stmt, 4, 0);
      assert(rc == SQLITE_OK);
    } else {
      rc = sqlite3_bind_int64(
          queue_stmt,
          3,
          std::chrono::duration_cast<std::chrono::seconds>(
              style.expiration_time_.time_since_epoch()).count());
      rc = sqlite3_bind_int(queue_stmt, 4, 1);
      assert(rc == SQLITE_OK);
    }
    rc = sqlite3_bind_int64(queue_stmt, 5, message.message_type());
    assert(rc == SQLITE_OK);

    RETRY_WHILE_BUSY(rc, sqlite3_step(queue_stmt));
    assert(rc == SQLITE_DONE);
    queued_rows = sqlite3_changes(conn->connection());
    sqlite3_reset(queue_stmt);
  } else {
    if (style.broadcast_) {
      rc = sqlite3_bind_int64(
          conn->queue_message_for_all_receivers_in_set_stmt(),
          1,
          message_id);
      assert(rc == SQLITE_OK);
      rc = sqlite3_bind_int(
               conn->queue_message_for_all_receivers_in_set_stmt(),
               2,
               priority);
      assert(rc == SQLITE_OK);
      if (style.expiration_time_.time_since_epoch().count() == 0) {
        rc = sqlite3_bind_null(
                 conn->queue_message_for_all_receivers_in_set_stmt(),
                 3);
        assert(rc == SQLITE_OK);
        rc = sqlite3_bind_int(
                 conn->queue_message_for_all_receivers_in_set_stmt(),
                 4,
                 0);
        assert(rc == SQLITE_OK);
      } else {
        rc = sqlite3_bind_int64(
            conn->queue_message_for_all_receivers_in_set_stmt(),
            3,
            std::chrono::duration_cast<std::chrono::seconds>(
                style.expiration_time_.time_since_epoch()).count());
        assert(rc == SQLITE_OK);
        rc = sqlite3_bind_int(
                 conn->queue_message_for_all_receivers_in_set_stmt(),
                 4,
                 1);
        assert(rc == SQLITE_OK);
      }
      rc = sqlite3_bind_int64(
          conn->queue_message_for_all_receivers_in_set_stmt(),
          5,
          message.message_type());
      assert(rc == SQLITE_OK);

      std::vector<client_id>::const_iterator receiver_it
          = destination_address.explicit_recipients_.begin();
      while (receiver_it != destination_address.explicit_recipients_.end()) {
        receiver_it = internal::SQLiteConnection::BindIntegersToSetStatement(
            receiver_it,
            destination_address.explicit_recipients_.end(),
            conn->queue_message_for_all_receivers_in_set_stmt(),
            6);

        RETRY_WHILE_BUSY(
            rc,
            sqlite3_step(conn->queue_message_for_all_receivers_in_set_stmt()));
        assert(rc == SQLITE_DONE);
        queued_rows += sqlite3_changes(conn->connection());
        sqlite3_reset(conn->queue_message_for_all_receivers_in_set_stmt());
      }
    } else {
      rc = sqlite3_bind_int64(
          conn->queue_message_for_any_receiver_in_set_stmt(),
          1,
          message_id);
      assert(rc == SQLITE_OK);
      rc = sqlite3_bind_int(
               conn->queue_message_for_any_receiver_in_set_stmt(),
               2,
               priority);
      assert(rc == SQLITE_OK);
      if (style.expiration_time_.time_since_epoch().count() == 0) {
        rc = sqlite3_bind_null(
                 conn->queue_message_for_any_receiver_in_set_stmt(),
                 3);
        assert(rc == SQLITE_OK);
        rc = sqlite3_bind_int(
                 conn->queue_message_for_any_receiver_in_set_stmt(),
                 4,
                 0);
        assert(rc == SQLITE_OK);
      } else {
        rc = sqlite3_bind_int64(
            conn->queue_message_for_any_receiver_in_set_stmt(),
            3,
            std::chrono::duration_cast<std::chrono::seconds>(
                style.expiration_time_.time_since_epoch()).count());
        assert(rc == SQLITE_OK);
        rc = sqlite3_bind_int(
                 conn->queue_message_for_any_receiver_in_set_stmt(),
                 4,
                 1);
        assert(rc == SQLITE_OK);
      }
      rc = sqlite3_bind_int64(
          conn->queue_message_for_any_receiver_in_set_stmt(),
          5,
          message.message_type());
      assert(rc == SQLITE_OK);

      std::vector<client_id>::const_iterator receiver_it
          = destination_address.explicit_recipients_.begin();
      while (receiver_it != destination_address.explicit_recipients_.end()) {
        receiver_it = internal::SQLiteConnection::BindIntegersToSetStatement(
            receiver_it,
            destination_address.explicit_recipients_.end(),
            conn->queue_message_for_any_receiver_in_set_stmt(),
            6);

        RETRY_WHILE_BUSY(
            rc,
            sqlite3_step(conn->queue_message_for_any_receiver_in_set_stmt()));
        assert(rc == SQLITE_DONE);
        queued_rows += sqlite3_changes(conn->connection());
        sqlite3_reset(conn->queue_message_for_any_receiver_in_set_stmt());
        if (queued_rows) {
          break;
        }
      }
    }
  }
  assert(queued_rows > 0);
  if (!style.broadcast_) {
    assert(queued_rows == 1);
  }

  // Commit and clean up.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());

  if (cancellation_token != nullptr) {
    cancellation_token->message_id_ = message_id;
  }
  return SendStatus::kOK;
}

void SQLiteMessageBus::CancelMessage(
    const client_id sender_id,
    const CancellationToken &cancellation_token) {
  std::vector<std::int64_t> message_ids(1, cancellation_token.message_id_);
  CancelInternal(message_ids);
}

std::size_t SQLiteMessageBus::CountQueuedMessagesForClient(
    const client_id receiver_id) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc = sqlite3_bind_int64(conn->count_queued_messages_for_client_stmt(),
                              1,
                              receiver_id);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(
      rc,
      sqlite3_step(conn->count_queued_messages_for_client_stmt()));
  assert(rc == SQLITE_ROW);
  const std::size_t messages_queued = sqlite3_column_int64(
      conn->count_queued_messages_for_client_stmt(),
      0);
  RETRY_WHILE_BUSY(
      rc,
      sqlite3_step(conn->count_queued_messages_for_client_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->count_queued_messages_for_client_stmt());

  return messages_queued;
}

void SQLiteMessageBus::InsertSendableUnchecked(
    const client_id client,
    const message_type_id message_type) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc = sqlite3_bind_int64(conn->register_sender_stmt(), 1, client);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->register_sender_stmt(), 2, message_type);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->register_sender_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->register_sender_stmt());
}

void SQLiteMessageBus::InsertReceivableUnchecked(
    const client_id client,
    const message_type_id message_type,
    const internal::ThreadsafeSet<message_type_id> &receivable_set) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc = sqlite3_bind_int64(conn->register_receiver_stmt(), 1, client);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->register_receiver_stmt(), 2, message_type);
  assert(rc == SQLITE_OK);
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->register_receiver_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->register_receiver_stmt());
}

std::int64_t SQLiteMessageBus::SendUnchecked(
    const internal::QueuedMessage &message,
    const std::vector<client_id> &verified_receivers,
    CancellationToken *cancellation_token,
    std::uint32_t *sender_message_counter) {
  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  // Begin immediate transaction.
  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  // Insert the message itself.
  rc = sqlite3_bind_int64(conn->insert_message_stmt(),
                          1,
                          message.GetAnnotatedMessage().sender);
  assert(rc == SQLITE_OK);
  if (message.GetExpirationTime().time_since_epoch().count() == 0) {
    rc = sqlite3_bind_null(conn->insert_message_stmt(), 2);
  } else {
    rc = sqlite3_bind_int64(
        conn->insert_message_stmt(),
        2,
        std::chrono::duration_cast<std::chrono::seconds>(
            message.GetExpirationTime().time_since_epoch()).count());
  }
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->insert_message_stmt(), 3, message.GetPriority());
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int64(conn->insert_message_stmt(),
                          4,
                          message.GetAnnotatedMessage()
                              .tagged_message.message_type());
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(conn->insert_message_stmt(),
                        5,
                        cancellation_token == nullptr ? 0 : 1);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_blob(conn->insert_message_stmt(),
                         6,
                         message.GetAnnotatedMessage()
                             .tagged_message.message(),
                         message.GetAnnotatedMessage()
                             .tagged_message.message_bytes(),
                         SQLITE_STATIC);
  assert(rc == SQLITE_OK);

  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->insert_message_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_int64 message_id = sqlite3_last_insert_rowid(conn->connection());
  sqlite3_reset(conn->insert_message_stmt());

  // Queue the message for each receiver.
  rc = sqlite3_bind_int64(
      conn->queue_message_for_clients_in_set_unchecked_stmt(),
      1,
      message_id);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(
      conn->queue_message_for_clients_in_set_unchecked_stmt(),
      2,
      message.GetPriority());
  assert(rc == SQLITE_OK);
  if (message.GetExpirationTime().time_since_epoch().count() == 0) {
    rc = sqlite3_bind_null(
        conn->queue_message_for_clients_in_set_unchecked_stmt(),
        3);
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_int(
        conn->queue_message_for_clients_in_set_unchecked_stmt(),
        4,
        0);
    assert(rc == SQLITE_OK);
  } else {
    rc = sqlite3_bind_int64(
        conn->queue_message_for_clients_in_set_unchecked_stmt(),
        3,
        std::chrono::duration_cast<std::chrono::seconds>(
            message.GetExpirationTime().time_since_epoch()).count());
    assert(rc == SQLITE_OK);
    rc = sqlite3_bind_int(
        conn->queue_message_for_clients_in_set_unchecked_stmt(),
        4,
        1);
    assert(rc == SQLITE_OK);
  }

  sqlite3_int64 queued_rows = 0;
  std::vector<client_id>::const_iterator receiver_it
      = verified_receivers.begin();
  while (receiver_it != verified_receivers.end()) {
    receiver_it = internal::SQLiteConnection::BindIntegersToSetStatement(
        receiver_it,
        verified_receivers.end(),
        conn->queue_message_for_clients_in_set_unchecked_stmt(),
        5);

    RETRY_WHILE_BUSY(
        rc,
        sqlite3_step(conn->queue_message_for_clients_in_set_unchecked_stmt()));
    assert(rc == SQLITE_DONE);
    queued_rows += sqlite3_changes(conn->connection());
    sqlite3_reset(conn->queue_message_for_clients_in_set_unchecked_stmt());
  }

  assert(static_cast<std::size_t>(queued_rows) == verified_receivers.size());

  // Commit.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());

  if (cancellation_token != nullptr) {
    cancellation_token->message_id_ = message_id;
  }
  return message_id;
}

internal::MemoryMirrorDeleteBatch* SQLiteMessageBus::CreateDeleteBatch(
    const client_id receiver_id) const {
  return new SQLiteMemoryMirrorDeleteBatch(receiver_id);
}

void SQLiteMessageBus::DeleteMessagesUnchecked(
    const internal::MemoryMirrorDeleteBatch &delete_batch) {
  const SQLiteMemoryMirrorDeleteBatch &delete_batch_cast
      = static_cast<const SQLiteMemoryMirrorDeleteBatch&>(delete_batch);
  if (delete_batch_cast.message_ids_.empty()) {
    return;
  }

  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  std::vector<std::int64_t>::const_iterator it
      = delete_batch_cast.message_ids_.begin();
  while (it != delete_batch_cast.message_ids_.end()) {
    rc = sqlite3_bind_int64(conn->delete_queued_messages_stmt(),
                            1,
                            delete_batch_cast.receiver_id_);
    assert(rc == SQLITE_OK);

    it = internal::SQLiteConnection::BindIntegersToSetStatement(
        it,
        delete_batch_cast.message_ids_.end(),
        conn->delete_queued_messages_stmt(),
        2);

    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->delete_queued_messages_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->delete_queued_messages_stmt());
  }

  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());
}

void SQLiteMessageBus::CancelMessageUnchecked(
    const CancellationToken &cancellation_token) {
  std::vector<int64_t> message_ids(1, cancellation_token.message_id_);
  CancelInternal(message_ids);
}

void SQLiteMessageBus::CancelMessagesUnchecked(
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  std::vector<std::int64_t> message_ids;

  while (adapter->Valid()) {
    if ((*adapter)->in_memory_cancel_flag.Valid()) {
      message_ids.push_back((*adapter)->deletion_token.message_id);
    }
    adapter->Next();
  }

  CancelInternal(message_ids);
}

std::unordered_map<client_id,
                   internal::PersistentBusStateInterface::TempClientHandle>*
    SQLiteMessageBus::LoadState() {
  std::unique_ptr<std::unordered_map<client_id, TempClientHandle>> client_map(
      new std::unordered_map<client_id, TempClientHandle>());

  if (!connection_pool_.CheckDatabaseInitialized()) {
    return client_map.release();
  }

  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_xact_stmt());

  // Create client records.
  sqlite3_stmt *select_clients = conn->select_connected_clients_stmt();
  RETRY_WHILE_BUSY(rc, sqlite3_step(select_clients));
  while (rc == SQLITE_ROW) {
    client_map->emplace(sqlite3_column_int64(select_clients, 0),
                        TempClientHandle());
    RETRY_WHILE_BUSY(rc, sqlite3_step(select_clients));
  }
  assert(rc == SQLITE_DONE);
  sqlite3_reset(select_clients);

  // Load sendable types.
  sqlite3_stmt *select_sendable = conn->select_all_sendable_stmt();
  RETRY_WHILE_BUSY(rc, sqlite3_step(select_sendable));
  while (rc == SQLITE_ROW) {
    client_map->find(sqlite3_column_int64(select_sendable, 0))
        ->second.sendable->emplace(sqlite3_column_int64(select_sendable, 1));
    RETRY_WHILE_BUSY(rc, sqlite3_step(select_sendable));
  }
  assert(rc == SQLITE_DONE);
  sqlite3_reset(select_sendable);

  // Load receivable types.
  sqlite3_stmt *select_receivable = conn->select_all_receivable_stmt();
  RETRY_WHILE_BUSY(rc, sqlite3_step(select_receivable));
  while (rc == SQLITE_ROW) {
    client_map->find(sqlite3_column_int64(select_receivable, 0))
        ->second.receivable->emplace(
            sqlite3_column_int64(select_receivable, 1));
    RETRY_WHILE_BUSY(rc, sqlite3_step(select_receivable));
  }
  assert(rc == SQLITE_DONE);
  sqlite3_reset(select_sendable);

  // Reconstruct cancellation tokens for cancellable messages.
  std::unordered_map<std::int64_t, internal::SharedBool> cancellation_flags;
  sqlite3_stmt *select_cancellable = conn->select_cancellable_messages_stmt();
  RETRY_WHILE_BUSY(rc, sqlite3_step(select_cancellable));
  while (rc == SQLITE_ROW) {
    sqlite3_int64 message_id = sqlite3_column_int64(select_cancellable, 0);
    cancellation_flags.emplace(message_id, internal::SharedBool(false));
    RETRY_WHILE_BUSY(rc, sqlite3_step(select_cancellable));
  }
  assert(rc == SQLITE_DONE);
  sqlite3_reset(select_cancellable);

  // Load messages.
  sqlite3_stmt *select_messages = conn->select_all_queued_messages_stmt();
  RETRY_WHILE_BUSY(rc, sqlite3_step(select_messages));
  while (rc == SQLITE_ROW) {
    client_id receiver_id = sqlite3_column_int64(select_messages, 0);
    std::int64_t message_id = sqlite3_column_int64(select_messages, 1);

    std::unordered_map<client_id, TempClientHandle>::iterator client_it
        = client_map->find(receiver_id);
    assert(client_it != client_map->end());

    TaggedMessage msg(sqlite3_column_blob(select_messages, 2),
                      sqlite3_column_bytes(select_messages, 2),
                      sqlite3_column_int64(select_messages, 3));

    std::chrono::time_point<std::chrono::high_resolution_clock> send_time(
        std::chrono::seconds(sqlite3_column_int64(select_messages, 6)));
    std::chrono::time_point<std::chrono::high_resolution_clock>
        expiration_time;
    if (sqlite3_column_type(select_messages, 7) != SQLITE_NULL) {
      expiration_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>(
              std::chrono::seconds(sqlite3_column_int64(select_messages, 7)));
    }

    internal::SharedBool cancellation_flag;
    std::unordered_map<std::int64_t, internal::SharedBool>::const_iterator
        cancellation_flag_it = cancellation_flags.find(message_id);
    if (cancellation_flag_it != cancellation_flags.end()) {
      cancellation_flag = cancellation_flag_it->second;
    }

    internal::QueuedMessage queued_msg(
        sqlite3_column_int64(select_messages, 4),
        sqlite3_column_int(select_messages, 5),
        send_time,
        expiration_time,
        cancellation_flag,
        std::move(msg));
    queued_msg.SetMessageID(message_id);
    client_it->second.queued_messages.emplace_back(std::move(queued_msg));

    RETRY_WHILE_BUSY(rc, sqlite3_step(select_messages));
  }
  assert(rc == SQLITE_DONE);
  sqlite3_reset(select_messages);

  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());

  return client_map.release();
}

std::size_t SQLiteMessageBus::ReceiveIfAvailableImpl(
    const client_id receiver_id,
    const Priority minimum_priority,
    const std::size_t max_messages,
    const bool delete_immediately,
    internal::ContainerPusher *pusher) {
  // NOTE(chasseur): This operation consists of 2 seperate SQL statements that
  // are NOT combined in a single transaction. This is by design. The first
  // statement attempts to get pending messages for the client and is
  // read-only. The second statement is run only if messages are successfully
  // retrieved AND 'delete_immediately' is true, and deletes the corresponding
  // entries from the queued_message table, requiring a write-lock. A row from
  // queued_message will only ever be deleted by this method (or by
  // Disconnect() called by the same receiver), or by CancelMessage(). The only
  // case where we may have a phantom read from the first statement that will
  // disappear before the second is run is if the message is cancelled, which
  // does not violate the contract of message cancellation (logically, this
  // is equivalent to a serial execution where the client which calls this
  // method receives the method BEFORE it is cancelled). We can, however, avoid
  // eagerly acquiring locks in the case where there are no pending messages
  // for a client by running 2 seperate transactions.

  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  // Get the next messages, if any, that should be delivered to this receiver.
  sqlite3_stmt *next_messages_stmt
      = (max_messages == 0) ? conn->get_next_messages_stmt()
                            : conn->get_next_messages_with_limit_stmt();
  int rc = sqlite3_bind_int64(next_messages_stmt, 1, receiver_id);
  assert(rc == SQLITE_OK);
  rc = sqlite3_bind_int(next_messages_stmt, 2, minimum_priority);
  assert(rc == SQLITE_OK);
  if (max_messages != 0) {
    rc = sqlite3_bind_int64(next_messages_stmt, 3, max_messages);
    assert(rc == SQLITE_OK);
  }

  std::vector<sqlite3_int64> received_message_ids;
  while (rc != SQLITE_DONE) {
    RETRY_WHILE_BUSY(rc, sqlite3_step(next_messages_stmt));
    assert((rc == SQLITE_ROW) || (rc == SQLITE_DONE));
    if (rc == SQLITE_ROW) {
      received_message_ids.push_back(sqlite3_column_int64(next_messages_stmt,
                                                          0));
      // Fill in message metadata and copy out the message.
      AnnotatedMessage message;
      message.sender = sqlite3_column_int64(next_messages_stmt, 1);
      message.send_time
          = std::chrono::time_point<std::chrono::high_resolution_clock>(
              std::chrono::seconds(
                  sqlite3_column_int64(next_messages_stmt, 2)));
      message_type_id message_type
          = sqlite3_column_int64(next_messages_stmt, 3);
      const void *message_bytes
          = sqlite3_column_blob(next_messages_stmt, 4);
      int message_bytes_size
          = sqlite3_column_bytes(next_messages_stmt, 4);
      message.tagged_message.set_message(message_bytes,
                                         message_bytes_size,
                                         message_type);
      message.deletion_token.message_id
          = sqlite3_column_int64(next_messages_stmt, 0);
      pusher->Push(std::move(message));
    }
  }
  sqlite3_reset(next_messages_stmt);

  if (delete_immediately && !received_message_ids.empty()) {
    // Delete the entries in queued_message.
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->begin_immediate_xact_stmt());

    // Delete expired messages while we're at it.
    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->delete_expired_messages_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->delete_expired_messages_stmt());

    std::vector<sqlite3_int64>::iterator it = received_message_ids.begin();
    while (it != received_message_ids.end()) {
      rc = sqlite3_bind_int64(conn->delete_queued_messages_stmt(),
                              1,
                              receiver_id);
      assert(rc == SQLITE_OK);

      it = internal::SQLiteConnection::BindIntegersToSetStatement(
          it,
          received_message_ids.end(),
          conn->delete_queued_messages_stmt(),
          2);

      RETRY_WHILE_BUSY(rc, sqlite3_step(conn->delete_queued_messages_stmt()));
      assert(rc == SQLITE_DONE);
      sqlite3_reset(conn->delete_queued_messages_stmt());
    }

    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->commit_xact_stmt());
  }

  // All done.
  return received_message_ids.size();
}

void SQLiteMessageBus::DeleteImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  // Make a vector of message IDs.
  std::vector<std::int64_t> message_ids;
  while (adapter->Valid()) {
    message_ids.push_back((*adapter)->deletion_token.message_id);
    adapter->Next();
  }

  DeleteInternal(receiver_id, message_ids);
}

void SQLiteMessageBus::CancelImpl(
    const client_id receiver_id,
    internal::IteratorAdapter<const AnnotatedMessage> *adapter) {
  // Make a vector of message IDs.
  std::vector<std::int64_t> message_ids;
  while (adapter->Valid()) {
    message_ids.push_back((*adapter)->deletion_token.message_id);
    adapter->Next();
  }
  CancelInternal(message_ids);
}

void SQLiteMessageBus::DeleteInternal(
    const client_id receiver_id,
    const std::vector<std::int64_t> &message_ids) {
  if (message_ids.empty()) {
    return;
  }

  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  // Begin immediate transaction in anticipation of writing.
  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  // Delete expired messages while we're at it.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->delete_expired_messages_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->delete_expired_messages_stmt());

  std::vector<std::int64_t>::const_iterator it = message_ids.begin();
  while (it != message_ids.end()) {
    // Delete entries in queued_message table.
    rc = sqlite3_bind_int64(conn->delete_queued_messages_stmt(),
                            1,
                            receiver_id);
    assert(rc == SQLITE_OK);

    it = internal::SQLiteConnection::BindIntegersToSetStatement(
        it,
        message_ids.end(),
        conn->delete_queued_messages_stmt(),
        2);

    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->delete_queued_messages_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->delete_queued_messages_stmt());
  }

  // Commit transaction.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());
}

void SQLiteMessageBus::CancelInternal(
    const std::vector<std::int64_t> &message_ids) {
  if (message_ids.empty()) {
    return;
  }

  internal::PooledSQLiteConnection conn = connection_pool_.GetConnection();

  // Begin immediate transaction in anticipation of writing.
  int rc;
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->begin_immediate_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->begin_immediate_xact_stmt());

  std::vector<std::int64_t>::const_iterator it = message_ids.begin();
  while (it != message_ids.end()) {
    // Delete entries in queued_message table.
    it = internal::SQLiteConnection::BindIntegersToSetStatement(
        it,
        message_ids.end(),
        conn->cancel_messages_stmt(),
        1);

    RETRY_WHILE_BUSY(rc, sqlite3_step(conn->cancel_messages_stmt()));
    assert(rc == SQLITE_DONE);
    sqlite3_reset(conn->cancel_messages_stmt());
  }

  // Commit transaction.
  RETRY_WHILE_BUSY(rc, sqlite3_step(conn->commit_xact_stmt()));
  assert(rc == SQLITE_DONE);
  sqlite3_reset(conn->commit_xact_stmt());
}

}  // namespace tmb
