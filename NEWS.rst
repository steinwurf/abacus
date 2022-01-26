News for abacus
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Major: metrics::initialize_metric() only take 1 argument "name".
         Metrics are initialized with incrementally increasing index.
* Major: Changed "title" argument to "scope".
* Major: Added a scope member to metrics object which prefixes the names of
         the metrics.
* Minor: Added add_scope() member function to class metrics.
* Minor: Added default argument for pretty-print for the to_json() member
         functions. Default is false.
* Minor: Removed title from the class metrics data-header.
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

