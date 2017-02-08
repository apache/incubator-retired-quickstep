# Quickstep Build Guide

**Contents**
* [Basic Instructions](#basic-instructions)
  * [Prerequisites](#prerequisites)
  * [Building](#building)
  * [Running Quickstep](#running-quickstep)
  * [Running Tests](#running-tests)
  * [Configuring with CMake](#configuring-with-cmake)
* [Advanced Configuration](#advanced-configuration)
* [Appendix](#appendix)
  * [Building on Windows](#building-on-windows)
  * [Building in Vagrant](#building-in-vagrant)


**Short Version**

```sh
cd third_party
./download_and_patch_prerequisites.sh
cd ../build
cmake ..
make -j4 quickstep_cli_shell
./quickstep_cli_shell -initialize_db=true
```

# Basic Instructions

## Prerequisites

- C++ compiler that supports the C++14 standard (GCC 4.9+ or Clang 3.4+ are good)
- [cmake](http://www.cmake.org/download/) 2.8.6+
- curl

All these programs should be available on your distro's package manager.

**Optional**

- GNU Bison and Flex (They will be used to build the parser and lexer, but pre-processed copies are provided)

## Building

Once cmake finishes, you are ready to actually build quickstep by running
`make` (or `nmake` on Windows) (this will also build bundled third-party
libraries as necesary). If you want to see the actual commands that make is
running, you can do `make VERBOSE=1`. It is highly recommended to do a parallel
make to speed up the build time, which you can do with `make -jX`, where X is
the number of parallel jobs (the number of CPU cores on your system is a good
choice, unless you are low on RAM, in which case you may want to reduce the
number of jobs).

## Running Quickstep

To use quickstep, just run `quickstep_cli_shell` in the build directory. For the
first time user, run once with `-initialize_db=true` to set up an empty catalog.
Quickstep has number of command-line flags that control its behavior. Run
`quickstep_cli_shell --help` to see a listing of the options and how to use
them.

## Running Tests

Quickstep comes with an extensive suite of unit tests. After a successful
build, you can run the whole test suite by doing `make test` or `ctest`. If
you use `ctest`, you may also run tests in parallel with `ctest -jX`, where
X is the number of parallel jobs (as with `make`, your number of CPU cores is
usually a good choice).

## Configuring with CMake

CMake recommends building outside of the source tree (a recommendation which we
follow). For your convenience, a "build" directory with a skeleton of files
needed for running quickstep is provided. cd into the build directory before
running cmake.

Like a conventional configure script, you can configure some settings about how
quickstep is built when you invoke cmake. The most important is the build type.
You can build an unoptimized build with debugging information by doing:

```
cmake -D CMAKE_BUILD_TYPE=Debug ..
```

You can build a fast, optimized release build by doing:

```
cmake -D CMAKE_BUILD_TYPE=Release ..
```

The first time you check out the Quickstep source repo, you will also need to
fetch some third-party dependencies. Do this by running the following commands 
in the root quickstep directory:

```
cd third_party && ./download_and_patch_prerequisites.sh
```

# Advanced Configuration

There are a number of advanced options you can pass to CMake to control how
Quickstep is built. These all have sensible defaults, so you may skip this
section and go straight to "Building" below if you are not interested.

* **Using a non-default compiler**: CMake will automatically find and use your
  system's default C and C++ compilers. If you wish to use some other
  compilers, you can specify them on the cmake command line with the
  `CMAKE_C_COMPILER` and `CMAKE_CXX_COMPILER` options. For example, if you
  wish to use clang instead of gcc, you would do this:

```
cmake -D CMAKE_BUILD_TYPE=Release -D CMAKE_C_COMPILER=clang -D CMAKE_CXX_COMPILER=clang++ ../
```

* **Disabling TCMalloc**: You can configure whether quickstep should use
  tcmalloc (it does by default). tcmalloc stands for thread-cacheing malloc, it
  is a very high-performance replacement for the standard libc malloc (it's
  faster than standard malloc in virtually every case, at the cost of slightly
  increased up-front memory usage. It performs especially well in
  multi-threaded use). Nevertheless, there are a couple of cases where you
  don't want to use tcmalloc. The first is when you plan on using valgrind to
  check for memory leaks (using tcmalloc causes valgrind to report 0 bytes
  leaked for every program). The second is if your OS doesn't work with
  tcmalloc for some reason (we default to building without tcmalloc if CMake
  detects an OS which is known not to work with it). Or, you may just want to
  compile a DEBUG build faster by skipping an optional dependency. To build
  without tcmalloc, add `-D USE_TCMALLOC=0` to your command line.
* **Disabling Linenoise**: You can also configure whether quickstep should use
  the linenoise command-line editing library, which provides command history
  and editing in the interactive quickstep shell. This is usually desirable, so
  it is enabled by default if CMake detects that the headers it depends on are
  available (as they are on most modern UNIX systems). If you wish to override
  the default setting for your OS, you can add `-D USE_LINENOISE=0` or
  `-D USE_LINENOISE=1` to your command line.
* **Vector Copy Elision Level**: Quickstep's vectorized expression-evaluation
  system can be configured to build extra code that helps avoid making
  intermediate copies of data, at the cost of increased code complexity and
  build times. The `VECTOR_COPY_ELISION_LEVEL` option controls how aggressively
  copies are avoided, and the default level of `selection` is a good balance
  between efficiency and build time. `-D VECTOR_COPY_ELISION_LEVEL=none` will
  build the fastest, while
  `-D VECTOR_COPY_ELISION_LEVEL=joinwithbinaryexpressions` will compile *all*
  the optional code and theoretically be the most efficient at runtime. To
  quote the documentation for this option:
> This options controls whether extra code paths for vectorized evaluation
> of expressions and predicates directly on values inside storage blocks
> without copying into ColumnVectors is enabled. This may improve
> performance, but causes a combinatorial explosion of templated code to
> be compiled, which may cause very long builds (especially in Release
> mode) and very large executables (especially in Debug mode). The
> valid settings for this option are, in order of increasing code paths:
> none (attribute values are always copied into ColumnVectors before
> computing expressions on them), selection (copies are elided for
> single-relation SELECT operations), join (copies are also elided for
> expressions in the output of hash joins, but binary expressions will
> only elide a copy on one side, not both), and joinwithbinaryexpressions
> (copies are elided for both sides of a binary expression over a join).

* **Predicate Short-Circuiting**: The option
  `ENABLE_VECTOR_PREDICATE_SHORT_CIRCUIT` can be turned on or off to control
  how Quickstep evaluates complex predicates. Quoting from the option's
  documentation:
> If enabled, vectorized predicate evaluation for conjunctions and
> disjunctions will keep track of a filter of tuples already known to
> match or not match the overall predicate to reduce the number of
> comparisons that actually need to be checked. Otherwise sub-predicates
> will always be checked for all tuples in a block and their match sets
> will be intersected/unioned after the fact. Turning this option on is
> expected to reduce the number of comparisons performed, but it changes
> the inner loop for predicate evaluation from simply incrementing a
> tuple_id to scanning for bits set in a filter. Which option performs
> best is likely to be dependent on the selectivity of sub-predicates for
> a particular query.

* **Rebuild Indices On Overflow**: If an `UPDATE` query causes an IndexSubBlock
  to run out of space, it is possible that the index can be made to fit the
  changed value if it is rebuilt to be more compact. If rebuilding fails or is
  not attempted, the index will be marked invalid and will not be used. By
  default, the Quickstep storage engine will always try to rebuild an index if
  it runs out of space, but this behavior can be disabled by setting
  `-D REBUILD_INDEX_ON_UPDATE_OVERFLOW=0`.

* **Building With libc++**: The Clang compiler is usually used with the
  system-default C++ standard library (on most Linux systems, this is GNU
  libstdc++, which is packaged with GCC). Clang can also be used with the LLVM
  project's own C++ standard library implementation libc++ (in fact, recent
  versions of FreeBSD and Mac OS X use libc++ as the system default C++
  standard library). If you are using Clang on a system that has libc++
  installed but doesn't use it by default, add `-D USE_LIBCXX=1` to make
  Clang use libc++.

* **Link-Time Optimization**: Some compilers support link-time optimization,
  where all the objects linked into an executable are analyzed and optimized
  together as if they were a single translation unit. This potentially enables
  some optimizations that might not otherwise be possible, but LTO requires a
  *lot* of processing time and a fair amount of RAM. LTO can be turned on for
  release builds with GCC or ICC by doing `-D ENABLE_LTO=1`. Be aware that the
  build may take a very long time.

# Appendix

## Building on Windows

To build on Windows, you will need some variety of Microsoft's C++ compiler and
the nmake tool (either from Visual Studio, Visual C++ Express, or the Windows
SDK). Only Visual Studio 2015 or higher is sufficiently modern to build
Quickstep.

Once you have the necessary tools installed, run the "Visual Studio Command
Prompt" (use the 64-bit version if you have it). Change into the build
directory and run:

    cmake -G "NMake Makefiles" ..

The `-G "NMake Makefiles"` option tells CMake to generate makefiles for the nmake
tool instead of project files for Visual Studio. You can also specify the usual
cmake options described below like `-D CMAKE_BUILD_TYPE=Release`.

Once cmake finishes, run `nmake` to actually build quickstep. Unfortunately,
nmake does not support parallel jobs like UNIX make, so you're in for a bit of
a wait.

## Building in Vagrant

For your convenience, we have provided Vagrant virtual machine configurations
that have a complete development environment for Quickstep with all necessary
tools and dependencies already installed. [See here for instructions on how to
use them](build/vagrant/README.md).
