#pragma once

/// @file frame_bench.h
/// Zero-dependency micro-benchmark harness using std::chrono.
///
/// Usage:
///   #include "frame_bench.h"
///
///   FRAME_BENCHMARK(myBench, 100000)
///   {
///       volatile auto result = someFunction("input");
///   }
///
///   int main() { return FRAME_RUN_BENCHMARKS(); }

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace frame::bench {

using BenchFn = void (*)();

struct BenchmarkCase
{
    std::string name;
    std::size_t iterations;
    BenchFn body;
};

inline std::vector<BenchmarkCase> &registry()
{
    static std::vector<BenchmarkCase> cases;
    return cases;
}

struct Registrar
{
    Registrar(const char *name, std::size_t iterations, BenchFn body)
    {
        registry().push_back({.name = name, .iterations = iterations, .body = body});
    }
};

inline int runAll()
{
    using Clock = std::chrono::high_resolution_clock;
    using Micros = std::chrono::duration<double, std::micro>;

    constexpr int nameWidth = 40;
    constexpr int numWidth = 12;

    std::cout << std::left << std::setw(nameWidth) << "benchmark" << std::right
              << std::setw(numWidth) << "iterations" << std::setw(numWidth) << "min (us)"
              << std::setw(numWidth) << "avg (us)" << std::setw(numWidth) << "max (us)" << '\n';
    std::cout << std::string(static_cast<std::size_t>(nameWidth + (numWidth * 4)), '-') << '\n';

    for (const auto &bc : registry()) {
        double minUs = std::numeric_limits<double>::max();
        double maxUs = 0.0;
        double totalUs = 0.0;

        for (std::size_t i = 0; i < bc.iterations; ++i) {
            const auto start = Clock::now();
            bc.body();
            const auto elapsed = Micros(Clock::now() - start).count();

            totalUs += elapsed;
            minUs = std::min(minUs, elapsed);
            maxUs = std::max(maxUs, elapsed);
        }

        const double avgUs = totalUs / static_cast<double>(bc.iterations);

        std::cout << std::left << std::setw(nameWidth) << bc.name << std::right << std::fixed
                  << std::setprecision(3) << std::setw(numWidth) << bc.iterations
                  << std::setw(numWidth) << minUs << std::setw(numWidth) << avgUs
                  << std::setw(numWidth) << maxUs << '\n';
    }

    return 0;
}

} // namespace frame::bench

// ---------------------------------------------------------------------------
// Public macros
// ---------------------------------------------------------------------------

/// Register a benchmark. Use like: FRAME_BENCHMARK(name, iterations) { ... }
// NOLINTBEGIN(misc-use-anonymous-namespace,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,bugprone-throwing-static-initialization)
#define FRAME_BENCHMARK(name, iterations)                                                          \
    static void frameBench_##name();                                                               \
    static ::frame::bench::Registrar frameBenchReg_##name(#name, (iterations), frameBench_##name); \
    static void frameBench_##name()
// NOLINTEND(misc-use-anonymous-namespace,cert-err58-cpp,cppcoreguidelines-avoid-non-const-global-variables,bugprone-throwing-static-initialization)

/// Run all registered benchmarks and print results.
#define FRAME_RUN_BENCHMARKS() ::frame::bench::runAll()
