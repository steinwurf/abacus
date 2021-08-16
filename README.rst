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
    |    Metrics    | Library 1 inherits metrics  |    Metrics    |
    |       1       +---------------------------> |     0 + 1     |
    +---------------+                             +---------------+

Examples: TODO

.. contents:: Table of Contents:
   :local:

Usage
=====

TODO
