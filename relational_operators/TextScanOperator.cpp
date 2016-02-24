/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
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

#include "relational_operators/TextScanOperator.hpp"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/TextScanOperator.pb.h"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Glob.hpp"

#include "gflags/gflags.h"
#include "glog/logging.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"

using std::isxdigit;
using std::size_t;
using std::sscanf;
using std::string;

namespace quickstep {

DEFINE_uint64(textscan_split_blob_size, 2,
              "Size of blobs in number of slots the input text files "
              "are split into in the TextScanOperator.");

// Check if blob size is positive.
static bool ValidateTextScanSplitBlobSize(const char *flagname,
                                          std::uint64_t blob_size) {
  if (blob_size == 0) {
    LOG(ERROR) << "--" << flagname << " must be greater than 0";
    return false;
  }

  return true;
}

static const bool text_scan_split_blob_size_dummy = gflags::RegisterFlagValidator(
    &FLAGS_textscan_split_blob_size, &ValidateTextScanSplitBlobSize);

namespace {

// Detect whether '*search_string' contains a row-terminator (either line-feed
// or carriage-return + line-feed) immediately before 'end_pos'. If
// 'process_escape_sequences' is true, this function will also eliminate
// false-positives from an escaped row-terminator. Returns the number of
// characters in the row-terminator, or 0 if no terminator is detected.
inline unsigned DetectRowTerminator(const char *search_string,
                                    std::size_t end_pos,
                                    const bool process_escape_sequences) {
  if (end_pos == 0) {
    // Empty string.
    return 0;
  }

  if (search_string[end_pos - 1] != '\n') {
    // String doesn't end in newline.
    return 0;
  }

  if (end_pos == 1) {
    // String is the single newline character.
    return 1;
  }

  const bool have_carriage_return = (search_string[end_pos - 2] == '\r');
  if (have_carriage_return && (end_pos == 2)) {
    // String is CR-LF and nothing else.
    return 2;
  }

  std::size_t backslashes = 0;
  // Count consecutive backslashes preceding the terminator. If there is an odd
  // number of backslashes, then the terminator is escaped and doesn't count as
  // a real terminator. If there is an even number of backslashes, then each
  // pair is an escaped backslash literal and the terminator still counts.
  if (process_escape_sequences) {
    end_pos = end_pos - 2 - have_carriage_return;
    while (end_pos != 0) {
      if (search_string[end_pos] == '\\') {
        ++backslashes;
        --end_pos;
        if ((end_pos == 0) && (search_string[0] == '\\')) {
          // Don't forget to count a backslash at the very beginning of a string.
          ++backslashes;
        }
      } else {
        break;
      }
    }
  }

  if (backslashes & 0x1) {
    return 0;
  } else {
    return 1 + have_carriage_return;
  }
}

}  // namespace

bool TextScanOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    const tmb::client_id foreman_client_id,
    tmb::MessageBus *bus) {
  const std::vector<std::string> files = utility::file::GlobExpand(file_pattern_);
  if (parallelize_load_) {
    // Parallel implementation: Split work orders are generated for each file
    // being bulk-loaded. (More than one file can be loaded, because we support
    // glob() semantics in file name.) These work orders read the input file,
    // and split them in the blobs that can be parsed independently.
    if (blocking_dependencies_met_) {
      if (!work_generated_) {
        // First, generate text-split work orders.
        for (const auto &file : files) {
          container->addNormalWorkOrder(
              new TextSplitWorkOrder(file,
                                     process_escape_sequences_,
                                     op_index_,
                                     foreman_client_id,
                                     bus),
              op_index_);
          ++num_split_work_orders_;
        }
        work_generated_ = true;
        return false;
      } else {
        // Check if there are blobs to parse.
        while (!text_blob_queue_.empty()) {
          const TextBlob blob_work = text_blob_queue_.popOne();
          container->addNormalWorkOrder(
              new TextScanWorkOrder(blob_work.blob_id,
                                    blob_work.size,
                                    field_terminator_,
                                    process_escape_sequences_,
                                    output_destination_index_),
              op_index_);
        }
        // Done if all split work orders are completed, and no blobs are left to
        // process.
        return num_done_split_work_orders_.load(std::memory_order_acquire) == num_split_work_orders_ &&
               text_blob_queue_.empty();
      }
    }
    return false;
  } else {
    // Serial implementation.
    if (blocking_dependencies_met_ && !work_generated_) {
      for (const auto &file : files) {
        container->addNormalWorkOrder(
            new TextScanWorkOrder(file,
                                  field_terminator_,
                                  process_escape_sequences_,
                                  output_destination_index_),
            op_index_);
      }
      work_generated_ = true;
    }
    return work_generated_;
  }
}

