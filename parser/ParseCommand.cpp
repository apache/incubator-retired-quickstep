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

#include <cstdio>
#include <map>
#include <string>

#include "parser/ParseString.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

using std::string;
using std::map;

namespace quickstep {

std::map<const std::string, std::string> ParseHelpCommand::help_messages_;

void ParseHelpCommand::execute() {
  // If the user did not enter any query text, display a generic message.
  if (!query_) {
    printf("Enter '.help {Sql Keyword} for usage information.'\n");
    return;
  }

  // Look up the query in the map. If an exact match is not found, suggest
  // the closest keyword.
  const string lower_query = ToLower(query_->value());
  const map<const string, string> &message_map = getHelpMessageMap();

  // Try to find an exact match.
  auto exact_itr = message_map.find(lower_query);

  if (exact_itr != message_map.end()) {
    printf("%s\n", exact_itr->second.c_str());
    return;
  }
  // If an exact match cannot be found, try to find a close match and suggest
  // this keyword.
  printf("Keyword %s not found\n", query_->value().c_str());
  for (auto itr = message_map.begin();
       itr != message_map.end();
       ++itr) {
    // Check if the query begins with the map key, or visa-versa.
    string key_string = itr->first;
    if (key_string.length() > lower_query.length()) {
      if (key_string.compare(0, lower_query.length(), lower_query) == 0) {
        printf("did you mean %s?\n", key_string.c_str());
        return;
      }
    } else {
      if (lower_query.compare(0, key_string.length(), key_string) == 0) {
        printf("did you mean %s?\n", key_string.c_str());
        return;
      }
    }
  }
}

const map<const string, string>& ParseHelpCommand::getHelpMessageMap() {
  if (help_messages_.empty()) {
    // Construct the map.
    help_messages_["create"] = "CREATE TABLE";
    help_messages_["type"] = "INT | FLOAT | VARCHAR | CHAR";
    help_messages_["blockproperties"] = "[BLOCKPROPERTIES]";
  }
  return help_messages_;
}

}  // namespace quickstep
