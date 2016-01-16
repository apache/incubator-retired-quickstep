/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "utility/textbased_test/TextBasedTestDriver.hpp"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <istream>
#include <set>
#include <string>
#include <vector>

#include "utility/textbased_test/TextBasedTest.hpp"

#include "glog/logging.h"

namespace quickstep {

namespace {

static const char *kSameAsAboveString = "[same as above]";

/**
 * @brief Returns a newline-terminated line from \p is.
 *
 * @param is The istream object from which line characters are extracted.
 * @param line The string object to store the extracted line.
 * @return True if \p line is not empty.
 */
bool GetLineWithNewLineNotTrimmed(std::istream *is, std::string *line) {
  line->clear();

  std::istream::sentry se(*is, true);
  if (se) {
    std::streambuf* sb = is->rdbuf();
    for (;;) {
      const char c = sb->sgetc();
      switch (c) {
        case '\n':
          line->push_back(c);
          sb->sbumpc();
          return true;
        case '\r':
          *line += c;
          if (sb->snextc() == '\n') {
            line->push_back(sb->sgetc());
            sb->sbumpc();
          }
          return true;
        case EOF:
          is->setstate(std::ios_base::eofbit);
          if (line->empty()) {
            return false;
          }
          return true;
        default:
          line->push_back(c);
          sb->sbumpc();
      }
    }
  }

  return false;
}

}  // namespace

TextBasedTestDriver::TextBasedTestDriver(std::istream *input_stream, TextBasedTestRunner *test_runner)
    : input_stream_(input_stream),
      test_runner_(test_runner) {
  CHECK(input_stream_->good());
}

TextBasedTestDriver::~TextBasedTestDriver() {
  clearTestCases();
}

void TextBasedTestDriver::clearTestCases() {
  for (const TextBasedTestCase *test_case : test_cases_) {
    delete test_case;
  }
  test_cases_.clear();
}

std::vector<TextBasedTestCase*>* TextBasedTestDriver::populateAndReturnTestCases() {
  populateTestCases();
  return &test_cases_;
}

void TextBasedTestDriver::writeActualOutput(std::ostream *out) const {
  const TextBasedTestCase *previous_case = nullptr;
  for (const TextBasedTestCase *test_case : test_cases_) {
    if (previous_case != nullptr) {
      *out << "==" << std::endl;
    }
    *out << test_case->preceding_text;
    *out << test_case->input_text;
    *out << "--" << std::endl;
    if (previous_case != nullptr &&
        test_case->actual_output_text == previous_case->actual_output_text) {
      *out << kSameAsAboveString << std::endl;
    } else {
      *out << test_case->actual_output_text;
    }
    previous_case = test_case;
  }
}

void TextBasedTestDriver::writeActualOutputToFile(const std::string &output_filename) const {
  std::ofstream file_stream(output_filename);
  CHECK(file_stream.is_open()) << "Output file " << output_filename << " cannot be written to";
  writeActualOutput(&file_stream);
}

void TextBasedTestDriver::populateTestCases() {
  TextBasedTestCase *test_case = readNextTestCase();
  while (test_case != nullptr) {
    CHECK(!std::all_of(test_case->input_text.begin(), test_case->input_text.end(), ::isspace)) << "Input text is empty";
    test_cases_.push_back(test_case);
    test_case = readNextTestCase();
  }
}

TextBasedTestCase* TextBasedTestDriver::readNextTestCase() {
  if (input_stream_->eof()) {
    return nullptr;
  }

  std::string line;
  std::ostringstream test_input;
  std::ostringstream non_test_input;
  std::ostringstream test_output;

  // Read leading comments.
  while (GetLineWithNewLineNotTrimmed(input_stream_, &line)) {
    DCHECK(!line.empty());
    if (line.at(0) == '[') {
      break;
    }
    if (line.at(0) != '#') {
      if (!std::all_of(line.begin(), line.end(), ::isspace)) {
        break;
      }
    }
    non_test_input << line;
  }

  if (line.empty()) {
    // We are reaching the end.
    CHECK(input_stream_->eof());
    return nullptr;
  }

  // Read option specifications.
  std::set<std::string> options;
  std::set<std::string> default_options;
  if (line.at(0) == '[') {
    readOptions(line, &options, &default_options);
    non_test_input << line;
    while (GetLineWithNewLineNotTrimmed(input_stream_, &line)) {
      DCHECK(!line.empty());
      if (line.at(0) == '[') {
        readOptions(line, &options, &default_options);
      } else {
        if (!std::all_of(line.begin(), line.end(), ::isspace)) {
          break;
        }
      }
      non_test_input << line;
    }
  }

  if (!default_options.empty()) {
    default_options_ = std::move(default_options);
  }
  options.insert(default_options_.begin(), default_options_.end());

  CHECK(!line.empty() || IsInputOutputSeparator(line)) << "Has options but no input test text";
  test_input << line;

  while (GetLineWithNewLineNotTrimmed(input_stream_, &line)) {
    if (IsInputOutputSeparator(line)) {
      break;
    }
    test_input << line;
  }

  while (GetLineWithNewLineNotTrimmed(input_stream_, &line)) {
    if (IsTestCaseSeparator(line)) {
      break;
    }
    test_output << line;
  }

  if (test_output.str().compare(0, ::strlen(kSameAsAboveString), kSameAsAboveString) == 0) {
    CHECK(!test_cases_.empty()) << "Cannot specify \"same as above\" in the first test case";
    test_output.str("");
    test_output << test_cases_.back()->expected_output_text;
  }

  return new TextBasedTestCase(non_test_input.str(),
                               test_input.str(),
                               test_output.str(),
                               std::move(options),
                               test_runner_);
}

void TextBasedTestDriver::readOptions(const std::string &line,
                                      std::set<std::string> *options_out,
                                      std::set<std::string> *default_options_out) {
  DCHECK(!line.empty() && line.at(0) == '[') << line;
  std::string token;
  std::istringstream line_stream(line);
  line_stream.get();

  // Create a temporary vector to store the options in the current line.
  // The options cannot be directly inserted to <options_out> and
  // copy the content to <default_options_out> if default is specified,
  // because <options_out> may contain non-default options given in
  // preceding lines.
  std::vector<std::string> options;
  while (line_stream >> token) {
    size_t bracket_pos = token.find(']');
    if (bracket_pos != std::string::npos) {
      CHECK_EQ(bracket_pos + 1, token.size()) << "Cannot have text after options";
      if (token.size() > 1) {
        options.push_back(token.substr(0, bracket_pos));
      }
      break;
    }
    options.push_back(token);
  }

  CHECK(!(line_stream >> token)) << "Cannot have text after options";

  bool is_default = false;
  std::vector<std::string>::const_iterator start_it = options.begin();
  std::vector<std::string>::const_iterator end_it = options.end();
  if (start_it != end_it && *start_it == "default") {
    ++start_it;
    is_default = true;
  }

  CHECK(start_it != end_it) << "Has no options specified in the brackets";

  if (is_default) {
    default_options_out->insert(start_it, end_it);
  }

  while (start_it != end_it) {
    CHECK(isRegisteredOption(*start_it)) << "Option " << *start_it << " is not found";
    options_out->insert(*start_it);
    ++start_it;
  }
}

bool TextBasedTestDriver::IsInputOutputSeparator(const std::string &str) {
  if (str.size() > 1 && str.at(0) == '-' && str.at(1) == '-') {
    return true;
  }
  return false;
}

bool TextBasedTestDriver::IsTestCaseSeparator(const std::string &str) {
  if (str.size() > 1 && str.at(0) == '=' && str.at(1) == '=') {
    return true;
  }
  return false;
}

bool TextBasedTestDriver::isValidOption(const std::string &name) const {
  if (name.empty()) return false;

  for (const char c : name) {
    if (!std::isalnum(c) && c != '_') {
      return false;
    }
  }
  return true;
}

void TextBasedTestDriver::registerOptions(const std::vector<std::string> &option_names) {
  for (const std::string &option_name : option_names) {
    registerOption(option_name);
  }
}

void TextBasedTestDriver::registerOption(const std::string &option_name) {
  CHECK(isValidOption(option_name))
      << "Option " << option_name << " is not valid (must contain only alphanumeric characters and underscores)";
  registered_options_.insert(option_name);
}

bool TextBasedTestDriver::isRegisteredOption(const std::string &option_name) const {
  return registered_options_.find(option_name) != registered_options_.end();
}

}  // namespace quickstep
