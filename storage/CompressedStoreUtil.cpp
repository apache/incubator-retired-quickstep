/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "storage/CompressedStoreUtil.hpp"

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <utility>

#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "compression/CompressionDictionary.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "types/DoubleType.hpp"
#include "types/FloatType.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/TypeID.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

PredicateTransformResult CompressedAttributePredicateTransformer::TransformPredicateOnCompressedAttribute(
    const CatalogRelationSchema &relation,
    const ComparisonPredicate &predicate,
    const CompressionDictionary *dictionary,
    const std::int64_t maximum_truncated_value) {
  // The predicate must be a comparison of a compressed attribute with
  // a literal.
  DCHECK(predicate.isAttributeLiteralComparisonPredicate());

  const CatalogAttribute *comparison_attribute = nullptr;
  bool left_literal = false;
  if (predicate.getLeftOperand().hasStaticValue()) {
    DCHECK(predicate.getRightOperand().getDataSource() == Scalar::kAttribute);
    comparison_attribute
        = &(static_cast<const ScalarAttribute&>(predicate.getRightOperand()).getAttribute());
    left_literal = true;
  } else {
    DCHECK(predicate.getLeftOperand().getDataSource() == Scalar::kAttribute);
    comparison_attribute
        = &(static_cast<const ScalarAttribute&>(predicate.getLeftOperand()).getAttribute());
    left_literal = false;
  }

  DCHECK_EQ(relation.getID(), comparison_attribute->getParent().getID());
  TypedValue comparison_literal;
  const Type *comparison_literal_type;
  if (left_literal) {
    comparison_literal = predicate.getLeftOperand().getStaticValue().makeReferenceToThis();
    comparison_literal_type = &predicate.getLeftOperand().getType();
  } else {
    comparison_literal = predicate.getRightOperand().getStaticValue().makeReferenceToThis();
    comparison_literal_type = &predicate.getRightOperand().getType();
  }

  if (comparison_literal.isNull()) {
    // Comparisons with null are always false.
    return PredicateTransformResult(PredicateTransformResultType::kNone);
  }

  if (predicate.getComparison().getComparisonID() == ComparisonID::kEqual) {
    return TransformEqualPredicateOnCompressedAttribute(comparison_literal,
                                                        *comparison_literal_type,
                                                        dictionary,
                                                        maximum_truncated_value);
  } else if (predicate.getComparison().getComparisonID() == ComparisonID::kNotEqual) {
    return TransformNotEqualPredicateOnCompressedAttribute(comparison_literal,
                                                           *comparison_literal_type,
                                                           dictionary,
                                                           maximum_truncated_value);
  } else {
    if (left_literal) {
      switch (predicate.getComparison().getComparisonID()) {
        case ComparisonID::kLess:
          return TransformOtherComparisonPredicateOnCompressedAttribute(ComparisonID::kGreater,
                                                                        comparison_literal,
                                                                        *comparison_literal_type,
                                                                        dictionary,
                                                                        maximum_truncated_value);
        case ComparisonID::kLessOrEqual:
          return TransformOtherComparisonPredicateOnCompressedAttribute(ComparisonID::kGreaterOrEqual,
                                                                        comparison_literal,
                                                                        *comparison_literal_type,
                                                                        dictionary,
                                                                        maximum_truncated_value);
        case ComparisonID::kGreater:
          return TransformOtherComparisonPredicateOnCompressedAttribute(ComparisonID::kLess,
                                                                        comparison_literal,
                                                                        *comparison_literal_type,
                                                                        dictionary,
                                                                        maximum_truncated_value);
        case ComparisonID::kGreaterOrEqual:
          return TransformOtherComparisonPredicateOnCompressedAttribute(ComparisonID::kLessOrEqual,
                                                                        comparison_literal,
                                                                        *comparison_literal_type,
                                                                        dictionary,
                                                                        maximum_truncated_value);
        default:
          FATAL_ERROR("Unexpected ComparisonID in "
                      "CompressedAttributePredicateTransformer::getMatchesForPredicate()");
      }
    } else {
      return TransformOtherComparisonPredicateOnCompressedAttribute(
          predicate.getComparison().getComparisonID(),
          comparison_literal,
          *comparison_literal_type,
          dictionary,
          maximum_truncated_value);
    }
  }
}

