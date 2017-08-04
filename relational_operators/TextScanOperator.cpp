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

#include "relational_operators/TextScanOperator.hpp"

#include "relational_operators/RelationalOperatorsConfig.h"  // For QUICKSTEP_HAVE_UNISTD.

#ifdef QUICKSTEP_HAVE_UNISTD
#include <unistd.h>
#endif  // QUICKSTEP_HAVE_UNISTD

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/Flags.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageConfig.h"  // For QUICKSTEP_HAVE_FILE_MANAGER_HDFS.

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
#include <hdfs/hdfs.h>
#endif  // QUICKSTEP_HAVE_FILE_MANAGER_HDFS

#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/ColumnVector.hpp"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/BulkIoConfiguration.hpp"
#include "utility/Glob.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

// Text segment size set to 256KB.
DEFINE_uint64(textscan_text_segment_size, 0x40000,
              "Size of text segment in bytes the input text files "
              "are split into in the TextScanOperator.");

// Check if the segment size is positive.
static bool ValidateTextScanTextSegmentSize(const char *flagname,
                                            std::uint64_t text_segment_size) {
  if (text_segment_size == 0) {
    LOG(ERROR) << "--" << flagname << " must be greater than 0";
    return false;
  }

  return true;
}

static const volatile bool text_scan_text_segment_size_dummy =
    gflags::RegisterFlagValidator(
        &FLAGS_textscan_text_segment_size, &ValidateTextScanTextSegmentSize);

namespace {

std::size_t GetFileSize(const std::string &file_name) {
  // Use standard C libary to retrieve the file size.
  FILE *fp = std::fopen(file_name.c_str(), "rb");
  // TODO(quickstep-team): Decent handling of exceptions at query runtime.
  if (fp == nullptr) {
    throw std::runtime_error("Can not open file " + file_name + " for reading");
  }
  std::fseek(fp, 0, SEEK_END);
  const std::size_t file_size = std::ftell(fp);
  std::fclose(fp);

  return file_size;
}

}  // namespace

bool TextScanOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  DCHECK(query_context != nullptr);

  const std::vector<std::string> files = utility::file::GlobExpand(file_pattern_);

  CHECK_NE(files.size(), 0u)
      << "No files matched '" << file_pattern_ << "'. Exiting.";

  InsertDestination *output_destination =
      query_context->getInsertDestination(output_destination_index_);

  if (work_generated_) {
    return true;
  }

  for (const std::string &file : files) {
#ifdef QUICKSTEP_HAVE_UNISTD
    // Check file permissions before trying to open it.
    const int access_result = access(file.c_str(), R_OK);
    CHECK_EQ(0, access_result)
        << "File " << file << " is not readable due to permission issues.";
#endif  // QUICKSTEP_HAVE_UNISTD

    const std::size_t file_size = GetFileSize(file);

    std::size_t text_offset = 0;
    for (size_t num_full_segments = file_size / FLAGS_textscan_text_segment_size;
         num_full_segments > 0;
         --num_full_segments, text_offset += FLAGS_textscan_text_segment_size) {
      container->addNormalWorkOrder(
          new TextScanWorkOrder(query_id_,
                                file,
                                text_offset,
                                FLAGS_textscan_text_segment_size,
                                options_->getDelimiter(),
                                options_->escapeStrings(),
                                output_destination),
          op_index_);
    }

    // Deal with the residual partial segment whose size is less than
    // 'FLAGS_textscan_text_segment_size'.
    if (text_offset < file_size) {
      container->addNormalWorkOrder(
          new TextScanWorkOrder(query_id_,
                                file,
                                text_offset,
                                file_size - text_offset,
                                options_->getDelimiter(),
                                options_->escapeStrings(),
                                output_destination),
          op_index_);
    }
  }

  work_generated_ = true;
  return true;
}

bool TextScanOperator::getAllWorkOrderProtos(WorkOrderProtosContainer *container) {
  const std::vector<std::string> files = utility::file::GlobExpand(file_pattern_);
  if (work_generated_) {
    return true;
  }

  for (const std::string &file : files) {
    const std::size_t file_size = GetFileSize(file);

    size_t text_offset = 0;
    for (size_t num_full_segments = file_size / FLAGS_textscan_text_segment_size;
         num_full_segments > 0;
         --num_full_segments, text_offset += FLAGS_textscan_text_segment_size) {
      container->addWorkOrderProto(
          createWorkOrderProto(file, text_offset,
                               FLAGS_textscan_text_segment_size),
          op_index_);
    }

    // Deal with the residual partial segment whose size is less than
    // 'FLAGS_textscan_text_segment_size'.
    if (text_offset < file_size) {
      container->addWorkOrderProto(
          createWorkOrderProto(file, text_offset, file_size - text_offset),
          op_index_);
    }
  }

  work_generated_ = true;
  return true;
}

