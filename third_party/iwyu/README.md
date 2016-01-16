# IWYU in Quickstep

## Why use *include-what-you-use* (or IWYU)?
"Include what you use" means for every symbol (type, function, variable,
or macro) that you use in foo.cpp, it should `#include` a .h file that exports
the declaration of that symbol. This puts us in a state where every file
includes the headers it needs to declare the symbols that it uses.  **When
every file includes what it uses, then it is possible to edit any file and
remove unused headers, without fear of accidentally breaking the upwards
dependencies of that file.  It also becomes easy to automatically track and
update dependencies in the source code.**

Additonally, IWYU suggests forward declaration wherever feasible, which in
turn reduces code size and compile time.

See [this page](https://code.google.com/p/include-what-you-use/wiki/WhyIWYU)
for more details.

## Installing IWYU

### Gentoo Linux
IWYU is available in portage at `dev-util/include-what-you-use`, but it may
require keywording with `~amd64` (or equivalent for another architecture) to be
installed. Note that IWYU version numbers in portage correspond to the
LLVM/clang version they require. If you do not also have `~` keyworded LLVM and
Clang, then you should keyword the specific version of IWYU that is compatible
with your installed LLVM.

Edit `/etc/portage/package.keywords` (or, if you manage that as a directory, a
sensibly-named file underneath it) if necessary, adding a line like:
```
dev-util/include-what-you-use ~amd64
```

Or, if you need to keyword a specific version to match your LLVM libraries,
something like:
```
=dev-util/include-what-you-use-3.5 ~amd64
```

Then just emerge the package:
```
emerge --ask dev-util/include-what-you-use
```

### MacOS X
If you are using [Homebrew](http://brew.sh/) to manage packages on your Mac,
the easiest way to install is by using using the recipe at
[jasonmp85/homebrew-iwyu](https://github.com/jasonmp85/homebrew-iwyu)

```
brew tap jasonmp85/homebrew-iwyu
brew install iwyu
```

If you're not using homebrew, there is a pre-built IWYU binary for Mac
available at the
[project download page](http://include-what-you-use.org/downloads/).

### Other Operating Systems
There are prebuilt IWYU binaries available on the
[project download page](http://include-what-you-use.org/downloads/) for Ubuntu
Linux, Mac OS X, and FreeBSD (all on x86-64) and Win32. If your OS/Arch doesn't
have prebuilt binaries available, you will have to download the source from
that page and compile it against LLVM yourself.

## Running IWYU
We include a helper script (`third_party/iwyu/iwyu_helper.py`) to simplify
using IWYU in our code base in two ways:

* Helper script understands the `clang` arguments (like defines) and user
  includes for the quickstep project, and invokes IWYU with correct arguments.
* IWYU uses `libclang` to find all symbols in the given header or source
  file. However, `libclang` is quirky with respect to including the correct
  system include directories. 

The following command will try to figure out the missing system include
directories, construct the appropriate IWYU command to run, and invoke IWYU.

```
./third_party/iwyu/iwyu_helper.py <path-to-hpp-or-cpp-file(s)>
```

The helper script lets you provides additional overrides and specify new
overrides. See next section for more details.

## Custom IWYU configuration

### .iwyu.imp

IWYU is not perfect, and it has support for defining a mapping file to specify
overrides for incorrectly deduced header files where symbols are defined. See
the section named IWYU Mappings in
[IWYU README](https://github.com/include-what-you-use/include-what-you-use) for
more details about the format of IWYU mappings.

The helper script picks up `.iwyu.imp` in the quickstep root directory (if
present), and uses the mappings in the file.

### .iwyu_conf.py

The helper also lets you set a custom configuration via a python source file.
The python source is supposed to define a dictionary named `CONFIG` with
optional entries for:

* system includes (`system-includes`),
* user includes (`user-includes`),
* IWYU mappings files (`mappings`),
* custom clang arguments (`args`),
* option to ignore default configuration (`ignore-defaults`).

Sample configuration:

```
CONFIG = {
  'system-includes': ['/a', '/b'],
  'user-includes': ['./include', './build/include'],
  'mappings': ['sample.imp'],
  'args': ['-std=c++11', '-x', 'c++'],
  'ignore-defaults': True,
}
```

See `third_party/iwyu/sample_iwyu_conf.py` for more detailed sample configuration. 

## Default IWYU configuration
The helper script uses the following default configuration:

* `system-includes`: Runs `clang ... -v /dev/null` and `iwyu ... -v /dev/null`
  and figures out the missing system include directories in IWYU.
* `user-includes`: Uses standard user include directories in the quickstep
  project. For example, `.`, `./build`, `./third_party/glog/src`, etc. See the
  helper script for more details.
* `args`: Standard Clang args to compile C++ and a few Quickstep specific
  hash-defines.
* `mappings`: Uses `third_party/iwyu/{PLATFORM}.imp` if present.
