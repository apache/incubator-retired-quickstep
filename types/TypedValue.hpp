/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_TYPES_TYPED_VALUE_HPP_
#define QUICKSTEP_TYPES_TYPED_VALUE_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <functional>

#include "types/DatetimeLit.hpp"
#include "types/IntervalLit.hpp"
#include "types/TypeID.hpp"
#include "types/TypedValue.pb.h"
#include "types/port/strnlen.hpp"
#include "utility/Macros.hpp"

#include "third_party/farmhash/farmhash.h"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief A value with Type information attached. May be a literal which owns
 *        its own data, or a reference to data which is "owned" elsewhere (e.g.
 *        in a StorageBlock).
 *
 * @note std::memset may be used to zero-out memory that will hold one or more
 *       TypedValues. It will be as if those TypedValues were constructed with
 *       the default zero-argument constructor. HOWEVER, it is only safe to
 *       overwrite a TypedValue with memset/memcpy/memmove if
 *       ownsOutOfLineData() is false (otherwise memory leaks WILL occur).
 * @note std::memcpy and std::memmove are only safe to use with a TypedValue if
 *       ownsOutOfLineData() is false. Otherwise, double-frees may occur.
 **/
class TypedValue {
 public:
  /**
   * @brief Default constructor. Creates an int 0.
   **/
  TypedValue()
      : value_info_(0) {
    value_union_.long_value = 0;
  }

  /**
   * @brief Copy constructor. If copying from a literal, copies underlying
   *        data.
   **/
  TypedValue(const TypedValue &orig)
      : value_union_(orig.value_union_),
        value_info_(orig.value_info_) {
    if (ownsOutOfLineData()) {
      convertToLiteral();
    }
  }

  /**
   * @brief Move constructor. If moving from a literal that owns out-of-line
   *        data, the new TypedValue takes ownership of that data.
   **/
  TypedValue(TypedValue &&orig) noexcept
      : value_union_(orig.value_union_),
        value_info_(orig.value_info_) {
    orig.value_info_ = 0;
  }

  /**
   * @brief Constructor for a literal value of IntType.
   **/
  explicit TypedValue(const int literal_int)
      : value_info_(static_cast<std::uint64_t>(kInt)) {
    // Zero-out all bytes in the union for fastEqualCheck().
    value_union_.long_value = 0;
    value_union_.int_value = literal_int;
  }

  /**
   * @brief Constructor for a literal value of LongType.
   **/
  explicit TypedValue(const std::int64_t literal_long)
      : value_info_(static_cast<std::uint64_t>(kLong)) {
    value_union_.long_value = literal_long;
  }

  /**
   * @brief Constructor for a literal value of FloatType.
   **/
  explicit TypedValue(const float literal_float)
      : value_info_(static_cast<std::uint64_t>(kFloat)) {
    // Zero-out all bytes in the union for fastEqualCheck().
    value_union_.long_value = 0;
    value_union_.float_value = literal_float;
  }

  /**
   * @brief Constructor for a literal value of DoubleType.
   **/
  explicit TypedValue(const double literal_double)
      : value_info_(static_cast<std::uint64_t>(kDouble)) {
    value_union_.double_value = literal_double;
  }

  /**
   * @brief Constructor for a literal value of DatetimeType.
   **/
  explicit TypedValue(const DatetimeLit literal_datetime)
      : value_info_(static_cast<std::uint64_t>(kDatetime)) {
    value_union_.datetime_value = literal_datetime;
  }

  /**
   * @brief Constructor for a literal value of DatetimeIntervalType.
   **/
  explicit TypedValue(const DatetimeIntervalLit literal_datetime_interval)
      : value_info_(static_cast<std::uint64_t>(kDatetimeInterval)) {
    value_union_.datetime_interval_value = literal_datetime_interval;
  }

