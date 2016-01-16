/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
