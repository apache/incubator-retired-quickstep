/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "cli/LineReaderDumb.hpp"

#include <cstddef>
#include <cstdio>
#include <string>

using std::feof;
using std::fflush;
using std::fgetc;
using std::fgets;
using std::printf;
using std::string;
using std::ungetc;

namespace quickstep {

namespace {
static constexpr std::size_t kReadBufferSize = 1024;
}  // namespace

LineReaderDumb::LineReaderDumb(const string &default_prompt, const string &continue_prompt)
    : LineReader(default_prompt, continue_prompt) {
}

std::string LineReaderDumb::getLineInternal(const bool continuing) {
  if (feof(stdin)) {
    printf("\n");
    return string("");
  }

  printf("%s", continuing ? continue_prompt_.c_str() : default_prompt_.c_str());
  fflush(stdout);

  string line;
  char read_buffer[kReadBufferSize];
  while (fgets(read_buffer, sizeof(read_buffer), stdin) != nullptr) {
    line.append(read_buffer);
    if ((!line.empty()) && (line.back() == '\n')) {
      return line;
    } else {
      if (feof(stdin)) {
        break;
      }
      const int next_char = fgetc(stdin);
      if (next_char == EOF) {
        break;
      } else {
        ungetc(next_char, stdin);
      }
    }
  }

  // The above while loop terminated because fgets() hit EOF before it got to
  // a newline.
  printf("\n");
  return string("");
}

}  // namespace quickstep
