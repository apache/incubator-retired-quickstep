/**
 *   Licensed to the Apache Software Foundation (ASF) under one
 *   or more contributor license agreements.  See the NOTICE file
 *   distributed with this work for additional information
 *   regarding copyright ownership.  The ASF licenses this file
 *   to you under the Apache License, Version 2.0 (the
 *   "License"); you may not use this file except in compliance
 *   with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing,
 *   software distributed under the License is distributed on an
 *   "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 *   KIND, either express or implied.  See the License for the
 *   specific language governing permissions and limitations
 *   under the License.
 *   limitations under the License.
 **/

#ifndef QUICKSTEP_TYPES_DECIMAL_LIT_HPP_
#define QUICKSTEP_TYPES_DECIMAL_LIT_HPP_

#include <cstdint>
#include <cmath>

namespace quickstep {

/** \addtogroup Type
 *  @{
 */

/**
 * @brief A literal representing fixed-precision decimal.
 **/
struct DecimalLit {
  /**
   * @brief Underlying data type.
   **/
  typedef std::int64_t data_type;

  DecimalLit() = default;

  explicit DecimalLit(const int value)
    : data_(static_cast<data_type>(value * kMaxFractionInt)) {
  }

  explicit DecimalLit(const std::int64_t value)
    : data_(static_cast<data_type>(value * kMaxFractionInt)) {
  }

  explicit DecimalLit(const float value)
    : data_(static_cast<data_type>(value * kMaxFractionInt)) {
  }

  explicit DecimalLit(const double value)
    : data_(static_cast<data_type>(value * kMaxFractionInt)) {
  }

  inline explicit operator int() const {
    return static_cast<int>(getIntegerPart());
  }

  inline explicit operator std::int64_t() const {
    return static_cast<std::int64_t>(getIntegerPart());
  }

  inline explicit operator float() const {
    return static_cast<float>(data_) / kMaxFractionInt;
  }

  inline explicit operator double() const {
    return static_cast<double>(data_) / kMaxFractionInt;
  }

  data_type data_;

  /**
   * @brief Number of decimals after point.
   **/
  static constexpr std::int64_t kPrecisionWidth = 2;

  /**
   * @brief Normalization factor between Decimal type and floating point types.
   *        It is always equal to pow(10, kPrecisionWidth).
   **/
  static constexpr std::int64_t kMaxFractionInt = 100;

  /**
   * @brief Getter for the fractional part of the Decimal type.
   *
   * @return Fractional part of the Decimal number as unsigned.
   **/
  inline std::uint64_t getFractionalPart() const {
    return static_cast<std::uint64_t>(static_cast<std::uint64_t>(std::abs(data_)) % kMaxFractionInt);
  }

  /**
   * @brief Getter for the integer part of the Decimal type.
   *
   * @return Integer part of the Decimal number as signed.
   **/
  inline std::int64_t getIntegerPart() const {
    return static_cast<std::int64_t>(data_ / kMaxFractionInt);
  }

  /**
   * @brief Operator overloading for "less than".
   *
   * @param other Other DecimalLit to be compared.
   * @return True if this is less than other,
   *         false otherwise.
   **/
  inline bool operator<(const DecimalLit& other) const {
    return data_ < other.data_;
  }

  /**
   * @brief Operator overloading for "greater than".
   *
   * @param other Other DecimalLit to be compared.
   * @return True if this is greater than other,
   *         false otherwise.
   **/
  inline bool operator>(const DecimalLit& other) const {
    return data_ > other.data_;
  }

  /**
   * @brief Operator overloading for "less than or equal to".
   *
   * @param other Other DecimalLit to be compared.
   * @return True if this is less than or equal to other,
   *         false otherwise.
   **/
  inline bool operator<=(const DecimalLit& other) const {
    return data_ <= other.data_;
  }

  /**
   * @brief Operator overloading for "greater than or equal to".
   *
   * @param other Other DecimalLit to be compared.
   * @return True if this is greater than or equal to other,
   *         false otherwise.
   **/
  inline bool operator>=(const DecimalLit& other) const {
    return data_ >= other.data_;
  }

  /**
   * @brief Operator overloading for "equal to".
   *
   * @param other Other DecimalLit to be compared.
   * @return True if this is equal to other,
   *         false otherwise.
   **/
  inline bool operator==(const DecimalLit& other) const {
    return data_ == other.data_;
  }

  /**
   * @brief Operator overloading for "not equal to".
   *
   * @param other Other DecimalLit to be compared.
   * @return True if this is not equal to other,
   *         false otherwise.
   **/
  inline bool operator!=(const DecimalLit& other) const {
    return data_ != other.data_;
  }

  /**
   * @brief Operator overloading for "negate".
   *
   * @return Negative of this.
   **/
  inline DecimalLit operator-() const {
    DecimalLit result;
    result.data_ = -result.data_;
    return result;
  }

  /**
   * @brief Operator overloading for "plus".
   *
   * @param other Other DecimalLit to be added.
   * @return Sum of this and other.
   **/
  inline DecimalLit operator+(const DecimalLit& other) const {
    DecimalLit result;
    result.data_ = data_ + other.data_;
    return result;
  }

  /**
   * @brief Operator overloading for "subtract".
   *
   * @param other Other DecimalLit to be subtract.
   * @return Subtraction of other from this.
   **/
  inline DecimalLit operator-(const DecimalLit& other) const {
    DecimalLit result;
    result.data_ = data_ - other.data_;
    return result;
  }

  /**
   * @brief Operator overloading for "multiply".
   *
   * @param other Other DecimalLit to be multiplied.
   * @return Multiplication of this and other.
   **/
  inline DecimalLit operator*(const DecimalLit& other) const {
    DecimalLit result;
    result.data_ = (data_ * other.data_) / kMaxFractionInt;
    return result;
  }

  /**
   * @brief Operator overloading for "division".
   *
   * @param other Divisor DecimalLit.
   * @return Division of this with other.
   **/
  inline DecimalLit operator/(const DecimalLit& other) const {
    DecimalLit result;
    result.data_ = (data_ * kMaxFractionInt) / other.data_;
    return result;
  }

  /**
   * @brief Operator overloading for "modulo".
   *
   * @param other Mod DecimalLit.
   * @return This modulo other.
   **/
  inline DecimalLit operator%(const DecimalLit& other) const {
    DecimalLit result;
    result.data_ = data_ % other.data_;
    return result;
  }

  /**
   * @brief Operator overloading for "inplace add".
   *
   * @param other DecimalLit to be added.
   * @return Reference to this.
   **/
  inline DecimalLit& operator+=(const DecimalLit& other) {
    data_ += other.data_;
    return *this;
  }

  /**
   * @brief Operator overloading for "inplace subtract".
   *
   * @param other DecimalLit to be subtracted.
   * @return Reference to this.
   **/
  inline DecimalLit& operator-=(const DecimalLit& other) {
    data_ -= other.data_;
    return *this;
  }

  /**
   * @brief Operator overloading for "inplace multiply".
   *
   * @param other DecimalLit to be multiplied.
   * @return Reference to this.
   **/
  inline DecimalLit& operator*=(const DecimalLit& other) {
    data_ = (data_ * other.data_) / kMaxFractionInt;
    return *this;
  }

  /**
   * @brief Operator overloading for "inplace divide".
   *
   * @param other DecimalLit to be divided.
   * @return Reference to this.
   **/
  inline DecimalLit& operator/=(const DecimalLit& other) {
    data_ = (data_ * kMaxFractionInt) / other.data_;
    return *this;
  }
};

//** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_DECIMAL_LIT_HPP_