  /**
   * @brief Constructor for a literal value of YearMonthIntervalType.
   **/
  explicit TypedValue(const YearMonthIntervalLit literal_year_month_interval)
      : value_info_(static_cast<std::uint64_t>(kYearMonthInterval)) {
    value_union_.year_month_interval_value = literal_year_month_interval;
  }

  /**
   * @brief Constructor for a reference to non-numeric data.
   *
   * @param type_id The ID of the value's Type.
   * @param value_ptr A pointer to the underlying data to represent.
   * @param value_size The number of bytes of data at value_ptr.
   *
   * @note To make a literal value, call ensureNotReference() on the TypedValue
   *       created by this constructor.
   **/
  TypedValue(const TypeID type_id,
             const void* value_ptr,
             const std::size_t value_size)
      : value_info_(static_cast<std::uint64_t>(type_id) | (value_size << kSizeShift)) {
    DCHECK(value_ptr != nullptr);
    DCHECK_EQ(value_size, getDataSize());
    value_union_.out_of_line_data = value_ptr;
  }

  /**
   * @brief Constructor for a NULL value of the specified Type.
   *
   * @param type_id The ID of the value's Type.
   **/
  explicit TypedValue(const TypeID type_id)
      : value_info_(static_cast<std::uint64_t>(type_id) | kNullIndicatorMask) {
  }

  /**
   * @brief Destructor.
   **/
  ~TypedValue() {
    if (ownsOutOfLineData()) {
      std::free(const_cast<void*>(value_union_.out_of_line_data));
    }
  }

  /**
   * @brief Assignment operator. If assigning from a literal, copies underlying
   *        data.
   **/
  TypedValue& operator=(const TypedValue &rhs) {
    if (this != &rhs) {
      if (ownsOutOfLineData()) {
        std::free(const_cast<void*>(value_union_.out_of_line_data));
      }

      value_union_ = rhs.value_union_;
      value_info_ = rhs.value_info_;
      if (ownsOutOfLineData()) {
        convertToLiteral();
      }
    }

    return *this;
  }

  /**
   * @brief Move assignment operator. If moving from a literal that owns
   *        out-of-line data, this TypedValue takes ownership of that data.
   **/
  TypedValue& operator=(TypedValue &&rhs) {
    if (this != &rhs) {
      if (ownsOutOfLineData()) {
        std::free(const_cast<void*>(value_union_.out_of_line_data));
      }

      value_union_ = rhs.value_union_;
      value_info_ = rhs.value_info_;
      rhs.value_info_ = 0;
    }

    return *this;
  }

  /**
   * @brief Create a new literal TypedValue with pre-allocated out-of-line
   *        data.
   * @warning The memory at value_ptr must be allocated with malloc() or
   *          similar (e.g. malloc_with_alignment()), NOT with the new operator
   *          or mmap().
   *
   * @param type_id The ID of the value's Type.
   * @param value_ptr A pointer to the underlying data to represent. The new
   *        TypedValue will take ownership of this memory.
   * @param value_size The number of bytes of data at value_ptr.
   **/
  static TypedValue CreateWithOwnedData(const TypeID type_id,
                                        void *value_ptr,
                                        const std::size_t value_size) {
    TypedValue val(type_id, value_ptr, value_size);
    val.value_info_ |= kOwnershipMask;
    return val;
  }

  /**
   * @brief Determine if TypedValue will use an in-line data representation
   *        for instances of a given type.
   *
   * @param type_id The ID of the type to check.
   * @return Whether TypedValue represents instances of the type denoted by
   *         type_id in-line.
   **/
  static bool RepresentedInline(const TypeID type_id) {
    switch (type_id) {
      case kInt:
      case kLong:
      case kFloat:
      case kDouble:
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
      case kNullType:
        return true;
      default:
        return false;
    }
  }

  /**
   * @brief Clears this value, making it equivalent to a default-constructed
   *        TypedValue (int 0). Frees any owned out-of-line data.
   **/
  inline void clear() {
    if (ownsOutOfLineData()) {
      std::free(const_cast<void*>(value_union_.out_of_line_data));
    }

    value_info_ = 0;
    value_union_.long_value = 0;
  }

