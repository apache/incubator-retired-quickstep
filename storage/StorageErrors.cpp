/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
