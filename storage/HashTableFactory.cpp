/**
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include "storage/HashTableFactory.hpp"

#include <cstdio>
#include <string>
#include <vector>

#include "storage/HashTable.pb.h"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

bool ValidateHashTableImplTypeString(const char *flagname,
                                     const std::string &value) {
  if ((value == "SeparateChaining") || (value == "LinearOpenAddressing")) {
    return true;
  } else {
    std::fprintf(stderr,
                 "--%s must be either SeparateChaining or LinearOpenAddressing\n",
                 flagname);
    return false;
  }
}

serialization::HashTableImplType HashTableImplTypeProtoFromString(
    const std::string &hash_table_impl_string) {
  if (hash_table_impl_string == "LinearOpenAddressing") {
    return serialization::HashTableImplType::LINEAR_OPEN_ADDRESSING;
  } else if (hash_table_impl_string == "SeparateChaining") {
    return serialization::HashTableImplType::SEPARATE_CHAINING;
  } else {
    LOG(FATAL) << "Unrecognized string for HashTable implementation type: "
               << hash_table_impl_string;
  }
}

serialization::HashTableImplType SimplifyHashTableImplTypeProto(
    const serialization::HashTableImplType proto_impl_type,
    const std::vector<const Type*> &key_types) {
  if ((proto_impl_type == serialization::HashTableImplType::SEPARATE_CHAINING)
      && (key_types.size() == 1)
      && (TypedValue::HashIsReversible(key_types.front()->getTypeID()))) {
    return serialization::HashTableImplType::SIMPLE_SCALAR_SEPARATE_CHAINING;
  } else {
    return proto_impl_type;
  }
}

}  // namespace quickstep