  /**
   * @brief Determine if this value is NULL.
   *
   * @return Whether this is a NULL value.
   **/
  inline bool isNull() const {
    return value_info_ & kNullIndicatorMask;
  }

  /**
   * @brief Determine if this value is a reference (i.e. it points to
   *        out-of-line data which it does not own).
   *
   * @return Whether this is a reference.
   **/
  inline bool isReference() const {
    return ((value_info_ & kSizeBitsMask) && !(value_info_ & kOwnershipMask));
  }

  /**
   * @brief Determine if this value points to out-of-line data which it owns.
   * @note This is not simply the opposite of isReference(), because a value
   *       may be a literal (i.e. not a reference), but still not have any
   *       out-of-line data which it owns. This is the case for all NULL
   *       values and all numeric values.
   *
   * @returns Whether this value owns out-of-line data.
   **/
  inline bool ownsOutOfLineData() const {
    return ((value_info_ & kOwnershipMask) && (value_info_ & kSizeBitsMask));
  }

  /**
   * @brief Get the TypeID of this value.
   *
   * @return This value's TypeID.
   **/
  inline TypeID getTypeID() const {
    return static_cast<TypeID>(value_info_ & kTypeIDMask);
  }

  /**
   * @brief Get the size of this value's data.
   * @warning Only usable with non-NULL values.
   *
   * @return The size, in bytes, of this value's data.
   **/
  inline std::size_t getDataSize() const {
    DCHECK(!isNull());
    switch (getTypeID()) {
      case kInt:
      case kFloat:
        return sizeof(int);
      case kLong:
      case kDouble:
        return sizeof(std::int64_t);
      case kDatetime:
        return sizeof(DatetimeLit);
      case kDatetimeInterval:
        return sizeof(DatetimeIntervalLit);
      case kYearMonthInterval:
        return sizeof(YearMonthIntervalLit);
      default:
        return value_info_ >> kSizeShift;
    }
  }

  /**
   * @brief Determine if this value is plausibly a valid instance of a given
   *        Type.
   * @warning This is NOT authoritative and in some circumstances can give
   *          false positives. It only checks TypeID, nullability, and length,
   *          but does not know about additional properties of Types that are
   *          not available to a TypedValue (e.g. the precision of a DECIMAL
   *          type). This is intended mainly as a sanity check for debugging
   *          assertions.
   * @note For variable-length types, this assumes that a TypedValue is a valid
   *       instance of the type if it has the same TypeID and its size is
   *       between type.minimumByteLength() and type.maximumByteLength(),
   *       inclusive. This is true for VarChar, but may need to be modified if
   *       other variable-length types are added for which this is not true.
   *
   * @param type The signature of the Type to check.
   * @return Whether this value is a valid instance of type.
   **/
  bool isPlausibleInstanceOf(const TypeSignature type) const;

  /**
   * @brief Ensure that this TypedValue is not a reference (i.e. force it to
   *        be a literal), copying out-of-line data if necessary.
   *
   * @return A reference to this TypedValue (this a fluent modifier).
   **/
  inline TypedValue& ensureNotReference() {
    if (isReference()) {
      convertToLiteral();
    }
    return *this;
  }

  /**
   * @brief Make a reference pointing to the data represented by this
   *        TypedValue.
   * @note If this is a literal which is represented in-line, then a literal
   *       TypedValue will actually be returned, which is just as efficient as
   *       a reference and requires less indirection.
   *
   * @return A reference to this TypedValue's data.
   **/
  inline TypedValue makeReferenceToThis() const {
    if (ownsOutOfLineData()) {
      return TypedValue(getTypeID(), value_union_.out_of_line_data, getDataSize());
    } else {
      return *this;
    }
  }

