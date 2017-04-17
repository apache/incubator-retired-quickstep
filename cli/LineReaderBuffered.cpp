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

#include "cli/LineReaderBuffered.hpp"

#include <string>

namespace quickstep {

LineReaderBuffered::LineReaderBuffered(const std::string &default_prompt,
                                       const std::string &continue_prompt)
    : LineReader(default_prompt, continue_prompt),
      buffer_empty_(true) {}

LineReaderBuffered::LineReaderBuffered() : LineReader("", ""), buffer_empty_(true) {}

std::string LineReaderBuffered::getLineInternal(const bool continuing) {
  // This method is called when the leftover_ string is depleted.
  buffer_empty_ = true;
  return "";
}

}  // namespace quickstep
