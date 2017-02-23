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

#ifndef QUICKSTEP_STORAGE_STORAGE_ERRORS_HPP_
#define QUICKSTEP_STORAGE_STORAGE_ERRORS_HPP_

#include <cstddef>
#include <exception>
#include <string>

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Exception thrown when the memory provided to a block or subblock is
 *        too small for even basic metadata.
 **/
class BlockMemoryTooSmall : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param block_type The type of block or subblock that we were attempting
   *                   to create.
   * @param block_size The number of bytes allocated to the block or subblock.
   **/
  BlockMemoryTooSmall(const std::string &block_type, const std::size_t block_size);

  ~BlockMemoryTooSmall() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when trying to access a block which isn't in memory.
 **/
class BlockNotFoundInMemory : public std::exception {
 public:
  virtual const char* what() const throw() {
    return "BlockNotFoundInMemory: The specified block was not found in memory";
  }
};

/**
 * @brief Exception thrown when trying to load a block which can't be found on
 *        the persistent storage.
 **/
class BlockNotFoundInPersistentStorage : public std::exception {
 public:
  virtual const char* what() const throw() {
    return "BlockNotFoundInPersistentStorage: The specified block was not found in the persistent storage";
  }
};

/**
 * @brief Exception thrown when the persistent storage is not in the proper
 *        format.
 **/
class CorruptPersistentStorage : public std::exception {
 public:
  virtual const char* what() const throw() {
    return "CorruptPersistentStorage: The persistent storage is corrupt";
  }
};

/**
 * @brief Exception thrown when there is an error while writing to an open
 *        file (e.g. running out of space).
 **/
class FileReadError : public std::exception {
 public:
  explicit FileReadError(const std::string &filename);

  virtual ~FileReadError() throw() {
  }

  virtual const char *what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when there is an error while writing to an open
 *        file (e.g. running out of space).
 **/
class FileWriteError : public std::exception {
 public:
  explicit FileWriteError(const std::string &filename);

  virtual ~FileWriteError() throw() {
  }

  virtual const char *what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception throw when a re-loaded block appears to be corrupted.
 **/
class MalformedBlock : public std::exception {
 public:
  virtual const char* what() const throw() {
    return "MalformedBlock: A reconstituted block appears to be malformed";
  }
};

/**
 * @brief Exception thrown when an attempt to allocate memory fails.
 **/
class OutOfMemory : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param num_slots The number of slots to allocate.
   **/
  explicit OutOfMemory(const std::size_t num_slots);

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when attempting to insert a tuple which is so large
 *        that it can't fit in an empty block.
 **/
class TupleTooLargeForBlock : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param tuple_size The size of the huge tuple in bytes.
   **/
  explicit TupleTooLargeForBlock(const std::size_t tuple_size);

  ~TupleTooLargeForBlock() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

  /**
   * @brief Get the size of the tuple that caused this exception.
   *
   * @param return The size of the tuple that caused this exception, in bytes.
   **/
  std::size_t getTupleSize() const {
    return tuple_size_;
  }

 private:
  std::size_t tuple_size_;
  std::string message_;
};

/**
 * @brief Exception thrown when a file can't be deleted.
 **/
class UnableToDeleteFile : public std::exception {
 public:
  explicit UnableToDeleteFile(const std::string &filename);

  virtual ~UnableToDeleteFile() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when a file can't be opened.
 **/
class UnableToOpenFile : public std::exception {
 public:
  explicit UnableToOpenFile(const std::string &filename);

  virtual ~UnableToOpenFile() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_ERRORS_HPP_