void TextScanOperator::receiveFeedbackMessage(const WorkOrder::FeedbackMessage &msg) {
  switch (msg.type()) {
    case kSplitWorkOrderCompletionMessage: {
      num_done_split_work_orders_.fetch_add(1, std::memory_order_release);
      break;
    }
    case kNewTextBlobMessage: {
      serialization::TextBlob proto;
      CHECK(proto.ParseFromArray(msg.payload(), msg.payload_size()));
      text_blob_queue_.push(TextBlob(proto.blob_id(), proto.size()));
      break;
    }
    default:
      LOG(ERROR) << "Unknown feedback message type for TextScanOperator";
  }
}


TextScanWorkOrder::TextScanWorkOrder(const std::string &filename,
                                     const char field_terminator,
                                     const bool process_escape_sequences,
                                     const QueryContext::insert_destination_id output_destination_index)
    : is_file_(true),
      filename_(filename),
      field_terminator_(field_terminator),
      text_blob_(0),
      text_size_(0),
      process_escape_sequences_(process_escape_sequences),
      output_destination_index_(output_destination_index) {
}

TextScanWorkOrder::TextScanWorkOrder(const block_id text_blob,
                                     const std::size_t text_size,
                                     const char field_terminator,
                                     const bool process_escape_sequences,
                                     const QueryContext::insert_destination_id output_destination_index)
    : is_file_(false),
      field_terminator_(field_terminator),
      text_blob_(text_blob),
      text_size_(text_size),
      process_escape_sequences_(process_escape_sequences),
      output_destination_index_(output_destination_index) {
}

void TextScanWorkOrder::execute(QueryContext *query_context,
                                CatalogDatabase *catalog_database,
                                StorageManager *storage_manager) {
  DCHECK(query_context != nullptr);
  DCHECK(storage_manager != nullptr);

  InsertDestination *destination =
      query_context->getInsertDestination(output_destination_index_);
  DCHECK(destination != nullptr);

  const CatalogRelationSchema &relation = destination->getRelation();

  string current_row_string;
  if (is_file_) {
    FILE *file = std::fopen(filename_.c_str(), "r");
    if (file == nullptr) {
      throw TextScanReadError(filename_);
    }

    bool have_row = false;
    do {
      current_row_string.clear();
      have_row = readRowFromFile(file, &current_row_string);
      if (have_row) {
        Tuple tuple = parseRow(current_row_string, relation);
        destination->insertTupleInBatch(tuple);
      }
    } while (have_row);

    std::fclose(file);
  } else {
    BlobReference blob = storage_manager->getBlob(text_blob_);
    const char *blob_pos = static_cast<const char*>(blob->getMemory());
    const char *blob_end = blob_pos + text_size_;
    bool have_row = false;
    do {
      current_row_string.clear();
      have_row = readRowFromBlob(&blob_pos, blob_end, &current_row_string);
      if (have_row) {
        Tuple tuple = parseRow(current_row_string, relation);
        destination->insertTupleInBatch(tuple);
      }
    } while (have_row);
  }
}

char TextScanWorkOrder::ParseOctalLiteral(const std::string &row_string,
                                          std::size_t *start_pos) {
  const std::size_t stop_pos = std::min(row_string.length(), *start_pos + 3);

  int value = 0;
  for (; *start_pos < stop_pos; ++*start_pos) {
    int char_value = row_string[*start_pos] - '0';
    if ((char_value >= 0) && (char_value < 8)) {
      value = value * 8 + char_value;
    } else {
      return value;
    }
  }

  return value;
}

