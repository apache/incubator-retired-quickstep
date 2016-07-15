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

#ifndef QUICKSTEP_TYPES_TYPE_HPP_
#define QUICKSTEP_TYPES_TYPE_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>

#include "types/Type.pb.h"
#include "types/TypeID.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

struct DateLit;
struct DatetimeIntervalLit;
struct DatetimeLit;
struct YearMonthIntervalLit;

/** \addtogroup Types
 *  @{
 */

// TODO(chasseur) Adding a new Type and wiring it up in all the right places is
// a complicated process, as evidenced by the steps outlined below. A
// boilerplate template system to automate some of the process would be
// very desirable.

/**
 * @brief A type in Quickstep's type system. Each exact concrete Type is a
 *        singleton.
 *
 * @note The following is a brief (and not very detailed) guide to adding a new
 *       Type to Quickstep:
 *         1. Add a new TypeID and name for the Type in TypeID.hpp.
 *         2. Add a new TypeID to Type.proto, and if necessary define any
 *            extensions needed to serialize/deserialize the Type.
 *         3. Write a new subclass of Type to represent the Type. Concrete
 *            Types are Meyers-style singletons. See DatetimeType for a simple
 *            example of a Type that doesn't have any extra parameters, and
 *            see CharType for an example that has different instances for
 *            different lengths.
 *         4. Modify TypeFactory so that it can instantiate and deserialize
 *            your Type.
 *         5. Modify the SQL lexer (parser/SqlLexer.lpp) to add a token for
 *            your Type's name, and modify the SQL parser
 *            (parser/SqlParser.ypp) to add a case for your Type in the
 *            'data_type' node.
 *         6. Modify the SQL parser to parse literal values of your class as
 *            a ParseLiteralValue (there is WIP on string-parsing that will
 *            eliminate this requirement soon).
 *         7. (OPTIONAL) If your new Type is fixed-length and values are 8
 *            bytes or less, you may want to modify TypedValue to store it
 *            inline. This is purely a performance optimization and is not
 *            necessary for correctness.
 *         8. (OPTIONAL) If you want to be able to use existing operations from
 *            types/operations on your Type (e.g. if your type is "number-like"
 *            and you want to do arithmetic on it), modify the operations in
 *            question to take the new Type into account.
 *         8.a. If you wish your new Type to be sortable or compressable, you
 *              should modify LessComparison (or the BasicComparison base
 *              class) to deal with it.
 *         8.b. If you wish your new Type to be usable as a key in a HashTable,
 *              you should modify EqualComparison (or the BasicComparison base
 *              class) to deal with it. Note that the default hash function
 *              assumes that two values that have (some) differing bytes are
 *              never considered equal. If this is not the case, you must also
 *              modify TypedValue::getHash() to deal with your Type as a
 *              special case.
 **/
class Type {
 public:
  /**
   * @brief Categories of intermediate supertypes.
   **/
  enum SuperTypeID {
    kNumeric = 0,  // Fixed-length numeric types (Int, Long, Float, Double)
    kAsciiString,  // ASCII strings (Char, VarChar)
    kOther         // Others (Date, Datetime, DatetimeInterval, YearMonthInterval)
  };

  /**
   * @brief Virtual destructor.
   **/
  virtual ~Type() {
  }

  /**
   * @brief Generate a serialized Protocol Buffer representation of this Type.
   *
   * @return The serialized Protocol Buffer representation of this Type.
   **/
  virtual serialization::Type getProto() const;

  /**
   * @brief Determine what supertype this type belongs to.
   *
   * @return The ID of the supertype this type belongs to.
   **/
  inline SuperTypeID getSuperTypeID() const {
    return super_type_id_;
  }

  /**
   * @brief Determine the TypeID of this object.
   *
   * @return The ID of this type.
   **/
  inline TypeID getTypeID() const {
    return type_id_;
  }

  /**
   * @brief Determine whether this Type allows NULL values.
   *
   * @return Whether this Type allows NULLs.
   **/
  inline bool isNullable() const {
    return nullable_;
  }

