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

#include "compression/CompressionDictionaryLite.hpp"

#include <cstddef>
#include <cstdint>

#include "types/Type.hpp"

#include "glog/logging.h"

using std::size_t;
using std::uint32_t;

namespace quickstep {

CompressionDictionaryLite::CompressionDictionaryLite(
    const Type &type,
    const void *dictionary_memory,
    const std::size_t dictionary_memory_size)
    : type_(type),
      type_is_variable_length_(type_.isVariableLength()),
      dictionary_memory_(dictionary_memory),
      dictionary_memory_size_(dictionary_memory_size),
      number_of_codes_including_null_(*static_cast<const uint32_t*>(dictionary_memory_)),
      type_fixed_byte_length_(type_.maximumByteLength()) {
  uint32_t non_null_codes = number_of_codes_including_null_;
  if (*(static_cast<const uint32_t*>(dictionary_memory_) + 1)
      == number_of_codes_including_null_) {
    ++number_of_codes_including_null_;
  }

  for (code_length_bits_ = 32; code_length_bits_ > 0; --code_length_bits_) {
    if ((number_of_codes_including_null_ - 1) >> (code_length_bits_ - 1)) {
      break;
    }
  }

  if (type_is_variable_length_) {
    if (dictionary_memory_size_ <
        2 * sizeof(uint32_t) + non_null_codes * (sizeof(uint32_t) + type_.minimumByteLength())) {
      LOG(FATAL) << "Attempted to create a CompressionDictionary with "
                 << dictionary_memory_size_ << " bytes of memory, which is insufficient for "
                 << non_null_codes << " entries of type " << type_.getName() << ".";
    }

    variable_length_data_region_ = static_cast<const char*>(dictionary_memory_)
                                   + (non_null_codes + 2) * sizeof(uint32_t);

    DCHECK(paranoidOffsetsCheck());
  } else {
    if (non_null_codes * type_fixed_byte_length_ + 2 * sizeof(uint32_t)
        < dictionary_memory_size_) {
      LOG(FATAL) << "Attempted to create a CompressionDictionary with "
                 << dictionary_memory_size_ << " bytes of memory, which is insufficient for "
                 << non_null_codes << " entries of type " << type_.getName() << ".";
    }
  }

  // NOTE(chasseur): If dictionary_memory_size_ is larger than the required
  // amount of memory, it's not strictly an error, but there will be wasted
  // space.
}

CompressionDictionaryLite::~CompressionDictionaryLite() {
}

bool CompressionDictionaryLite::paranoidOffsetsCheck() const {
  uint32_t num_codes = *static_cast<const uint32_t*>(dictionary_memory_);
  size_t variable_length_offset = (num_codes + 2) * sizeof(uint32_t);
  const uint32_t *offsets_array = static_cast<const uint32_t*>(dictionary_memory_) + 2;

  size_t last_offset = variable_length_offset - 1;
  for (uint32_t code = 0; code < num_codes; ++code) {
    size_t value_offset = variable_length_offset + offsets_array[code];
    if ((value_offset >= dictionary_memory_size_)
        || (value_offset <= last_offset)) {
      return false;
    }
    last_offset = value_offset;
  }

  return true;
}

}  // namespace quickstep
