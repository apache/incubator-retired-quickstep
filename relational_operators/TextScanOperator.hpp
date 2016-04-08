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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_TEXT_SCAN_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_TEXT_SCAN_OPERATOR_HPP_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <string>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "storage/StorageBlob.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"
#include "utility/ThreadSafeQueue.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class InsertDestination;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief Exception thrown when a text file can't be opened for reading.
 **/
class TextScanReadError : public std::exception {
 public:
  explicit TextScanReadError(const std::string &filename)
      : message_("TextScanReadError: Unable to read file ") {
    message_.append(filename);
    message_.append(" for text scan");
  }

  ~TextScanReadError() throw() {}

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when improperly formatted data is encountered while
 *        scanning a text file.
 **/
class TextScanFormatError : public std::exception {
 public:
  explicit TextScanFormatError(const std::string reason)
      : message_("TextScanFormatError: Malformed data encountered during text scan (") {
    message_.append(reason);
    message_.push_back(')');
  }

  ~TextScanFormatError() throw() {}

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief A structure for text data blobs.
 */
struct TextBlob {
  TextBlob(const block_id text_blob_id, const std::size_t text_size)
      : blob_id(text_blob_id), size(text_size) {}
  block_id blob_id;
  std::size_t size;
};

/**
 * @brief An operator which reads tuples from a text file and inserts them into
 *        a relation.
 **/
class TextScanOperator : public RelationalOperator {
 public:
  enum FeedbackMessageType : WorkOrder::FeedbackMessageType {
    kNewTextBlobMessage,
    kSplitWorkOrderCompletionMessage,
  };

  /**
   * @brief Constructor
   *
   * @param file_pattern The glob-like file pattern of the sources to load. The
   *        pattern could include * (wildcard for multiple chars) and ?
   *        (wildcard for single char). It defaults to single file load, if a
   *        file is specified.
   * @param field_terminator The string which separates attribute values in
   *        the text file.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param parallelize_load Parallelize the load process by th spliting file
   *        into blobs, and generating separate work-orders for each of them.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert tuples.
   **/
  TextScanOperator(const std::string &file_pattern,
                   const char field_terminator,
                   const bool process_escape_sequences,
                   const bool parallelize_load,
                   const CatalogRelation &output_relation,
                   const QueryContext::insert_destination_id output_destination_index)
      : file_pattern_(file_pattern),
        field_terminator_(field_terminator),
        process_escape_sequences_(process_escape_sequences),
        parallelize_load_(parallelize_load),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        num_done_split_work_orders_(0),
        num_split_work_orders_(0),
        work_generated_(false) {}

  ~TextScanOperator() override {}

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

  void receiveFeedbackMessage(const WorkOrder::FeedbackMessage &msg) override;

 private:
  const std::string file_pattern_;
  const char field_terminator_;
  const bool process_escape_sequences_;
  const bool parallelize_load_;

  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;

  ThreadSafeQueue<TextBlob> text_blob_queue_;
  std::atomic<std::uint32_t> num_done_split_work_orders_;
  std::uint32_t num_split_work_orders_;

  // Indicates if work order to load file is generated for non-parallel load, and
  // if work order to split file to blobs is generated for parallel load.
  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(TextScanOperator);
};

/**
 * @brief A WorkOrder produced by TextScanOperator
 **/
class TextScanWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor
   *
   * @param filename The name of the text file to bulk insert.
   * @param field_terminator The string which separates attribute values in
   *        the text file.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param output_destination The InsertDestination to insert tuples.
   * @param storage_manager The StorageManager to use.
   **/
  TextScanWorkOrder(
      const std::string &filename,
      const char field_terminator,
      const bool process_escape_sequences,
      InsertDestination *output_destination,
      StorageManager *storage_manager);

  /**
   * @brief Constructor.
   *
   * @param text_blob Blob ID containing the data to be scanned.
   * @param text_size Size of the data in the blob.
   * @param field_terminator The character which separates attribute values in
   *        the text file.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param output_destination The InsertDestination to write the read tuples.
   * @param storage_manager The StorageManager to use.
   */
  TextScanWorkOrder(
      const block_id text_blob,
      const std::size_t text_size,
      const char field_terminator,
      const bool process_escape_sequences,
      InsertDestination *output_destination,
      StorageManager *storage_manager);

  ~TextScanWorkOrder() override {}

  // FIXME(chasseur): Rollback if an exception is thrown after some tuples have
  // already been inserted.
  /**
   * @exception TextScanReadError The text file could not be opened for
   *            reading.
   * @exception TextScanFormatError Malformed data was encountered in the
   *            text file.
   * @exception TupleTooLargeForBlock A tuple in the text file was too large
   *            to fit in a StorageBlock.
   **/
  void execute() override;