  /**
   * @brief Get this Type's signature.
   *
   * @note The signature does not necessarily uniquely identify this Type. It
   *       merely provides some basic information for debugging that can be
   *       passed to TypedValue::isPlausibleInstanceOf().
   *
   * @return This Type's signature.
   **/
  inline TypeSignature getSignature() const {
    TypeSignature sig;
    sig.id = type_id_;
    sig.nullable = nullable_;
    switch (type_id_) {
      case kChar:
        sig.length = maximum_byte_length_;
        break;
      case kVarChar:
        sig.length = maximum_byte_length_ - 1;
        break;
      default:
        sig.length = 0;
    }
    return sig;
  }

  /**
   * @brief Get a nullable (but otherwise identical) version of this type.
   *
   * @return This Type's nullable counterpart (or this Type itself if already
   *         nullable).
   **/
  virtual const Type& getNullableVersion() const = 0;

  /**
   * @brief Get a non-nullable (but otherwise identical) version of this type.
   *
   * @return This Type's non-nullable counterpart (or this Type itself if
   *         already non-nullable).
   **/
  virtual const Type& getNonNullableVersion() const = 0;

  /**
   * @brief Determine whether data items of this type have variable
   *        byte-length.
   *
   * @return Whether this is a variable-length type.
   **/
  inline bool isVariableLength() const {
    return minimum_byte_length_ != maximum_byte_length_;
  }

  /**
   * @brief Determine the minimum number of bytes used by data items of this
   *        type.
   * @note If isVariableLength() is false, this is equivalent to
   *       maximumByteLength().
   *
   * @return The minimum number of bytes used by any data item of this type.
   **/
  inline std::size_t minimumByteLength() const {
    return minimum_byte_length_;
  }

  /**
   * @brief Determine the maximum number of bytes used by data items of this
   *        type.
   * @note If isVariableLength() is false, this is equivalent to
   *       minimumByteLength().
   *
   * @return The maximum number of bytes used by any data item of this type.
   **/
  inline std::size_t maximumByteLength() const {
    return maximum_byte_length_;
  }

  /**
   * @brief Estimate the average number of bytes used by data items of this
   *        type. These estimates may be used in query planning and determining
   *        StorageBlock layout.
   * @note If isVariableLength() is false, this is the same as
   *       minimumByteLength() and maximumByteLength().
   *
   * @return An estimate of the average number of bytes used by data items of
   *         this type.
   **/
  virtual std::size_t estimateAverageByteLength() const = 0;

  /**
   * @brief Determine whether this Type is exactly the same as another.
   * @note Because all exact types are singletons, a simple pointer
   *       equality-check is usable here, but this method should be used in
   *       case this behavior might change in the future.
   *
   * @param other The Type to check for equality.
   * @return Whether this Type and other are the same.
   **/
  bool equals(const Type &other) const {
    return (this == &other);
  }

  /**
   * @brief Determine whether data items of another Type can be converted to
   *        this type.
   * @note This method only determines whether coercion is possible (truncation
   *       or loss of precision may still occur). To determine if coercion is
   *       possible without loss of precision, use isSafelyCoercibleFrom()
   *       instead.
   * @note It is NOT possible to coerce a nullable type to a non-nullable type,
   *       even if coercion would otherwise be possible.
   *
   * @param original_type The original Type for coercion to this Type.
   * @return true if coercion is supported, false otherwise.
   **/
  virtual bool isCoercibleFrom(const Type &original_type) const = 0;

  /**
   * @brief Determine whether data items of another type can be coerced (used
   *        as or converted to) this Type without truncation or loss of
   *        precision.
   * @note It is NOT possible to coerce a nullable type to a non-nullable type,
   *       even if coercion would otherwise be possible.
   * @note Integer types are safely coercible to other integer or
   *       floating-poin types of equal or greater length.
   * @note Floating-point types are safely coercible to other floating-point
   *       types of equal or greater precision.
   * @note ASCII string types are safely coercible to other ASCII string types
   *       of equal or greater maximum length.
   * @warning Integer types are considered safely coercible to floating-point
   *          types of the same length, although for some large integer values
   *          this can lead to rounding off some of the lowest-magnitude binary
   *          digits.
   *
   * @param original_type The original Type for coercion to this Type.
   * @return true if coercion is supported, false otherwise.
   **/
  virtual bool isSafelyCoercibleFrom(const Type &original_type) const = 0;

