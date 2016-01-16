/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "storage/HashTableFactory.hpp"

#include <cstdio>
#include <string>

#include "storage/HashTable.pb.h"

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

HashTableImplType HashTableImplTypeFromString(
    const std::string &hash_table_impl_string) {
  if (hash_table_impl_string == "LinearOpenAddressing") {
    return HashTableImplType::kLinearOpenAddressing;
  } else if (hash_table_impl_string == "SeparateChaining") {
    return HashTableImplType::kSeparateChaining;
  } else {
    LOG(FATAL) << "Unrecognized string for HashTable implementation type: "
               << hash_table_impl_string;
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

}  // namespace quickstep
