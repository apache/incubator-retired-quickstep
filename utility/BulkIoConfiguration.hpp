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

#ifndef QUICKSTEP_UTILITY_BULK_IO_CONFIGURATION_HPP_
#define QUICKSTEP_UTILITY_BULK_IO_CONFIGURATION_HPP_

#include <memory>
#include <string>

#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/**
 * @brief External file format for bulk I/O.
 */
enum class BulkIoFormat {
  kCsv = 0,
  kText
};

class BulkIoConfiguration;
typedef std::shared_ptr<const BulkIoConfiguration> BulkIoConfigurationPtr;

/**
 * @brief Detailed file format configuration for bulk I/O (i.e. COPY operations)
 *        that moves data between Quickstep tables and external files.
 */
class BulkIoConfiguration {
 public:
  /**
   * @brief Constructor.
   *
   * @param format External file format.
   */
  explicit BulkIoConfiguration(const BulkIoFormat format)
      : format_(format) {
    initializeDefaultParameters(format);
  }

  /**
   * @brief Get the external file format.
   *
   * @return The external file format.
   */
  inline BulkIoFormat getFormat() const {
    return format_;
  }

  /**
   * @brief Get the external file format's name.
   *
   * @return The external file format's name.
   */
  inline std::string getFormatName() const {
    switch (format_) {
      case BulkIoFormat::kCsv:
        return "CSV";
      case BulkIoFormat::kText:
        return "TEXT";
      default:
        break;
    }
    LOG(FATAL) << "Unexpected format in BulkIoConfiguration::getFormatName()";
  }

  /**
   * @brief Get the delimiter character (which is the character that separates
   *        attribute values in external files).
   *
   * @return The delimiter character.
   */
  inline char getDelimiter() const {
    return delimiter_;
  }

  /**
   * @brief Set the delimiter character.
   *
   * @param delimiter The delimiter character to set.
   */
  inline void setDelimiter(const char delimiter) {
    delimiter_ = delimiter;
  }

  /**
   * @brief Check whether to encode/decode between special characters and escape
   *        sequences.
   *
   * @return Whether to encode/decode between special characters and escape
   *         sequences.
   */
  inline bool escapeStrings() const {
    return escape_strings_;
  }

  /**
   * @brief Set whether to encode/decode between special characters and escape
   *        sequences.
   *
   * @param escape_strings A bool value to set that indicates whether to
   *        encode/decode between special characters and escape sequences.
   */
  inline void setEscapeStrings(const bool escape_strings) {
    escape_strings_ = escape_strings;
  }

  /**
   * @brief Check whether the external files contain headers (for CSV format only).
   *
   * @return Whether the external files contain headers.
   */
  inline bool hasHeader() const {
    return header_;
  }

  /**
   * @brief Set whether the external files contain headers (for CSV format only).
   *
   * @param header A bool value to set that indicates whether the external files
   *        contain headers.
   */
  inline void setHeader(const bool header) {
    header_ = header;
  }

  /**
   * @brief Get the quote character (for CSV format only).
   *
   * @return The quote character.
   */
  inline char getQuoteCharacter() const {
    return quote_;
  }

  /**
   * @brief Set the quote character (for CSV format only).
   *
   * @param quote The quote character to set.
   */
  inline void setQuoteCharacter(const char quote) {
    quote_ = quote;
  }

  /**
   * @brief Get the string that represents a null value.
   *
   * @return The string that represents a null value.
   */
  inline const std::string& getNullString() const {
    return null_string_;
  }

  /**
   * @brief Set the string that represents a null value.
   *
   * @param null_string The string to set that represents a null value.
   */
  inline void setNullString(const std::string &null_string) {
    null_string_ = null_string;
  }

 private:
  // Initialize default options for CSV and TEXT formats.
  void initializeDefaultParameters(const BulkIoFormat format);

  const BulkIoFormat format_;

  char delimiter_;
  bool escape_strings_;
  bool header_;
  char quote_;
  std::string null_string_;

  DISALLOW_COPY_AND_ASSIGN(BulkIoConfiguration);
};

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BULK_IO_CONFIGURATION_HPP_