char TextScanWorkOrder::ParseHexLiteral(const std::string &row_string,
                                        std::size_t *start_pos) {
  const std::size_t stop_pos = std::min(row_string.length(), *start_pos + 2);

  int value = 0;
  for (; *start_pos < stop_pos; ++*start_pos) {
    if (!std::isxdigit(row_string[*start_pos])) {
      break;
    }

    int char_value;
    if (std::isdigit(row_string[*start_pos])) {
      char_value = row_string[*start_pos] - '0';
    } else if (std::islower(row_string[*start_pos])) {
      char_value = row_string[*start_pos] - 'a' + 10;
    } else {
      char_value = row_string[*start_pos] - 'A' + 10;
    }

    value = value * 16 + char_value;
  }

  return value;
}

bool TextScanWorkOrder::readRowFromFile(FILE *file, std::string *row_string) const {
  // Read up to 1023 chars + null-terminator at a time.
  static constexpr std::size_t kRowBufferSize = 1024;
  char row_buffer[kRowBufferSize];
  for (;;) {
    char *read_string = std::fgets(row_buffer, sizeof(row_buffer), file);
    if (read_string == nullptr) {
      if (std::feof(file)) {
        if (row_string->empty()) {
          return false;
        } else {
          throw TextScanFormatError("File ended without delimiter");
        }
      } else {
        throw TextScanReadError(filename_);
      }
    }

    // Append the contents of the buffer to '*row_string', and see if we've
    // reached a genuine row-terminator yet.
    row_string->append(row_buffer);
    if (removeRowTerminator(row_string)) {
      row_string->push_back(field_terminator_);
      return true;
    }
  }
}

bool TextScanWorkOrder::readRowFromBlob(const char **start_pos,
                                        const char *end_pos,
                                        std::string *row_string) const {
  while (*start_pos != end_pos) {
    const char *next_newline = static_cast<const char*>(std::memchr(
        *start_pos,
        '\n',
        end_pos - *start_pos));

    if (next_newline == nullptr) {
      throw TextScanFormatError("File ended without delimiter");
    }

    // Append the blob's contents through the next newline to '*row_string',
    // and see if we've reached a genuine row-terminator yet.
    row_string->append(*start_pos, next_newline - *start_pos + 1);
    *start_pos = next_newline + 1;
    if (removeRowTerminator(row_string)) {
      row_string->push_back(field_terminator_);
      return true;
    }
  }

  if (row_string->empty()) {
    return false;
  } else {
    throw TextScanFormatError("File ended without delimiter");
  }
}

bool TextScanWorkOrder::removeRowTerminator(std::string *row_string) const {
  unsigned row_term_chars = DetectRowTerminator(row_string->c_str(),
                                                row_string->length(),
                                                process_escape_sequences_);
  if (row_term_chars == 0) {
    return false;
  } else {
    row_string->resize(row_string->length() - row_term_chars);
    return true;
  }
}

