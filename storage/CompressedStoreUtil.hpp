/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_COMPRESSED_STORE_UTIL_HPP_
#define QUICKSTEP_STORAGE_COMPRESSED_STORE_UTIL_HPP_

#include <cstddef>
#include <cstdint>

#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ComparisonPredicate;
class CompressionDictionary;
class Type;
class TypedValue;

/** \addtogroup Storage
 *  @{
 */

enum class PredicateTransformResultType {
  kAll = 0,
  kNone,
  kBasicComparison,
  kRangeComparison
};

/**
 * @brief The return value of a call to TransformPredicateOnCompressedAttribute.
 *        This structure represents a transformed predicate, which might
 *        indicate to get all (non-null) values, get no values, or use a
 *        transformed comparison predicate.
 */
struct PredicateTransformResult {
  /**
   * @brief The constructor used for constructing a kAll or kNone result.
   *
   * @param type_arg The transform result type.
   **/
  explicit PredicateTransformResult(PredicateTransformResultType type_arg)
      : type(type_arg),
        comp(ComparisonID::kNumComparisonIDs),
        first_literal(0),
        second_literal(0),
        exclude_nulls(false) {
    DEBUG_ASSERT(type_arg == PredicateTransformResultType::kAll
        || type_arg == PredicateTransformResultType::kNone);
  }

  /**
   * @brief The constructor used for constructing a kBasicComparison result.
   *
   * @param type_arg The transform result type.
   * @param comp_arg The comparator of the transformed predicate.
   * @param literal_arg The code of the literal in the transformed predicate.
   **/
  PredicateTransformResult(PredicateTransformResultType type_arg,
                           ComparisonID comp_arg,
                           std::uint32_t literal_arg)
      : type(type_arg),
        comp(comp_arg),
        first_literal(literal_arg),
        second_literal(0),
        exclude_nulls(false) {
    DEBUG_ASSERT(type_arg == PredicateTransformResultType::kBasicComparison);
  }

  /**
   * @brief The constructor used for constructing a kRangeComparison result.
   *        The range can be represented as [first_literal, second_literal).
   *
   * @param type_arg The transform result type.
   * @param first_literal_arg The code of first literal in the transformed
   *        predicate.
   * @param second_literal_arg The code of the second literal in the
   *        transformed predicate.
   **/
  PredicateTransformResult(PredicateTransformResultType type_arg,
                           std::uint32_t first_literal_arg,
                           std::uint32_t second_literal_arg)
      : type(type_arg),
        comp(ComparisonID::kNumComparisonIDs),
        first_literal(first_literal_arg),
        second_literal(second_literal_arg),
        exclude_nulls(false) {
    DEBUG_ASSERT(type_arg == PredicateTransformResultType::kRangeComparison);
  }

  /**
   * @brief The constructor used for constructing a kBasicComparison result
   *        with a Comparison::kNotEqual comparator.
   *
   * @param type_arg The transform result type.
   * @param comp_arg The comparator of the transformed predicate.
   * @param literal_arg The code of the literal in the kNotEqual comparator.
   * @param null_arg The code of NULL.
   * @param exclude_null_arg True if the transformed predicate excludes NULLs.
   **/
  PredicateTransformResult(PredicateTransformResultType type_arg,
                           ComparisonID comp_arg,
                           std::uint32_t literal_arg,
                           std::uint32_t null_arg,
                           bool exclude_nulls_arg)
      : type(type_arg),
        comp(comp_arg),
        first_literal(literal_arg),
        second_literal(null_arg),
        exclude_nulls(exclude_nulls_arg) {
    DEBUG_ASSERT(type_arg == PredicateTransformResultType::kBasicComparison);
    DEBUG_ASSERT(comp_arg == ComparisonID::kNotEqual);
  }

  PredicateTransformResultType type;
  ComparisonID comp;
  std::uint32_t first_literal;
  std::uint32_t second_literal;
  bool exclude_nulls;
};

/**
 * @brief A class which contains static helper methods for transforming
 *        a comparison predicate on a compressed attribute. This class is
 *        used by CompressedTupleStorageSubBlock.
 **/
