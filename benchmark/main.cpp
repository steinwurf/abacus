// Copyright (c) Steinwurf ApS 2020.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst
// file.

#include <abacus/metrics.hpp>
#include <benchmark/benchmark.h>

static void assign_metrics(benchmark::State& state, int v)
{
    state.SetLabel("assign_metrics");

    std::map<std::string, abacus::type> infos = {
        {"0", abacus::boolean{abacus::kind::GAUGE, ""}},
        {"1", abacus::uint64{abacus::kind::GAUGE, ""}},
        {"2", abacus::int64{abacus::kind::GAUGE, ""}},
        {"3", abacus::float64{abacus::kind::GAUGE, ""}},
        {"4", abacus::boolean{abacus::kind::GAUGE, ""}},
        {"5", abacus::float64{abacus::kind::GAUGE, ""}},
        {"6",
         abacus::enum8{
             "",
             {{0, {"", ""}}, {1, {"", ""}}, {2, {"", ""}}, {3, {"", ""}}}}}};
    abacus::metrics metrics(infos);

    auto m0 = metrics.initialize_metric<abacus::boolean>("0", false);
    auto m1 = metrics.initialize_metric<abacus::uint64>("1", 0);
    auto m2 = metrics.initialize_metric<abacus::int64>("2", 0);
    auto m3 = metrics.initialize_metric<abacus::float64>("3", 0.0);
    auto m4 = metrics.initialize_metric<abacus::boolean>("4", true);
    auto m5 = metrics.initialize_metric<abacus::float64>("5", 3.14);
    auto m6 = metrics.initialize_metric<abacus::enum8>("6", 1);

    for (auto _ : state)
    {
        m0 = true;
        m1 += 1;
        m2 -= 1;
        ++m3;
        m4 = false;
        --m5;
        m6 = 4;
    }

    state.SetItemsProcessed(state.iterations());
}

static void CustomArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kMicrosecond);
}

BENCHMARK_CAPTURE(assign_metrics, basic, 10)->Apply(CustomArguments);

BENCHMARK_MAIN();
