// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_SQLITE_CONNECTION_POOL_H_
#define TMB_INTERNAL_SQLITE_CONNECTION_POOL_H_

#include <memory>
#include <string>
#include <utility>

#include "tmb/internal/lock_free_stack.h"
#include "tmb/internal/sqlite_connection.h"

namespace tmb {
namespace internal {

/**
 * @brief A shared pool of reusable SQLiteConnections. A client
 *        transaction/thread may "check out" a connection for exclusive use
 *        by calling GetConnection(), and that connection will automatically
 *        be returned to the pool when the client is done using it.
 **/
class SQLiteConnectionPool {
 public:
  SQLiteConnectionPool(const std::string &db_filename,
                       const bool use_wal)
      : db_filename_(db_filename),
        use_wal_(use_wal) {
  }

  ~SQLiteConnectionPool() {
  }

  bool CheckConnection() {
    SQLiteConnection conn(db_filename_.c_str());
    return conn.Connected();
  }

  bool CheckDatabaseInitialized() {
    SQLiteConnection conn(db_filename_.c_str());
    if (!conn.Connected()) {
      return false;
    }
    return conn.CheckDatabaseInitialized();
  }

  void InitializeDatabase() {
    SQLiteConnection *connection = new SQLiteConnection(db_filename_.c_str());
    connection->InitializeDatabase(use_wal_);
    AddToPool(connection);
  }

  PooledSQLiteConnection GetConnection() {
    std::unique_ptr<SQLiteConnection> connection;
    if (!available_connections_.Pop(&connection)) {
      connection.reset(new SQLiteConnection(db_filename_.c_str()));
      connection->Prepare();
    }
    return PooledSQLiteConnection(connection.release(), this);
  }

 private:
  inline void AddToPool(SQLiteConnection *connection) {
    available_connections_.Emplace(connection);
  }

  const std::string db_filename_;
  const bool use_wal_;
  LockFreeStack<std::unique_ptr<SQLiteConnection>> available_connections_;

  friend class PooledSQLiteConnection;

  // Disallow copy and assign:
  SQLiteConnectionPool(const SQLiteConnectionPool &orig) = delete;
  SQLiteConnectionPool& operator=(const SQLiteConnectionPool &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_SQLITE_CONNECTION_POOL_H_
