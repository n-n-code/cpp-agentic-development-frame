#include "frame_bench.h"
#include "projectcore.h"

// WHAT: Benchmark normalizedProjectName with typical and large inputs.
// HOW:  Run the function many times and report min/avg/max duration.
// WHY:  Establishes a baseline for the core string-normalization path
//       so regressions are visible before they reach production.

FRAME_BENCHMARK(normalizedProjectName_typical, 100000)
{
    volatile auto result = frame::normalizedProjectName("  Example Project Name  ");
    (void)result;
}

FRAME_BENCHMARK(normalizedProjectName_short, 100000)
{
    volatile auto result = frame::normalizedProjectName("a");
    (void)result;
}

FRAME_BENCHMARK(trimCopy_padded, 100000)
{
    volatile auto result = frame::trimCopy("   some padded input   ");
    (void)result;
}

int main()
{
    return FRAME_RUN_BENCHMARKS();
}
