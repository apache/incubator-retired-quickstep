/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGG_FUNC_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGG_FUNC_HPP_

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "expressions/aggregation/AggregationID.hpp"
#include "utility/Macros.hpp"
#include "types/IntType.hpp"
#include "types/LongType.hpp"
#include "types/FloatType.hpp"
#include "types/DoubleType.hpp"

#include "glog/logging.h"

namespace quickstep {

class ColumnVector;
class StorageManager;
class Type;

/** \addtogroup Expressions
 *  @{
 */

struct InvalidType {};

template <typename T, typename U>
struct is_different : std::true_type {};

template <typename T>
struct is_different<T, T> : std::false_type {};

class Sum {
 public:
  Sum() {}

  template <typename ArgType>
  struct AggState {
    typedef InvalidType T;
    typedef InvalidType AtomicT;
    typedef InvalidType ResultT;
  };

  template <typename ArgType>
  struct HasAtomicImpl :
      is_different<InvalidType,
                   typename AggState<ArgType>::AtomicT> {};

  template <typename ArgType>
  inline static void InitAtomic(typename AggState<ArgType>::AtomicT *state) {
    state->store(0, std::memory_order_relaxed);
  }

  template <typename ArgType>
  inline static void MergeArgAtomic(const typename ArgType::cpptype &value,
                                    typename AggState<ArgType>::AtomicT *state) {
    LOG(FATAL) << "Not implemented";
  }

  template <typename ArgType>
  inline static void FinalizeAtomic(const typename AggState<ArgType>::AtomicT &state,
                                    typename AggState<ArgType>::ResultT *result) {
    LOG(FATAL) << "Not implemented";
  }

  template <typename ArgType>
  inline static void InitUnsafe(typename AggState<ArgType>::T *state) {
    *state = 0;
  }

  template <typename ArgType>
  inline static void MergeArgUnsafe(const typename ArgType::cpptype &value,
                                    typename AggState<ArgType>::T *state) {
    *state += value;
  }

  template <typename ArgType>
  inline static void FinalizeUnsafe(const typename AggState<ArgType>::T &state,
                                    typename AggState<ArgType>::ResultT *result) {
    *result = state;
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(Sum);
};

//------------------------------------------------------------------------------
// Implementation of Sum for IntType
template <>
struct Sum::AggState<IntType> {
  typedef std::int64_t T;
  typedef std::atomic<std::int64_t> AtomicT;
  typedef std::int64_t ResultT;
};

template <>
inline void Sum::MergeArgAtomic<IntType>(const IntType::cpptype &value,
                                         AggState<IntType>::AtomicT *state) {
  state->fetch_add(value, std::memory_order_relaxed);
}

template <>
inline void Sum::FinalizeAtomic<IntType>(const AggState<IntType>::AtomicT &state,
                                         AggState<IntType>::ResultT *result) {
  *result = state.load(std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
// Implementation of Sum for LongType
template <>
struct Sum::AggState<LongType> {
  typedef std::int64_t T;
  typedef std::atomic<std::int64_t> AtomicT;
  typedef std::int64_t ResultT;
};

template <>
inline void Sum::MergeArgAtomic<LongType>(const LongType::cpptype &value,
                                          AggState<LongType>::AtomicT *state) {
  state->fetch_add(value, std::memory_order_relaxed);
}

template <>
inline void Sum::FinalizeAtomic<LongType>(const AggState<LongType>::AtomicT &state,
                                          AggState<LongType>::ResultT *result) {
  *result = state.load(std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
// Implementation of Sum for FloatType
template <>
struct Sum::AggState<FloatType> {
  typedef double T;
  typedef std::atomic<double> AtomicT;
  typedef double ResultT;
};

template <>
inline void Sum::MergeArgAtomic<FloatType>(const FloatType::cpptype &value,
                                           AggState<FloatType>::AtomicT *state) {
  AggState<FloatType>::T state_val = state->load(std::memory_order_relaxed);
  while (!state->compare_exchange_weak(state_val, state_val + value)) {}
}

template <>
inline void Sum::FinalizeAtomic<FloatType>(const AggState<FloatType>::AtomicT &state,
                                           AggState<FloatType>::ResultT *result) {
  *result = state.load(std::memory_order_relaxed);
}

//------------------------------------------------------------------------------
// Implementation of Sum for DoubleType
template <>
struct Sum::AggState<DoubleType> {
  typedef double T;
  typedef std::atomic<double> AtomicT;
  typedef double ResultT;
};

template <>
inline void Sum::MergeArgAtomic<DoubleType>(const DoubleType::cpptype &value,
                                            AggState<DoubleType>::AtomicT *state) {
  AggState<DoubleType>::T state_val = state->load(std::memory_order_relaxed);
  while (!state->compare_exchange_weak(state_val, state_val + value)) {}
}

template <>
inline void Sum::FinalizeAtomic<DoubleType>(const AggState<DoubleType>::AtomicT &state,
                                            AggState<DoubleType>::ResultT *result) {
  *result = state.load(std::memory_order_relaxed);
}

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGG_FUNC_HPP_
