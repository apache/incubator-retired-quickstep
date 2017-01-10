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

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <vector>

#include "utility/EqualsAnyConstant.hpp"

#include "benchmark/benchmark.h"

namespace quickstep {

namespace {

static constexpr std::size_t kNumSampleValues = 1024;

std::vector<int> CreateSamples(const int max_value) {
  std::random_device hw_rand;
  std::mt19937 mtgen(hw_rand());
  std::uniform_int_distribution<int> dist(0, max_value);

  std::vector<int> samples;
  samples.reserve(kNumSampleValues);
  for (std::size_t i = 0; i < kNumSampleValues; ++i) {
    samples.push_back(dist(mtgen));
  }

  return samples;
}

}  // namespace

static void BM_EqualsAnyConstant_1Constant(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 1);
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_EqualsAnyConstant_1Constant)->Arg(4)->Arg(8)->Arg(16)->Arg(128)->Arg(1024);

static void BM_EqualsAnyConstant_4Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 1, 3, 4, 7);
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_EqualsAnyConstant_4Constants)->Arg(8)->Arg(16)->Arg(128)->Arg(1024);

static void BM_EqualsAnyConstant_8Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 1, 3, 4, 7, 8, 9, 10, 12);
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_EqualsAnyConstant_8Constants)->Arg(16)->Arg(128)->Arg(1024);

static void BM_EqualsAnyConstant_12Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = QUICKSTEP_EQUALS_ANY_CONSTANT(checked, 1, 3, 4, 7, 8, 9, 10, 12, 13, 15, 16, 18);
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_EqualsAnyConstant_12Constants)->Arg(24)->Arg(128)->Arg(1024);


// Benchmark the naive way of doing things with or-over-equals.
static void BM_NaiveEqualsAnyConstant_1Constant(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = (checked == 1);
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_NaiveEqualsAnyConstant_1Constant)->Arg(4)->Arg(8)->Arg(16)->Arg(128)->Arg(1024);

static void BM_NaiveEqualsAnyConstant_4Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = ((checked == 1) || (checked == 3) || (checked == 4) || (checked == 7));
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_NaiveEqualsAnyConstant_4Constants)->Arg(8)->Arg(16)->Arg(128)->Arg(1024);

static void BM_NaiveEqualsAnyConstant_8Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = ((checked == 1) || (checked == 3) || (checked == 4) || (checked == 7)
                 || (checked == 8) || (checked == 9) || (checked == 10) || (checked == 12));
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_NaiveEqualsAnyConstant_8Constants)->Arg(16)->Arg(128)->Arg(1024);

static void BM_NaiveEqualsAnyConstant_12Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = ((checked == 1) || (checked == 3) || (checked == 4) || (checked == 7)
                 || (checked == 8) || (checked == 9) || (checked == 10) || (checked == 12)
                 || (checked == 13) || (checked == 15) || (checked == 16) || (checked == 18));
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_NaiveEqualsAnyConstant_12Constants)->Arg(24)->Arg(128)->Arg(1024);


// Benchmark using std::find().
static void BM_FindAnyConstant_1Constant(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  static constexpr std::array<int, 1> constants = {{1}};
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = (std::find(constants.begin(), constants.end(), checked) != constants.end());
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_FindAnyConstant_1Constant)->Arg(4)->Arg(8)->Arg(16)->Arg(128)->Arg(1024);

static void BM_FindAnyConstant_4Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  static constexpr std::array<int, 4> constants = {{1, 3, 4, 7}};
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = (std::find(constants.begin(), constants.end(), checked) != constants.end());
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_FindAnyConstant_4Constants)->Arg(8)->Arg(16)->Arg(128)->Arg(1024);

static void BM_FindAnyConstant_8Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  static constexpr std::array<int, 8> constants = {{1, 3, 4, 7, 8, 9, 10, 12}};
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = (std::find(constants.begin(), constants.end(), checked) != constants.end());
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_FindAnyConstant_8Constants)->Arg(16)->Arg(128)->Arg(1024);

static void BM_FindAnyConstant_12Constants(benchmark::State &state) {  // NOLINT(runtime/references)
  state.PauseTiming();
  static constexpr std::array<int, 12> constants = {{1, 3, 4, 7, 8, 9, 10, 12, 13, 15, 16, 18}};
  std::vector<int> samples = CreateSamples(state.range(0));
  BENCHMARK_UNUSED bool matched;
  state.ResumeTiming();
  while (state.KeepRunning()) {
    for (const int checked : samples) {
      matched = (std::find(constants.begin(), constants.end(), checked) != constants.end());
    }
  }
  state.SetItemsProcessed(state.iterations() * kNumSampleValues);
}
BENCHMARK(BM_FindAnyConstant_12Constants)->Arg(24)->Arg(128)->Arg(1024);

}  // namespace quickstep

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, argv);
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
