//   Copyright 2014-2015 Quickstep Technologies LLC.
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

#ifndef TMB_INTERNAL_SQLITE_CONNECTION_H_
#define TMB_INTERNAL_SQLITE_CONNECTION_H_

#include <sqlite3.h>
#include <cassert>

namespace tmb {
namespace internal {

class SQLiteConnectionPool;

/**
 * @brief A single connection to a SQLite database, along with prepared
 *        statements for all of the underlying SQL statements used by
 *        SQLiteMessageBus.
 *
 * @warning NOT threadsafe. Must be used by only one transaction/thread at a
 *          time (but see SQLiteConnectionPool).
 **/
class SQLiteConnection {
 public:
  explicit SQLiteConnection(const char *filename);

  ~SQLiteConnection();

  inline bool Connected() const {
    return connected_;
  }

  bool CheckDatabaseInitialized();

  void InitializeDatabase(const bool use_wal);

  void Prepare();

  template <typename IteratorT>
  static IteratorT BindIntegersToSetStatement(IteratorT first,
                                              IteratorT last,
                                              sqlite3_stmt *stmt,
                                              int first_set_param,
                                              int num_set_params = -1) {
    if (num_set_params < 0) {
      num_set_params = sqlite3_bind_parameter_count(stmt) + 1 -first_set_param;
    }

    int rc = SQLITE_OK;
    for (int param_num = first_set_param;
         param_num < first_set_param + num_set_params;
         ++param_num) {
      if (first == last) {
        rc = sqlite3_bind_null(stmt, param_num);
      } else {
        rc = sqlite3_bind_int64(stmt, param_num, *first);
        ++first;
      }
      assert(rc == SQLITE_OK);
    }

    return first;
  }

  inline sqlite3* connection() const {
    return internal_connection_;
  }

  inline sqlite3_stmt* begin_xact_stmt() const {
    return begin_xact_stmt_;
  }

  inline sqlite3_stmt* begin_immediate_xact_stmt() const {
    return begin_immediate_xact_stmt_;
  }

  inline sqlite3_stmt* commit_xact_stmt() const {
    return commit_xact_stmt_;
  }

  inline sqlite3_stmt* rollback_xact_stmt() const {
    return rollback_xact_stmt_;
  }

  inline sqlite3_stmt* insert_client_stmt() const {
    assert(prepared_);
    return insert_client_stmt_;
  }

  inline sqlite3_stmt* disconnect_client_stmt() const {
    assert(prepared_);
    return disconnect_client_stmt_;
  }

  inline sqlite3_stmt* delete_sender_entries_for_client_stmt() const {
    assert(prepared_);
    return delete_sender_entries_for_client_stmt_;
  }

  inline sqlite3_stmt* delete_receiver_entries_for_client_stmt() const {
    assert(prepared_);
    return delete_receiver_entries_for_client_stmt_;
  }

  inline sqlite3_stmt* delete_queued_messages_for_client_stmt() const {
    assert(prepared_);
    return delete_queued_messages_for_client_stmt_;
  }

  inline sqlite3_stmt* register_sender_stmt() const {
    assert(prepared_);
    return register_sender_stmt_;
  }

  inline sqlite3_stmt* register_receiver_stmt() const {
    assert(prepared_);
    return register_receiver_stmt_;
  }

  inline sqlite3_stmt* check_client_connected_stmt() const {
    assert(prepared_);
    return check_client_connected_stmt_;
  }

  inline sqlite3_stmt* check_sender_registered_stmt() const {
    assert(prepared_);
    return check_sender_registered_stmt_;
  }

  inline sqlite3_stmt* check_receiver_registered_stmt() const {
    assert(prepared_);
    return check_receiver_registered_stmt_;
  }

  inline sqlite3_stmt* count_clients_in_set_stmt() const {
    assert(prepared_);
    return count_clients_in_set_stmt_;
  }

  inline sqlite3_stmt* count_receivers_in_set_stmt() const {
    assert(prepared_);
    return count_receivers_in_set_stmt_;
  }

  inline sqlite3_stmt* check_any_receiver_stmt() const {
    assert(prepared_);
    return check_any_receiver_stmt_;
  }

  inline sqlite3_stmt* insert_message_stmt() const {
    assert(prepared_);
    return insert_message_stmt_;
  }

  inline sqlite3_stmt* queue_message_for_all_receivers_stmt() const {
    assert(prepared_);
    return queue_message_for_all_receivers_stmt_;
  }

  inline sqlite3_stmt* queue_message_for_any_receiver_stmt() const {
    assert(prepared_);
    return queue_message_for_any_receiver_stmt_;
  }

  inline sqlite3_stmt* queue_message_for_all_receivers_in_set_stmt() const {
    assert(prepared_);
    return queue_message_for_all_receivers_in_set_stmt_;
  }

  inline sqlite3_stmt* queue_message_for_any_receiver_in_set_stmt() const {
    assert(prepared_);
    return queue_message_for_any_receiver_in_set_stmt_;
  }

  inline sqlite3_stmt* queue_message_for_clients_in_set_unchecked_stmt()
      const {
    assert(prepared_);
    return queue_message_for_clients_in_set_unchecked_stmt_;
  }

  inline sqlite3_stmt* get_next_messages_stmt() const {
    assert(prepared_);
    return get_next_messages_stmt_;
  }

  inline sqlite3_stmt* get_next_messages_with_limit_stmt() const {
    assert(prepared_);
    return get_next_messages_with_limit_stmt_;
  }

