/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_CLI_LINE_READER_HPP_
#define QUICKSTEP_CLI_LINE_READER_HPP_

#include <string>

#include "utility/Macros.hpp"

using std::size_t;
using std::string;

namespace quickstep {

/** \addtogroup CLI
 *  @{
 */

/**
 * @brief An interface for reading user input from command lines.
 **/
class LineReader {
 public:
  /**
   * @brief Constructor.
   *
   * @param default_prompt The default command prompt.
   * @param continue_prompt The prompt given when a command extends over more
   *        than one line.
   **/
  LineReader(const std::string &default_prompt,
             const std::string &continue_prompt)
      : default_prompt_(default_prompt), continue_prompt_(continue_prompt) {
  }

  /**
   * @brief Virtual destructor.
   **/
  virtual ~LineReader() {
  }

  /**
   * @brief Obtain the next full SQL command from user input.
   *
   * @return The next user-inputted command.
   **/
  std::string getNextCommand();

 protected:
  /**
   * @brief Get the next newline-terminated line from input.
   *
   * @param continuing True if continuing a command from a previous line, false
   *        otherwise.
   * @return The next line of input.
   **/
  virtual std::string getLineInternal(const bool continuing) = 0;

  const std::string default_prompt_;
  const std::string continue_prompt_;
  std::string leftover_;

 private:
  enum LineState {
    kNormal,
    kSingleQuote,
    kSingleQuoteWithEscape,
    kDoubleQuote,
    kDoubleQuoteWithEscape,
    kComment,
    kCommand
  };

  bool getMoreInput(std::string *input_buffer, bool *continuing);

  DISALLOW_COPY_AND_ASSIGN(LineReader);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_LINE_READER_HPP_
