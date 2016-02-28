/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include "parser/ParseCommand.hpp"

#include <string>
#include <cstdio>

namespace quickstep {

void ParseHelpCommand::execute() {
  // If the user did not enter any query text, display a generic message.
  if (!query_) {
    printf("Enter '.help {Sql Keyword} for usage information.'\n");
    return;
  }
  printf("Entered Query: %s\n", query_->value().c_str());
}

}  // namespace quickstep
