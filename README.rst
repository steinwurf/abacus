========
Abacus
========

.. image:: ./abacus.gif

Abacus is a C++ metrics library. With this, you can create metrics for a library that can be carried through dependencies all the way to the top-level application like so:

.. code-block:: text

    +---------------+                             +---------------+
    |    Library    |          Dependency         |    Library    |
    |    Level 1    |            ----->           |    Level 0    |
    +-------+-------+                             +-------+-------+
            |                                             |
            |                                             |
            |  Library specific metrics                   |  Library specific metrics
            |                                             |
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

The class metric is the fundamental class. It's constructed with a pointer to a 64-bit unsigned integer that points to the value of the counter.
One can increment the metric using either the regular integer +, += or ++ operators.

The class metrics contains metrics. It takes 3 arguments in it's constructor:
* max_name_bytes - The maximum number of bytes allowed for the title of the metrics object and for the name of each metric
* max_metrics - The maximum number of metrics it will hold
* title - A string containing the title of the metrics object

The metrics allocates at construction sequential memory to hold the names and values of metric objects given these dimensions

The class view takes the memory of a metrics object and contains functions to read this memory and its pointed to Contents

The class view_iterator takes the memory of multiple metrics objects and separates them into views. This class is for example used in cases, when you want to access metrics from a dependency and mix them with the metrics of the current library.
