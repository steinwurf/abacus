========
Abacus
========

|Linux make-specs| |Windows make-specs| |MacOS make-specs| |Linux CMake| |Windows CMake| |MacOS CMake| |Raspberry Pi| |Valgrind| |No Assertions| |Clang Format| |Cppcheck|

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

.. |Raspberry Pi| image:: https://github.com/steinwurf/abacus/actions/workflows/raspberry_pi.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/raspberry_pi.yml

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