serialization::WorkOrder* TextScanOperator::createWorkOrderProto(
    const std::string &filename,
    const std::size_t text_offset,
    const std::size_t text_segment_size) {
  serialization::WorkOrder *proto = new serialization::WorkOrder;
  proto->set_work_order_type(serialization::TEXT_SCAN);
  proto->set_query_id(query_id_);

  proto->SetExtension(serialization::TextScanWorkOrder::filename, filename);
  proto->SetExtension(serialization::TextScanWorkOrder::text_offset, text_offset);
  proto->SetExtension(serialization::TextScanWorkOrder::text_segment_size, text_segment_size);
  proto->SetExtension(serialization::TextScanWorkOrder::field_terminator,
                      options_->getDelimiter());
  proto->SetExtension(serialization::TextScanWorkOrder::process_escape_sequences,
                      options_->escapeStrings());
  proto->SetExtension(serialization::TextScanWorkOrder::insert_destination_index,
                      output_destination_index_);

  return proto;
}

void TextScanWorkOrder::execute() {
  const CatalogRelationSchema &relation = output_destination_->getRelation();
  std::vector<Tuple> tuples;
  bool is_faulty;

  std::vector<TypedValue> vector_tuple_returned;
  constexpr std::size_t kSmallBufferSize = 0x4000;
  const size_t buffer_size = std::max(text_segment_size_, kSmallBufferSize);
  char *buffer = reinterpret_cast<char *>(malloc(buffer_size));

  bool use_hdfs = false;
  std::size_t bytes_read;

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  hdfsFS hdfs = nullptr;
  hdfsFile file_handle = nullptr;

  if (FLAGS_use_hdfs) {
    use_hdfs = true;
    hdfs = static_cast<hdfsFS>(hdfs_);

    file_handle = hdfsOpenFile(hdfs, filename_.c_str(), O_RDONLY, buffer_size,
                               0 /* default replication */, 0 /* default block size */);
    if (file_handle == nullptr) {
      LOG(ERROR) << "Failed to open file " << filename_
                 << " with error: " << strerror(errno);
      return;
    }

    if (hdfsSeek(hdfs, file_handle, text_offset_)) {
      LOG(ERROR) << "Failed to seek in file " << filename_
                 << " with error: " << strerror(errno);

      hdfsCloseFile(hdfs, file_handle);
      return;
    }

    bytes_read = hdfsRead(hdfs, file_handle, buffer, text_segment_size_);
    while (bytes_read != text_segment_size_) {
      bytes_read += hdfsRead(hdfs, file_handle,
                             buffer + bytes_read,
                             text_segment_size_ - bytes_read);
    }
  }
#endif  // QUICKSTEP_HAVE_FILE_MANAGER_HDFS

  FILE *file = nullptr;
  if (!use_hdfs) {
    // Avoid unused-private-field warning.
    (void) hdfs_;

    // Read text segment into buffer.
    file = std::fopen(filename_.c_str(), "rb");
    std::fseek(file, text_offset_, SEEK_SET);
    bytes_read = std::fread(buffer, 1, text_segment_size_, file);

    if (bytes_read != text_segment_size_) {
      std::fclose(file);
      throw TextScanReadError(filename_);
    }
  }

  // Locate the first newline character.
  const char *buffer_end = buffer + text_segment_size_;
  const char *row_ptr = buffer;
  if (text_offset_ != 0) {
    while (row_ptr < buffer_end && *row_ptr != '\n') {
      ++row_ptr;
    }
  } else {
    --row_ptr;
  }
  if (row_ptr >= buffer_end) {
    // This block does not even contain a newline character.
    return;
  }

  // Locate the last newline character.
  const char *end_ptr = buffer_end - 1;
  while (end_ptr > row_ptr && *end_ptr != '\n') {
    --end_ptr;
  }

  // Advance both row_ptr and end_ptr by 1.
  ++row_ptr;
  ++end_ptr;
  // Now row_ptr is pointing to the first character RIGHT AFTER the FIRST newline
  // character in this text segment, and end_ptr is pointing to the first character
  // RIGHT AFTER the LAST newline character in this text segment.

  // Process the tuples which are between the first newline character and the
  // last newline character. SKIP any row which is corrupt instead of ABORTING the
  // whole COPY operation.
  while (row_ptr < end_ptr) {
    if (*row_ptr == '\r' || *row_ptr == '\n') {
      // Skip empty lines.
      ++row_ptr;
    } else {
      vector_tuple_returned = parseRow(&row_ptr, relation, &is_faulty);
      if (is_faulty) {
        // Skip faulty rows
        LOG(INFO) << "Faulty row found. Hence switching to next row.";
      } else {
        // Convert vector returned to tuple only when a valid row is encountered.
        tuples.emplace_back(Tuple(std::move(vector_tuple_returned)));
      }
    }
  }
  // Process the tuple that is right after the last newline character.
  // NOTE(jianqiao): dynamic_read_size is trying to balance between the cases
  // that the last tuple is very small / very large.
  std::size_t dynamic_read_size = 1024;
  std::string row_string;

  const size_t dynamic_read_offset = text_offset_ + (end_ptr - buffer);
  if (use_hdfs) {
#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
    if (hdfsSeek(hdfs, file_handle, dynamic_read_offset)) {
      LOG(ERROR) << "Failed to seek in file " << filename_
                 << " with error: " << strerror(errno);

      hdfsCloseFile(hdfs, file_handle);
      return;
    }
#endif  // QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  } else {
    std::fseek(file, dynamic_read_offset, SEEK_SET);
  }

  bool has_reached_end = false;
  do {
    if (use_hdfs) {
#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
      bytes_read = hdfsRead(hdfs, file_handle, buffer, dynamic_read_size);

      // Read again when acrossing the HDFS block boundary.
      if (bytes_read != dynamic_read_size) {
        bytes_read += hdfsRead(hdfs, file_handle,
                               buffer + bytes_read,
                               dynamic_read_size - bytes_read);
      }
#endif  // QUICKSTEP_HAVE_FILE_MANAGER_HDFS
    } else {
      bytes_read = std::fread(buffer, 1, dynamic_read_size, file);
    }

    std::size_t bytes_to_copy = bytes_read;

    for (std::size_t i = 0; i < bytes_read; ++i) {
      if (buffer[i] == '\n') {
        bytes_to_copy = i + 1;
        has_reached_end = true;
        break;
      }
    }
    if (!has_reached_end && bytes_read != dynamic_read_size) {
      has_reached_end = true;
    }

    row_string.append(buffer, bytes_to_copy);
    dynamic_read_size = std::min(dynamic_read_size * 2, kSmallBufferSize);
  } while (!has_reached_end);

  if (!row_string.empty()) {
    if (row_string.back() != '\n') {
      row_string.push_back('\n');
    }
    row_ptr = row_string.c_str();

    vector_tuple_returned = parseRow(&row_ptr, relation, &is_faulty);
    if (is_faulty) {
      // Skip the faulty row.
      LOG(INFO) << "Faulty row found. Hence switching to next row.";
    } else {
      // Convert vector returned to tuple only when a valid row is encountered.
      tuples.emplace_back(Tuple(std::move(vector_tuple_returned)));
    }
  }

  if (use_hdfs) {
#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
    hdfsCloseFile(hdfs, file_handle);
#endif  // QUICKSTEP_HAVE_FILE_MANAGER_HDFS
  } else {
    std::fclose(file);
  }

  free(buffer);

  // Store the tuples in a ColumnVectorsValueAccessor for bulk insert.
  ColumnVectorsValueAccessor column_vectors;
  std::size_t attr_id = 0;
  for (const auto &attribute : relation) {
    const Type &attr_type = attribute.getType();
    if (attr_type.isVariableLength()) {
      std::unique_ptr<IndirectColumnVector> column(
          new IndirectColumnVector(attr_type, tuples.size()));
      for (const auto &tuple : tuples) {
        column->appendTypedValue(tuple.getAttributeValue(attr_id));
      }
      column_vectors.addColumn(column.release());
    } else {
      std::unique_ptr<NativeColumnVector> column(
          new NativeColumnVector(attr_type, tuples.size()));
      for (const auto &tuple : tuples) {
        column->appendTypedValue(tuple.getAttributeValue(attr_id));
      }
      column_vectors.addColumn(column.release());
    }
    ++attr_id;
  }

  // Bulk insert the tuples.
  output_destination_->bulkInsertTuples(&column_vectors);
}

