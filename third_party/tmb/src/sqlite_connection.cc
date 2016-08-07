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

#include "tmb/internal/sqlite_connection.h"

#include <sqlite3.h>
#include <cassert>

#include "tmb/internal/sqlite_connection_pool.h"

namespace {

const char kCreateTables[] =
    "CREATE TABLE client(\n"
    "    client_id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    connect_time DATETIME NOT NULL,\n"
    "    disconnect_time DATETIME NULL);\n"
    "CREATE TABLE sendable(\n"
    "    client_id INTEGER REFERENCES client(client_id) ON DELETE CASCADE,\n"
    "    type_id UNSIGNED INTEGER NOT NULL,\n"
    "        PRIMARY KEY(client_id, type_id));\n"
    "CREATE TABLE receivable(\n"
    "    client_id INTEGER REFERENCES client(client_id) ON DELETE CASCADE,\n"
    "    type_id UNSIGNED INTEGER NOT NULL,\n"
    "        PRIMARY KEY(client_id, type_id));\n"
    "CREATE TABLE message(\n"
    "    message_id INTEGER PRIMARY KEY AUTOINCREMENT,\n"
    "    sender_id INTEGER REFERENCES client(client_id) ON DELETE NO ACTION,\n"
    "    send_time DATETIME NOT NULL,\n"
    "    expiration_time DATETIME NULL,\n"
    "    priority UNSIGNED TINYINT NOT NULL,\n"
    "    type_id UNSIGNED INTEGER NOT NULL,\n"
    "    cancellable TINYINT NOT NULL,\n"
    "    payload BLOB NOT NULL);\n"
    "CREATE TABLE queued_message(\n"
    "    receiver_id INTEGER REFERENCES client(client_id) ON DELETE CASCADE,\n"
    "    message_id INTEGER REFERENCES message(message_id)\n"
    "        ON DELETE CASCADE,\n"
    "    priority UNSIGNED TINYINT NOT NULL,\n"
    "    expiration_time DATETIME NULL,\n"
    "    has_expiration_time TINYINT NOT NULL,\n"
    "        PRIMARY KEY(receiver_id, message_id));\n";

const char kCreateIndices[] =
    "CREATE INDEX receiveable_index\n"
    "    ON receivable(type_id);\n"
    "CREATE INDEX queue_index\n"
    "    ON queued_message(receiver_id, priority DESC,\n"
    "                      has_expiration_time DESC, expiration_time ASC,\n"
    "                      message_id ASC);\n"
    "CREATE INDEX queue_cancellation_index\n"
    "    ON queued_message(message_id);\n"
    "CREATE INDEX queue_expiration_index\n"
    "    ON queued_message(expiration_time);\n";

const char kDropTables[] =
    "DROP TABLE IF EXISTS queued_message;\n"
    "DROP TABLE IF EXISTS message;\n"
    "DROP TABLE IF EXISTS receivable;\n"
    "DROP TABLE IF EXISTS sendable;\n"
    "DROP TABLE IF EXISTS client;\n";

// NOTE(chasseur): For simplicity, we just check if the queued_message table
// exists. We could be more rigorous if necessary.
const char kCheckDatabaseInitialized[] =
    "SELECT EXISTS (SELECT name FROM sqlite_master\n"
    "               WHERE type='table' AND name='queued_message');\n";

const char kInsertClient[] =
    "INSERT INTO client(connect_time) VALUES(DATETIME('now'));\n";

const char kDisconnectClient[] =
    "UPDATE client\n"
    "    SET disconnect_time = DATETIME('now')\n"
    "    WHERE disconnect_time IS NULL AND client_id = ?;\n";

const char kDeleteSenderEntriesForClient[] =
    "DELETE FROM sendable WHERE client_id = ?;\n";

const char kDeleteReceiverEntriesForClient[] =
    "DELETE FROM receivable WHERE client_id = ?;\n";

const char kDeleteQueuedMessagesForClient[] =
    "DELETE FROM queued_message\n"
    "    WHERE receiver_id = ?;\n";

const char kRegisterSender[] =
    "INSERT INTO sendable(client_id, type_id) VALUES(?, ?);\n";

const char kRegisterReceiver[] =
    "INSERT INTO receivable(client_id, type_id) VALUES(?, ?);\n";

const char kCheckClientConnected[] =
    "SELECT EXISTS (SELECT client_id FROM client\n"
    "    WHERE disconnect_time IS NULL and client_id = ?);\n";

const char kCheckSenderRegistered[] =
    "SELECT EXISTS (SELECT client_id FROM sendable\n"
    "    WHERE client_id = ? AND type_id = ?);\n";

const char kCheckReceiverRegistered[] =
    "SELECT EXISTS (SELECT client_id FROM receivable\n"
    "    WHERE client_id = ? AND type_id = ?);\n";

const char kCountClientsInSet[] =
    "SELECT COUNT(*) FROM client\n"
    "    WHERE disconnect_time IS NULL\n"
    "          AND client_id IN (?, ?, ?, ?, ?, ?, ?, ?);\n";

const char kCountReceiversInSet[] =
    "SELECT COUNT(*) from receivable\n"
    "    WHERE type_id = ?\n"
    "          AND client_id IN (?, ?, ?, ?, ?, ?, ?, ?)\n";

const char kCheckAnyReceiver[] =
    "SELECT EXISTS (SELECT client_id FROM receivable WHERE type_id = ?);\n";

const char kInsertMessage[] =
    "INSERT INTO message(sender_id, send_time, expiration_time, priority,\n"
    "                    type_id, cancellable, payload)\n"
    "    VALUES(?, DATETIME('now'), DATETIME(?, 'unixepoch'), ?, ?, ?, ?);\n";

const char kQueueMessageForAllReceivers[] =
    "INSERT INTO queued_message(receiver_id, message_id, priority,\n"
    "                           expiration_time, has_expiration_time)\n"
    "    SELECT client_id, ?, ?, DATETIME(?, 'unixepoch'), ?\n"
    "        FROM receivable\n"
    "        WHERE type_id = ?;\n";

const char kQueueMessageForAnyReceiver[] =
    "INSERT INTO queued_message(receiver_id, message_id, priority,\n"
    "                           expiration_time, has_expiration_time)\n"
    "    SELECT client_id, ?, ?, DATETIME(?, 'unixepoch'), ?\n"
    "        FROM receivable\n"
    "        WHERE type_id = ?\n"
    "        LIMIT 1;\n";

const char kQueueMessageForAllReceiversInSet[] =
    "INSERT INTO queued_message(receiver_id, message_id, priority,\n"
    "                           expiration_time, has_expiration_time)\n"
    "    SELECT client_id, ?, ?, DATETIME(?, 'unixepoch'), ?\n"
    "        FROM receivable\n"
    "        WHERE type_id = ?\n"
    "              AND client_id IN (?, ?, ?, ?, ?, ?, ?, ?);\n";

const char kQueueMessageForAnyReceiverInSet[] =
    "INSERT INTO queued_message(receiver_id, message_id, priority,\n"
    "                           expiration_time, has_expiration_time)\n"
    "    SELECT client_id, ?, ?, DATETIME(?, 'unixepoch'), ?\n"
    "        FROM receivable\n"
    "        WHERE type_id = ?\n"
    "              AND client_id IN (?, ?, ?, ?, ?, ?, ?, ?)\n"
    "        LIMIT 1;\n";

// TODO(chasseur): This implicitly checks that a client at least exists in the
// clients table, which isn't actually necessary for the unchecked version.
// Find a way to avoid the nested SELECT without repeating common parameters.
const char kQueueMessageForClientsInSetUnchecked[] =
    "INSERT INTO queued_message(receiver_id, message_id, priority,\n"
    "                           expiration_time, has_expiration_time)\n"
    "    SELECT client_id, ?, ?, DATETIME(?, 'unixepoch'), ?\n"
    "        FROM client\n"
    "        WHERE client_id IN (?, ?, ?, ?, ?, ?, ?, ?);\n";

const char kGetNextMessages[] =
    "SELECT message.message_id, message.sender_id, message.send_time,\n"
    "       message.type_id, message.payload\n"
    "    FROM message, queued_message\n"
    "    WHERE message.message_id = queued_message.message_id\n"
    "          AND queued_message.receiver_id = ?\n"
    "          AND queued_message.priority >= ?\n"
    "          AND (queued_message.has_expiration_time = 0\n"
    "               OR queued_message.expiration_time >= DATETIME('now'))\n"
    "    ORDER BY queued_message.priority DESC,\n"
    "             queued_message.has_expiration_time DESC,\n"
    "             queued_message.expiration_time ASC,\n"
    "             queued_message.message_id ASC;\n";

const char kGetNextMessagesWithLimit[] =
    "SELECT message.message_id, message.sender_id, message.send_time,\n"
    "       message.type_id, message.payload\n"
    "    FROM message, queued_message\n"
    "    WHERE message.message_id = queued_message.message_id\n"
    "          AND queued_message.receiver_id = ?\n"
    "          AND queued_message.priority >= ?\n"
    "          AND (queued_message.has_expiration_time = 0\n"
    "               OR queued_message.expiration_time >= DATETIME('now'))\n"
    "    ORDER BY queued_message.priority DESC,\n"
    "             queued_message.has_expiration_time DESC,\n"
    "             queued_message.expiration_time ASC,\n"
    "             queued_message.message_id ASC\n"
    "    LIMIT ?;\n";

const char kDeleteQueuedMessages[] =
    "DELETE FROM queued_message\n"
    "    WHERE receiver_id = ? AND message_id IN (?, ?, ?, ?, ?, ?, ?, ?);\n";

const char kDeleteExpiredMessages[] =
    "DELETE FROM queued_message WHERE expiration_time < DATETIME('now');\n";

const char kCancelMessages[] =
    "DELETE FROM message WHERE message_id IN (?, ?, ?, ?, ?, ?, ?, ?)\n"
    "                          AND cancellable = 1;\n";

const char kSelectConnectedClients[] =
    "SELECT client_id FROM client WHERE disconnect_time IS NULL;\n";

const char kSelectAllSendable[] =
    "SELECT client_id, type_id FROM sendable;\n";

const char kSelectAllReceivable[] =
    "SELECT client_id, type_id FROM receivable;\n";

const char kSelectCancellableMessages[] =
    "SELECT message_id FROM message WHERE cancellable = 1;\n";

const char kSelectAllQueuedMessages[] =
    "SELECT queued_message.receiver_id, message.message_id,\n"
    "       message.payload, message.type_id, message.sender_id,\n"
    "       message.priority, message.send_time, message.expiration_time\n"
    "    FROM message, queued_message\n"
    "    WHERE message.message_id = queued_message.message_id;\n";

const char kCountQueuedMessagesForClient[] =
    "SELECT COUNT(*) FROM queued_message WHERE receiver_id = ?;\n";

}  // namespace

