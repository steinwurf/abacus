========
Abacus
========

|Linux make-specs| |Windows make-specs| |MacOS make-specs| |Linux CMake| |Windows CMake| |MacOS CMake| |Valgrind| |No Assertions| |Clang Format| |Cppcheck|

.. |Linux make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/linux_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/linux_mkspecs.yml
   
.. |Windows make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/windows_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/windows_mkspecs.yml

.. |MacOS make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/macos_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/macos_mkspecs.yml
   
.. |Linux CMake| image:: https://github.com/steinwurf/abacus/actions/workflows/linux_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/linux_cmake.yml

.. |Windows CMake| image:: https://github.com/steinwurf/abacus/actions/workflows/windows_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/windows_cmake.yml
   
.. |MacOS CMake| image:: https://github.com/steinwurf/abacus/actions/workflows/macos_cmake.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/macos_cmake.yml

.. |Clang Format| image:: https://github.com/steinwurf/abacus/actions/workflows/clang-format.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/clang-format.yml

.. |No Assertions| image:: https://github.com/steinwurf/abacus/actions/workflows/nodebug.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/nodebug.yml

.. |Valgrind| image:: https://github.com/steinwurf/abacus/actions/workflows/valgrind.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/valgrind.yml

.. |Cppcheck| image:: https://github.com/steinwurf/abacus/actions/workflows/cppcheck.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/cppcheck.yml

.. image:: ./abacus.gif

Abacus is a C++ metrics library. We wanted to create a library that allows us to
create counters in our other libraries that:

* Are fast to increment/update
* Are fast to copy/transfer between dependencies
* Can separate hot and cold memory
* Can keep everything in one contiguous memory block


With this, you can create metrics for a library that can be carried through
dependencies all the way to the top-level application like so:

.. code-block:: text

    +---------------+                             +---------------+
    |    Library    |          Dependency         |    Library    |
    |       1       |            ----->           |       0       |
    +-------+-------+                             +-------+-------+
            |                                             |
            |                                             |
            |  Library specific metrics                   |  Library specific
            |                                             |      metrics
            v                                             v
    +-------+-------+                             +-------+-------+
    |               |                             |               |
    |    Metrics    | Library 0 inherits metrics  |    Metrics    |
    |       1       +---------------------------> |     0 + 1     |
    +---------------+                             +---------------+

.. contents:: Table of Contents:
   :local:

Examples / Usage
================
There are a few examples of usage in the examples folder.

Technical
=========

The library can be separated into 4 classes:

* metric
* metrics
* view
* view_iterator

The class metric is the fundamental class. It's constructed with a pointer to a
64-bit unsigned integer that points to the value of the counter. One can
increment the metric using either the regular integer +, += or ++ operators.

The class metrics contains metrics. It takes 3 arguments in it's constructor:

* max_name_bytes - The maximum number of bytes allowed for the title of the
  metrics object and for the name of each metric

* max_metrics - The maximum number of metrics it will hold

* title - A string containing the title of the metrics object

The metrics allocates at construction sequential memory to hold the names and
values of metric objects given these dimensions

The class view takes the memory of a metrics object and contains functions to
read this memory and its pointed to contents

The class view_iterator takes the memory of multiple metrics objects and
separates them into views. This class is for example used in cases, when you
want to access metrics from a dependency and use with the metrics of the current
library.

There is also some documentation in the header-files of the classes for
supplementary info.