std::vector<TypedValue> TextScanWorkOrder::parseRow(const char **row_ptr,
                                                    const CatalogRelationSchema &relation,
                                                    bool *is_faulty) const {
  std::vector<TypedValue> attribute_values;
  // Always assume current row is not faulty initially.
  *is_faulty = false;

  bool is_null_literal;
  bool has_reached_end_of_line = false;
  std::string value_str;
  for (const auto &attr : relation) {
    if (has_reached_end_of_line) {
      // Do not abort if one of the row is faulty.
      // Set is_faulty to true and SKIP the current row.
      *is_faulty = true;
      LOG(INFO) << "Row has too few fields.";
      return attribute_values;
    }

    value_str.clear();

    extractFieldString(row_ptr,
                       &is_null_literal,
                       &has_reached_end_of_line,
                       &value_str);

    if (is_null_literal) {
      // NULL literal.
      if (!attr.getType().isNullable()) {
        *is_faulty = true;
        LOG(INFO) << "NULL literal '\\N' was specified for a column with a "
                   "non-nullable Type.";
        skipFaultyRow(row_ptr);
        return attribute_values;
      }
      attribute_values.emplace_back(attr.getType().makeNullValue());
    } else {
      attribute_values.emplace_back();
      if (!attr.getType().parseValueFromString(value_str, &(attribute_values.back()))) {
        // Do not abort if one of the row is faulty.
        *is_faulty = true;
        LOG(INFO) << "Failed to parse value.";
        skipFaultyRow(row_ptr);
        return attribute_values;
      }
    }
  }

  if (!has_reached_end_of_line) {
    // Do not abort if one of the row is faulty.
    // Set is_faulty to true and SKIP the current row.
    *is_faulty = true;
    LOG(INFO) << "Row has too many fields.";
    skipFaultyRow(row_ptr);
  }

  return attribute_values;
}

