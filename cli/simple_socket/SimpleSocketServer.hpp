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

#ifndef QUICKSTEP_CLI_SIMPLE_SOCKET_SIMPLE_SOCKET_SERVER_HPP_
#define QUICKSTEP_CLI_SIMPLE_SOCKET_SIMPLE_SOCKET_SERVER_HPP_

#include <sys/socket.h>
#include <netinet/in.h>

#include <cstring>
#include <iostream>
#include <memory>
#include <thread>

#include "cli/simple_socket/SimpleSocketConnection.hpp"
#include "utility/Macros.hpp"
#include "utility/ThreadSafeQueue.hpp"

#include "glog/logging.h"

namespace quickstep {

class SimpleSocketServer {
 public:
  SimpleSocketServer(const int port)
      : port_(port) {}

  ~SimpleSocketServer() {
    stop();
  }

  void start() {
    CHECK(main_loop_thread_ == nullptr);
    CHECK(connections_.empty());
    main_loop_thread_ = std::make_unique<std::thread>([this] {
      this->mainLoop(this->port_);
    });
  }

  void stop() {
    if (main_loop_thread_ != nullptr) {
      main_loop_thread_ = nullptr;
    }
  }

  SimpleSocketConnection* waitForConnection() {
    return connections_.popOne().release();
  }

 private:
  void mainLoop(const int port) {
    main_socket_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(main_socket_fd_,
               SOL_SOCKET, SO_REUSEADDR,
               &option,
               sizeof(option));
    CHECK(main_socket_fd_ >= 0) << "Error opening socket";

    constexpr socklen_t sockaddr_size = sizeof(sockaddr_in);
    sockaddr_in server_address;
    std::memset(&server_address, 0, sockaddr_size);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    const int bind_retval =
        bind(main_socket_fd_,
             reinterpret_cast<const sockaddr*>(&server_address),
             sockaddr_size);
    CHECK(bind_retval >= 0) << "Error binding socket";

    const int listen_retval = listen(main_socket_fd_, 32);
    CHECK(listen_retval >= 0) << "Error listening to socket connection";

    while (true) {
      socklen_t client_addr_len = sockaddr_size;
      sockaddr_in client_address;
      const int client_socket_fd =
          accept(main_socket_fd_,
                 reinterpret_cast<sockaddr*>(&client_address),
                 &client_addr_len);
      CHECK(client_socket_fd >= 0) << "Error accepting socket connection";
      connections_.push(std::make_unique<SimpleSocketConnection>(client_socket_fd));
    }
  }

  const int port_;
  std::unique_ptr<std::thread> main_loop_thread_;
  int main_socket_fd_;
  ThreadSafeQueue<std::unique_ptr<SimpleSocketConnection>> connections_;

  DISALLOW_COPY_AND_ASSIGN(SimpleSocketServer);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_SIMPLE_SOCKET_SIMPLE_SOCKET_SERVER_HPP_