namespace {

// Helper class for figuring out information about comparisons with truncated
// values. Has different partial specializations for integer types and
// floating-point types.
template <typename NumericLiteralT, typename Enable = void>
class TruncationHelper {
};

// Partial specialization for integer types.
template <typename NumericLiteralT>
class TruncationHelper<
    NumericLiteralT,
    typename std::enable_if<std::is_integral<NumericLiteralT>::value>::type> {
 public:
  // Whether an int64_t represents 'right_value' exactly. For Int and Long,
  // this is always true.
  static bool LongIsExact(const TypedValue &right_literal) {
    return true;
  }

  // Get 'right_literal' truncated to a code.
  static std::uint32_t GetAsCode(const TypedValue &right_literal) {
    return right_literal.getLiteral<NumericLiteralT>();
  }

  // Get the effective literal value to use in place of 'right_literal' when
  // evaluating the comparison specified by 'comp'. Since LongIsExact() is
  // always true for integer types, no adjustment is needed.
  static std::int64_t GetEffectiveLiteralForComparison(const ComparisonID comp,
                                                       const TypedValue &right_literal) {
    return right_literal.getLiteral<NumericLiteralT>();
  }

  // Determine whether 'value' is in the range of truncated codes between 0 and
  // 'maximum_truncated_value'.
  static bool ValueInRange(const TypedValue &value,
                           const std::int64_t maximum_truncated_value) {
    const std::int64_t value_as_int64 = value.getLiteral<NumericLiteralT>();
    return (value_as_int64 >= 0) && (value_as_int64 <= maximum_truncated_value);
  }
};

// Partial specialization for floating-point types.
template <typename NumericLiteralT>
class TruncationHelper<
    NumericLiteralT,
    typename std::enable_if<std::is_floating_point<NumericLiteralT>::value>::type> {
 public:
  // Whether an int64_t represents 'right_value' exactly. For floating-point
  // types, 'right_value' may have a fractional part or a large exponent that
  // makes it out-of-range.
  static bool LongIsExact(const TypedValue &right_literal) {
    const double value_as_double = right_literal.getLiteral<NumericLiteralT>();
    return value_as_double == static_cast<std::int64_t>(value_as_double);
  }

  // Get 'right_literal' truncated to a code.
  static std::uint32_t GetAsCode(const TypedValue &right_literal) {
    return right_literal.getLiteral<NumericLiteralT>();
  }

  // Get the effective literal value to use in place of 'right_literal' when
  // evaluating the comparison specified by 'comp'. If LongIsExact() is true,
  // then no adjustment is needed. Otherwise, the value is rounded up or down
  // as needed to make the comparison evaluate correctly.
  static std::int64_t GetEffectiveLiteralForComparison(const ComparisonID comp,
                                                       const TypedValue &right_literal) {
    const double value_as_double = right_literal.getLiteral<NumericLiteralT>();
    const std::int64_t value_as_int64 = value_as_double;
    if (value_as_double == value_as_int64) {
      // The value has no fractional part, so no adjustment is needed.
      return value_as_int64;
    }

    switch (comp) {
      case ComparisonID::kLess:
      case ComparisonID::kGreaterOrEqual:
        return std::ceil(value_as_double);
      case ComparisonID::kLessOrEqual:
      case ComparisonID::kGreater:
        return std::floor(value_as_double);
      default: {
        LOG(FATAL) << "Unexpected ComparisonID (not <, <=, >, or >=)";
      }
    }
  }

  // Determine whether 'value' is in the range of truncated codes between 0 and
  // 'maximum_truncated_value'.
  static bool ValueInRange(const TypedValue &value,
                           const std::int64_t maximum_truncated_value) {
    const NumericLiteralT value_literal = value.getLiteral<NumericLiteralT>();
    return (value_literal >= 0) && (value_literal <= maximum_truncated_value);
  }
};

}  // namespace

