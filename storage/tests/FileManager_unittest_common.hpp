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

#ifndef QUICKSTEP_STORAGE_TESTS_FILEMANAGER_UNITTEST_COMMON_HPP_
#define QUICKSTEP_STORAGE_TESTS_FILEMANAGER_UNITTEST_COMMON_HPP_

#include <cstddef>
#include <cstring>
#include <memory>

#include "gtest/gtest.h"

#include "storage/FileManager.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"

namespace quickstep {

static const char kSampleString[kSlotSizeBytes] =
  "All animals are equal, but some animals are more equal than others.";

template <typename FileManagerImpl>
class FileManagerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    block_domain_ = 1;
    // NOTE(zuyu): The storage path here should consist of all existing
    //             directories. Otherwise, tests would fail with a
    //             fopen error 'No such file or directory'.
    file_manager_.reset(new FileManagerImpl("./"));
    block_ = BlockIdUtil::GetBlockId(block_domain_,
                                     file_manager_->getMaxUsedBlockCounter(block_domain_));
  }

  void runOneBlockFileWithOneSlotTest(const char *expected) {
    ASSERT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));

    const std::size_t num_slots = (std::strlen(expected) + 1 + kSlotSizeBytes - 1)
                                  / kSlotSizeBytes;
    const std::size_t length = num_slots * kSlotSizeBytes;
    ++block_;
    ASSERT_TRUE(file_manager_->writeBlockOrBlob(block_, expected, length));
    EXPECT_EQ(num_slots, file_manager_->numSlots(block_));
    EXPECT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));

    char *checked = new char[length]();
    ASSERT_TRUE(file_manager_->readBlockOrBlob(block_, checked, length));
    EXPECT_EQ(0, std::memcmp(expected, checked, length));
    delete[] checked;

    ASSERT_TRUE(file_manager_->deleteBlockOrBlob(block_));
    --block_;
    EXPECT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));
  }

  void runOneBlockFileWithMultipleSlotsTest(const std::size_t num_slots) {
    ASSERT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));

    const std::size_t length = num_slots * kSlotSizeBytes;
    char *expected = new char[length];
    memset(expected, 0xac, length);

    ++block_;
    ASSERT_TRUE(file_manager_->writeBlockOrBlob(block_, expected, length));
    EXPECT_EQ(num_slots, file_manager_->numSlots(block_));
    EXPECT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));

    char *checked = new char[length]();
    ASSERT_TRUE(file_manager_->readBlockOrBlob(block_, checked, length));
    EXPECT_EQ(0, std::memcmp(expected, checked, length));
    delete[] expected;
    delete[] checked;

    ASSERT_TRUE(file_manager_->deleteBlockOrBlob(block_));
    --block_;
    EXPECT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));
  }

  void runMultipleBlockFilesTest(const int num_files, const std::size_t num_slots = 1) {
    ASSERT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));

    const std::size_t length = num_slots * kSlotSizeBytes;
    char *expected = new char[length];
    memset(expected, 0xac, length);

    char *checked = new char[length]();

    for (int i = 0; i < num_files; ++i) {
      ++block_;
      ASSERT_TRUE(file_manager_->writeBlockOrBlob(block_, expected, length));
      EXPECT_EQ(num_slots, file_manager_->numSlots(block_));
      EXPECT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));

      ASSERT_TRUE(file_manager_->readBlockOrBlob(block_, checked, length));
      EXPECT_EQ(0, std::memcmp(expected, checked, length));
      memset(checked, 0x0, length);
    }

    delete[] expected;
    delete[] checked;

    for (int i = 0; i < num_files; ++i) {
      ASSERT_TRUE(file_manager_->deleteBlockOrBlob(block_));
      --block_;
      EXPECT_EQ(BlockIdUtil::Counter(block_), file_manager_->getMaxUsedBlockCounter(block_domain_));
    }
  }

  void runOneBlockFileInAnotherDomain(const char *expected) {
    const block_id_domain block_other_domain = block_domain_ + 10;
    block_id block = BlockIdUtil::GetBlockId(block_other_domain,
                                             file_manager_->getMaxUsedBlockCounter(block_other_domain));

    const std::size_t num_slots = (std::strlen(expected) + 1 + kSlotSizeBytes - 1)
                                  / kSlotSizeBytes;
    const std::size_t length = num_slots * kSlotSizeBytes;
    ++block;
    ASSERT_TRUE(file_manager_->writeBlockOrBlob(block, expected, length));
    EXPECT_EQ(num_slots, file_manager_->numSlots(block));
    EXPECT_EQ(BlockIdUtil::Counter(block), file_manager_->getMaxUsedBlockCounter(block_other_domain));

    char *checked = new char[length]();
    ASSERT_TRUE(file_manager_->readBlockOrBlob(block, checked, length));
    EXPECT_EQ(0, std::memcmp(expected, checked, length));
    delete[] checked;

    ASSERT_TRUE(file_manager_->deleteBlockOrBlob(block));
    --block;
    EXPECT_EQ(BlockIdUtil::Counter(block), file_manager_->getMaxUsedBlockCounter(block_other_domain));
  }

  std::unique_ptr<FileManager> file_manager_;
  block_id_domain block_domain_;
  block_id block_;
};
TYPED_TEST_CASE_P(FileManagerTest);

TYPED_TEST_P(FileManagerTest, OneBlockFileWithOneSlotTest) {
  this->runOneBlockFileWithOneSlotTest(kSampleString);
}

TYPED_TEST_P(FileManagerTest, OneBlockFileWithMultipleSlotsTest) {
  this->runOneBlockFileWithMultipleSlotsTest(5);
}

TYPED_TEST_P(FileManagerTest, MultipleBlockFilesTest) {
  this->runMultipleBlockFilesTest(3);
}

TYPED_TEST_P(FileManagerTest, OneBlockFileInAnotherDomainTest) {
  this->runOneBlockFileInAnotherDomain(kSampleString);
}

REGISTER_TYPED_TEST_CASE_P(FileManagerTest,
                           OneBlockFileWithOneSlotTest,
                           OneBlockFileWithMultipleSlotsTest,
                           MultipleBlockFilesTest,
                           OneBlockFileInAnotherDomainTest);

}  // namespace quickstep

// Macro to automatically instantiate tests for the specificied FileManager
// implementation template.
#define QUICKSTEP_TEST_FILEMANAGER_IMPL(FileManagerImpl)                \
namespace quickstep {                                                   \
                                                                        \
typedef ::testing::Types<FileManagerImpl> _CommonTestTypes;             \
INSTANTIATE_TYPED_TEST_CASE_P(FileManagerImpl,                          \
                              FileManagerTest,                          \
                              _CommonTestTypes);                        \
                                                                        \
}  /* namespace quickstep */                                            \
struct quickstep_filemanager_unittest_dummy  /* NOLINT(build/class) */


#endif  // QUICKSTEP_STORAGE_TESTS_FILEMANAGER_UNITTEST_COMMON_HPP_