  /**
   * @brief Get the literal value represented by this TypedValue,
   *        including four numeric types (int, std::int64_t, float, and double) and
   *        three date types (DatetimeLit, DatetimeIntervalLit, and YearMonthIntervalLit).
   * @warning This must only be used with the exactly correct underlying
   *          C++ type of a literal value. For numeric types,
   *          see also getNumericCoerced().
   *
   * @return The underlying literal value represented by this TypedValue.
   **/
  template <typename LiteralType>
  LiteralType getLiteral() const;

  /**
   * @brief Get the out-of-line data which this TypedValue points to.
   * @warning This must only be used with a TypedValue which has out-of-line
   *          data (i.e. not NULL, and not a numeric nor a date type
   *          which is represented in-line).
   *
   * @return The out-of-line data this TypedValue points to.
   **/
  inline const void* getOutOfLineData() const {
    DCHECK(!(getTypeID() == kInt
                   || getTypeID() == kLong
                   || getTypeID() == kFloat
                   || getTypeID() == kDouble
                   || getTypeID() == kDatetime
                   || getTypeID() == kDatetimeInterval
                   || getTypeID() == kYearMonthInterval));
    DCHECK(!isNull());
    return value_union_.out_of_line_data;
  }

  /**
   * @brief Get the length of the ASCII string this TypedValue represents,
   *        not counting a null terminator character, if any (same behavior
   *        as strlen).
   * @warning This must only be used with string types (i.e. Char and VarChar).
   *
   * @return The length of the string represented by this TypedValue.
   **/
  inline std::size_t getAsciiStringLength() const {
    DCHECK(getTypeID() == kChar || getTypeID() == kVarChar);
    DCHECK(!isNull());
    if (getTypeID() == kChar) {
      return strnlen(static_cast<const char*>(value_union_.out_of_line_data),
                     getDataSize());
    } else {
      return getDataSize() - 1;
    }
  }

  /**
   * @brief Get a pointer to the underlying data represented by this TypedValue.
   * @warning This must not be used with null values.
   *
   * @return A raw pointer to this TypedValue's data.
   **/
  inline const void* getDataPtr() const {
    DCHECK(!isNull());
    switch (getTypeID()) {
      case kInt:
        return &(value_union_.int_value);
      case kLong:
        return &(value_union_.long_value);
      case kFloat:
        return &(value_union_.float_value);
      case kDouble:
        return &(value_union_.double_value);
      case kDatetime:
        return &(value_union_.datetime_value);
      case kDatetimeInterval:
        return &(value_union_.datetime_interval_value);
      case kYearMonthInterval:
        return &(value_union_.year_month_interval_value);
      default:
        return value_union_.out_of_line_data;
    }
  }

  /**
   * @brief Copy this value's underlying data to the specified memory location.
   * @warning This must not be used with null values.
   *
   * @param destination The memory location where this value's underlying data
   *        should be copied. At least getDataSize() bytes of data must be
   *        available to write at destination.
   **/
  inline void copyInto(void *destination) const {
    DCHECK(!isNull());
    switch (getTypeID()) {
      case kInt:
        *static_cast<int*>(destination) = value_union_.int_value;
        return;
      case kLong:
        *static_cast<std::int64_t*>(destination) = value_union_.long_value;
        return;
      case kFloat:
        *static_cast<float*>(destination) = value_union_.float_value;
        return;
      case kDouble:
        *static_cast<double*>(destination) = value_union_.double_value;
        return;
      case kDatetime:
        *static_cast<DatetimeLit*>(destination) = value_union_.datetime_value;
        return;
      case kDatetimeInterval:
        *static_cast<DatetimeIntervalLit*>(destination) = value_union_.datetime_interval_value;
        return;
      case kYearMonthInterval:
        *static_cast<YearMonthIntervalLit*>(destination) = value_union_.year_month_interval_value;
        return;
      default:
        std::memcpy(destination,
                    value_union_.out_of_line_data,
                    value_info_ >> kSizeShift);
        return;
    }
  }

