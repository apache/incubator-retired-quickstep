/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_BIT_MANIPULATION_HPP_
#define QUICKSTEP_UTILITY_BIT_MANIPULATION_HPP_

#include <cstdint>

#include "utility/UtilityConfig.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Count the number of ones in a word, using a fast assembly instruction
 *        if possible.
 *
 * @param word A word to count ones in.
 * @return The number of 1-bits in word.
 **/
template <typename UIntType>
inline int population_count(UIntType word) {
  int count = 0;
  while (word) {
    if (word & 0x1U) {
      ++count;
    }
    word >>= 1;
  }
  return count;
}

#ifdef QUICKSTEP_HAVE_BUILTIN_POPCOUNT
template <>
inline int population_count<std::uint8_t>(std::uint8_t word) {
  return __builtin_popcount(word);
}

template <>
inline int population_count<std::uint16_t>(std::uint16_t word) {
  return __builtin_popcount(word);
}

template <>
inline int population_count<unsigned>(unsigned word) {
  return __builtin_popcount(word);
}

template <>
inline int population_count<unsigned long>(unsigned long word) {  // NOLINT(runtime/int)
  return __builtin_popcountl(word);
}

template <>
inline int population_count<unsigned long long>(unsigned long long word) {  // NOLINT(runtime/int)
  return __builtin_popcountll(word);
}
#endif

/**
 * @brief Count the number of leading zeroes before the first one in a word,
 *        using a fast assembly instruction if possible.
 * @note This can also be used to count leading ones before the first zero by
 *       bit-flipping word as ~word.
 * @warning The result is undefined if word is zero.
 *
 * @param word A word to count leading zeroes in.
 * @return The number leading 0-bits before the first 1-bit in word.
 **/
template <typename UIntType>
inline int leading_zero_count(UIntType word) {
  if (word) {
    constexpr UIntType maxone = static_cast<UIntType>(0x1) << ((sizeof(UIntType) << 3) - 1);
    int count = 0;
    while (!(word & maxone)) {
      ++count;
      word <<= 1;
    }
    return count;
  } else {
    return sizeof(UIntType) << 3;
  }
}

#ifdef QUICKSTEP_HAVE_BUILTIN_CLZ
template <>
inline int leading_zero_count<std::uint8_t>(std::uint8_t word) {
  return __builtin_clz(word) - 24;
}

template <>
inline int leading_zero_count<std::uint16_t>(std::uint16_t word) {
  return __builtin_clz(word) - 16;
}

template <>
inline int leading_zero_count<unsigned>(unsigned word) {
  return __builtin_clz(word);
}

template <>
inline int leading_zero_count<unsigned long>(unsigned long word) {  // NOLINT(runtime/int)
  return __builtin_clzl(word);
}

template <>
inline int leading_zero_count<unsigned long long>(unsigned long long word) {  // NOLINT(runtime/int)
  return __builtin_clzll(word);
}
#endif

/**
 * @brief Count the number of trailing zeroes behind the last one in a word,
 *        using a fast assembly instruction if possible.
 * @note This can also be used to count trailing ones behind the first zero by
 *       bit-flipping word as ~word.
 * @warning The result is undefined if word is zero.
 *
 * @param word A word to count trailing zeroes in.
 * @return The number trailing 0-bits behind the last 1-bit in word.
 **/
template <typename UIntType>
inline int trailing_zero_count(UIntType word) {
  if (word) {
    int count = 0;
    while (!(word & 0x1U)) {
      ++count;
      word >>= 1;
    }
    return count;
  } else {
    return sizeof(UIntType) << 3;
  }
}

#ifdef QUICKSTEP_HAVE_BUILTIN_CTZ
template <>
inline int trailing_zero_count<std::uint8_t>(std::uint8_t word) {
  return __builtin_ctz(word);
}

template <>
inline int trailing_zero_count<std::uint16_t>(std::uint16_t word) {
  return __builtin_ctz(word);
}

template <>
inline int trailing_zero_count<unsigned>(unsigned word) {
  return __builtin_ctz(word);
}

template <>
inline int trailing_zero_count<unsigned long>(unsigned long word) {  // NOLINT(runtime/int)
  return __builtin_ctzl(word);
}

template <>
inline int trailing_zero_count<unsigned long long>(unsigned long long word) {  // NOLINT(runtime/int)
  return __builtin_ctzll(word);
}
#endif

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_BIT_MANIPULATION_HPP_