class CompressedAttributePredicateTransformer {
 public:
  /**
   * @brief Transform a predicate on a compressed attribute of
   *        a CompressedTupleStorageSubBlock.
   * @note Currently this method can only support a simple comparison of a
   *       literal value with a compressed attribute.
   *
   * @param relation The relation which the comparison attribute belongs to.
   * @param predicate A predicate to attempt to transform.
   * @param dictionary A dictionary used to compress this attribute. If nullptr,
   *        we use the truncation compression.
   * @param maximum_truncated_value The maximum truncated value in the
   *        compressed attribute. This argument is useful only when dictionary
   *        is nullptr.
   * @return A PredicateTransformResult that contains the transformed predicate.
   **/
  static PredicateTransformResult TransformPredicateOnCompressedAttribute(
      const CatalogRelationSchema &relation,
      const ComparisonPredicate &predicate,
      const CompressionDictionary *dictionary,
      const std::int64_t maximum_truncated_value);

  /**
   * @brief Get a long value for a comparison of a truncated attribute with a
   *        literal TypedValue. If the literal is a Float or Double with a
   *        fractional part, the value will be adjusted so that the comparison
   *        will still evaluate correctly.
   *
   * @param comp The ID of the comparison being evaluated (the order is
   *        'attribute comp literal'). Must be kLess, kLessOrEqual, kGreater,
   *        or kGreaterOrEqual (kEqual and kNotEqual are not supported).
   * @param right_literal A literal of a numeric type (Int, Long, Float, or
   *        Double) to get the effective truncated value for.
   * @param right_literal_type The specific concrete Type that right_literal
   *        belongs to.
   * @return The value of right_literal as a long, adjusted as necessary so
   *         that the specified comparison still evaluates correctly.
   **/
  static std::int64_t GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(
      const ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type);

  /**
   * @brief Determine if a comparison must always be true for any possible
   *        value of a truncated attribute.
   * @warning The attribute must be truncation compressed.
   *
   * @param comp The comparison to evaluate.
   * @param right_literal The literal value on the right side of the
   *        comparison.
   * @param right_literal_type The specific concrete Type that right_literal
   *        belongs to.
   * @param maximum_truncated_value The maximum truncated value in the
   *        attribute on the left side of the comparison.
   * @return true if the comparison must always be true for all possible
   *         values of the attribute.
   **/
  static bool CompressedComparisonIsAlwaysTrueForTruncatedAttribute(
      const ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type,
      const std::int64_t maximum_truncated_value);

  /**
   * @brief Determine if a comparison must always be false for any possible
   *        value of a truncated attribute.
   * @warning The attribute must be truncation compressed.
   *
   * @param comp The comparison to evaluate.
   * @param right_literal The literal value on the right side of the
   *        comparison.
   * @param right_literal_type The specific concrete Type that right_literal
   *        belongs to.
   * @param maximum_truncated_value The maximum truncated value in the
   *        attribute on the left side of the comparison.
   * @return true if the comparison must always be false for all possible
   *         values of the attribute.
   **/
  static bool CompressedComparisonIsAlwaysFalseForTruncatedAttribute(
      const ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type,
      const std::int64_t maximum_truncated_value);

 private:
  // Undefined default constructor - class is all static and should not be
  // instantiated.
  CompressedAttributePredicateTransformer();

  static PredicateTransformResult TransformEqualPredicateOnCompressedAttribute(
      const TypedValue &right_literal,
      const Type &right_literal_type,
      const CompressionDictionary *dictionary,
      const std::int64_t maximum_truncated_value);

  static PredicateTransformResult TransformNotEqualPredicateOnCompressedAttribute(
      const TypedValue &right_literal,
      const Type &right_literal_type,
      const CompressionDictionary *dictionary,
      const std::int64_t maximum_truncated_value);

  static PredicateTransformResult TransformOtherComparisonPredicateOnCompressedAttribute(
      const ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type,
      const CompressionDictionary *dictionary,
      const std::int64_t maximum_truncated_value);

  DISALLOW_COPY_AND_ASSIGN(CompressedAttributePredicateTransformer);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_COLUMN_STORE_UTIL_HPP_
