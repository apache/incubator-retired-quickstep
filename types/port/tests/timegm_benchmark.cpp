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

#include <cstddef>
#include <ctime>
#include <random>
#include <vector>

#include "types/port/TypesPortConfig.h"
#include "types/port/timegm.hpp"

#include "benchmark/benchmark.h"

namespace quickstep {

namespace {

static constexpr std::size_t kNumSamples = 100;
static constexpr std::size_t kMaxThreads = 8;

// Initialize random valid values of broken-time structs.
template <typename TM>
void PreInitSamples(const std::size_t num_samples, std::vector<TM> *times) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> year(0, 100);
  std::uniform_int_distribution<> month(0, 11);
  std::uniform_int_distribution<> day(1, 28);
  std::uniform_int_distribution<> hours(0, 23);
  std::uniform_int_distribution<> min_sec(0, 59);
  TM time = {};
  for (std::size_t i = 0; i < num_samples; ++i) {
    time.tm_year = year(gen);
    time.tm_mon = month(gen);
    time.tm_mday = day(gen);
    time.tm_hour = hours(gen);
    time.tm_min = min_sec(gen);
    time.tm_sec = min_sec(gen);
    times->push_back(time);
  }
}

}  // namespace

// Benchmark mktime().
static void BM_mktime(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<std::tm> times;
  PreInitSamples(kNumSamples, &times);
  state.ResumeTiming();

  std::size_t i = 0;
  std::time_t time BENCHMARK_UNUSED;
  while (state.KeepRunning()) {
    time = std::mktime(&times[i]);
    if (++i == kNumSamples) {
      i = 0;
    }
  }
}
BENCHMARK(BM_mktime)->ThreadRange(1, kMaxThreads);

#if defined(QUICKSTEP_HAVE_TIMEGM)

// Benchmark system timegm().
static void BM_timegmWithOriginal(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<struct tm> times;
  PreInitSamples(kNumSamples, &times);
  state.ResumeTiming();

  std::size_t i = 0;
  std::time_t time BENCHMARK_UNUSED;
  while (state.KeepRunning()) {
    time = timegmWithOriginal(&times[i]);
    if (++i == kNumSamples) {
      i = 0;
    }
  }
}
BENCHMARK(BM_timegmWithOriginal)->ThreadRange(1, kMaxThreads);

#endif  // defined(QUICKSTEP_HAVE_TIMEGM)

#if defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)

// Benchmark custom timegm() implementation using tzset() and mktime().
static void BM_timegmWithTZGMT(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<struct tm> times;
  PreInitSamples(kNumSamples, &times);
  state.ResumeTiming();

  std::size_t i = 0;
  std::time_t time BENCHMARK_UNUSED;
  while (state.KeepRunning()) {
    time = timegmWithTZGMT(&times[i]);
    if (++i == kNumSamples) {
      i = 0;
    }
  }
}
BENCHMARK(BM_timegmWithTZGMT)->ThreadRange(1, kMaxThreads);

#endif  // defined(QUICKSTEP_HAVE_SETENV) && defined(QUICKSTEP_HAVE_TZSET)

// Benchmark custom timegm() implementation using precomputed timezone offsets.
static void BM_timegmWithPrecomputedOffset(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<struct tm> times;
  PreInitSamples(kNumSamples, &times);
  state.ResumeTiming();

  std::size_t i = 0;
  std::time_t time BENCHMARK_UNUSED;
  while (state.KeepRunning()) {
    time = timegmWithPrecomputedOffset(&times[i]);
    if (++i == kNumSamples) {
      i = 0;
    }
  }
}
BENCHMARK(BM_timegmWithPrecomputedOffset)->ThreadRange(1, kMaxThreads);

// Benchmark custom timegm() implementation.
static void BM_timegmCustom(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<struct tm> times;
  PreInitSamples(kNumSamples, &times);
  state.ResumeTiming();

  std::size_t i = 0;
  std::time_t time BENCHMARK_UNUSED;
  while (state.KeepRunning()) {
    time = timegmCustom(&times[i]);
    if (++i == kNumSamples) {
      i = 0;
    }
  }
}
BENCHMARK(BM_timegmCustom)->ThreadRange(1, kMaxThreads);

}  // namespace quickstep

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, const_cast<const char **>(argv));
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
