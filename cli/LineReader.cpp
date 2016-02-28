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

#include "cli/LineReader.hpp"

#include <algorithm>
#include <cctype>
#include <string>

using std::ispunct;
using std::size_t;
using std::string;

namespace quickstep {

std::string LineReader::getNextCommand() {
  string multiline_buffer(leftover_);

  // Whether we are continuing to read a command across multiple lines of
  // input.
  bool continuing = false;

  // The state of our micro command-scanner. Either normal SQL (where the whole
  // command terminates with a semicolon), various types of quoted strings
  // (which terminate with a closing quote, putting the state back to normal),
  // or SQL comments (which terminate with a newline, putting the state back to
  // normal).
  LineState line_state = kNormal;

  // The position we are scanning the multiline_buffer from.
  size_t scan_position = 0;
  // The position of a special character, which might terminate a statement or
  // signify a change in the line_state.
  for (;;) {
    size_t special_char_location = string::npos;
    switch (line_state) {
      case kNormal:
        // A semicolon to end the SQL command, or any character which might
        // put us into a different mode.
        special_char_location = multiline_buffer.find_first_of(";'\"-eE.", scan_position);
        break;
      case kSingleQuote:
        // A single quote which ends the string (note that two successive
        // single-quotes in a string, which the lexer will make into one
        // single-quote, will still be fine here: we will go into kNormal
        // and then, on the next iteration, immediately back into
        // kSingleQuote).
        special_char_location = multiline_buffer.find_first_of('\'', scan_position);
        break;
      case kSingleQuoteWithEscape:
        // A single quote which ends the string, or the beginning of an escape
        // sequence \' which does not end the string.
        special_char_location = multiline_buffer.find_first_of("'\\", scan_position);
        break;
      case kDoubleQuote:
        // A double quote which ends the string. As above, two successive
        // double-quotes will still get handled properly.
        special_char_location = multiline_buffer.find_first_of('"', scan_position);
        break;
      case kDoubleQuoteWithEscape:
        // A single quote which ends the string, or the beginning of an escape
        // sequence \" which does not end the string.
        special_char_location = multiline_buffer.find_first_of("\"\\", scan_position);
        break;
      case kComment:
      case kCommand:
        // A newline which ends the command and comment and resumes normal SQL parsing.
        special_char_location = multiline_buffer.find_first_of('\n', scan_position);
        break;
    }

    if (special_char_location == string::npos) {
      // No special character found in the buffer. Get some more input.
      scan_position = multiline_buffer.size();
      if (!getMoreInput(&multiline_buffer, &continuing)) {
        return string("");
      }
    } else {
      switch (line_state) {
        case kNormal:
          // In kNormal, we may end the SQL command or switch into one of the
          // other states.
          switch (multiline_buffer[special_char_location]) {
            case ';':
              // Command finished. Return it.
              leftover_ = multiline_buffer.substr(special_char_location + 1);
              // Clear 'leftover_' if it is blank to avoid counting the remaining
              // lines in the previous command in computing the positions of each parser node.
              if (std::all_of(leftover_.begin(), leftover_.end(), ::isspace)) {
                leftover_.clear();
              }
              return multiline_buffer.substr(0, special_char_location + 1);
            case '\'':
              // Starting a single-quote string.
              line_state = kSingleQuote;
              scan_position = special_char_location + 1;
              break;
            case '"':
              // Starting a double-quote string.
              line_state = kDoubleQuote;
              scan_position = special_char_location + 1;
              break;
            case '-':
              // Possibly starting a comment. We must peek ahead to the next
              // character to be sure.
              if (multiline_buffer.size() > special_char_location + 1) {
                if (multiline_buffer[special_char_location + 1] == '-') {
                  line_state = kComment;
                  scan_position = special_char_location + 2;
                } else {
                  // False alarm.
                  scan_position = special_char_location + 1;
                }
              } else {
                // No next character is available. Get more input and try
                // again.
                scan_position = special_char_location;
                if (!getMoreInput(&multiline_buffer, &continuing)) {
                  return string("");
                }
              }
              break;
            case 'e':
            case 'E':
              // e' or E' begins a quoted string with escape sequences. First,
              // check that the e or E either begins the string or is
              // immediately preceded by whitespace or punctuation.
              if ((special_char_location == 0)
                  || ::isspace(multiline_buffer[special_char_location - 1])
                  || ::ispunct(multiline_buffer[special_char_location - 1])) {
                // Peek ahead to see if the next character is a single-quote.
                if (multiline_buffer.size() > special_char_location + 1) {
                  if (multiline_buffer[special_char_location + 1] == '\'') {
                    line_state = kSingleQuoteWithEscape;
                    scan_position = special_char_location + 2;
                  } else if (multiline_buffer[special_char_location + 1] == '\"') {
                    line_state = kDoubleQuoteWithEscape;
                    scan_position = special_char_location + 2;
                  } else {
                    // False alarm. Just a normal character.
                    scan_position = special_char_location + 1;
                  }
                } else {
                  // Can't peek ahead yet. Get more input and re-scan from
                  // current position.
                  scan_position = special_char_location;
                  if (!getMoreInput(&multiline_buffer, &continuing)) {
                    return string("");
                  }
                }
              } else {
                scan_position = special_char_location + 1;
              }
              break;
            case '.':
              // If the dot begins the line, begin a command search.
              if (scan_position == 0) {
                line_state = kCommand;
              }
              break;
            default:
              FATAL_ERROR("Unexpected special character in LineReader::getNextCommand()");
          }
          break;
        case kSingleQuote:
        case kDoubleQuote:
        case kComment:
          // Reached the terminal character for this state. Go back to normal
          // SQL mode.
          line_state = kNormal;
          scan_position = special_char_location + 1;
          break;
        case kSingleQuoteWithEscape:
          if (multiline_buffer[special_char_location] == '\'') {
            line_state = kNormal;
            scan_position = special_char_location + 1;
          } else {
            // Skip past an escape character.
            scan_position = special_char_location + 2;
          }
          break;
        case kDoubleQuoteWithEscape:
          if (multiline_buffer[special_char_location] == '\"') {
            line_state = kNormal;
            scan_position = special_char_location + 1;
          } else {
            // Skip past an escape character.
            scan_position = special_char_location + 2;
          }
          break;
        case kCommand:
          if (multiline_buffer[special_char_location] == '\n') {
            // Command finished. Return it.
            leftover_ = multiline_buffer.substr(special_char_location + 1);
            // Clear 'leftover_' if it is blank to avoid counting the remaining
            // lines in the previous command in computing the positions of each parser node.
            if (std::all_of(leftover_.begin(), leftover_.end(), ::isspace)) {
              leftover_.clear();
            }
            return multiline_buffer.substr(0, special_char_location + 1);
          }
          break;
      }
    }
  }
}

bool LineReader::getMoreInput(std::string *input_buffer, bool *continuing) {
  string nextline = getLineInternal(*continuing);
  if (nextline.empty()) {
    return false;
  } else {
    if (*continuing
        || !std::all_of(nextline.begin(), nextline.end(), ::isspace)) {
      // Don't show the continuing prompt if a blank line was entered.
      *continuing = true;
    }
    input_buffer->append(nextline);
    return true;
  }
}

}  // namespace quickstep
