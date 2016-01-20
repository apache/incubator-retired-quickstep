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
#include "utility/HashPair.hpp"
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
    value_union_.hash64 = 0;
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
    // Zero-out all bytes in the union for getHash() and fastEqualCheck().
    value_union_.hash64 = 0;
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
    // Zero-out all bytes in the union for getHash() and fastEqualCheck().
    value_union_.hash64 = 0;

    // Canonicalize negative zero.
    value_union_.float_value = literal_float == 0 ? 0 : literal_float;
  }

  /**
   * @brief Constructor for a literal value of DoubleType.
   **/
  explicit TypedValue(const double literal_double)
      : value_info_(static_cast<std::uint64_t>(kDouble)) {
    // Canonicalize negative zero.
    value_union_.double_value = literal_double == 0 ? 0 : literal_double;
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
    // Although 'value_union_' should never be accessed when the null bit is
    // set, we zero it out anyway to prevent a false positive warning when the
    // compiler may complain about 'value_union_' being read without being
    // initialized.
    value_union_.long_value = 0;
  }

  /**
   * @brief Constructor for reversing a hash function, recovering the original
   *        value.
   *
   * @warning This is only usable for Types that have a reversible hash
   *          function. Check HashIsReversible() if unsure.
   *
   * @param type_id The ID of the value's Type.
   * @param hash The value's hash.
   **/
  inline TypedValue(const TypeID type_id, const std::size_t hash);

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
   * @brief Determine if the hash function getHash() is reversible for
   *        instances of a given type.
   *
   * @param type_id The ID of the type to check.
   * @return Whether the hash function getHash() is reversible for instances of
   *         the type denoted by type_id.
   **/
  static bool HashIsReversible(const TypeID type_id) {
    switch (type_id) {
      case kInt:
      case kFloat:
        return sizeof(value_union_.int_value) <= sizeof(std::size_t);
      case kLong:
      case kDouble:
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
        return sizeof(value_union_) <= sizeof(std::size_t);
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
    value_union_.hash64 = 0;
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
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
        return sizeof(value_union_);
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
    if (value_info_ & kSizeBitsMask) {
      // Data is out-of-line.
      DCHECK(!RepresentedInline(getTypeID()));
      return value_union_.out_of_line_data;
    } else {
       // According to the C standard, a pointer to a union points to each of
       // its members and vice versa (i.e. every non-bitfield member of a union
       // is always aligned at the front of the union itself).
      DCHECK(RepresentedInline(getTypeID()));
      return &value_union_;
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
    const std::size_t out_of_line_size = value_info_ >> kSizeShift;
    if (out_of_line_size != 0) {
      std::memcpy(destination,
                  value_union_.out_of_line_data,
                  value_info_ >> kSizeShift);
    } else {
      switch (getTypeID()) {
        case kInt:
        case kFloat:
          // 4 bytes byte-for-byte copy.
          *static_cast<int*>(destination) = value_union_.int_value;
          break;
        default:
          // 8 bytes byte-for-byte copy.
          *static_cast<ValueUnion*>(destination) = value_union_;
      }
    }
  }

  /**
   * @brief Get a hash of this TypedValue.
   * @note A trivial bit-for-bit identity hash is used for inline literal
   *       values (although on 32-bit systems bits from 64-bit wide scalars
   *       will be mixed by CombineHashes()), and FarmHash is used for all
   *       other values.
   * @note String values which are the same will hash to the same value, even
   *       if they are different types (i.e. Char or VarChar with different
   *       width parameters). This is intentional.
   * @warning This must not be used with null values.
   *
   * @return A hash of this TypedValue.
   **/
  inline std::size_t getHash() const {
    switch (getTypeID()) {
      case kInt:
      case kLong:
      case kFloat:
      case kDouble:
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
        return getHashScalarLiteral();
      case kChar:
      case kVarChar:
        return getHashAsciiString();
      default:
        return getHashOutOfLineNonString();
    }
  }

  /**
   * @brief Simplified version of getHash() that is only for scalar literals
   *        that are represented inline.
   *
   * @note A trivial bit-for-bit identity hash is used for inline scalar
   *       literals (although on 32-bit systems bits from 64-bit wide scalars
   *       will be mixed by CombineHashes()).
   * @warning It is an error to call this for a TypedValue where
   *          RepresentedInline() is false. If in doubt, use the generic
   *          getHash() method instead.
   * @warning This must not be used with null values.
   *
   * @return A hash of this TypedValue.
   **/
  inline std::size_t getHashScalarLiteral() const;

  /**
   * @brief Simplified version of getHash() that is only for ASCII string types
   *        (i.e. instances of CharType or VarCharType).
   *
   * @note FarmHash is the hash function used for strings.
   * @note Strings which are the same according to lexicographical comparison
   *       will hash to the same value, even if they are different types (e.g.
   *       CharType vs. VarCharType and/or string types with different width
   *       parameters). This is intentional.
   * @warning It is an error to call this for a TypedValue where getTypeID() is
   *          not kChar or kVarChar. If in doubt, use the generic getHash()
   *          method instead.
   * @warning This must not be used with null values.
   *
   * @return A hash of this TypedValue.
   **/
  inline std::size_t getHashAsciiString() const {
    DCHECK(!isNull());
    DCHECK(getTypeID() == kChar || getTypeID() == kVarChar);

    // Don't hash a null-terminator or any bytes that follow it.
    return util::Hash(static_cast<const char*>(value_union_.out_of_line_data),
                      getAsciiStringLength());
  }

  /**
   * @brief Simplified version of getHash() that is only for types that are
   *        represented out-of-line but are NOT ASCII strings.
   *
   * @note FarmHash is the hash function used for types represented
   *       out-of-line.
   * @warning It is an error to call this for a TypedValue where
   *          RepresentedInline() is true, or if getTypeID() is kChar or
   *          kVarChar. If in doubt, use the generic getHash() instead.
   * @warning This must not be used with null values.
   *
   * @return A hash of this TypedValue.
   **/
  inline std::size_t getHashOutOfLineNonString() const {
    DCHECK(!isNull());
    DCHECK(!RepresentedInline(getTypeID())
           && !(getTypeID() == kChar || getTypeID() == kVarChar));
    return util::Hash(static_cast<const char*>(value_union_.out_of_line_data),
                      getDataSize());
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
      case kFloat:
      case kDouble:
      case kDatetime:
      case kDatetimeInterval:
      case kYearMonthInterval:
        // NOTE(chasseur): We simply do a byte-for-byte equality check of
        // 'value_union_' for types that are represented inline. For types that
        // are effectively 64-bit integers (LONG, DATETIME, and both INTERVAL
        // types) this is exactly the same as normal integer equality. For
        // types shorter than 64 bits (INT and FLOAT) we zero-out the union
        // when constructing the TypedValue, so the non-value bytes of the
        // union will always compare the same. Finally, for floating-point
        // types (FLOAT and DOUBLE), we always convert negative zero to
        // ordinary zero so that bitwise comparison will give correct results
        // for exact equality (although the usual caveats about rounding errors
        // for floating-point arithmetic still apply).
        return value_union_.hash64 == other.value_union_.hash64;
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

  // Templated helper method for getHashScalarLiteral(). Has implementations
  // for 64-bit systems (always the trivial identity hash) and 32-bit systems
  // (the trivial identity hash for scalars that are themselves 32 bits like
  // INT and FLOAT, otherwise CombineHashes() applied to the lower and upper
  // 4 bytes of 'value_union_').
  template <bool size_t_64bit>
  inline std::size_t getHashScalarLiteralImpl() const;

  // Helper method for hash-reversing constructor.
  template <bool size_t_64bit>
  inline void reverseHash(const std::size_t hash);

  union ValueUnion {
    int int_value;
    std::int64_t long_value;
    float float_value;
    double double_value;
    const void* out_of_line_data;
    DatetimeLit datetime_value;
    DatetimeIntervalLit datetime_interval_value;
    YearMonthIntervalLit year_month_interval_value;

    // Used to interpret the bytes of any of the inline fields above as a
    // 64-bit integer, providing a trivial reversible hash function.
    std::uint64_t hash64;

    // For 32-bit systems, this struct splits the 8 bytes of the ValueUnion
    // into 2 4-byte fields that we can use CombineHashes() on.
    struct Hash32 {
      std::uint32_t a;
      std::uint32_t b;
    } hash32;
  } value_union_;

  // Some static asserts that guarantee certain assumptions about ValueUnion
  // hold.
  static_assert(sizeof(ValueUnion) == sizeof(std::uint64_t),
                "TypedValue::ValueUnion must fit in 64 bits.");

  static_assert(sizeof(ValueUnion) == sizeof(ValueUnion::Hash32),
                "TypedValue::ValueUnion::Hash32 must cover all of ValueUnion.");

  static_assert(sizeof(ValueUnion) == sizeof(std::int64_t)
                    && sizeof(ValueUnion) == sizeof(double)
                    && sizeof(ValueUnion) == sizeof(DatetimeLit)
                    && sizeof(ValueUnion) == sizeof(DatetimeIntervalLit)
                    && sizeof(ValueUnion) == sizeof(YearMonthIntervalLit),
                "All inline fields of TypedValue::ValueUnion except for "
                "int_value and float_value should be 64 bits.");

  static_assert(sizeof(int) == 4 && sizeof(float) == 4,
                "TypedValue::ValueUnion::int_value and "
                "TypedValue::ValueUnion::float_value should both be 32 bits.");

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

// Explicit specializations of getHashScalarLiteralImpl().

// 32-bit size_t.
template <>
inline std::size_t TypedValue::getHashScalarLiteralImpl<false>() const {
  switch (getTypeID()) {
    case kInt:
    case kFloat:
      return value_union_.hash32.a;
    default:
      return CombineHashes(value_union_.hash32.a, value_union_.hash32.b);
  }
}

// 64-bit size_t.
template <>
inline std::size_t TypedValue::getHashScalarLiteralImpl<true>() const {
  return value_union_.hash64;
}

// Implementation of TypedValue::getHashScalarLiteral() is written out-of-line
// here because instantiations of TypedValue::getHashScalarLiteralImpl() must
// come after the explicit specializations above.
inline std::size_t TypedValue::getHashScalarLiteral() const {
  DCHECK(!isNull());
  DCHECK(RepresentedInline(getTypeID()));
  return getHashScalarLiteralImpl<sizeof(std::size_t) == sizeof(std::uint64_t)>();
}

// Explicit specializations of reverseHash().

// 32-bit size_t.
template <>
inline void TypedValue::reverseHash<false>(const std::size_t hash) {
  value_union_.hash32.a = hash;
  value_union_.hash32.b = 0;
}

// 64-bit size_t.
template <>
inline void TypedValue::reverseHash<true>(const std::size_t hash) {
  value_union_.hash64 = hash;
}

// Implementation of hash-reversing constructor is written out-of-line here
// because instantiations of TypedValue::reverseHash() must come after the
// explicit specializations above.
inline TypedValue::TypedValue(const TypeID type_id,
                              const std::size_t hash)
    : value_info_(static_cast<std::uint64_t>(type_id)) {
  DCHECK(HashIsReversible(type_id));
  reverseHash<sizeof(std::size_t) == sizeof(std::uint64_t)>(hash);
}

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPED_VALUE_HPP_
