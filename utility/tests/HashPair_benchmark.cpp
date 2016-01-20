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

#include <functional>
#include <utility>

#include "utility/HashPair.hpp"

#include "benchmark/benchmark.h"

static void BM_HashDouble(benchmark::State &state) {  // NOLINT(runtime/references)
  struct std::hash<double> h;
  double value = 0.42;
  while (state.KeepRunning()) {
    h(value);
  }
}
BENCHMARK(BM_HashDouble);

static void BM_HashPairDoubleDouble(benchmark::State &state) {  // NOLINT(runtime/references)
  struct std::hash<std::pair<double, double>> h;
  std::pair<double, double> pair = std::make_pair(0.42, 0.42);
  while (state.KeepRunning()) {
    h(pair);
  }
}
BENCHMARK(BM_HashPairDoubleDouble);

int main(int argc, char **argv) {
  benchmark::Initialize(&argc, const_cast<const char **>(argv));
  benchmark::RunSpecifiedBenchmarks();
  return 0;
}
