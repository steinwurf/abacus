News for abacus
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Major: Added support for wrapping uint64_t, double, int64_t and bool with
         class metric<>() through the value_type enum.
* Major: Added metric_description, type and is_constant. Changed the memory
         memory layout thereafter.
* Minor: Added enum value_type to parameterize metric<>() and
         initialize_metric() with.
* Major: Added a struct metric_info, used in a vector to construct metrics.
* Major: metrics::initialize_metric() is now templated.
         Metrics are initialized with incrementally increasing index.
* Major: Switched from using a title to using a scope, that is not part of the
         memory layout.
* Major: Added a scope member to metrics object which prefixes the names of
         the metrics.
* Minor: Added add_scope() member function to class metrics.
* Major: Removed title from the class metrics data-header.
* Major: Removed the metrics::set_metrics_title() member funciton

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