  /**
   * @brief Get a hash of this TypedValue.
   * @note The std::hash function is used for numeric values, and FarmHash is
   *       used for all other values.
   * @note String values which are the same will hash to the same value, even
   *       if they are different types (i.e. Char or VarChar with different
   *       width parameters). This is intentional.
   * @warning This must not be used with null values.
   *
   * @return A hash of this TypedValue.
   **/
  inline std::size_t getHash() const {
    DCHECK(!isNull());
    switch (getTypeID()) {
      case kInt: {
        return std::hash<int>()(value_union_.int_value);
      }
      case kLong: {
        return std::hash<std::int64_t>()(value_union_.long_value);
      }
      case kFloat: {
        return std::hash<float>()(value_union_.float_value);
      }
      case kDouble: {
        return std::hash<double>()(value_union_.double_value);
      }
      case kDatetime: {
        return std::hash<std::int64_t>()(value_union_.datetime_value.ticks);
      }
      case kDatetimeInterval: {
        return std::hash<std::int64_t>()(value_union_.datetime_interval_value.interval_ticks);
      }
      case kYearMonthInterval: {
        return std::hash<std::int64_t>()(value_union_.year_month_interval_value.months);
      }
      case kChar:
      case kVarChar: {
        // Don't hash a null-terminator or any bytes that follow it.
        std::size_t len = getAsciiStringLength();
        return util::Hash(static_cast<const char*>(value_union_.out_of_line_data),
                          len);
      }
      default:
        return util::Hash(static_cast<const char*>(value_union_.out_of_line_data),
                          getDataSize());
    }
  }

  /**
   * @brief Quickly check if two TypedValues of the same Type are equal.
   * @note This is intended mainly for use in hash tables to quickly check if
   *       two TypedValues that return equal values for getHash() are actually
   *       equal to each other. EqualComparison should be used instead for
   *       general use.
   * @warning This must not be used with null values.
   * @warning Must only be used with two TypedValues of the same Type.
   *
   * @param other Another TypedValue to check for equality with this one.
   * @return Whether this TypedValue is equal to other.
   **/
  inline bool fastEqualCheck(const TypedValue &other) const {
    DCHECK(!isNull());
    DCHECK(!other.isNull());
    DCHECK_EQ(getTypeID(), other.getTypeID());
    switch (getTypeID()) {
      case kInt:
      case kLong:
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
        return value_union_.long_value == other.value_union_.long_value;
      // NOTE(chasseur): Signed zero and subnormals mean bitwise equality isn't
      // always suited for floats.
      case kFloat:
        return value_union_.float_value == other.value_union_.float_value;
      case kDouble:
        return value_union_.double_value == other.value_union_.double_value;
      case kChar:
      case kVarChar: {
        const std::size_t len = getAsciiStringLength();
        const std::size_t other_len = other.getAsciiStringLength();
        return ((len == other_len)
                && (std::memcmp(static_cast<const char*>(value_union_.out_of_line_data),
                                static_cast<const char*>(other.value_union_.out_of_line_data),
                                len) == 0));
      }
      default: {
        const std::size_t len = getDataSize();
        const std::size_t other_len = other.getDataSize();
        return ((len == other_len)
                && (std::memcmp(value_union_.out_of_line_data,
                                other.value_union_.out_of_line_data,
                                len) == 0));
      }
    }
  }

  /**
   * @brief Generate a serialized Protocol Buffer representation
   *        of this TypedValue.
   *
   * @return The serialized Protocol Buffer representation of this TypedValue.
   **/
  serialization::TypedValue getProto() const;

  /**
   * @brief Generate a TypedValue from that TypedValue's serialized
   *        Protocol Buffer representation.
   *
   * @param proto A serialized Protocol Buffer representation of a TypedValue,
   *        originally generated by getProto().
   * @return The TypedValue described by proto.
   **/
  static TypedValue ReconstructFromProto(const serialization::TypedValue &proto);

