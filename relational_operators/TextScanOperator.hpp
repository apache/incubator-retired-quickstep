/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include <cctype>
#include <cstddef>
#include <exception>
#include <string>

#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "query_execution/QueryContext.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "relational_operators/WorkOrder.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelationSchema;
class InsertDestination;
class StorageManager;
class WorkOrderProtosContainer;
class WorkOrdersContainer;

namespace serialization { class WorkOrder; }

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
 * @brief An operator which reads tuples from a text file and inserts them into
 *        a relation.
 **/
class TextScanOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor
   *
   * @param query_id The ID of the query to which this operator belongs.
   * @param file_pattern The glob-like file pattern of the sources to load. The
   *        pattern could include * (wildcard for multiple chars) and ?
   *        (wildcard for single char). It defaults to single file load, if a
   *        file is specified.
   * @param field_terminator The string which separates attribute values in
   *        the text file.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param output_relation The output relation.
   * @param output_destination_index The index of the InsertDestination in the
   *        QueryContext to insert tuples.
   **/
  TextScanOperator(const std::size_t query_id,
                   const std::string &file_pattern,
                   const char field_terminator,
                   const bool process_escape_sequences,
                   const CatalogRelation &output_relation,
                   const QueryContext::insert_destination_id output_destination_index)
      : RelationalOperator(query_id),
        file_pattern_(file_pattern),
        field_terminator_(field_terminator),
        process_escape_sequences_(process_escape_sequences),
        output_relation_(output_relation),
        output_destination_index_(output_destination_index),
        work_generated_(false) {}

  ~TextScanOperator() override {}

  std::string getName() const override {
    return "TextScanOperator";
  }

  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  bool getAllWorkOrderProtos(WorkOrderProtosContainer *container) override;

  QueryContext::insert_destination_id getInsertDestinationID() const override {
    return output_destination_index_;
  }

  const relation_id getOutputRelationID() const override {
    return output_relation_.getID();
  }

 private:
  serialization::WorkOrder* createWorkOrderProto(const std::string &filename,
                                                 const std::size_t text_offset,
                                                 const std::size_t text_segment_size);

  const std::string file_pattern_;
  const char field_terminator_;
  const bool process_escape_sequences_;

  const CatalogRelation &output_relation_;
  const QueryContext::insert_destination_id output_destination_index_;

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
   * @param query_id The ID of the query to which this WorkOrder belongs.
   * @param filename The name of the text file to bulk insert.
   * @param text_offset The start position in the text file to start text scan.
   * @param text_segment_size The size of text segment to be scanned.
   * @param field_terminator The character which separates attribute values in
   *        the text file.
   * @param process_escape_sequences Whether to decode escape sequences in the
   *        text file.
   * @param output_destination The InsertDestination to insert tuples.
   **/
  TextScanWorkOrder(
      const std::size_t query_id,
      const std::string &filename,
      const std::size_t text_offset,
      const std::size_t text_segment_size,
      const char field_terminator,
      const bool process_escape_sequences,
      InsertDestination *output_destination)
      : WorkOrder(query_id),
        filename_(filename),
        text_offset_(text_offset),
        text_segment_size_(text_segment_size),
        field_terminator_(field_terminator),
        process_escape_sequences_(process_escape_sequences),
        output_destination_(DCHECK_NOTNULL(output_destination)) {}

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
  /**
   * @brief Extract a field string starting at \p *field_ptr. This method also
   *        expands escape sequences if \p process_escape_sequences_ is true.
   *        Throws TextScanFormatError if text was malformed.
   *
   * @param field_ptr \p *field_ptr points to the current position of the input
   *        char stream for parsing. The overall char stream must end with a
   *        newline character. After the call, \p *field_ptr will be modified to
   *        the start position of the NEXT field string.
   * @param is_null_literal OUTPUT parameter. Set to true if the NULL-literal
   *        string "\N" was found.
   * @param has_reached_end_of_line OUTPUT parameter. Set to true if the newline
   *        character was encountered.
   * @param field_string OUTPUT parameter. Set to the extracted field string.
   */
  void extractFieldString(const char **field_ptr,
                          bool *is_null_literal,
                          bool *has_reached_end_of_line,
                          std::string *field_string) const;

  /**
   * @brief Make a tuple by parsing all of the individual fields from a char stream.
   *
   * @param \p *row_ptr points to the current position of the input char stream
   *        for parsing. The overall char stream must end with a newline character.
   *        After the call, \p *row_ptr will be modified to the start position of
   *        the NEXT text row.
   * @param relation The relation schema for the tuple.
   * @return The tuple parsed from the char stream.
   */
  Tuple parseRow(const char **row_ptr,
                 const CatalogRelationSchema &relation) const;

  /**
   * @brief Parse up to three octal digits (0-7) starting at \p *literal_ptr as
   *        a char literal. \p *literal_ptr will be modified to the last position
   *        of the parsed octal digits.
   *
   * @param literal_ptr \p *literal_ptr points to the current position of the
   *        input char stream for parsing. The overall char stream must end with
   *        a newline character.
   * @return The char literal from the parsed octal digits.
   */
  inline static char ParseOctalLiteral(const char **literal_ptr) {
    int value = 0;
    const char *ptr = *literal_ptr;
    for (int i = 0; i < 3; ++i, ++ptr) {
      const int char_value = *ptr - '0';
      if ((char_value >= 0) && (char_value < 8)) {
        value = value * 8 + char_value;
      } else {
        break;
      }
    }
    *literal_ptr = ptr - 1;
    return value;
  }

  /**
   * @brief Parse up to two hexadecimal digits (0-F, case insensitive) starting
   *        at \p *literal_ptr as a char literal. \p *literal_ptr will be modified
   *        to the last position of the parsed octal digits.
   *
   * @param literal_ptr \p *literal_ptr points to the current position of the
   *        input char stream for parsing. The overall char stream must end with
   *        a newline character.
   * @return The char literal from the parsed hexadecimal digits.
   */
  inline static char ParseHexLiteral(const char **literal_ptr) {
    int value = 0;
    const char *ptr = *literal_ptr;
    for (int i = 0; i < 2; ++i, ++ptr) {
      const char c = *ptr;
      int char_value;
      if (std::isdigit(c)) {
        char_value = c - '0';
      } else if (c >= 'a' && c <= 'f') {
        char_value = c - 'a' + 10;
      } else if (c >= 'A' && c <= 'F') {
        char_value = c - 'A' + 10;
      } else {
        break;
      }
      value = value * 16 + char_value;
    }
    *literal_ptr = ptr - 1;
    return value;
  }

  const std::string filename_;
  const std::size_t text_offset_;
  const std::size_t text_segment_size_;
  const char field_terminator_;
  const bool process_escape_sequences_;

  InsertDestination *output_destination_;

  DISALLOW_COPY_AND_ASSIGN(TextScanWorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_TEXT_SCAN_OPERATOR_HPP_
