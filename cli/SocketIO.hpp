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

#ifndef QUICKSTEP_CLI_SOCKET_IO_HPP_
#define QUICKSTEP_CLI_SOCKET_IO_HPP_

#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "cli/IOInterface.hpp"
#include "cli/LineReaderBuffered.hpp"
#include "cli/simple_socket/SimpleSocketConnection.hpp"
#include "cli/simple_socket/SimpleSocketContent.hpp"
#include "cli/simple_socket/SimpleSocketServer.hpp"
#include "utility/Macros.hpp"
#include "utility/MemStream.hpp"
#include "utility/StringUtil.hpp"
#include "utility/ThreadSafeQueue.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DECLARE_int32(cli_socket_port);
DECLARE_string(cli_socket_ip);

class SocketIOHandle final : public IOHandle {
 public:
  explicit SocketIOHandle(SimpleSocketConnection *conn)
      : conn_(conn),
        request_(conn_->getRequest()) {
    const auto *it = request_.get("query");
    CHECK(it != nullptr);
    LineReaderBuffered line_reader;
    line_reader.setBuffer(std::string(it->first, it->second));
    while (!line_reader.bufferEmpty()) {
      std::string command = line_reader.getNextCommand();
      if (!command.empty()) {
        commands_.emplace_back(std::move(command));
      }
    }
  }

  ~SocketIOHandle() override {
    conn_->sendResponse(out_stream_.str(), err_stream_.str());
  }

  StringPiece data() const override {
    const StringPiece *entry = request_.get("data");
    return entry == nullptr ? StringPiece(nullptr, 0) : *entry;
  }

  FILE* out() override {
    return out_stream_.file();
  }

  FILE* err() override {
    return err_stream_.file();
  }

  std::vector<std::string> getCommands() const override {
    return commands_;
  }

 private:
  std::unique_ptr<SimpleSocketConnection> conn_;
  const SimpleSocketContent &request_;
  MemStream out_stream_, err_stream_;
  std::vector<std::string> commands_;

  DISALLOW_COPY_AND_ASSIGN(SocketIOHandle);
};

/**
 * A network interface that uses native socket to accept commands.
 */
class SocketIO final : public IOInterface {
 public:
  SocketIO()
      : server_(FLAGS_cli_socket_port) {
    server_.start();
  }

  ~SocketIO() override {
    server_.stop();
  }

  IOHandle* getNextIOHandle() override {
    return new SocketIOHandle(server_.waitForConnection());
  }

 private:
  SimpleSocketServer server_;

  DISALLOW_COPY_AND_ASSIGN(SocketIO);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_SOCKET_IO_HPP_
