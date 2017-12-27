# Developing Quickstep with IDEs

## Who should read this document?
Any developer who prefers to work with IDEs instead of a terminal and
vi, emacs, or the like. In other words, this document aims to make it easier
for developers of Quickstep to work with IDEs. Over time, there will be
information about working with other IDEs, but to start out, here are
instructions for working with XCode on OSX.

## Developing Quickstep with Xcode on OSX
The instructions here were first written and verified on OSX El Capitan,
v.10.11.2, using Xcode v.7.2.

### 1: Install Xcode and command line tools
First, you will need to download and install Xcode and the associated command
line tools. There are multiple ways to do this, including going to
https://developer.apple.com/xcode/ and downloading both Xcode and the command
line tools from there. Another way that works well if you do not have Xcode
already installed is to simply open up the Terminal app and enter:

```
cc
```

This command should trigger a sequence of downloads to get you both Xcode
and the assocaited command line tools.

### 2: Install cmake
Unfortunately, the command line tools do not package `cmake`, which is needed
to build Quickstep. You can install cmake using brew as follows:

Visit http://brew.sh to determine the instructions to install brew. It will look
something like:

```
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
```

Once you have brew installed, simply use brew to install cmake from the
Terminal app by typing:

```
brew install cmake
```

### 3: Build Quicktep
Checkout the Quickstep code from git, and also checkout the associated submodules.
If you have not read it already, this would be good time to read the file
[BUILDING.md](BUILDING.md), but do not run the cmake command mentioned there. Instead, go
into the build directory of the Quickstep code that you checked out
(i.e. the directory `quickstep/build`) and run the following command
in your Terminal app:


```
cmake -D CMAKE_BUILD_TYPE=Debug -D USE_LINENOISE=0 -G "Xcode" ..
```

This will take a minute but will build the Xcode project file. You can start
Xcode by typing in:

```
open QUICKSTEP.xcodeproj
```

Xcode should start and the first thing you should see is a pop-up box that asks
if you want to `Manually Manage Schemes` or `Automatically Create Schemes`.
Pick the latter and let Xcode genreate the schemes, which will essentially
generate a build target for each target in the cmake files. There are a lot of
these, but that is ok -- you can pick just the target you want to build easily.

Xcode will take a while to index the files. When it is done, you should see at
the top of your screen a button with the text: `ALL_BUILD > My Mac`

Click on that button, and a long list of targets will show up. Pick
`quickstep_cli_shell` as the target -- this creates the main quickstep program.
(Above, you can also pick other targets if you are working on a different more
focused part of the code).

To build the source, select from the top menu bar `Product -> Build` or simply
hit Command-B. Xcode should spin into action and build the sources. It will take
a while, but the nice thing is that Xcode automatically will try to parallelize
the build. So, you should be done with building in few minutes.

You should end up with a directory `quickstep/build/Debug`, where the binary is
created. You will need to go into this directory and issue the command:

```
cp -R ../qsstor .
```

The above command ensures that you now have the appropriate directory structure
and a starting catalog file to start Quickstep. The default database is called
simply "default" and no relations in it upfront.

There are other ways of specifying where Quickstep stores the data and catalog.
In particular there is a  `-storage_path` option that could be an alternative
way of specifying the storage path and avoiding the copy above. You can find
these and other command line options by typing:

```
./quickstep_cli_shell -help
```


### 4: Debug Quickstep
Now you can debug as you would any normal process in Xcode. Note the
linenoise option in the cmake command above is important if you are going
to run quickstep from Xcode (by hitting the "play" button). If you are
curious why we have that option, see
https://github.com/antirez/linenoise/issues/85. Quickstep uses the linenoise
package, and Xcode's embedded terminal has limited functionality. With the
cmake option above, we turn off using linenoise.

Sometimes you may want to run quickstep from the command line and still
debug with Xcode. For that scenario, you do the following:
a) In Xcode, go to `Debug -> Attach to Processs` and type in
`quickstep_cli_shell`. If you want to set a breakpoint, you can do
that here. Open the file in which you want to insert a breakpoint,
go to the line in the code where you want to set a breakpoint,
and hit `Debug -> Breakpoints -> Add Breakpoint at Current Line` or
simply press `Command-\`.

b) Then go to the Terminal app, and go to the Debug directory using:

```
cd quickstep/build/Debug
```

c) Start quickstep using something like `./quickstep_cli.shell` or
`./quickstep_cli_shell < query.sql` where `query.sql` contains the SQL commands
you want to run/debug. Xcode will automatically attach the quickstep process
when it starts up. It you had set a breakpoint and the program executes that
code, then Xcode (lldb) will stop at the breakpoint. Or, if there is a crash,
you can examine the stack in Xcode.

### 5: Unit Tests
Individual unit tests show up as target schemas, so you can simply select them
and run the unit test of interest.

Running all the unit tests is complicated, and simply picking the `RUN_TESTS`
does not work. So, this is a known limitation at this point. You can, however,
follow the instructions for a [BUILDING.md](command-line build) with cmake and
then run `ctest` to run the full suite of unit tests.

### 6: Other known issues

#### Modifying CMake Files
If you change any of the cmake files (such as any of the CMakeLists.txt
files), then you will have to [redo step 3](#3-build-quicktep) above to
create a new Xcode project file.

#### Running Python Validation Scripts
Quickstep developers have a few python scripts that are used to mechanically
check code. These scripts are written in Python 2 and are not compatible with
Python 3, so they use `python2` as the interpreter in their shebangs. While
OSX does include some version of Python 2, it does not have a symlink for
`python2`, so the scripts will fail to run out of the box. An easy fix is just
to create a symlink yourself:

```
sudo ln -s /usr/bin/python2.7 /usr/local/bin/python2
```

(Note that if you have an older version of Mac OS X, you may need to replace
`python2.7` in the above command with `python2.6`, or whatever the most recent
2.X version is on your machine.)

After putting the symlink in place, you should be able to run
`./lint_everything.py` (which applies a modified version of
Google cpplint to all C++ sources) and `./validate_cmakelists.py` (which checks
that dependencies in CMakeLists.txt files exactly match included headers in C++
sources) from the root quickstep source directory to check your code. There is
a third validation script `./cyclic_dependency.py` that checks that there are
no cyclic linking dependencies, but this script also requires that the python
networkx package is installed
([https://networkx.github.io/documentation/latest/install.html](see
instructions here)).
