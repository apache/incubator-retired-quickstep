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

#ifndef QUICKSTEP_CLI_LINE_READER_LINE_NOISE_HPP_
#define QUICKSTEP_CLI_LINE_READER_LINE_NOISE_HPP_

#include <string>

#include "cli/LineReader.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup CLI
 *  @{
 */

/**
 * @brief An implementation of LineReader that uses linenoise to provide
 *        command-history and editing support.
 **/
class LineReaderLineNoise : public LineReader {
 public:
  LineReaderLineNoise(const std::string &default_prompt,
                      const std::string &continue_prompt);

  ~LineReaderLineNoise() override {
  }

 protected:
  std::string getLineInternal(const bool continuing) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(LineReaderLineNoise);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_LINE_READER_LINE_NOISE_HPP_
