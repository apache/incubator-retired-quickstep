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

#ifndef QUICKSTEP_CLI_LOCAL_IO_HPP_
#define QUICKSTEP_CLI_LOCAL_IO_HPP_

#include <cstdio>
#include <string>

#include "cli/CliConfig.h"
#include "cli/IOInterface.hpp"
#include "cli/LineReader.hpp"
#ifdef QUICKSTEP_USE_LINENOISE
#include "cli/LineReaderLineNoise.hpp"
typedef quickstep::LineReaderLineNoise LineReaderImpl;
#else
#include "cli/LineReaderDumb.hpp"
typedef quickstep::LineReaderDumb LineReaderImpl;
#endif
#include "utility/Macros.hpp"

namespace quickstep {

class LocalIOHandle final : public IOHandle {
 public:
  LocalIOHandle() {}

  explicit LocalIOHandle(const std::string &command)
      : command_(command) {}

  ~LocalIOHandle() override {}

  FILE *in() override {
    return stdin;
  }

  FILE *out() override {
    return stdout;
  }

  FILE *err() override {
    return stderr;
  }

  std::string getCommand() const override {
    return command_;
  }

 private:
  const std::string command_;

  DISALLOW_COPY_AND_ASSIGN(LocalIOHandle);
};

/**
 * IO class for getting commands from stdin via an interactive line reader.
 */
class LocalIO final : public IOInterface {
 public:
  LocalIO() : line_reader_("quickstep> ",
                           "      ...> ") {}

  ~LocalIO() override {}

  IOHandle* getNextIOHandle() override {
    return new LocalIOHandle(line_reader_.getNextCommand());
  }

 private:
  LineReaderImpl line_reader_;

  DISALLOW_COPY_AND_ASSIGN(LocalIO);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_LOCAL_IO_HPP_
