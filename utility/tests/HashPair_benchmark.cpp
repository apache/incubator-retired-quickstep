/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