 private:
  // Parse up to three octal digits (0-7) starting at '*start_pos' in
  // 'row_string' as a char literal. '*start_pos' will be modified to
  // the first position AFTER the parsed octal digits.
  static char ParseOctalLiteral(const std::string &row_string,
                                std::size_t *start_pos);

  // Parse up to two hexadecimal digits (0-F, case insensitive) starting at
  // '*start_pos' in 'row_string' as a char literal. '*start_pos' will be
  // modified to the first position AFTER the parsed hexadecimal digits.
  static char ParseHexLiteral(const std::string &row_string,
                              std::size_t *start_pos);

  // Read the next text row from the open FILE stream '*file' into
  // '*row_string'. Returns false if end-of-file is reached and there are no
  // more rows, true if a row string was successfully read. For ease of
  // parsing, '*row_string' has the trailing row-terminator removed and
  // replaced with a field-terminator.
  bool readRowFromFile(FILE *file, std::string *row_string) const;

  // Read the next text from blob memory starting at '**start_pos' and ending
  // at '*end_pos' into '*row_string'. Returns false if the end of the blob is
  // reached and there are no more rows, true if a row was successfully read.
  // For ease of parsing, '*row_string' has the trailing row-terminator removed
  // and replaced with a field-terminator. After call '*start_pos' points to
  // first character AFTER the read row in the blob.
  bool readRowFromBlob(const char **start_pos,
                       const char *end_pos,
                       std::string *row_string) const;

  // Trim a row-terminator (newline or carriage-return + newline) off the end
  // of '*row_string'. Returns true if the row-terminator was successfully
  // removed, false if '*row_string' did not end in a row-terminator.
  bool removeRowTerminator(std::string *row_string) const;

  // Extract a field string starting at '*start_pos' in 'row_string' into
  // '*field_string'. This method also expands escape sequences if
  // 'process_escape_sequences_' is true. Returns true if a field string was
  // successfully extracted, false in the special case where the NULL-literal
  // string "\N" was found. Throws TextScanFormatError if text was malformed.
  bool extractFieldString(const std::string &row_string,
                          std::size_t *start_pos,
                          std::string *field_string) const;

  // Make a tuple by parsing all of the individual fields specified in
  // 'row_string'.
  Tuple parseRow(const std::string &row_string, const CatalogRelationSchema &relation) const;

  const bool is_file_;
  const std::string filename_;
  const char field_terminator_;
  const block_id text_blob_;
  const std::size_t text_size_;
  const bool process_escape_sequences_;

  InsertDestination *output_destination_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(TextScanWorkOrder);
};

/**
 * @brief A WorkOrder to split the file into blobs of text that can be processed
 * separately.
 **/
class TextSplitWorkOrder : public WorkOrder {
 public:
  /**
   * @brief Constructor.
   * @param filename File to split into row-aligned blobs.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param storage_manager The StorageManager to use.
   * @param operator_index Operator index of the current operator. This is used
   *                       to send new-work available message to Foreman.
   * @param scheduler_client_id The TMB client ID of the scheduler thread.
   * @param bus A pointer to the TMB.
   */
  TextSplitWorkOrder(const std::string filename,
                     const bool process_escape_sequences,
                     StorageManager *storage_manager,
                     const std::size_t operator_index,
                     const tmb::client_id scheduler_client_id,
                     MessageBus *bus)
      : filename_(filename),
        process_escape_sequences_(process_escape_sequences),
        storage_manager_(DCHECK_NOTNULL(storage_manager)),
        operator_index_(operator_index),
        scheduler_client_id_(scheduler_client_id),
        bus_(DCHECK_NOTNULL(bus)) {}

  /**
   * @exception TextScanReadError The text file could not be opened for
   *            reading.
   */
  void execute() override;

 private:
  // Allocate a new blob.
  void allocateBlob();

  // Find the last row terminator in current blob.
  std::size_t findLastRowTerminator();

  // Send the blob info to its operator via TMB.
  void sendBlobInfoToOperator(const bool write_row_aligned);

  // Get the writeable address (unwritten chunk) in current blob.
  inline char* writeableBlobAddress() {
    return static_cast<char*>(text_blob_->getMemoryMutable()) + written_;
  }

  // Number of bytes remaining to be written.
  inline std::size_t remainingBlobBytes() const {
    return blob_size_ - written_;
  }

  const std::string filename_;  // File to split.
  const bool process_escape_sequences_;

  StorageManager *storage_manager_;

  const std::size_t operator_index_;  // Opeartor index.
  const tmb::client_id scheduler_client_id_;  // The scheduler's TMB client ID.
  MessageBus *bus_;

  MutableBlobReference text_blob_;  // Mutable reference to current blob.
  block_id text_blob_id_;  // Current blob ID.
  std::size_t written_ = 0;  // Bytes written in current blob.
  std::size_t blob_size_ = 0;  // Size of the current blob.

  DISALLOW_COPY_AND_ASSIGN(TextSplitWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_TEXT_SCAN_OPERATOR_HPP_
