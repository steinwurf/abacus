#include <abacus/metrics.hpp>
#include <benchmark/benchmark.h>
#include <map>
#include <string>

// Helper function to create metric definitions
std::map<abacus::name, abacus::type> create_metric_infos()
{
    return {{abacus::name{"0"},
             abacus::boolean{abacus::gauge, abacus::description{""},
                             abacus::required}},
            {abacus::name{"1"},
             abacus::uint64{abacus::gauge, abacus::description{""},
                            abacus::required}},
            {abacus::name{"2"},
             abacus::int64{abacus::gauge, abacus::description{""},
                           abacus::required}},
            {abacus::name{"3"},
             abacus::float64{abacus::gauge, abacus::description{""},
                             abacus::required}},
            {abacus::name{"4"},
             abacus::boolean{abacus::gauge, abacus::description{""},
                             abacus::required}},
            {abacus::name{"5"},
             abacus::float64{abacus::gauge, abacus::description{""},
                             abacus::required}},
            {abacus::name{"6"},
             abacus::enum8{
                 abacus::description{""},
                 {{0, {"", ""}}, {1, {"", ""}}, {2, {"", ""}}, {3, {"", ""}}},
                 abacus::required}}};
}

// Benchmark for metric initialization
static void BM_MetricInitialization(benchmark::State& state)
{
    state.SetLabel("Metric Initialization");
    for (auto _ : state)
    {
        abacus::metrics metrics(create_metric_infos());
        auto m0 = metrics.initialize_required<abacus::boolean>("0", false);
        auto m1 = metrics.initialize_required<abacus::uint64>("1", 0);
        auto m2 = metrics.initialize_required<abacus::int64>("2", 0);
        auto m3 = metrics.initialize_required<abacus::float64>("3", 0.0);
        auto m4 = metrics.initialize_required<abacus::boolean>("4", true);
        auto m5 = metrics.initialize_required<abacus::float64>("5", 3.14);
        auto m6 = metrics.initialize_required<abacus::enum8>("6", 1);

        (void)m0;
        (void)m1;
        (void)m2;
        (void)m3;
        (void)m4;
        (void)m5;
        (void)m6;
    }
    state.SetItemsProcessed(state.iterations());
}

// Benchmark for assignment operations
static void BM_AssignMetrics(benchmark::State& state)
{
    state.SetLabel("Assign Metrics");
    abacus::metrics metrics(create_metric_infos());
    auto m0 = metrics.initialize_required<abacus::boolean>("0", false);
    auto m1 = metrics.initialize_required<abacus::uint64>("1", 0);
    auto m2 = metrics.initialize_required<abacus::int64>("2", 0);
    auto m3 = metrics.initialize_required<abacus::float64>("3", 0.0);
    auto m4 = metrics.initialize_required<abacus::boolean>("4", true);
    auto m5 = metrics.initialize_required<abacus::float64>("5", 3.14);
    auto m6 = metrics.initialize_required<abacus::enum8>("6", 1);

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

// Benchmark for accessing metrics
static void BM_AccessMetrics(benchmark::State& state)
{
    state.SetLabel("Access Metrics");
    abacus::metrics metrics(create_metric_infos());
    auto m0 = metrics.initialize_required<abacus::boolean>("0", false);
    auto m1 = metrics.initialize_required<abacus::uint64>("1", 0);
    auto m2 = metrics.initialize_required<abacus::int64>("2", 0);
    auto m3 = metrics.initialize_required<abacus::float64>("3", 0.0);

    for (auto _ : state)
    {
        bool val0 = m0.value();
        uint64_t val1 = m1.value();
        int64_t val2 = m2.value();
        double val3 = m3.value();

        benchmark::DoNotOptimize(val0);
        benchmark::DoNotOptimize(val1);
        benchmark::DoNotOptimize(val2);
        benchmark::DoNotOptimize(val3);
    }

    state.SetItemsProcessed(state.iterations());
}

// Benchmark for incrementing uint64 metric
static void BM_IncrementUint64(benchmark::State& state)
{
    state.SetLabel("Increment Uint64 Metric");
    abacus::metrics metrics(create_metric_infos());
    auto m1 = metrics.initialize_required<abacus::uint64>("1", 0);

    for (auto _ : state)
    {
        m1 += 1;
    }

    state.SetItemsProcessed(state.iterations());
}

// Apply custom arguments to all benchmarks
static void CustomArguments(benchmark::internal::Benchmark* b)
{
    b->Unit(benchmark::kMicrosecond);
}

BENCHMARK(BM_MetricInitialization)->Apply(CustomArguments);
BENCHMARK(BM_AssignMetrics)->Apply(CustomArguments);
BENCHMARK(BM_AccessMetrics)->Apply(CustomArguments);
BENCHMARK(BM_IncrementUint64)->Apply(CustomArguments);

BENCHMARK_MAIN();