std::int64_t CompressedAttributePredicateTransformer::GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(
    const ComparisonID comp,
    const TypedValue &right_literal,
    const Type &right_literal_type) {
  switch (right_literal_type.getTypeID()) {
    case kInt:
      return TruncationHelper<IntType::cpptype>::GetEffectiveLiteralForComparison(
          comp,
          right_literal);
    case kLong:
      return TruncationHelper<LongType::cpptype>::GetEffectiveLiteralForComparison(
          comp,
          right_literal);
    case kFloat:
      return TruncationHelper<FloatType::cpptype>::GetEffectiveLiteralForComparison(
          comp,
          right_literal);
    case kDouble:
      return TruncationHelper<DoubleType::cpptype>::GetEffectiveLiteralForComparison(
          comp,
          right_literal);
    default: {
      LOG(FATAL) << "Type not recognized as numeric: " << right_literal_type.getName();
    }
  }
}

bool CompressedAttributePredicateTransformer::CompressedComparisonIsAlwaysTrueForTruncatedAttribute(
    const ComparisonID comp,
    const TypedValue &right_literal,
    const Type &right_literal_type,
    const std::int64_t maximum_truncated_value) {
  // First, check equality and inequality.
  switch (comp) {
    case ComparisonID::kEqual:
      return false;
    case ComparisonID::kNotEqual:
      // When comparing against truncated codes, not-equal is always true if
      // 'right_literal' has a fractional part, or if it is less than zero or
      // greater than the largest truncated value.
      switch (right_literal_type.getTypeID()) {
        case kInt: {
          return (!TruncationHelper<IntType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<IntType::cpptype>::ValueInRange(right_literal,
                                                                       maximum_truncated_value));
        }
        case kLong: {
          return (!TruncationHelper<LongType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<LongType::cpptype>::ValueInRange(right_literal,
                                                                        maximum_truncated_value));
        }
        case kFloat: {
          return (!TruncationHelper<FloatType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<FloatType::cpptype>::ValueInRange(right_literal,
                                                                         maximum_truncated_value));
        }
        case kDouble: {
          return (!TruncationHelper<DoubleType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<DoubleType::cpptype>::ValueInRange(right_literal,
                                                                          maximum_truncated_value));
        }
        default: {
          LOG(FATAL) << "Type not recognized as numeric: " << right_literal_type.getName();
        }
      }
    default:
      break;
  }

  std::int64_t effective_literal
      = GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(comp,
                                                                    right_literal,
                                                                    right_literal_type);

  switch (comp) {
    case ComparisonID::kLess:
      if (effective_literal > maximum_truncated_value) {
        return true;
      } else {
        return false;
      }
    case ComparisonID::kLessOrEqual:
      if (effective_literal >= maximum_truncated_value) {
        return true;
      } else {
        return false;
      }
    case ComparisonID::kGreater:
      if (effective_literal < 0) {
        return true;
      } else {
        return false;
      }
    case ComparisonID::kGreaterOrEqual:
      if (effective_literal <= 0) {
        return true;
      } else {
        return false;
      }
    default:
      FATAL_ERROR("Unexpected ComparisonID in CompressedAttributePredicateTransformer::"
                  "CompressedComparisonIsAlwaysTrueForTruncatedAttribute()");
  }
}

bool CompressedAttributePredicateTransformer::CompressedComparisonIsAlwaysFalseForTruncatedAttribute(
    const ComparisonID comp,
    const TypedValue &right_literal,
    const Type &right_literal_type,
    const std::int64_t maximum_truncated_value) {
  // First, check equality and inequality.
  switch (comp) {
    case ComparisonID::kEqual:
      // When comparing against truncated codes, equal is always false if
      // 'right_literal' has a fractional part, or if it is less than zero or
      // greater than the largest truncated value.
      switch (right_literal_type.getTypeID()) {
        case kInt: {
          return (!TruncationHelper<IntType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<IntType::cpptype>::ValueInRange(right_literal,
                                                                       maximum_truncated_value));
        }
        case kLong: {
          return (!TruncationHelper<LongType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<LongType::cpptype>::ValueInRange(right_literal,
                                                                        maximum_truncated_value));
        }
        case kFloat: {
          return (!TruncationHelper<FloatType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<FloatType::cpptype>::ValueInRange(right_literal,
                                                                         maximum_truncated_value));
        }
        case kDouble: {
          return (!TruncationHelper<DoubleType::cpptype>::LongIsExact(right_literal))
                 || (!TruncationHelper<DoubleType::cpptype>::ValueInRange(right_literal,
                                                                          maximum_truncated_value));
        }
        default: {
          LOG(FATAL) << "Type not recognized as numeric: " << right_literal_type.getName();
        }
      }
    case ComparisonID::kNotEqual:
      return false;
    default:
      break;
  }

  std::int64_t effective_literal
      = GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(comp,
                                                                    right_literal,
                                                                    right_literal_type);
  switch (comp) {
    case ComparisonID::kLess:
      if (effective_literal <= 0) {
        return true;
      } else {
        return false;
      }
    case ComparisonID::kLessOrEqual:
      if (effective_literal < 0) {
        return true;
      } else {
        return false;
      }
    case ComparisonID::kGreater:
      if (effective_literal >= maximum_truncated_value) {
        return true;
      } else {
        return false;
      }
    case ComparisonID::kGreaterOrEqual:
      if (effective_literal > maximum_truncated_value) {
        return true;
      } else {
        return false;
      }
    default:
      FATAL_ERROR("Unexpected ComparisonID in CompressedAttributePredicateTransformer::"
                  "CompressedComparisonIsAlwaysFalseForTruncatedAttribute()");
  }
}