  /**
   * @brief Check whether a serialization::TypedValue is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a TypedValue,
   *        originally generated by getProto().
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::TypedValue &proto);

 private:
  friend class CharType;
  friend class VarCharType;

  static const std::uint64_t kTypeIDMask = static_cast<std::uint64_t>(0x3F);
  static const std::uint64_t kNullIndicatorMask = static_cast<std::uint64_t>(0x40);
  static const std::uint64_t kOwnershipMask = static_cast<std::uint64_t>(0x80);
  static const std::uint64_t kNonSizeBitsMask = static_cast<std::uint64_t>(0xFF);
  static const std::uint64_t kSizeBitsMask = ~kNonSizeBitsMask;
  static const unsigned kSizeShift = 8;

  TypedValue(const TypeID type_id,
             void* value_ptr,
             const std::size_t value_size,
             bool owned)
      : value_info_(static_cast<std::uint64_t>(type_id)
                    | (owned ? kOwnershipMask : 0)
                    | (value_size << kSizeShift)) {
    DCHECK(value_ptr != nullptr);
    DCHECK_EQ(value_size, getDataSize());
    value_union_.out_of_line_data = value_ptr;
  }

  inline void markType(const TypeID type_id) {
    DCHECK_EQ(static_cast<std::uint64_t>(type_id),
              static_cast<std::uint64_t>(type_id) & kTypeIDMask);
    value_info_ = (value_info_ & ~kTypeIDMask) | static_cast<std::uint64_t>(type_id);
  }

  inline void convertToLiteral() {
    const std::size_t data_size = getDataSize();
    void* data_copy = std::malloc(data_size);
    std::memcpy(data_copy, value_union_.out_of_line_data, data_size);
    value_union_.out_of_line_data = data_copy;

    value_info_ |= kOwnershipMask;
  }

  union ValueUnion {
    int int_value;
    std::int64_t long_value;
    float float_value;
    double double_value;
    const void* out_of_line_data;
    DatetimeLit datetime_value;
    DatetimeIntervalLit datetime_interval_value;
    YearMonthIntervalLit year_month_interval_value;
  } value_union_;

  // Lowest-order 6 bits are the TypeID, the 7th-lowest bit is the NULL
  // indicator, and the 8th-lowest bit indicates whether
  // '*value_union_.out_of_line_data' is owned by this TypedValue. The
  // higher-order 7 bytes (shifted right) are the size of out-of-line data.
  std::uint64_t value_info_;
};

/** @} */

// Explicit specializations of getLiteral().
template <>
inline int TypedValue::getLiteral<int>() const {
  DCHECK_EQ(kInt, getTypeID());
  DCHECK(!isNull());
  return value_union_.int_value;
}

template <>
inline std::int64_t TypedValue::getLiteral<std::int64_t>() const {
  DCHECK_EQ(kLong, getTypeID());
  DCHECK(!isNull());
  return value_union_.long_value;
}

template <>
inline float TypedValue::getLiteral<float>() const {
  DCHECK_EQ(kFloat, getTypeID());
  DCHECK(!isNull());
  return value_union_.float_value;
}

template <>
inline double TypedValue::getLiteral<double>() const {
  DCHECK_EQ(kDouble, getTypeID());
  DCHECK(!isNull());
  return value_union_.double_value;
}

template <>
inline DatetimeLit TypedValue::getLiteral<DatetimeLit>() const {
  DCHECK_EQ(kDatetime, getTypeID());
  DCHECK(!isNull());
  return value_union_.datetime_value;
}

template <>
inline DatetimeIntervalLit TypedValue::getLiteral<DatetimeIntervalLit>() const {
  DCHECK_EQ(kDatetimeInterval, getTypeID());
  DCHECK(!isNull());
  return value_union_.datetime_interval_value;
}

template <>
inline YearMonthIntervalLit TypedValue::getLiteral<YearMonthIntervalLit>() const {
  DCHECK_EQ(kYearMonthInterval, getTypeID());
  DCHECK(!isNull());
  return value_union_.year_month_interval_value;
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPED_VALUE_HPP_