void TextScanWorkOrder::skipFaultyRow(const char **field_ptr) const {
  const char *cur_ptr = *field_ptr;
  // Move row pointer to the end of faulty row.
  for (;; ++cur_ptr) {
    const char c = *cur_ptr;
    if (c == '\n') {
        break;
    }
  }
  *field_ptr = cur_ptr + 1;
}

void TextScanWorkOrder::extractFieldString(const char **field_ptr,
                                           bool *is_null_literal,
                                           bool *has_reached_end_of_line,
                                           std::string *field_string) const {
  const char *cur_ptr = *field_ptr;
  *is_null_literal = false;

  // Check for NULL literal string.
  if (process_escape_sequences_ && cur_ptr[0] == '\\' && cur_ptr[1] == 'N') {
    cur_ptr += 2;

    // Skip '\r'
    if (*cur_ptr == '\r') {
      ++cur_ptr;
    }

    const char c = *cur_ptr;
    if (c == field_terminator_ || c == '\n') {
      *is_null_literal = true;
      *has_reached_end_of_line = (c == '\n');
      *field_ptr = cur_ptr + 1;
      return;
    }
  }

  // Not a NULL literal string, rewind cur_ptr to the start position for parsing.
  cur_ptr = *field_ptr;

  if (!process_escape_sequences_) {
    // Simply copy until field_terminator or '\n'.
    for (;; ++cur_ptr) {
      const char c = *cur_ptr;
      if (c == field_terminator_) {
        *has_reached_end_of_line = false;
        break;
      } else if (c == '\n') {
        *has_reached_end_of_line = true;
        break;
      }

      // Ignore '\r'
      if (c != '\r') {
        field_string->push_back(c);
      }
    }
  } else {
    for (;; ++cur_ptr) {
      const char c = *cur_ptr;
      if (c == '\\') {
        ++cur_ptr;
        const char first_escaped_character = *cur_ptr;
        switch (first_escaped_character) {
          case '0':  // Fallthrough for octal digits.
          case '1':
          case '2':
          case '3':
          case '4':
          case '5':
          case '6':
          case '7':
            field_string->push_back(ParseOctalLiteral(&cur_ptr));
            break;
          case 'N': {
            // Null literal after some other column data.
            throw TextScanFormatError(
                "Null indicator '\\N' encountered in text scan mixed in with "
                "other column data.");
          }
          case '\\':
            // Backslash.
            field_string->push_back('\\');
            break;
          case 'b':
            // Backspace.
            field_string->push_back('\b');
            break;
          case 'f':
            // Form-feed.
            field_string->push_back('\f');
            break;
          case 'n':
            // Newline.
            field_string->push_back('\n');
            break;
          case 'r':
            // Carriage return.
            field_string->push_back('\r');
            break;
          case 't':
            // Tab.
            field_string->push_back('\t');
            break;
          case 'v':
            // Vertical tab.
            field_string->push_back('\v');
            break;
          case 'x':
            if (std::isxdigit(cur_ptr[1])) {
              // Hexidecimal char literal.
              ++cur_ptr;
              field_string->push_back(ParseHexLiteral(&cur_ptr));
            } else {
              // Just an escaped 'x' with no hex digits.
              field_string->push_back('x');
            }
            break;
          case '\n':
            throw TextScanFormatError(
                "Backslash line splicing is not supported.");
          default:
            // Append escaped character as-is.
            field_string->push_back(first_escaped_character);
            break;
        }
      } else if (c == field_terminator_) {
        *has_reached_end_of_line = false;
        break;
      } else if (c == '\n') {
        *has_reached_end_of_line = true;
        break;
      } else {
        if (c != '\r') {
          // Ignore '\r'
          field_string->push_back(c);
        }
      }
    }
  }
  *field_ptr = cur_ptr + 1;
}

}  // namespace quickstep
