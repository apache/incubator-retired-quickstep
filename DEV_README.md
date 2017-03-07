# Apache Quickstep (Incubating)

[![Travis Widget]][Travis]

[Travis]: https://travis-ci.org/apache/incubator-quickstep
[Travis Widget]: https://travis-ci.org/apache/incubator-quickstep.svg?branch=master

Apache Quickstep is an experimental high-performance database engine designed with the
aim of Data at Bare-Metal Speed. It began life in 2011 as a
[research project at the University of Wisconsin](https://quickstep.cs.wisc.edu)
and was acquired by [Pivotal](https://pivotal.io) in 2015.
Quickstep entered incubation at the
[Apache Software Foundation](https://www.apache.org) in April, 2016.

## Getting Started (Building)

A [build guide](BUILDING.md) is available which includes instructions for
building Quickstep for the first time. You may also find it useful to use one
of the [pre-made Vagrant boxes](build/vagrant) for Quickstep that are already
set up with all of the development tools needed to build Quickstep.

## Documentation

All publicly-visible classes and functions in the Quickstep code base have
Doxygen documentation. Simply run `doxygen` in the root of the Quickstep source
to generate browsable HTML documentation. Of course, the Doxygen comments
should also be useful when reading header files directly.

In addition to the Doxygen and inline code comments explaining implementation
details, a high-level overview for each module that comprises Quickstep is
included in the README files in each subdirectory.

## Architectural Overview

Quickstep is composed of several different modules that handle different
concerns of a database system. The main modules are:

* [Utility](utility) - Reusable general-purpose code that is used by many
  other modules.
* [Threading](threading) - Provides a cross-platform abstraction for threads
  and synchronization primitives that abstracts the underlying OS threading
  features.
* [Types](types) - The core type system used across all of Quickstep. Handles
  details of how SQL types are stored, parsed, serialized & deserialized, and
  converted. Also includes basic containers for typed values (tuples and
  column-vectors) and low-level operations that apply to typed values (e.g.
  basic arithmetic and comparisons).
* [Catalog](catalog) - Keeps track of database schema as well as physical
  storage information for relations (e.g. which physical blocks store a
  relation's data, and any physical partitioning and placement information).
* [Storage](storage) - Handles the physical storage of relation data in
  self-contained, self-describing blocks, both in-memory and on persistent
  storage (disk or a distributed filesystem). Also includes some heavyweight
  run-time data structures used in query processing (e.g. hash tables for join
  and aggregation). Includes a buffer manager component for managing memory
  use and a file manager component that handles data persistence.
* [Compression](compression) - A simple implementation of ordered dictionary
  compression. Several storage formats in the Storage module are capable of
  storing compressed column data and evaluating some expressions directly on
  compressed data without decompressing. The common code supporting compression
  is in this module.
* [Expressions](expressions) - This module builds on the simple operations
  provided by the Types module to support arbitrarily complex expressions over
  data, including scalar expressions, predicates, and aggregate functions with
  and without grouping.
* [Relational Operators](relational_operators) - This module provides the
  building blocks for queries in Quickstep. A query is represented as a
  directed acyclic graph of relational operators, each of which is responsible
  for applying some relational-algebraic operation(s) to tranform its input.
  Operators generate individual self-contained "work orders" that can be
  executed independently. Most operators are parallelism-friendly and generate
  one work-order per storage block of input.
* [Query Execution](query_execution) - Handles the actual scheduling and
  execution of work from a query at runtime. The central class is the Foreman,
  an independent thread with a global view of the query plan and progress. The
  Foreman dispatches work-orders to stateless Worker threads and monitors their
  progress, and also coordinates streaming of partial results between producers
  and consumers in a query plan DAG to maximize parallelism. This module also
  includes the QueryContext class, which holds global shared state for an
  individual query and is designed to support easy
  serialization/deserialization for distributed execution.
* [Parser](parser) - A simple SQL lexer and parser that parses SQL syntax into
  an abstract syntax tree for consumption by the Query Optimizer.
* [Query Optimizer](query_optimizer) - Takes the abstract syntax tree generated
  by the parser and transforms it into a runable query-plan DAG for the Query
  Execution module. The Query Optimizer is responsible for resolving references
  to relations and attributes in the query, checking it for semantic
  correctness, and applying optimizations (e.g. filter pushdown, column
  pruning, join ordering) as part of the transformation process.
* [Command-Line Interface](cli) - An interactive SQL shell interface to
  Quickstep.

## Licensing

Quickstep is licensed under the Apache License, Version 2.0. See [LICENSE](https://github.com/apache/incubator-quickstep/blob/master/LICENSE) for the full license text.

## Disclaimer
Apache Quickstep is an effort undergoing incubation at the Apache Software
Foundation (ASF), sponsored by the Apache Incubator PMC.

Incubation is required of all newly accepted projects until a further
review indicates that the infrastructure, communications, and decision
making process have stabilized in a manner consistent with other
successful ASF projects.

While incubation status is not necessarily a reflection of the
completeness or stability of the code, it does indicate that the
project has yet to be fully endorsed by the ASF.
