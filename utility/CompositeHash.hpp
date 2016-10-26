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

#ifndef QUICKSTEP_UTILITY_COMPOSITE_HASH_HPP_
#define QUICKSTEP_UTILITY_COMPOSITE_HASH_HPP_

#include <cstddef>
#include <vector>

#include "types/TypedValue.hpp"
#include "utility/HashPair.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief Compute the hash value of a composite key.
 *
 * @param key A vector of TypedValues which together form the composite key.
 * @return The hash value.
 **/
static std::size_t HashCompositeKey(const std::vector<TypedValue> &key) {
  DCHECK(!key.empty());
  std::size_t hash = key.front().getHash();
  for (std::vector<TypedValue>::const_iterator key_it = key.begin() + 1;
       key_it != key.end();
       ++key_it) {
    hash = CombineHashes(hash, key_it->getHash());
  }
  return hash;
}

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_COMPOSITE_HASH_HPP_
