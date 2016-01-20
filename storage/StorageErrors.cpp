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

#include "storage/StorageErrors.hpp"

#include <string>

namespace quickstep {

BlockMemoryTooSmall::BlockMemoryTooSmall(const std::string &block_type,
                                         const std::size_t block_size)
    : message_("BlockMemoryTooSmall: ") {
  message_.append(std::to_string(block_size));
  message_.append(" bytes is too small to create a block/subblock of type ");
  message_.append(block_type);
}

FileReadError::FileReadError(const std::string &filename)
    : message_("FileReadError: Error while reading file ") {
  message_.append(filename);
}

FileWriteError::FileWriteError(const std::string &filename)
    : message_("FileWriteError: Error while writing file ") {
  message_.append(filename);
}

TupleTooLargeForBlock::TupleTooLargeForBlock(const std::size_t tuple_size)
    : tuple_size_(tuple_size),
      message_("TupleTooLargeForBlock: ") {
  if (tuple_size_ == 0) {
    message_.append("Tuple of unknown size is too large to insert into an empty block");
  } else {
    message_.append("Tuple of size ");
    message_.append(std::to_string(tuple_size_));
    message_.append(" bytes is too large to insert into an empty block");
  }
}

UnableToDeleteFile::UnableToDeleteFile(const std::string &filename)
    : message_("UnableToDeleteFile: filename ") {
  message_.append(filename);
}

UnableToOpenFile::UnableToOpenFile(const std::string &filename)
    : message_("UnableToOpenFile: filename ") {
  message_.append(filename);
}

}  // namespace quickstep