  /**
   * @brief Determine whether data items of this type are always guaranteed to
   *        be valid instances of another type WITHOUT coercion. For example,
   *        a non-nullable type is subsumed by a nullable type that is
   *        otherwise identical, and a shorter VARCHAR is subsumed by a longer
   *        VARCHAR.
   * @note Because of issues of endianness, INT is not subsumed by LONG.
   *       Similarly, because a different number of bits are allocated to
   *       digits and mantissa in IEEE 754, FLOAT is not subsumed by DOUBLE.
   *
   * @param other The target Type to check whether this Type is strictly
   *        subsumed by.
   * @return true if this Type is subsumed by other, false otherwise.
   **/
  virtual bool isSubsumedBy(const Type &other) const {
    return equals(other) || getNullableVersion().equals(other);
  }

  /**
   * @brief Get the name of this Type.
   * @note Default version just returns the name from kTypeNames. Subclasses
   *       may override this to provided additional information, like lengths.
   *
   * @return The human-readable name of this Type (if the Type is nullable,
   *         this will include the NULL specifier).
   **/
  virtual std::string getName() const {
    return std::string(kTypeNames[getTypeID()]).append(nullable_ ? " NULL" : "");
  }

  /**
   * @brief Determine the maximum number of characters it takes to print a
   *        value of this Type.
   *
   * @note This counts only the characters needed to print non-NULL values of
   *       this type. NULL values are not directly printable by either
   *       Type::printValueToString() or Type::printValueToFile().
   *
   * @return The maximum number of characters used to print a value of this
   *         Type.
   **/
  virtual int getPrintWidth() const = 0;

  /**
   * @brief "Print" a value of this Type as a human-readable string.
   * @warning It is an error to call this with a NULL value. This method prints
   *          non-NULL values only.
   *
   * @param value A value of this Type.
   * @return The human-readable string representation of value.
   **/
  virtual std::string printValueToString(const TypedValue &value) const = 0;

  /**
   * @brief Print the human-readable string representation of a value of this
   *        type to a FILE stream.
   * @warning It is an error to call this with a NULL value. This method prints
   *          non-NULL values only.
   *
   * @param value A value of this Type.
   * @param file An open FILE stream to print to.
   * @param padding If nonzero, left-pad the printed value with spaces up to
   *        this length. If padding is less than the number of characters
   *        needed to print the value, then more than padding characters will
   *        be printed (see getPrintWidth() for information about how long a
   *        printed string may be).
   **/
  virtual void printValueToFile(const TypedValue &value,
                                FILE *file,
                                const int padding = 0) const = 0;

  /**
   * @brief Make a TypedValue of this Type.
   *
   * @param value_ptr An untyped pointer to the data which will be represented
   *        as a TypedValue.
   * @param value_size The size of the data at value_ptr in bytes. This may be
   *        omitted for numeric types, but must be explicitly specified for any
   *        other type. For Char and VarChar, this should include the
   *        terminating null character, if any.
   * @return A TypedValue representing the data at value_ptr.
   **/
  inline TypedValue makeValue(const void* value_ptr,
                              const std::size_t value_size = 0) const {
    DCHECK(value_ptr != nullptr);
    switch (type_id_) {
      case kInt:
        return TypedValue(*static_cast<const int*>(value_ptr));
      case kLong:
        return TypedValue(*static_cast<const std::int64_t*>(value_ptr));
      case kFloat:
        return TypedValue(*static_cast<const float*>(value_ptr));
      case kDouble:
        return TypedValue(*static_cast<const double*>(value_ptr));
      case kDate:
        return TypedValue(*static_cast<const DateLit*>(value_ptr));
      case kDatetime:
        return TypedValue(*static_cast<const DatetimeLit*>(value_ptr));
      case kDatetimeInterval:
        return TypedValue(*static_cast<const DatetimeIntervalLit*>(value_ptr));
      case kYearMonthInterval:
        return TypedValue(*static_cast<const YearMonthIntervalLit*>(value_ptr));
      case kNullType: {
        LOG(FATAL) << "Calling makeValue() to make a non-null value for "
                   << "NullType, which is impossible.";
      }
      default:
        DCHECK_NE(value_size, 0u);
        DCHECK_LE(value_size, maximum_byte_length_);
        return TypedValue(type_id_, value_ptr, value_size);
    }
  }