bool TextScanWorkOrder::extractFieldString(const std::string &row_string,
                                           std::size_t *start_pos,
                                           std::string *field_string) const {
  // Check for NULL literal string.
  if (process_escape_sequences_
      && (row_string.length() - *start_pos >= 3)
      && (row_string[*start_pos] == '\\')
      && (row_string[*start_pos + 1] == 'N')
      && (row_string[*start_pos + 2] == field_terminator_)) {
    *start_pos += 3;
    return false;
  }

  // Scan up until terminator, expanding backslashed escape sequences as we go.
  std::size_t terminator_pos = row_string.find(field_terminator_, *start_pos);
  std::size_t scan_pos = *start_pos;

  if (process_escape_sequences_) {
    for (;;) {
      std::size_t backslash_pos = row_string.find('\\', scan_pos);
      if ((backslash_pos == std::string::npos) || (backslash_pos >= terminator_pos)) {
        // No more backslashes, or the next backslash is beyond the field
        // terminator.
        break;
      }

      // Copy up to the backslash.
      field_string->append(row_string, scan_pos, backslash_pos - scan_pos);

      if (backslash_pos + 1 == terminator_pos) {
        // The terminator we found was escaped by a backslash, so append the
        // literal terminator and re-scan for the next terminator character.
        field_string->push_back(field_terminator_);
        scan_pos = terminator_pos + 1;
        terminator_pos = row_string.find(field_terminator_, scan_pos);
        continue;
      }

      // Expand escape sequence.
      switch (row_string[backslash_pos + 1]) {
        case '0':  // Fallthrough for octal digits.
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
          // Octal char literal.
          scan_pos = backslash_pos + 1;
          field_string->push_back(ParseOctalLiteral(row_string, &scan_pos));
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
          scan_pos = backslash_pos + 2;
          break;
        case 'b':
          // Backspace.
          field_string->push_back('\b');
          scan_pos = backslash_pos + 2;
          break;
        case 'f':
          // Form-feed.
          field_string->push_back('\f');
          scan_pos = backslash_pos + 2;
          break;
        case 'n':
          // Newline.
          field_string->push_back('\n');
          scan_pos = backslash_pos + 2;
          break;
        case 'r':
          // Carriage return.
          field_string->push_back('\r');
          scan_pos = backslash_pos + 2;
          break;
        case 't':
          // Tab.
          field_string->push_back('\t');
          scan_pos = backslash_pos + 2;
          break;
        case 'v':
          // Vertical tab.
          field_string->push_back('\v');
          scan_pos = backslash_pos + 2;
          break;
        case 'x':
          if ((backslash_pos + 2 < row_string.length()) && std::isxdigit(row_string[backslash_pos + 2])) {
            // Hexidecimal char literal.
            scan_pos = backslash_pos + 2;
            field_string->push_back(ParseHexLiteral(row_string, &scan_pos));
          } else {
            // Just an escaped 'x' with no hex digits.
            field_string->push_back('x');
            scan_pos = backslash_pos + 2;
          }
          break;
        default:
          // Append escaped character as-is.
          field_string->push_back(row_string[backslash_pos + 1]);
          scan_pos = backslash_pos + 2;
          break;
      }
    }
  }

  DCHECK_NE(terminator_pos, std::string::npos);
  field_string->append(row_string, scan_pos, terminator_pos - scan_pos);
  *start_pos = terminator_pos + 1;
  return true;
}

Tuple TextScanWorkOrder::parseRow(const std::string &row_string, const CatalogRelationSchema &relation) const {
  std::vector<TypedValue> attribute_values;

  std::size_t pos = 0;
  std::string value_str;
  CatalogRelationSchema::const_iterator attr_it = relation.begin();
  while (pos < row_string.length()) {
    if (attr_it == relation.end()) {
      throw TextScanFormatError("Row has too many fields");
    }

    value_str.clear();
    if (extractFieldString(row_string, &pos, &value_str)) {
      attribute_values.emplace_back();
      if (!attr_it->getType().parseValueFromString(value_str, &(attribute_values.back()))) {
        throw TextScanFormatError("Failed to parse value");
      }
    } else {
      // NULL literal.
      if (!attr_it->getType().isNullable()) {
        throw TextScanFormatError(
            "NULL literal '\\N' was specified for a column with a "
            "non-nullable Type");
      }

      attribute_values.emplace_back(attr_it->getType().makeNullValue());
    }

    ++attr_it;
  }

  if (attr_it != relation.end()) {
    throw TextScanFormatError("Row has too few fields");
  }

  return Tuple(std::move(attribute_values));
}

void TextSplitWorkOrder::execute(QueryContext *query_context,
                                 CatalogDatabase *catalog_database,
                                 StorageManager *storage_manager) {
  std::FILE *file = std::fopen(filename_.c_str(), "r");
  if (!file) {
    throw TextScanReadError(filename_);
  }

  bool eof = false;
  do {
    // Allocate new blob, if current is empty.
    if (0 == remainingBlobBytes()) {
      allocateBlob(storage_manager);
    }

    // Read the into the unwritten part of blob.
    std::size_t bytes =
        std::fread(writeableBlobAddress(), 1, remainingBlobBytes(), file);
    eof = bytes < remainingBlobBytes();
    written_ += bytes;

    // Write the current blob to queue for processing.
    sendBlobInfoToOperator(storage_manager, !eof /* write_row_aligned */);
  } while (!eof);

  std::fclose(file);

  // Notify the operator about the completion of this Work Order.
  FeedbackMessage msg(TextScanOperator::kSplitWorkOrderCompletionMessage,
                      operator_index_,
                      nullptr /* payload */,
                      0 /* payload_size */,
                      false /* ownership */);
  SendFeedbackMessage(bus_, ClientIDMap::Instance()->getValue(), foreman_client_id_, msg);
}