PredicateTransformResult CompressedAttributePredicateTransformer
    ::TransformEqualPredicateOnCompressedAttribute(
        const TypedValue &right_literal,
        const Type &right_literal_type,
        const CompressionDictionary *dictionary,
        const std::int64_t maximum_truncated_value) {
  std::uint32_t match_code;
  if (dictionary) {
    match_code = dictionary->getCodeForTypedValue(right_literal, right_literal_type);
    if (match_code == dictionary->numberOfCodes()) {
      return PredicateTransformResult(PredicateTransformResultType::kNone);
    }
  } else {
    if (CompressedComparisonIsAlwaysFalseForTruncatedAttribute(ComparisonID::kEqual,
                                                               right_literal,
                                                               right_literal_type,
                                                               maximum_truncated_value)) {
      return PredicateTransformResult(PredicateTransformResultType::kNone);
    }
    // At this point, we know that 'right_literal' should not have a fractional
    // part and should be in the range of truncated codes.
    switch (right_literal_type.getTypeID()) {
      case kInt:
        DCHECK(TruncationHelper<IntType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<IntType::cpptype>::GetAsCode(right_literal);
        break;
      case kLong:
        DCHECK(TruncationHelper<LongType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<LongType::cpptype>::GetAsCode(right_literal);
        break;
      case kFloat:
        DCHECK(TruncationHelper<FloatType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<FloatType::cpptype>::GetAsCode(right_literal);
        break;
      case kDouble:
        DCHECK(TruncationHelper<DoubleType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<DoubleType::cpptype>::GetAsCode(right_literal);
        break;
      default:
        LOG(FATAL) << "Type not recognized as numeric: " << right_literal_type.getName();
    }
  }

  return PredicateTransformResult(PredicateTransformResultType::kBasicComparison,
                                  ComparisonID::kEqual,
                                  match_code);
}

PredicateTransformResult CompressedAttributePredicateTransformer
    ::TransformNotEqualPredicateOnCompressedAttribute(
        const TypedValue &right_literal,
        const Type &right_literal_type,
        const CompressionDictionary *dictionary,
        const std::int64_t maximum_truncated_value) {
  std::uint32_t match_code;
  std::uint32_t null_code = std::numeric_limits<std::uint32_t>::max();
  if (dictionary) {
    match_code = dictionary->getCodeForTypedValue(right_literal, right_literal_type);
    null_code = dictionary->getNullCode();
    if (match_code == dictionary->numberOfCodes()) {
      if (dictionary->containsNull()) {
        return PredicateTransformResult(PredicateTransformResultType::kBasicComparison,
                                        ComparisonID::kNotEqual,
                                        null_code);
      } else {
        // Get all tuples.
        return PredicateTransformResult(PredicateTransformResultType::kAll);
      }
    }
  } else {
    if (CompressedComparisonIsAlwaysTrueForTruncatedAttribute(ComparisonID::kNotEqual,
                                                              right_literal,
                                                              right_literal_type,
                                                              maximum_truncated_value)) {
      return PredicateTransformResult(PredicateTransformResultType::kAll);
    }
    // At this point, we know that 'right_literal' should not have a fractional
    // part and should be in the range of truncated codes.
    switch (right_literal_type.getTypeID()) {
      case kInt:
        DCHECK(TruncationHelper<IntType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<IntType::cpptype>::GetAsCode(right_literal);
        break;
      case kLong:
        DCHECK(TruncationHelper<LongType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<LongType::cpptype>::GetAsCode(right_literal);
        break;
      case kFloat:
        DCHECK(TruncationHelper<FloatType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<FloatType::cpptype>::GetAsCode(right_literal);
        break;
      case kDouble:
        DCHECK(TruncationHelper<DoubleType::cpptype>::LongIsExact(right_literal));
        match_code = TruncationHelper<DoubleType::cpptype>::GetAsCode(right_literal);
        break;
      default:
        LOG(FATAL) << "Type not recognized as numeric: " << right_literal_type.getName();
    }
  }

  if (null_code == std::numeric_limits<std::uint32_t>::max()) {
    return PredicateTransformResult(PredicateTransformResultType::kBasicComparison,
                                    ComparisonID::kNotEqual,
                                    match_code);
  } else {
    return PredicateTransformResult(PredicateTransformResultType::kBasicComparison,
                                    ComparisonID::kNotEqual,
                                    match_code,
                                    null_code,
                                    true);
  }
}

PredicateTransformResult CompressedAttributePredicateTransformer
    ::TransformOtherComparisonPredicateOnCompressedAttribute(
        const ComparisonID comp,
        const TypedValue &right_literal,
        const Type &right_literal_type,
        const CompressionDictionary *dictionary,
        const std::int64_t maximum_truncated_value) {
  std::pair<std::uint32_t, std::uint32_t> match_range;
  if (dictionary) {
    match_range = dictionary->getLimitCodesForComparisonTyped(comp,
                                                              right_literal,
                                                              right_literal_type);
    if (match_range.first >= match_range.second) {
      // No matches.
      return PredicateTransformResult(PredicateTransformResultType::kNone);
    }
    if (match_range.second == dictionary->numberOfCodes()) {
      // This trick lets us skip an unnecessary comparison.
      match_range.second = std::numeric_limits<std::uint32_t>::max();
    }
  } else {
    if (CompressedComparisonIsAlwaysTrueForTruncatedAttribute(comp,
                                                              right_literal,
                                                              right_literal_type,
                                                              maximum_truncated_value)) {
      return PredicateTransformResult(PredicateTransformResultType::kAll);
    }
    if (CompressedComparisonIsAlwaysFalseForTruncatedAttribute(comp,
                                                               right_literal,
                                                               right_literal_type,
                                                               maximum_truncated_value)) {
      return PredicateTransformResult(PredicateTransformResultType::kNone);
    }

    std::int64_t effective_literal
        = GetEffectiveLiteralValueForComparisonWithTruncatedAttribute(comp,
                                                                      right_literal,
                                                                      right_literal_type);
    switch (comp) {
      case ComparisonID::kLess:
        match_range.first = 0;
        match_range.second = effective_literal;
        break;
      case ComparisonID::kLessOrEqual:
        match_range.first = 0;
        match_range.second = effective_literal + 1;
        break;
      case ComparisonID::kGreater:
        match_range.first = effective_literal + 1;
        match_range.second = std::numeric_limits<std::uint32_t>::max();
        break;
      case ComparisonID::kGreaterOrEqual:
        match_range.first = effective_literal;
        match_range.second = std::numeric_limits<std::uint32_t>::max();
        break;
      default:
        FATAL_ERROR("Unexpected ComparisonID in CompressedAttributePredicateTransformer::"
                    "TransformOtherComparisonPredicateOnCompressedAttribute()");
    }
  }

  if (match_range.first == 0) {
    if (match_range.second == std::numeric_limits<std::uint32_t>::max()) {
      return PredicateTransformResult(PredicateTransformResultType::kAll);
    } else {
      return PredicateTransformResult(PredicateTransformResultType::kBasicComparison,
                                      ComparisonID::kLess,
                                      match_range.second);
    }
  } else if (match_range.second == std::numeric_limits<std::uint32_t>::max()) {
    return PredicateTransformResult(PredicateTransformResultType::kBasicComparison,
                                    ComparisonID::kGreaterOrEqual,
                                    match_range.first);
  } else {
    return PredicateTransformResult(PredicateTransformResultType::kRangeComparison,
                                    match_range.first,
                                    match_range.second);
  }
}

}  // namespace quickstep
