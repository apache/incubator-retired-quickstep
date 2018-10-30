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

#ifndef QUICKSTEP_CLI_SIMPLE_SOCKET_SIMPLE_SOCKET_CONTENT_HPP_
#define QUICKSTEP_CLI_SIMPLE_SOCKET_SIMPLE_SOCKET_CONTENT_HPP_

#include <cstddef>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

#include "utility/Macros.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {

class SimpleSocketContent {
 public:
  SimpleSocketContent() {}

  bool contains(const std::string &key) const {
    return fields_.find(key) != fields_.end();
  }

  const StringPiece* get(const std::string &key) const {
    const auto it = fields_.find(key);
    return it == fields_.end() ? nullptr : &it->second;
  }

  void setField(const std::string &key, const std::string &value) {
    storage_.emplace_back(value);
    const std::string &sv = storage_.back();
    fields_.emplace(key, StringPiece(sv.c_str(), sv.length()));
  }

  const std::unordered_map<std::string, StringPiece>& fields() const {
    return fields_;
  }

 private:
  void setField(const char *key, const std::size_t key_size,
                const char *value, const std::size_t value_size) {
    fields_.emplace(std::string(key, key_size),
                    StringPiece(value, value_size));
  }

  std::unordered_map<std::string, StringPiece> fields_;
  std::list<std::string> storage_;

  friend class SimpleSocketConnection;

  DISALLOW_COPY_AND_ASSIGN(SimpleSocketContent);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_SIMPLE_SOCKET_SIMPLE_SOCKET_CONTENT_HPP_
