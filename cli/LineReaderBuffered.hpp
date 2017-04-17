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

#ifndef QUICKSTEP_CLI_LINE_READER_BUFFERED_HPP_
#define QUICKSTEP_CLI_LINE_READER_BUFFERED_HPP_

#include <string>

#include "cli/LineReader.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class LineReaderBuffered : public LineReader {
 public:
  /**
   * @brief A line reader which accepts a string buffer.
   * Other line readers are meant to support some form of user interaction. This linereader does not and is intended for
   * programmer use- it does not print anything to stdout. Therefore it ignores any prompt strings given to the
   * inherited constructor.
   * @param default_prompt Not used by this line reader, but required by interface.
   * @param continue_prompt Not used by this line reader, but required by interface.
   */
  LineReaderBuffered(const std::string &default_prompt,
                     const std::string &continue_prompt);

  LineReaderBuffered();

  ~LineReaderBuffered() override {}

  /**
   * @brief Replaces the current buffer contents with new contents.
   * @param buffer Replacement text.
   */
  void setBuffer(std::string buffer) {
    leftover_ = buffer;
    buffer_empty_ = false;
  }

  /**
   * @brief This is set to true after getNextCommand is called and runs out of input to process.
   * @return True if the buffer has been consumed.
   */
  bool bufferEmpty() const {
    return buffer_empty_;
  }

 protected:
  std::string getLineInternal(const bool continuing) override;

 private:
  bool buffer_empty_;

  DISALLOW_COPY_AND_ASSIGN(LineReaderBuffered);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_LINE_READER_BUFFERED_HPP_
