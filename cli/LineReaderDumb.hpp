/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_CLI_LINE_READER_DUMB_HPP_
#define QUICKSTEP_CLI_LINE_READER_DUMB_HPP_

#include <string>

#include "cli/LineReader.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup CLI
 *  @{
 */

/**
 * @brief An implementation of LineReader that uses cstdio and doesn't support
 *        history or command editing.
 **/
class LineReaderDumb : public LineReader {
 public:
  LineReaderDumb(const std::string &default_prompt,
                 const std::string &continue_prompt);

 protected:
  std::string getLineInternal(const bool continuing) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(LineReaderDumb);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_LINE_READER_DUMB_HPP_