  inline sqlite3_stmt* delete_queued_messages_stmt() const {
    assert(prepared_);
    return delete_queued_messages_stmt_;
  }

  inline sqlite3_stmt* delete_expired_messages_stmt() const {
    assert(prepared_);
    return delete_expired_messages_stmt_;
  }

  inline sqlite3_stmt* cancel_messages_stmt() const {
    assert(prepared_);
    return cancel_messages_stmt_;
  }

  inline sqlite3_stmt* select_connected_clients_stmt() const {
    assert(prepared_);
    return select_connected_clients_stmt_;
  }

  inline sqlite3_stmt* select_all_sendable_stmt() const {
    assert(prepared_);
    return select_all_sendable_stmt_;
  }

  inline sqlite3_stmt* select_all_receivable_stmt() const {
    assert(prepared_);
    return select_all_receivable_stmt_;
  }

  inline sqlite3_stmt* select_cancellable_messages_stmt() const {
    assert(prepared_);
    return select_cancellable_messages_stmt_;
  }

  inline sqlite3_stmt* select_all_queued_messages_stmt() const {
    assert(prepared_);
    return select_all_queued_messages_stmt_;
  }

  inline sqlite3_stmt* count_queued_messages_for_client_stmt() const {
    assert(prepared_);
    return count_queued_messages_for_client_stmt_;
  }

 private:
  static const int kBusyTimeoutMS = 1000;

  void PrepareStatement(const char *sql, sqlite3_stmt **stmt);

  sqlite3 *internal_connection_;

  bool connected_;
  bool prepared_;

  sqlite3_stmt *begin_xact_stmt_;
  sqlite3_stmt *begin_immediate_xact_stmt_;
  sqlite3_stmt *commit_xact_stmt_;
  sqlite3_stmt *rollback_xact_stmt_;

  sqlite3_stmt *check_database_initialized_stmt_;

  sqlite3_stmt *insert_client_stmt_;

  sqlite3_stmt *disconnect_client_stmt_;
  sqlite3_stmt *delete_sender_entries_for_client_stmt_;
  sqlite3_stmt *delete_receiver_entries_for_client_stmt_;
  sqlite3_stmt *delete_queued_messages_for_client_stmt_;

  sqlite3_stmt *register_sender_stmt_;
  sqlite3_stmt *register_receiver_stmt_;

  sqlite3_stmt *check_client_connected_stmt_;
  sqlite3_stmt *check_sender_registered_stmt_;
  sqlite3_stmt *check_receiver_registered_stmt_;
  sqlite3_stmt *count_clients_in_set_stmt_;
  sqlite3_stmt *count_receivers_in_set_stmt_;
  sqlite3_stmt *check_any_receiver_stmt_;
  sqlite3_stmt *insert_message_stmt_;
  sqlite3_stmt *queue_message_for_all_receivers_stmt_;
  sqlite3_stmt *queue_message_for_any_receiver_stmt_;
  sqlite3_stmt *queue_message_for_all_receivers_in_set_stmt_;
  sqlite3_stmt *queue_message_for_any_receiver_in_set_stmt_;

  sqlite3_stmt *queue_message_for_clients_in_set_unchecked_stmt_;

  sqlite3_stmt *get_next_messages_stmt_;
  sqlite3_stmt *get_next_messages_with_limit_stmt_;
  sqlite3_stmt *delete_queued_messages_stmt_;
  sqlite3_stmt *delete_expired_messages_stmt_;

  sqlite3_stmt *cancel_messages_stmt_;

  sqlite3_stmt *select_connected_clients_stmt_;
  sqlite3_stmt *select_all_sendable_stmt_;
  sqlite3_stmt *select_all_receivable_stmt_;
  sqlite3_stmt *select_cancellable_messages_stmt_;
  sqlite3_stmt *select_all_queued_messages_stmt_;

  sqlite3_stmt *count_queued_messages_for_client_stmt_;

  // Disallow copy and assign:
  SQLiteConnection(const SQLiteConnection &orig) = delete;
  SQLiteConnection& operator=(const SQLiteConnection &rhs) = delete;
};


/**
 * @brief A scoped reference to a SQLiteConnection that automatically returns
 *        the connection to a SQLiteConnectionPool when it goes out of scope.
 **/
class PooledSQLiteConnection {
 public:
  PooledSQLiteConnection(
      PooledSQLiteConnection &&orig) noexcept  // NOLINT(build/c++11)
      : connection_(orig.connection_),
        pool_(orig.pool_) {
    orig.connection_ = nullptr;
    orig.pool_ = nullptr;
  }

  ~PooledSQLiteConnection();

  PooledSQLiteConnection& operator=(
      PooledSQLiteConnection &&rhs);  // NOLINT(build/c++11)

  inline bool valid() const {
    return connection_ != nullptr;
  }

  inline SQLiteConnection* get() {
    return connection_;
  }

  inline SQLiteConnection* operator->() {
    return connection_;
  }

 private:
  PooledSQLiteConnection(SQLiteConnection *connection,
                         SQLiteConnectionPool *pool)
      : connection_(connection),
        pool_(pool) {
  }

  SQLiteConnection *connection_;
  SQLiteConnectionPool *pool_;

  friend class SQLiteConnectionPool;

  // Disallow copy and assign:
  PooledSQLiteConnection(const PooledSQLiteConnection &orig) = delete;
  PooledSQLiteConnection& operator=(const PooledSQLiteConnection &rhs)
      = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_SQLITE_CONNECTION_H_
