/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "storage/SimpleScalarSeparateChainingHashTable.hpp"

#include <vector>

namespace quickstep {
namespace simple_scalar_separate_chaining_hash_table_detail {

const std::vector<bool> kKeyInlineGlobal(1, true);

}  // namespace simple_scalar_separate_chaining_hash_table_detail
}  // namespace quickstep
