/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_NUMERIC_TYPE_UNIFIER_HPP_
#define QUICKSTEP_TYPES_NUMERIC_TYPE_UNIFIER_HPP_

namespace quickstep {

class DoubleType;
class FloatType;
class IntType;
class LongType;

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A traits template that resolves what the "unifying" Type of two
 *        built-in numeric Types (currently IntType, LongType, FloatType, and
 *        DoubleType) is at compile time.
 *
 * If both argument types are built-in numeric Types, then the struct will
 * contain a typedef called "type" that refers to the unifying Type class.
 *
 * @note A "unifying" Type is a type that both argument types can be safely
 *       coerced to. It is not necessarily one of the input types (for example,
 *       the unifying type for LongType and FloatType is DoubleType).
 * @note This provides a subset of the functionality of
 *       TypeFactory::GetUnifyingType(). It only covers numeric types, and does
 *       not do any handling of nullability. It does, however, allow static
 *       resolution at compile-time.
 *
 * @tparam LeftType The first Quickstep numeric Type class to unify.
 * @tparam RightType The second Quickstep numeric Type class to unify.
 **/
template <typename LeftType, typename RightType>
struct NumericTypeUnifier {
};

/** @} */

// Explicit template specializations for all combinations of builtin numeric
// types.
template<>
struct NumericTypeUnifier<IntType, IntType> {
  typedef IntType type;
};

template<>
struct NumericTypeUnifier<IntType, LongType> {
  typedef LongType type;
};

template<>
struct NumericTypeUnifier<IntType, FloatType> {
  typedef FloatType type;
};

template<>
struct NumericTypeUnifier<IntType, DoubleType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<LongType, IntType> {
  typedef LongType type;
};

template<>
struct NumericTypeUnifier<LongType, LongType> {
  typedef LongType type;
};

template<>
struct NumericTypeUnifier<LongType, FloatType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<LongType, DoubleType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<FloatType, IntType> {
  typedef FloatType type;
};

template<>
struct NumericTypeUnifier<FloatType, LongType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<FloatType, FloatType> {
  typedef FloatType type;
};

template<>
struct NumericTypeUnifier<FloatType, DoubleType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<DoubleType, IntType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<DoubleType, LongType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<DoubleType, FloatType> {
  typedef DoubleType type;
};

template<>
struct NumericTypeUnifier<DoubleType, DoubleType> {
  typedef DoubleType type;
};

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_NUMERIC_TYPE_UNIFIER_HPP_
