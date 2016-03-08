/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *   University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARATORS_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARATORS_HPP_

#include <cstddef>
#include <string>

#include "catalog/CatalogTypedefs.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/port/strnlen.hpp"

#include "re2/stringpiece.h"
#include "re2/re2.h"

namespace quickstep {

class ColumnVector;
class TupleIdSequence;
class ValueAccessor;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Base class for UncheckedComparators which does pattern matching.
 **/
template <bool is_like_pattern, bool is_negation,
          bool left_nullable, bool right_nullable>
class PatternMatchingUncheckedComparator : public UncheckedComparator {
 public:
  /**
   * @brief Constructor.
   **/
  PatternMatchingUncheckedComparator(const std::size_t left_length,
                                     const std::size_t right_length)
    : left_length_(left_length),
      right_length_(right_length) {
  }

  inline bool compareDataPtrs(const void *left,
                              const void *right) const override {
    return compareDataPtrsInl(left, right);
  }

  inline bool compareDataPtrsInl(const void *left,
                                 const void *right) const {
    if ((left_nullable && (left == nullptr))
        || (right_nullable && (right == nullptr))) {
      return false;
    }

    const char* text = static_cast<const char*>(left);
    const re2::StringPiece text_piece(text, strnlen(text, left_length_));

    const char* pattern = static_cast<const char*>(right);
    std::string regex_pattern;
    re2::StringPiece pattern_piece;
    int orig_pattern_len = strnlen(pattern, right_length_);
    if (is_like_pattern) {
      regex_pattern = transformLikeToRegex(pattern, orig_pattern_len);
      pattern_piece.set(regex_pattern.c_str(), regex_pattern.size());
    } else {
      pattern_piece.set(pattern, orig_pattern_len);
    }
    const re2::RE2 re2_pattern(pattern_piece);
    return match(text_piece, re2_pattern);
  }

  inline bool compareTypedValues(const TypedValue &left,
                                 const TypedValue &right) const override {
    return compareDataPtrsInl(left.getOutOfLineData(), right.getOutOfLineData());
  }

  inline bool compareTypedValueWithDataPtr(const TypedValue &left,
                                           const void *right) const override {
    return compareDataPtrsInl(left.getOutOfLineData(), right);
  }

  inline bool compareDataPtrWithTypedValue(const void *left,
                                           const TypedValue &right) const override {
    return compareDataPtrsInl(left, right.getOutOfLineData());
  }

  TupleIdSequence* compareColumnVectors(
      const ColumnVector &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override;

  TupleIdSequence* compareColumnVectorAndStaticValue(
      const ColumnVector &left,
      const TypedValue &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override;

  TupleIdSequence* compareStaticValueAndColumnVector(
      const TypedValue &left,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override;

#ifdef QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION
  TupleIdSequence* compareSingleValueAccessor(
      ValueAccessor *accessor,
      const attribute_id left_id,
      const attribute_id right_id,
      const TupleIdSequence *filter) const override;

  TupleIdSequence* compareValueAccessorAndStaticValue(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const TypedValue &right,
      const TupleIdSequence *filter) const override;

  TupleIdSequence* compareStaticValueAndValueAccessor(
      const TypedValue &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter) const override;

  TupleIdSequence* compareColumnVectorAndValueAccessor(
      const ColumnVector &left,
      ValueAccessor *right_accessor,
      const attribute_id right_id,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override;

  TupleIdSequence* compareValueAccessorAndColumnVector(
      ValueAccessor *left_accessor,
      const attribute_id left_id,
      const ColumnVector &right,
      const TupleIdSequence *filter,
      const TupleIdSequence *existence_bitmap) const override;

  TypedValue accumulateValueAccessor(
      const TypedValue &current,
      ValueAccessor *accessor,
      const attribute_id value_accessor_id) const override;
#endif  // QUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION

  TypedValue accumulateColumnVector(
      const TypedValue &current,
      const ColumnVector &column_vector) const override;

 private:
  /**
   * @brief Checks whether the raw text matches pattern.
   *
   * @param text The raw text to be checked, must be not null.
   * @param pattern The pattern to be matched against.
   *
   * @return Whether text matches pattern.
   **/
  inline bool matchDataPtrWithPattern(const void *text,
                                      const re2::RE2 &pattern) const {
    const char* c_text = static_cast<const char*>(text);
    const re2::StringPiece text_piece(c_text, strnlen(c_text, left_length_));
    return match(text_piece, pattern);
  }

  /**
   * @brief Checks whether text matches pattern.
   *
   * @param text The text to be checked.
   * @param pattern The pattern to be matched against.
   *
   * @return Whether text matches pattern.
   **/
  inline bool match(const re2::StringPiece &text,
                    const re2::RE2 &pattern) const {
    if (is_negation) {
      return !(re2::RE2::FullMatch(text, pattern));
    } else {
      return re2::RE2::FullMatch(text, pattern);
    }
  }

  /**
   * @brief Tranform a LIKE pattern to the corresponding regular expression
   *        pattern.
   *
   * @param like_pattern The original LIKE pattern.
   * @param len The length of the LIKE pattern.
   *
   * @return The transformed regular expression pattern.
   */
  inline std::string transformLikeToRegex(const char *like_pattern,
                                          const size_t len) const {
    std::string regex_pattern;
    for (size_t i = 0; i < len; i++) {
      char c = like_pattern[i];
      switch (c) {
        case '_':
          regex_pattern.push_back('.');
          break;
        case '%':
          regex_pattern.append(".*");
          break;
        default:
          // Escape special characters
          if ((c < 'a' || c > 'z')
              && (c < 'A' || c > 'Z')
              && (c < '0' || c > '9')
              && !(c & 128)) {
            regex_pattern.push_back('\\');
          }
          regex_pattern.push_back(c);
          break;
      }
    }
    return regex_pattern;
  }

  const std::size_t left_length_;
  const std::size_t right_length_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_COMPARISONS_PATTERN_MATCHING_COMPARATORS_HPP_
