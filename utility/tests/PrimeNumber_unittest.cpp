/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <cstddef>
#include <limits>

#include "gtest/gtest.h"
#include "utility/PrimeNumber.hpp"

namespace quickstep {

TEST(PrimeNumberTest, GetNextPrimeTest) {
  // Test some small primes.
  EXPECT_EQ(2u, get_next_prime_number(0));
  EXPECT_EQ(2u, get_next_prime_number(2));
  EXPECT_EQ(3u, get_next_prime_number(3));
  EXPECT_EQ(5u, get_next_prime_number(4));
  EXPECT_EQ(5u, get_next_prime_number(5));
  EXPECT_EQ(7u, get_next_prime_number(6));
  EXPECT_EQ(53u, get_next_prime_number(49));
  EXPECT_EQ(211u, get_next_prime_number(200));
  EXPECT_EQ(907u, get_next_prime_number(890));
  EXPECT_EQ(10007u, get_next_prime_number(10000));
  EXPECT_EQ(30047u, get_next_prime_number(30030));
  EXPECT_EQ(65521u, get_next_prime_number(65520));
  EXPECT_EQ(65521u, get_next_prime_number(65521));

  // Try larger, non-baked-in primes.
  EXPECT_EQ(65537u, get_next_prime_number(65536));
  EXPECT_EQ(101917u, get_next_prime_number(101900));

  // Try some big Mersenne primes.
  EXPECT_EQ((1u << 17) - 1, get_next_prime_number(131064));
  EXPECT_EQ((1u << 17) - 1, get_next_prime_number(131071));

  EXPECT_EQ((1u << 19) - 1, get_next_prime_number(524270));
  EXPECT_EQ((1u << 19) - 1, get_next_prime_number(524287));

  EXPECT_EQ((1u << 31) - 1, get_next_prime_number(2147483647u));

  if (sizeof(std::size_t) >= 8) {
    EXPECT_EQ((1ull << 61) - 1, get_next_prime_number(2305843009213693951ull));

    // Search for the largest 64-bit prime, starting from the
    // second largest + 1.
    EXPECT_EQ(18446744073709551557ull,
              get_next_prime_number(18446744073709551534ull));
  }
}

TEST(PrimeNumberTest, GetPreviousPrimeTest) {
  // Numbers smaller than any prime.
  EXPECT_EQ(0u, get_previous_prime_number(0));
  EXPECT_EQ(0u, get_previous_prime_number(1));

  // Test some small primes.
  EXPECT_EQ(2u, get_previous_prime_number(2));
  EXPECT_EQ(3u, get_previous_prime_number(3));
  EXPECT_EQ(3u, get_previous_prime_number(4));
  EXPECT_EQ(5u, get_previous_prime_number(5));
  EXPECT_EQ(7u, get_previous_prime_number(8));
  EXPECT_EQ(47u, get_previous_prime_number(49));
  EXPECT_EQ(199u, get_previous_prime_number(200));
  EXPECT_EQ(887u, get_previous_prime_number(890));
  EXPECT_EQ(9973u, get_previous_prime_number(10000));
  EXPECT_EQ(30029u, get_previous_prime_number(30030));
  EXPECT_EQ(65519u, get_previous_prime_number(65520));
  EXPECT_EQ(65521u, get_previous_prime_number(65521));

  // Try larger, non-baked-in primes.
  EXPECT_EQ(65521u, get_previous_prime_number(65522));
  EXPECT_EQ(65521u, get_previous_prime_number(65536));
  EXPECT_EQ(65537u, get_previous_prime_number(65537));
  EXPECT_EQ(65537u, get_previous_prime_number(65538));
  EXPECT_EQ(101891u, get_previous_prime_number(101900));

  // Try some big Mersenne primes.
  EXPECT_EQ((1u << 17) - 1, get_previous_prime_number(131071));
  EXPECT_EQ((1u << 17) - 1, get_previous_prime_number(131076));

  EXPECT_EQ((1u << 19) - 1, get_previous_prime_number(524287));
  EXPECT_EQ((1u << 19) - 1, get_previous_prime_number(524290));

  EXPECT_EQ((1u << 31) - 1, get_previous_prime_number(2147483647u));

  if (sizeof(std::size_t) >= 8) {
    EXPECT_EQ((1ull << 61) - 1, get_previous_prime_number(2305843009213693951ull));

    // Search for the largest 64-bit prime, starting from the max size_t value.
    EXPECT_EQ(18446744073709551557ull,
              get_previous_prime_number(std::numeric_limits<std::size_t>::max()));
  }
}

}  // namespace quickstep
