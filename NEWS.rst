News for abacus
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Patch: Fix warnings reported by MSVC.

5.0.0
-----
* Major: Use waf-tools 5.
* Major: Use bourne 10.

4.2.2
-----
* Patch: Fix missing cstdint for newer compilers

4.2.1
-----
* Patch: Remove unneeded assert on unaligned memory access in the view.

4.2.0
-----
* Minor: Added support for "empty" metrics object with a count of 0.

4.1.0
-----
* Minor: Added `metrics::observe_metric()` to sample the value of a metric through a callback.

4.0.0
-----
* Major: Changed the protocol. New protocol version is 1.
* Major: Added min and max values to describe the range of a metric.
* Major: Changed the metric_flags to a metric_kind representing whether the metric is a counter, constant or a gauge.
* Major: Added units of metrics to the metadata.
* Major: Changed names of metric_type, metric_kind to type, kind.

3.0.1
-----
* Patch: Fixed an issue with the offset of the metric_flags in the raw-API

3.0.0
-----
* Minor: Added slim version of `to_json`.
* Minor: Added `abacus::protocol_version` to the API.
* Major: Split meta data and value data.
* Major: Removed `metrics::copy_storage` and `metrics::storage_bytes`,
  instead use `metrics::meta_data`, `metrics::meta_bytes`, `metrics::value_data`,
  and `metrics::value_bytes`.
* Major: Replaced `view::set_data` with `view::set_meta_data` and `view::set_value_data`.
* Major: The initialization of metrics are now stored in a bitmap instead of parts of the
  flags field.
* Major: Removed metrics count from the header, instead use the combination of
  8-byte-value metrics and 1-byte-value metrics.
* Major: General API change of member functions. Many has dropped the use of "metric", e.g.,
  `metrics::metric_name(std::size_t index)` is now `metrics::name(std::size_t index)`.
* Major: Refactored `src/abacus/detail/raw.hpp` to properly handle endianness.

2.1.1
-----
* Patch: Fix multiple definitions of `abacus::detail::to_json`.

2.1.0
-----
* Minor: Added value function to abacus::metric to access the underlying
  value.

2.0.0
-----
* Major: Added support for wrapping uint64_t, double, int64_t and bool with
         class metric<>() through the metric_type enum.
* Major: Added metric_description, type and flags. Changed the memory
         memory layout thereafter.
* Minor: Added enum metric_type to parameterize metric<>() and
         initialize_metric() with.
* Major: Added a struct metric_info, used in a vector in the metrics constructor.
* Major: metrics::initialize_metric() is now templated and takes a metric name.
* Major: Removed title from the class metrics data-header.
* Major: Removed the metrics::set_metrics_title() member funciton
* Major: Removed to_json() functions on metrics and view classes and added
         free functions.
* Major: Added a byte to the memory layout that indicates if the metrics are
         written as big endian.
* Major: Added a metric_flags enum to declare metric traits like if it's constant.
* Minor: The memory of the metrics object can now be accessed directly through
         data(), like it is for the view class.
* Minor: Added initialize_constant to metrics class.

1.1.2
-----
* Patch: Fixed a memory-alignment issue on ARM processors

1.1.1
-----
* Patch: Added missing const for data() method for the view class

1.1.0
-----
* Minor: Updated waf.
* Minor: Optimized view_iterator
* Minor: Added data() for the view class

1.0.0
-----
* Major: Initial release
* Major: Added level member to metrics class
* Minor: Added read_storage() to metrics class
* Major: Separated the metric class from metrics
* Major: Added class view to handle the raw memory of class metrics
* Minor: Added class view_iterator
* Minor: Moved to_json() member function to detail
