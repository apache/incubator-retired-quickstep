/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_HASH_PAIR_HPP_
#define QUICKSTEP_UTILITY_HASH_PAIR_HPP_

#include <cstddef>
#include <cstdint>
#include <functional>
#include <type_traits>
#include <utility>

namespace quickstep {

// Nested namespace for helper classes used by CombineHashes(). Not intended
// for public use.
namespace hash_combine_detail {

// Template with specializations for size_t that provide a CombineHashes
// method. The base version is empty and will cause compilation to fail if
// accidentally used with a type other than size_t.
template <typename HashT, typename Enable = void>
struct HashCombiner {
};

// Specialization for 64-bit size_t.
template <typename HashT>
struct HashCombiner<HashT,
                    typename std::enable_if<std::is_same<HashT, std::size_t>::value
                                            && (sizeof(HashT) == 8)>::type> {
  static inline std::size_t CombineHashes(std::size_t first_hash,
                                          std::size_t second_hash) {
    // Based on Hash128to64() from cityhash.
    static constexpr std::size_t kMul = UINT64_C(0x9ddfea08eb382d69);
    std::size_t a = (first_hash ^ second_hash) * kMul;
    a ^= (a >> 47);
    std::size_t b = (second_hash ^ a) * kMul;
    b ^= (b >> 47);
    b *= kMul;
    return b;
  }
};

// Specialization for 32-bit size_t.
template <typename HashT>
struct HashCombiner<HashT,
                    typename std::enable_if<std::is_same<HashT, std::size_t>::value
                                            && sizeof(HashT) == 4>::type> {
  static inline std::size_t CombineHashes(std::size_t first_hash,
                                          std::size_t second_hash) {
    // Based on hash_combine from Boost.
    first_hash ^= second_hash + 0x9e3779b9u + (first_hash << 6) + (first_hash >> 2);
    return first_hash;
  }
};

}  // namespace hash_combine_detail

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief Mix bits from two hashes to make a new composite hash.
 *
 * @note Combining hashes is not commutative, so CombineHashes(a, b) will
 *       not equal CombineHashes(b, a) (except in rare collisions).
 *
 * @param first_hash The first hash to combine.
 * @param second_hash The second hash to combine.
 * @return A composite hash mixing first_hash and second_hash.
 **/
inline std::size_t CombineHashes(std::size_t first_hash,
                                 std::size_t second_hash) {
  return hash_combine_detail::HashCombiner<std::size_t>::CombineHashes(first_hash, second_hash);
}

/** @} */

}  // namespace quickstep

namespace std {

// Add an additional specialization of the std::hash() functor for pairs of
// hashable types. This allows std::pair<X, Y> to be used as a key in
// std::unordered_set or std::unordered_map so long as std::hash is specialized
// for X and Y.
template <typename FirstT, typename SecondT>
struct hash<pair<FirstT, SecondT>> {
  size_t operator()(const pair<FirstT, SecondT> &arg) const {
    size_t first_hash = hash<FirstT>()(arg.first);
    size_t second_hash = hash<SecondT>()(arg.second);
    return ::quickstep::CombineHashes(first_hash, second_hash);
  }
};

}  // namespace std

#endif  // QUICKSTEP_UTILITY_HASH_PAIR_HPP_