namespace tmb {
namespace internal {

SQLiteConnection::SQLiteConnection(const char *filename)
    : internal_connection_(nullptr),
      connected_(false),
      prepared_(false) {
  // Open connection.
  int rc = sqlite3_open_v2(filename,
                           &internal_connection_,
                           SQLITE_OPEN_READWRITE
                               | SQLITE_OPEN_CREATE
                               | SQLITE_OPEN_NOMUTEX
                               | SQLITE_OPEN_PRIVATECACHE,
                           nullptr);
  if (rc != SQLITE_OK) {
    if (internal_connection_ != nullptr) {
      rc = sqlite3_close(internal_connection_);
      assert(rc == SQLITE_OK);
      internal_connection_ = nullptr;
      return;
    }
  }

  // Enable extended result codes for finer-grained error handling.
  rc = sqlite3_extended_result_codes(internal_connection_, 1);
  if (rc != SQLITE_OK) {
    rc = sqlite3_close(internal_connection_);
    assert(rc == SQLITE_OK);
    internal_connection_ = nullptr;
    return;
  }

  // Turn on foreign-key enforcement.
  rc = sqlite3_exec(internal_connection_,
                    "PRAGMA foreign_keys = ON;",
                    nullptr, nullptr, nullptr);
  if (rc != SQLITE_OK) {
    rc = sqlite3_close(internal_connection_);
    assert(rc == SQLITE_OK);
    internal_connection_ = nullptr;
    return;
  }

  // Set the busy retry timeout for transactions to wait on locks.
  rc = sqlite3_busy_timeout(internal_connection_, kBusyTimeoutMS);
  if (rc != SQLITE_OK) {
    rc = sqlite3_close(internal_connection_);
    assert(rc == SQLITE_OK);
    internal_connection_ = nullptr;
    return;
  }

  // Prepare non-schema dependent SQL statements.
  PrepareStatement("BEGIN TRANSACTION;", &begin_xact_stmt_);
  PrepareStatement("BEGIN IMMEDIATE TRANSACTION;",
                   &begin_immediate_xact_stmt_);
  PrepareStatement("COMMIT;", &commit_xact_stmt_);
  PrepareStatement("ROLLBACK;", &rollback_xact_stmt_);
  PrepareStatement(kCheckDatabaseInitialized,
                   &check_database_initialized_stmt_);

  connected_ = true;
}

SQLiteConnection::~SQLiteConnection() {
  if (connected_) {
    sqlite3_finalize(begin_xact_stmt_);
    sqlite3_finalize(begin_immediate_xact_stmt_);
    sqlite3_finalize(commit_xact_stmt_);
    sqlite3_finalize(rollback_xact_stmt_);
    sqlite3_finalize(check_database_initialized_stmt_);
  }

  if (prepared_) {
    sqlite3_finalize(insert_client_stmt_);

    sqlite3_finalize(disconnect_client_stmt_);
    sqlite3_finalize(delete_sender_entries_for_client_stmt_);
    sqlite3_finalize(delete_receiver_entries_for_client_stmt_);
    sqlite3_finalize(delete_queued_messages_for_client_stmt_);

    sqlite3_finalize(register_sender_stmt_);
    sqlite3_finalize(register_receiver_stmt_);

    sqlite3_finalize(check_client_connected_stmt_);
    sqlite3_finalize(check_sender_registered_stmt_);
    sqlite3_finalize(check_receiver_registered_stmt_);
    sqlite3_finalize(count_clients_in_set_stmt_);
    sqlite3_finalize(count_receivers_in_set_stmt_);
    sqlite3_finalize(check_any_receiver_stmt_);
    sqlite3_finalize(insert_message_stmt_);
    sqlite3_finalize(queue_message_for_all_receivers_stmt_);
    sqlite3_finalize(queue_message_for_any_receiver_stmt_);
    sqlite3_finalize(queue_message_for_all_receivers_in_set_stmt_);
    sqlite3_finalize(queue_message_for_any_receiver_in_set_stmt_);

    sqlite3_finalize(queue_message_for_clients_in_set_unchecked_stmt_);

    sqlite3_finalize(get_next_messages_stmt_);
    sqlite3_finalize(get_next_messages_with_limit_stmt_);
    sqlite3_finalize(delete_queued_messages_stmt_);
    sqlite3_finalize(delete_expired_messages_stmt_);

    sqlite3_finalize(cancel_messages_stmt_);

    sqlite3_finalize(select_connected_clients_stmt_);
    sqlite3_finalize(select_all_sendable_stmt_);
    sqlite3_finalize(select_all_receivable_stmt_);
    sqlite3_finalize(select_cancellable_messages_stmt_);
    sqlite3_finalize(select_all_queued_messages_stmt_);

    sqlite3_finalize(count_queued_messages_for_client_stmt_);
  }

  if (connected_) {
    int rc = sqlite3_close(internal_connection_);
    assert(rc == SQLITE_OK);
  }
}

bool SQLiteConnection::CheckDatabaseInitialized() {
  assert(connected_);
  int rc;
  do {
    rc = sqlite3_step(check_database_initialized_stmt_);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_ROW);
  bool initialized = sqlite3_column_int(check_database_initialized_stmt_, 0)
                     != 0;
  do {
    rc = sqlite3_step(check_database_initialized_stmt_);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_DONE);
  sqlite3_reset(check_database_initialized_stmt_);
  return initialized;
}

void SQLiteConnection::InitializeDatabase(const bool use_wal) {
  assert(connected_);
  int rc;

  // Write-ahead logging is only supported in SQLite 3.7.0+.
  if (use_wal && (SQLITE_VERSION_NUMBER >= 3007000)) {
    do {
      rc = sqlite3_exec(internal_connection_, "PRAGMA journal_mode = WAL;",
                        nullptr, nullptr, nullptr);
    } while (rc == SQLITE_BUSY);
    assert(rc == SQLITE_OK);
  }

  do {
    rc = sqlite3_step(begin_immediate_xact_stmt_);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_DONE);
  sqlite3_reset(begin_immediate_xact_stmt_);

  do {
    rc = sqlite3_exec(internal_connection_, kDropTables,
                      nullptr, nullptr, nullptr);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_OK);

  do {
    rc = sqlite3_exec(internal_connection_, kCreateTables,
                      nullptr, nullptr, nullptr);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_OK);

  do {
    rc = sqlite3_exec(internal_connection_, kCreateIndices,
                      nullptr, nullptr, nullptr);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_OK);

  do {
    rc = sqlite3_step(commit_xact_stmt_);
  } while (rc == SQLITE_BUSY);
  assert(rc == SQLITE_DONE);
  sqlite3_reset(commit_xact_stmt_);

  if (!prepared_) {
    Prepare();
  }
}

void SQLiteConnection::Prepare() {
  assert(connected_);

  PrepareStatement(kInsertClient, &insert_client_stmt_);

  PrepareStatement(kDisconnectClient, &disconnect_client_stmt_);
  PrepareStatement(kDeleteSenderEntriesForClient,
                   &delete_sender_entries_for_client_stmt_);
  PrepareStatement(kDeleteReceiverEntriesForClient,
                   &delete_receiver_entries_for_client_stmt_);
  PrepareStatement(kDeleteQueuedMessagesForClient,
                   &delete_queued_messages_for_client_stmt_);

  PrepareStatement(kRegisterSender, &register_sender_stmt_);
  PrepareStatement(kRegisterReceiver, &register_receiver_stmt_);

  PrepareStatement(kCheckClientConnected, &check_client_connected_stmt_);
  PrepareStatement(kCheckSenderRegistered, &check_sender_registered_stmt_);
  PrepareStatement(kCheckReceiverRegistered, &check_receiver_registered_stmt_);
  PrepareStatement(kCountClientsInSet, &count_clients_in_set_stmt_);
  PrepareStatement(kCountReceiversInSet, &count_receivers_in_set_stmt_);
  PrepareStatement(kCheckAnyReceiver, &check_any_receiver_stmt_);
  PrepareStatement(kInsertMessage, &insert_message_stmt_);
  PrepareStatement(kQueueMessageForAllReceivers,
                   &queue_message_for_all_receivers_stmt_);
  PrepareStatement(kQueueMessageForAnyReceiver,
                   &queue_message_for_any_receiver_stmt_);
  PrepareStatement(kQueueMessageForAllReceiversInSet,
                   &queue_message_for_all_receivers_in_set_stmt_);
  PrepareStatement(kQueueMessageForAnyReceiverInSet,
                   &queue_message_for_any_receiver_in_set_stmt_);

  PrepareStatement(kQueueMessageForClientsInSetUnchecked,
                   &queue_message_for_clients_in_set_unchecked_stmt_);

  PrepareStatement(kGetNextMessages, &get_next_messages_stmt_);
  PrepareStatement(kGetNextMessagesWithLimit,
                   &get_next_messages_with_limit_stmt_);
  PrepareStatement(kDeleteQueuedMessages, &delete_queued_messages_stmt_);
  PrepareStatement(kDeleteExpiredMessages, &delete_expired_messages_stmt_);

  PrepareStatement(kCancelMessages, &cancel_messages_stmt_);

  PrepareStatement(kSelectConnectedClients, &select_connected_clients_stmt_);
  PrepareStatement(kSelectAllSendable, &select_all_sendable_stmt_);
  PrepareStatement(kSelectAllReceivable, &select_all_receivable_stmt_);
  PrepareStatement(kSelectCancellableMessages,
                   &select_cancellable_messages_stmt_);
  PrepareStatement(kSelectAllQueuedMessages,
                   &select_all_queued_messages_stmt_);

  PrepareStatement(kCountQueuedMessagesForClient,
                   &count_queued_messages_for_client_stmt_);

  prepared_ = true;
}

void SQLiteConnection::PrepareStatement(const char *sql, sqlite3_stmt **stmt) {
  int rc = SQLITE_BUSY;
  while (rc == SQLITE_BUSY) {
    rc = sqlite3_prepare_v2(internal_connection_, sql, -1, stmt, nullptr);
  }
  assert(rc == SQLITE_OK);
}

PooledSQLiteConnection::~PooledSQLiteConnection() {
  if (valid()) {
    pool_->AddToPool(connection_);
  }
}

PooledSQLiteConnection& PooledSQLiteConnection::operator=(
    PooledSQLiteConnection &&rhs) {  // NOLINT(build/c++11)
  if (valid()) {
    pool_->AddToPool(connection_);
  }

  connection_ = rhs.connection_;
  pool_ = rhs.pool_;

  rhs.connection_ = nullptr;
  rhs.pool_ = nullptr;

  return *this;
}

}  // namespace internal
}  // namespace tmb