// Allocate new blob.
void TextSplitWorkOrder::allocateBlob(StorageManager *storage_manager) {
  text_blob_id_ = storage_manager->createBlob(FLAGS_textscan_split_blob_size);
  text_blob_ = storage_manager->getBlobMutable(text_blob_id_);
  blob_size_ = text_blob_->size();
  written_ = 0;
}

// Find the last row terminator in the blob.
std::size_t TextSplitWorkOrder::findLastRowTerminator() {
  std::size_t found = 0;
  const char *blob = static_cast<const char *>(text_blob_->getMemory());

  for (std::size_t index = written_;
       index != 0;
       --index) {
    if (DetectRowTerminator(blob, index, process_escape_sequences_)) {
      found = index;
      break;
    }
  }

  // TODO(quickstep-team): Design a way to handle long rows that are larger than
  // the configured blob size.
  CHECK_NE(0u, found) << "No row terminator found in " << FLAGS_textscan_split_blob_size
                      << "-slot chunk of " << filename_;
  return found;
}

void TextSplitWorkOrder::sendBlobInfoToOperator(StorageManager *storage_manager,
                                                const bool write_row_aligned) {
  std::size_t text_len = written_;
  std::string residue;
  if (write_row_aligned) {
    // Find last row terminator in current blob.
    text_len = findLastRowTerminator();

    // Copy the residual bytes after the last row terminator.
    residue = std::string(
        static_cast<char *>(text_blob_->getMemoryMutable()) + text_len,
        written_ - text_len);
  }

  // Notify the operator for the split-up blob.
  serialization::TextBlob proto;
  proto.set_blob_id(text_blob_id_);
  proto.set_size(text_len);

  const std::size_t payload_size = proto.ByteSize();
  // NOTE(zuyu): 'payload' gets released by FeedbackMessage's destructor.
  char *payload = static_cast<char *>(std::malloc(payload_size));
  CHECK(proto.SerializeToArray(payload, payload_size));

  FeedbackMessage feedback_msg(TextScanOperator::kNewTextBlobMessage,
                               operator_index_,
                               payload,
                               payload_size);
  SendFeedbackMessage(bus_, ClientIDMap::Instance()->getValue(), foreman_client_id_, feedback_msg);

  // Notify Foreman for the avaiable work order on the blob.
  serialization::WorkOrdersAvailableMessage message_proto;
  message_proto.set_operator_index(operator_index_);

  // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
  const size_t message_proto_length = message_proto.ByteSize();
  char *message_proto_bytes = static_cast<char*>(std::malloc(message_proto_length));
  CHECK(message_proto.SerializeToArray(message_proto_bytes, message_proto_length));

  tmb::TaggedMessage tagged_message(static_cast<const void *>(message_proto_bytes),
                                    message_proto_length,
                                    kWorkOrdersAvailableMessage);
  std::free(message_proto_bytes);

  // Send new work order available message to Foreman.
  const tmb::MessageBus::SendStatus send_status =
      QueryExecutionUtil::SendTMBMessage(
          bus_,
          ClientIDMap::Instance()->getValue(),
          foreman_client_id_,
          std::move(tagged_message));
  CHECK(send_status == tmb::MessageBus::SendStatus::kOK) << "Message could not "
      "be sent from thread with TMB client ID "
      << ClientIDMap::Instance()->getValue() << " to Foreman with TMB client "
      "ID " << foreman_client_id_;

  if (residue.size()) {
    // Allocate new blob, and copy residual bytes from last blob.
    allocateBlob(storage_manager);
    std::memcpy(writeableBlobAddress(), residue.data(), residue.size());
    written_ += residue.size();
  }
}

}  // namespace quickstep