  /**
   * @brief Make a NULL TypedValue of this Type.
   *
   * @return A NULL TypedValue of this Type.
   **/
  inline TypedValue makeNullValue() const {
    DCHECK(nullable_);
    return TypedValue(type_id_);
  }

  /**
   * @brief Make a "zero" value of this Type.
   * @note For arithmetic types that can be freely added, "zero" means the
   *       additive identity. For non-addable Types, there is no zero value and
   *       calling this method is an error.
   *
   * @return A zero value of this Type.
   **/
  virtual TypedValue makeZeroValue() const {
    LOG(FATAL) << "Called makeZeroValue() on Type " << getName()
               << ", which does not have a defined zero value.";
  }

  /**
   * @brief Parse a value of this type from its human-readable string
   *        representation.
   *
   * @param value_string The human-readable string representation of a value of
   *        this type.
   * @param value Pointer to a TypedValue that will be overwritten with the
   *        parsed value.
   * @return true if value_string was successfully parsed and value was
   *         written. false if value_string was not in the correct format.
   **/
  virtual bool parseValueFromString(const std::string &value_string,
                                    TypedValue *value) const = 0;

  /**
   * @brief Coerce a value of another Type to this Type.
   * @warning It is an error to call this method with an original_type that can
   *          not be coerced to this Type. If in doubt, check
   *          isCoercibleFrom().
   * @note The base Type class provides a default implementation of this method
   *       that only allows coercing from the exact same Type or, if this Type
   *       is nullable, from the non-nullable version of a Type or the special
   *       NullType. Types that allow additional coercions should override this
   *       method.
   *
   * @param original_value The value to coerce to this Type.
   * @param original_type The Type that original_value belongs to.
   * @return A new TypedValue that represents original_value as an instance of
   *         this Type.
   **/
  virtual TypedValue coerceValue(const TypedValue &original_value,
                                 const Type &original_type) const;

 protected:
  Type(const SuperTypeID super_type_id,
       const TypeID type_id,
       const bool nullable,
       const std::size_t minimum_byte_length,
       const std::size_t maximum_byte_length)
      : super_type_id_(super_type_id),
        type_id_(type_id),
        nullable_(nullable),
        minimum_byte_length_(minimum_byte_length),
        maximum_byte_length_(maximum_byte_length) {
  }

  const SuperTypeID super_type_id_;
  const TypeID type_id_;
  const bool nullable_;
  const std::size_t minimum_byte_length_;
  const std::size_t maximum_byte_length_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Type);
};

/**
 * @brief A superclass for ASCII string types.
 **/
class AsciiStringSuperType : public Type {
 public:
  bool isCoercibleFrom(const Type &original_type) const override;

  /**
   * @brief Get the character-length of this string type.
   *
   * @return The maximum length of a string of this type.
   **/
  inline std::size_t getStringLength() const {
    return length_;
  }

 protected:
  AsciiStringSuperType(const TypeID type_id,
                       const bool nullable,
                       const std::size_t minimum_byte_length,
                       const std::size_t maximum_byte_length,
                       const std::size_t string_length)
      : Type(Type::kAsciiString, type_id, nullable, minimum_byte_length, maximum_byte_length),
        length_(string_length) {
  }

  const std::size_t length_;

 private:
  DISALLOW_COPY_AND_ASSIGN(AsciiStringSuperType);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_HPP_
