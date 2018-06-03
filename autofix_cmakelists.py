#!/usr/bin/env python

# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

import os
import re
import sys

from collections import defaultdict

################################################################################
################################# Configurations ###############################
################################################################################

# Project name.
PROJECT_NAME = "quickstep"

# Don't scan these directories.
EXCLUDED_DIRECTORIES = frozenset(
    [ "./build",
      "./cmake",
      "./parser/preprocessed",
      "./release",
      "./storage/tests",
      "./third_party" ])

# Don't scan these files.
EXCLUDED_FILES = frozenset(
    [ "./empty_src.cpp" ])

# Third-party libraries.
THIRD_PARTY_LIBRARIES = \
    { "farmhash" : [ "farmhash" ],
      "gflags" : [ "${GFLAGS_LIB_NAME}" ],
      "glog" : [ "glog" ],
      "gtest/gtest.h" : [ "gtest", "gtest_main" ],
      "gtest/gtest_prod.h" : [ "gtest" ],
      "re2" : [ "re2" ],
      "tmb" : [ "tmb" ] }

# Explicitly ignored targets.
IGNORED_TARGETS = frozenset(
    [ "quickstep_cli_LineReader",
      "quickstep_cli_shell",
      "quickstep_parser_SqlLexer",
      "quickstep_parser_SqlParser",
      "quickstep_parser_SqlParserWrapper",
      "quickstep_threading_ConditionVariable",
      "quickstep_threading_Mutex",
      "quickstep_threading_SharedMutex",
      "quickstep_threading_Thread" ])

# Do not move these targets around.
ANCHORED_TARGETS = frozenset(
    [ "quickstep_cli_shell",
      "quickstep_utility_TextBasedTestDriver"] )

# Do not move these link dependencies around.
ANCHORED_LINKS = frozenset(
    [ "quickstep_cli_shell" ] )

# Source code file extensions under consideration.
SOURCE_CODE_FILE_EXTENSIONS = [ ".hpp", ".cpp" ]

# Maximum number of characters in one line.
MAX_LINE_WIDTH = 120

# Dummy source file name.
DUMMY_CPP_FILE_NAME = "empty_src.cpp"

# Whether to auto-fix target names according to default naming rule.
# E.g. ./types/containers/ColumnVector.*
#   -> quickstep_types_containers_ColumnVector
FORCE_DEFAULT_NAMING_RULE_FOR_LIBRARY = True
FORCE_DEFAULT_NAMING_RULE_FOR_EXECUTABLE = False


################################################################################
############################### Utility Functions ##############################
################################################################################

def TopDownVisitor(functor):
  def Wrapper(*args):
    # First invoke functor.
    functor(*args)

    # Recursively process all child nodes.
    for node in args[0].getChildren():
      getattr(node, functor.__name__)(*list(args)[1:])

  return Wrapper

def BottomUpVisitor(functor):
  def Wrapper(*args):
    # Recursively process all child nodes.
    for node in args[0].getChildren():
      getattr(node, functor.__name__)(*list(args)[1:])

    # Then invoke functor.
    functor(*args)

  return Wrapper

def ResolveIndirectFilename(path, filename):
  if filename.startswith("${CMAKE_CURRENT_SOURCE_DIR}"):
    return filename.replace("${CMAKE_CURRENT_SOURCE_DIR}", path)

  if filename.startswith("${PROJECT_SOURCE_DIR}"):
    return filename.replace("${PROJECT_SOURCE_DIR}", ".")

  return None


################################################################################
################################ Utility Classes ###############################
################################################################################

class LineStream:
  """A stream of lines
  """

  def __init__(self, lines):
    """
    Parameters
    ----------
    lines : List[String]
    """
    self.lines = lines
    self.pos = 0

  def hasNextLine(self):
    return self.pos < len(self.lines)

  def top(self):
    return self.lines[self.pos]

  def pop(self):
    line = self.lines[self.pos]
    self.pos += 1
    return line

  def size(self):
    return len(self.lines)


class CMakeGlobalContext:
  """Context for global information across all cmake files.

  Attributes
  ----------
  document : DocumentRoot
  referenced_files : Dict[String, Bool]
  file_index : Dict[String, String]
  target_index : Dict[String, String]
  dependencies : Dict[String, List[Tuple[String, List[String]]]]
  prior_dependencies : Dict[String, List[Tuple[String, List[String]]]]
  modules : Dict[String, List[String]]
  tests : Dict[String, Bool]
  subdirectories : List[String]
  """

  def __init__(self, document):
    """
    Parameters
    ----------
    document : DocumentRoot
    """
    self.document = document
    self.referenced_files = {}
    self.file_index = {}
    self.target_index = {}
    self.dependencies = defaultdict(list)
    self.prior_dependencies = defaultdict(list)
    self.modules = defaultdict(list)
    self.tests = {}
    self.subdirectories = []

  def addFileReference(self, filepath):
    """
    Parameters
    ----------
    filepath : String
    """
    self.referenced_files[filepath] = True


class CMakeLocalContext:
  """Context for processing a cmake file

  Attributes
  ----------
  sources : Dict[String, SourceGroup]
  """

  def __init__(self, sources):
    """
    Parameters
    ----------
    sources : Dict[String, SourceGroup]
    """
    self.sources = sources


class CMakeFormat:
  """Format information of a cmake node

  Attributes
  ----------
  indents : Int
  """

  def __init__(self):
    self.indents = 0

  def setIndentation(self, indents):
    """
    Parameters
    ----------
    indents : Int
    """
    self.indents = indents

  def getIndentationString(self):
    """
    Returns
    -------
    String
    """
    return " " * self.indents


################################################################################
################# CMake Command Intermediate Representations ###################
################################################################################

class CMakeNode(object):
  """Base class for various cmake nodes

  Attributes
  ----------
  mutated : Bool
  """

  def __init__(self):
    self.mutated = False

  def setMutated(self, mutated):
    """
    Parameters
    ----------
    mutated : Bool
    """
    self.mutated = mutated

  def isMutated(self):
    """
    Returns
    -------
    Bool
    """
    return self.mutated

  def getChildren(self):
    """
    Returns
    -------
    List[CMakeNode]
    """
    return []

  @staticmethod
  def GenerateDefaultTarget(path, name = None):
    """
    Parameters
    ----------
    path : String
    name : String

    Returns
    -------
    String
    """
    target = PROJECT_NAME + path[1:].replace("_", "").replace("/", "_")
    if name is not None:
      target += "_" + name
    return target

  @TopDownVisitor
  def autofixTargets(self, local_ctx):
    """
    Parameters
    ----------
    local_ctx : CMakeLocalContext
    """
    pass

  @TopDownVisitor
  def collectReferencedFiles(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    pass

  @TopDownVisitor
  def buildTargetIndex(self, path, output):
    """
    Parameters
    ----------
    path : String
    output : Dict[String, String]
    """
    pass

  @TopDownVisitor
  def collectTargets(self, output):
    """
    Parameters
    ----------
    output : List[CMakeTarget]
    """
    pass

  @TopDownVisitor
  def collectLinks(self, output):
    """
    Parameters
    ----------
    output : List[CMakeLink]
    """
    pass

  @TopDownVisitor
  def collectSubdirectories(self, output):
    """
    Parameters
    ----------
    output : List[CMakeSubdirectory]
    """
    pass

  @TopDownVisitor
  def collectDependencies(self, path, global_ctx, conditions):
    """
    Parameters
    ----------
    path : String
    global_ctx : CMakeGlobalContext
    conditions : List[String]
    """
    pass

  @TopDownVisitor
  def collectTests(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    pass

  @TopDownVisitor
  def autofixDependencies(self, global_ctx):
    """
    Parameters
    ----------
    local_ctx : CMakeLocalContext
    """
    pass


class CMakeApacheLicense(CMakeNode):
  """Apache license header
  """

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """

    header = """# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License."""

    output.append(header + "\n")


class CMakeLine(CMakeNode):
  """An unrecognized cmake line

  Attributes
  ----------
  line : String
  """

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeLine, self).__init__()

    if stream is None:
      return

    self.line = stream.pop().rstrip()

  @staticmethod
  def CreateEmptyLine():
    node = CMakeLine(None)
    node.line = ""
    return node

  @staticmethod
  def CreateLine(line):
    node = CMakeLine(None)
    node.line = line
    return node

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    output.append(self.line + "\n")


class CMakeIf(CMakeNode):
  """A cmake if() command

  Attributes
  ----------
  condition : String
  nodes : List[CMakeNode]
  """

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeIf, self).__init__()

    depth = 0
    lines = []
    while stream.hasNextLine():
      line = stream.pop()
      lines.append(line)

      if "endif(" in line or "endif (" in line:
        depth -= 1
        if depth == 0:
          break;
      else:
        line = line.strip()
        if line.startswith("if") and not line.endswith("{"):
          depth += 1

    self.nodes = []

    if len(lines) == 0:
      return

    stmt = lines[0]
    self.condition = stmt[stmt.find('(') + 1 : stmt.find(')')].strip()

    self.nodes.append(CMakeLine.CreateLine(lines[0].rstrip()))
    self.nodes.append(CMakeGroup(LineStream(lines[1:-1])))

    if len(lines) < 2:
      return

    self.nodes.append(CMakeLine.CreateLine(lines[-1].rstrip()))

  def getChildren(self):
    """
    Returns
    -------
    List[CMakeNode]
    """
    return self.nodes

  def autofixTargets(self, local_ctx):
    """Conditional targets require further semantic analysis.
    """
    pass

  def collectTargets(self, output):
    """Conditional targets require further semantic analysis.
    """
    pass

  def collectLinks(self, output):
    """Conditional targets require further semantic analysis.
    """
    pass

  def collectSubdirectories(self, output):
    """
    Parameters
    ----------
    output : List[CMakeDirectory]
    """
    for node in self.nodes:
      node.collectSubdirectories(output)

  def collectDependencies(self, path, global_ctx, conditions):
    """
    Parameters
    ----------
    path : String
    global_ctx : CMakeGlobalContext
    conditions : List[String]
    """
    child_conds = conditions + [self.condition]
    for node in self.nodes:
      node.collectDependencies(path, global_ctx, child_conds)

  def collectTests(self, global_ctx):
    """Conditional targets require further semantic analysis.
    """
    pass

  def autofixDependencies(self, global_ctx):
    """Conditional targets require further semantic analysis.
    """
    pass

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    for node in self.nodes:
      node.writeToStream(output)


class CMakeSubdirectory(CMakeNode):
  """A cmake add_subdirectory() command

  Attributes
  ----------
  format : CMakeFormat
  name : String
  arguments : List[String]
  multiline : Bool
  """

  # Static member variables
  COMMAND_SPLIT_PATTERN = re.compile("[ \t\r\n]+")

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeSubdirectory, self).__init__()

    if stream is None:
      return

    stmt = ""
    while stream.hasNextLine():
      line = stream.pop()
      stmt += line

      # NOTE(jianqiao): We assume that the original CMakeLists.txt file
      # is relatively "well written".
      if ")" in line:
        break

    self.format = CMakeFormat()
    self.format.setIndentation(len(stmt) - len(stmt.lstrip()))

    # Extract the content
    stmt = stmt[stmt.find('(') + 1 : stmt.find(')')]

    self.multiline = "\n" in stmt

    # Split into target + file list
    items = CMakeTarget.COMMAND_SPLIT_PATTERN.split(stmt)

    self.name = items[0]
    self.arguments = items[1:]

  @staticmethod
  def CreateDirectory(name):
    node = CMakeSubdirectory(None)
    node.format = CMakeFormat()
    node.name = name
    node.arguments = []
    node.multiline = False
    return node

  def collectSubdirectories(self, output):
    """
    Parameters
    ----------
    output : List[CMakeDirectory]
    """
    output.append(self)

  def isRegular(self):
    if self.name is None:
      return False

    return not (self.name.startswith("\"") or self.name.startswith("$"))

  def toStringSingleLine(self):
    """
    Returns
    -------
    String
    """
    return self.format.getIndentationString() + "add_subdirectory(" + \
           " ".join([self.name] + self.arguments) + ")"

  def toStringMultipleLines(self):
    """
    Returns
    -------
    String
    """
    output = self.format.getIndentationString() + "add_subdirectory("
    spaces = "\n" + " " * len(output)
    output += spaces.join([self.name] + self.arguments) + ")"
    return output

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    if self.name is None:
      return

    item = self.toStringSingleLine()

    # Use multi-line format if the single-line output is too long
    # or the original command is in multi-line mode and is not mutated.
    if (len(item) > MAX_LINE_WIDTH
        or (self.multiline and not self.isMutated())):
      item = self.toStringMultipleLines()

    output.append(item + "\n")


class CMakeTest(CMakeNode):
  """A cmake add_test() command

  Attributes
  ----------
  format : CMakeFormat
  target : String
  dependencies : List[String]
  multiline: Bool
  """

  # Static member variables
  COMMAND_SPLIT_PATTERN = re.compile("[ \t\r\n]+")

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeTest, self).__init__()

    stmt = ""
    while stream.hasNextLine():
      line = stream.pop()
      stmt += line

      # NOTE(jianqiao): We assume that the original CMakeLists.txt file
      # is relatively "well written".
      if ")" in line:
        break

    self.format = CMakeFormat()
    self.format.setIndentation(len(stmt) - len(stmt.lstrip()))

    # Extract the content
    stmt = stmt[stmt.find('(') + 1 : stmt.find(')')]

    self.multiline = "\n" in stmt

    # Split into test name + target
    items = CMakeTarget.COMMAND_SPLIT_PATTERN.split(stmt)
    assert len(items) >= 2

    self.target = items[0]
    self.dependencies = items[1:]

  def collectTests(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    global_ctx.tests[self.target] = True

  def toStringSingleLine(self):
    """
    Returns
    -------
    String
    """
    return self.format.getIndentationString() + "add_test(" + \
           " ".join([self.target] + self.dependencies) + ")"

  def toStringMultipleLines(self):
    """
    Returns
    -------
    String
    """
    output = self.format.getIndentationString() + "add_test("
    spaces = "\n" + " " * len(output)
    output += spaces.join([self.target] + self.dependencies) + ")"
    return output

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    item = self.toStringSingleLine()

    # Use multi-line format if the single-line output is too long
    # or the original command is in multi-line mode and is not mutated.
    if (len(item) > MAX_LINE_WIDTH
        or (self.multiline and not self.isMutated())):
      item = self.toStringMultipleLines()

    output.append(item + "\n")


class CMakeTarget(CMakeNode):
  """A cmake add_library() or add_executable() command

  Attributes
  ----------
  format : CMakeFormat
  type : String
  target : String
  name : String
  files : List[String]
  multiline: Bool
  """

  # Static member variables
  COMMAND_SPLIT_PATTERN = re.compile("[ \t\r\n]+")
  INCLUDE_SPLIT_PATTERN = re.compile("[/.]")

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeTarget, self).__init__()

    if stream is None:
      return

    stmt = ""
    while stream.hasNextLine():
      line = stream.pop()
      stmt += line

      # NOTE(jianqiao): We assume that the original CMakeLists.txt file
      # is relatively "well written".
      if ")" in line:
        break

    self.format = CMakeFormat()
    self.format.setIndentation(len(stmt) - len(stmt.lstrip()))

    # Figure out whether the target type.
    for type in ["add_library", "add_executable"]:
      if type in stmt:
        self.type = type
        break
    assert self.type is not None

    # Extract the content
    stmt = stmt[stmt.find('(') + 1 : stmt.find(')')]

    self.multiline = "\n" in stmt

    # Split into target + file list
    items = CMakeTarget.COMMAND_SPLIT_PATTERN.split(stmt)

    self.target = items[0]
    self.files = items[1:]

    self.name = None
    self.inferTargetName()

  @staticmethod
  def CreateLibrary(path, name, group):
    node = CMakeTarget(None)

    node.format = CMakeFormat()
    node.type = "add_library"
    node.target = CMakeNode.GenerateDefaultTarget(path, name)
    node.name = name
    node.multiline = False

    files = [(ext, name + ext) for ext in group.files]

    if not any([ext.startswith(".c") for ext in group.files]):
      depth = group.path.count("/")
      empty_cpp = "/".join([".."] * depth) + "/" + DUMMY_CPP_FILE_NAME
      files.append((".cpp", empty_cpp))

    node.files = [it[1] for it in sorted(files, key = lambda s : s[0])]

    return node

  def inferTargetName(self):
    """Get target name if this target contains only matched .hpp/.cpp files.
    """
    target_name = None

    for filename in self.files:
      if DUMMY_CPP_FILE_NAME in filename:
        continue

      if "/" in filename or "$" in filename:
        return

      name, _ = os.path.splitext(filename)

      if target_name is None:
        target_name = name
      elif target_name != name:
        return

    if target_name is None:
      return

    self.name = target_name

  def isTest(self):
    # Hardcoded rule that identifies extra test files.
    return "test" in self.target

  def isRegular(self, path):
    if self.isTest():
      return False

    if self.target == CMakeNode.GenerateDefaultTarget(path):
      return False

    return True

  def collectTests(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    if self.isTest():
      global_ctx.tests[self.target] = True

  def autofixTargets(self, local_ctx):
    """
    Parameters
    ----------
    local_ctx : CMakeLocalContext
    """
    name = self.name

    # TODO(quickstep-team): Handle the non-regular cases.
    if name is None:
      return

    if self.target in IGNORED_TARGETS:
      return

    if name not in local_ctx.sources:
      self.setMutated(True)
      self.files = []
      return

    group = local_ctx.sources[name]
    files = [(ext, name + ext) for ext in group.files]

    if not any([ext.startswith(".c") for ext in group.files]):
      depth = group.path.count("/")
      empty_cpp = "/".join([".."] * depth) + "/" + DUMMY_CPP_FILE_NAME
      files.append((".cpp", empty_cpp))

    files = [it[1] for it in sorted(files, key = lambda s : s[0])]

    if self.files != files:
      self.setMutated(True)
      self.files = files

    # Auto-fix target name.
    if ((FORCE_DEFAULT_NAMING_RULE_FOR_LIBRARY and self.type == "add_library")
        or (FORCE_DEFAULT_NAMING_RULE_FOR_EXECUTABLE and self.type == "add_executable")):
      target = CMakeNode.GenerateDefaultTarget(group.path, name)

      if target.startswith(self.target):
        # Likely a module-all-in-one target.
        pass
      elif self.target != target:
        self.setMutated(True)
        self.target = target

  def collectReferencedFiles(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    for filename in self.files:
      output.append(filename)

  def buildTargetIndex(self, path, output):
    """
    Parameters
    ----------
    path : String
    output : Dict[String, String]
    """
    for filename in self.files:
      filepath = path + "/" + filename

      # Prefer regular targets.
      if (filepath in output and self.name is None):
        continue

      output[filepath] = self.target

  def collectTargets(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    output.append(self)

  def collectDependencies(self, path, global_ctx, conditions):
    """
    Parameters
    ----------
    path : String
    global_ctx : CMakeGlobalContext
    conditions : List[String]
    """
    dependencies = []

    for filename in self.files:
      # Remove double quotes.
      if filename.startswith("\""):
        filename = filename[1:-1]

      # Resolve indirect file name.
      if filename.startswith("$"):
        source_fp = ResolveIndirectFilename(path, filename)
        if source_fp is None:
          continue
      else:
        source_fp = path + "/" + filename

      if source_fp not in global_ctx.file_index:
        continue

      # Get source file.
      source = global_ctx.file_index[source_fp]

      for ic in source.includes:
        if ic.isConditional():
          continue

        item = ic.item

        # Check if it is third-party include.
        if item in THIRD_PARTY_LIBRARIES:
          dependencies += THIRD_PARTY_LIBRARIES[item]
          continue

        label = CMakeTarget.INCLUDE_SPLIT_PATTERN.split(item)[0]
        if label in THIRD_PARTY_LIBRARIES:
          dependencies += THIRD_PARTY_LIBRARIES[label]
          continue

        # Check if it is a protobuf header.
        if item.endswith(".pb.h"):
          proto = CMakeNode.GenerateDefaultTarget("./" + item[:-5], "proto")
          dependencies.append(proto)
          continue

        # Get file path.
        filepath = "./" + item

        if filepath not in global_ctx.target_index:
          continue

        dependencies.append(global_ctx.target_index[filepath])

    global_ctx.dependencies[self.target] = \
        [(dependency, conditions) for dependency in dependencies]

  def toStringSingleLine(self):
    """
    Returns
    -------
    String
    """
    return self.format.getIndentationString() + self.type + "(" + \
           " ".join([self.target] + self.files) + ")"

  def toStringMultipleLines(self):
    """
    Returns
    -------
    String
    """
    output = self.format.getIndentationString() + self.type + "("
    spaces = "\n" + " " * len(output)
    output += spaces.join([self.target] + self.files) + ")"
    return output

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    if len(self.files) == 0:
      return

    item = self.toStringSingleLine()

    # Use multi-line format if the single-line output is too long
    # or the original command is in multi-line mode and is not mutated.
    if (len(item) > MAX_LINE_WIDTH
        or (self.multiline and not self.isMutated())):
      item = self.toStringMultipleLines()

    output.append(item + "\n")


class CMakeLink(CMakeNode):
  """A cmake target_link_libraries() command

  Attributes
  ----------
  format : CMakeFormat
  target : String
  dependencies : List[String]
  multiline : Bool
  """

  # Static member variables
  COMMAND_SPLIT_PATTERN = re.compile("[ \t\r\n]+")

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeLink, self).__init__()

    if stream is None:
      return

    stmt = ""
    while stream.hasNextLine():
      line = stream.pop()
      stmt += line

      # NOTE(jianqiao): We assume that the original CMakeLists.txt file
      # is relatively "well written".
      if ")" in line:
        break

    self.format = CMakeFormat()
    self.format.setIndentation(len(stmt) - len(stmt.lstrip()))

    # Extract the content
    stmt = stmt[stmt.find('(') + 1 : stmt.find(')')]

    self.multiline = "\n" in stmt

    # Split into target + file list
    items = CMakeLink.COMMAND_SPLIT_PATTERN.split(stmt)

    self.target = items[0]
    self.dependencies = items[1:]

    self.sortDependencies()

  def isTest(self):
    # Hardcoded rule that identifies extra test targets.
    return "test" in self.target

  def isRegular(self, path):
    if self.isTest():
      return False

    if self.target == CMakeNode.GenerateDefaultTarget(path):
      return False

    return True

  def collectLinks(self, output):
    """
    Parameters
    ----------
    output : List[CMakeLink]
    """
    output.append(self)

  def collectDependencies(self, path, global_ctx, conditions):
    """
    Parameters
    ----------
    path : String
    global_ctx : CMakeGlobalContext
    conditions : List[String]
    """
    global_ctx.prior_dependencies[self.target] += \
        [(dependency, conditions) for dependency in self.dependencies]

  def sortDependencies(self):
    self.dependencies = sorted(set(self.dependencies))

  def autofixDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    target = self.target

    # TODO(quickstep-team): Handle tests.
    if target in global_ctx.tests:
      return

    # TODO(quickstep-team): Handle protobuf files.
    if target.endswith("proto"):
      return

    if target in IGNORED_TARGETS:
      return

    self.setMutated(True)
    self.dependencies = [x for x in self.dependencies if x in IGNORED_TARGETS]

    if target in global_ctx.modules:
      normal_dependencies = {}
      conditional_dependencies = {}

      if target in global_ctx.prior_dependencies:
        for it in global_ctx.prior_dependencies[target]:
          if len(it[1]) == 0:
            normal_dependencies[it[0]] = True
          else:
            conditional_dependencies[it[0]] = True

      for submodule in global_ctx.modules[target]:
        if (submodule == target or submodule in global_ctx.tests):
          continue

        if (submodule not in global_ctx.dependencies
            and submodule not in global_ctx.prior_dependencies):
          continue

        if (submodule in normal_dependencies
            or submodule not in conditional_dependencies):
          self.dependencies.append(submodule)

      self.sortDependencies()
      return

    if target not in global_ctx.dependencies:
      self.dependencies = []
      return

    # TODO(quickstep-team): Handle dependencies with conditions.
    self.dependencies += [it[0] for it in global_ctx.dependencies[target]
                                if it[0] != target and len(it[1]) == 0]
    self.sortDependencies()

  def toStringSingleLine(self):
    """
    Returns
    -------
    String
    """
    return self.format.getIndentationString() + "target_link_libraries(" + \
           " ".join([self.target] + self.dependencies) + ")"

  def toStringMultipleLines(self):
    """
    Returns
    -------
    String
    """
    output = self.format.getIndentationString() + "target_link_libraries("
    spaces = "\n" + " " * len(output)
    output += spaces.join([self.target] + self.dependencies) + ")"
    return output

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    if len(self.dependencies) == 0:
      return

    item = None
    if not self.multiline:
      item = self.toStringSingleLine()

    if item is None or len(item) > MAX_LINE_WIDTH:
      item = self.toStringMultipleLines()

    output.append(item + "\n")


class CMakeGroup(CMakeNode):
  """A group of cmake commands

  Attributes
  ----------
  nodes : List[CMakeNode]
  """

  def __init__(self, stream):
    """
    Parameters
    ----------
    stream : LineStream
    """
    super(CMakeGroup, self).__init__()

    self.nodes = []

    if stream is None:
      return

    while stream.hasNextLine():
      line = stream.top().lstrip()

      if line.startswith("add_library"):
        self.nodes.append(CMakeTarget(stream))
        continue

      if line.startswith("add_executable"):
        self.nodes.append(CMakeTarget(stream))
        continue

      if line.startswith("add_subdirectory"):
        self.nodes.append(CMakeSubdirectory(stream))
        continue

      if line.startswith("add_test"):
        self.nodes.append(CMakeTest(stream))
        continue

      if line.startswith("if"):
        self.nodes.append(CMakeIf(stream))
        continue

      if line.startswith("target_link_libraries"):
        self.nodes.append(CMakeLink(stream))
        continue

      self.nodes.append(CMakeLine(stream))

  def getChildren(self):
    """
    Returns
    -------
    List[CMakeNode]
    """
    return self.nodes

  def writeToStream(self, output):
    """
    Parameters
    ----------
    output : List[String]
    """
    for node in self.nodes:
      node.writeToStream(output)


################################################################################
############################# CMakeList Abstraction ############################
################################################################################

class CMakeLists:
  """A CMakeLists.txt file

  Attributes
  ----------
  path : String
  root : CMakeGroup
  newfile :  Bool
  """

  def __init__(self, path):
    """
    Parameters
    ----------
    path : String
    """
    self.path = path

    if "CMakeLists.txt" in os.listdir(path):
      with open(self.path + "/CMakeLists.txt", "r") as file:
        self.root = CMakeGroup(LineStream(file.readlines()))
        self.newfile = False
    else:
      self.root = CMakeGroup(None)
      self.root.nodes.append(CMakeApacheLicense())
      self.root.nodes.append(CMakeLine.CreateEmptyLine())
      self.newfile = True

  def autofixTargets(self, ctx):
    """
    Parameters
    ----------
    ctx : CMakeLocalContext
    """
    self.root.autofixTargets(ctx)

  def collectReferences(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    # Collect all referenced files.
    unresolved_files = []
    self.root.collectReferencedFiles(unresolved_files)

    # Resolve file path.
    referenced_files = {}
    for filename in unresolved_files:
      if DUMMY_CPP_FILE_NAME in filename:
        continue

      if filename.startswith("\""):
        filename = filename[1:-1]

      if filename.startswith("$"):
        filepath = ResolveIndirectFilename(self.path, filename)
        if filepath is not None:
          referenced_files[filepath] = True
        continue

      filepath = filename
      if not filename.startswith("/"):
        filepath = self.path + "/" + filename

      referenced_files[filepath] = True

    # Register all references into global context.
    for filepath in referenced_files:
      global_ctx.addFileReference(filepath)

  def addMissingTargets(self, local_ctx, global_ctx):
    """
    Parameters
    ----------
    ctx : CMakeLocalContext
    """
    targets = {}

    # Collect all existing targets.
    for node in self.root.nodes:
      if isinstance(node, CMakeTarget):
        targets[node.target] = node

    # Get unreferenced file groups.
    unreferenced_names = []

    for name in local_ctx.sources:
      group = local_ctx.sources[name]

      for filepath in group.getFilePaths():
        if filepath not in global_ctx.referenced_files:
          unreferenced_names.append(name)
          break

    # Add targets for unferenced file groups.
    for name in unreferenced_names:
      node = CMakeTarget.CreateLibrary(self.path, name, local_ctx.sources[name])

      self.root.nodes.append(CMakeLine.CreateEmptyLine())
      self.root.nodes.append(node)

  def buildTargetIndex(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.root.buildTargetIndex(self.path, global_ctx.target_index)

  def collectDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.root.collectDependencies(self.path, global_ctx, [])

  def collectTests(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.root.collectTests(global_ctx)

  def addMissingDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    # First add missing target link entries.
    links = []
    self.root.collectLinks(links)
    links = set([node.target for node in links])

    targets = []
    self.root.collectTargets(targets)

    for e in targets:
      if (not e.isTest()) and e.target not in links:
        node = CMakeLink(None)
        node.format = CMakeFormat()
        node.target = e.target
        node.dependencies = []
        node.multiline = True

        self.root.nodes.append(CMakeLine.CreateEmptyLine())
        self.root.nodes.append(node)

    # Then register non-test targets into the current module.
    module = CMakeNode.GenerateDefaultTarget(self.path)

    global_ctx.modules[module] += [node.target for node in targets
                                               if not node.isTest()]

    # Then register the current module into upper level module.
    if "test" not in module and not self.isEmpty():
      global_ctx.modules[module[:module.rfind("_")]].append(module)

  def addMissingSubdirectories(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    if self.isEmpty():
      return

    nodes = []
    self.root.collectSubdirectories(nodes)

    subdirectories = {}
    for node in nodes:
      name = node.name

      if name.startswith("\""):
        name = name[1:-1]

      # Resolve indirect name.
      if name.startswith("$"):
        name = ResolveIndirectFilename(self.path, name)
        if name is None:
          continue

      if name in subdirectories:
        sys.stdout.write("Warning: Possibly duplicated add_subdirectory(" +
                         name + ") in" + self.path)

      subdirectories[name] = node

    residuals = []

    for subdir in global_ctx.subdirectories:
      if subdir.startswith(self.path):
        name = subdir[len(self.path)+1:]
        if name not in subdirectories:
          self.root.nodes.append(CMakeLine.CreateEmptyLine())
          self.root.nodes.append(CMakeSubdirectory.CreateDirectory(name))
        else:
          del subdirectories[name]
      else:
        residuals.append(subdir)

    for node in subdirectories.values():
      node.name = None

    global_ctx.subdirectories = residuals + [self.path]

  def autofixDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.root.autofixDependencies(global_ctx)

  def layoutTargets(self):
    nodes = self.root.nodes
    num_nodes = len(nodes)

    anchor_pos = -1
    for i in range(num_nodes):
      node = nodes[i]
      if isinstance(node, CMakeTarget):
        anchor_pos = i
        break

    if anchor_pos < 0:
      return

    targets = []
    suffix = []
    for i in range(anchor_pos, num_nodes):
      node = nodes[i]
      if (isinstance(nodes[i], CMakeTarget)
          and node.isRegular(self.path)
          and node.target not in ANCHORED_TARGETS):
        targets.append((node.target, node))
      else:
        suffix.append(node)

    output = [nodes[i] for i in range(anchor_pos)]
    output += [it[1] for it in sorted(targets, key = lambda s : s[0])]
    output += suffix

    self.root.nodes = output

  def layoutSubdirectories(self):
    nodes = self.root.nodes
    num_nodes = len(nodes)

    anchor_pos = -1
    for i in range(num_nodes):
      node = nodes[i]
      if isinstance(node, CMakeSubdirectory) and node.isRegular():
        anchor_pos = i
        break

    if anchor_pos < 0:
      return

    subdirectories = []
    suffix = []
    for i in range(anchor_pos, num_nodes):
      node = nodes[i]
      if isinstance(nodes[i], CMakeSubdirectory) and node.isRegular():
        subdirectories.append((node.name, node))
      else:
        suffix.append(node)

    output = [nodes[i] for i in range(anchor_pos)]
    output += [it[1] for it in sorted(subdirectories, key = lambda s : s[0])]
    output += suffix

    self.root.nodes = output

  def layoutLinkDependencies(self):
    nodes = self.root.nodes
    num_nodes = len(nodes)

    anchor_pos = -1
    for i in range(num_nodes):
      node = nodes[i]
      if isinstance(node, CMakeLink):
        anchor_pos = i
        break

    if anchor_pos < 0:
      return

    links = []
    suffix = []
    for i in range(anchor_pos, num_nodes):
      node = nodes[i]
      if (isinstance(nodes[i], CMakeLink)
          and node.isRegular(self.path)
          and node.target not in ANCHORED_LINKS):
        links.append((node.target, node))
      else:
        suffix.append(node)

    output = [nodes[i] for i in range(anchor_pos)]
    output += [it[1] for it in sorted(links, key = lambda s : s[0])]
    output += suffix

    self.root.nodes = output

  def collapseEmptyLines(self):
    output = []
    state = False
    for node in self.root.nodes:
      if (not isinstance(node, CMakeLine)
          or len(node.line) != 0):
        output.append(node)
        state = False
        continue

      if not state:
        output.append(node)

      state = True

    self.root.nodes = output

  def layout(self):
    """Rearrange the commands.
    """
    self.layoutSubdirectories()
    self.layoutTargets()
    self.layoutLinkDependencies()
    self.collapseEmptyLines()

  def isEmpty(self):
    return self.newfile and len(self.root.nodes) <= 2

  def writeToFile(self):
    if self.isEmpty():
      return

    output = []
    self.root.writeToStream(output)

    while len(output) > 0:
      line = output.pop()
      if len(line.strip()) != 0:
        output.append(line)
        break

    if len(output) == 0:
      return

    with open(self.path + "/CMakeLists.txt", "w") as file:
      file.write("".join(output))
      file.close()


################################################################################
########################## Source Code File Information ########################
################################################################################

class IncludeItem:
  """A #include item

  Attributes
  ----------
  item : String
  dependencies : List[String]
  """

  # Static member variables
  INCLUDE_PATTERN = re.compile("#include [<\"](.*)[>\"]")
  DEPENDENCY_PATTERN = re.compile("#ifdef *([^ ]*)")

  def __init__(self, text, dependencies):
    """Constructor
    """
    match = IncludeItem.INCLUDE_PATTERN.search(text)
    self.item = match.group(1)

    self.dependencies = []

    # Currently we consider only #ifdef dependencies
    for line in dependencies:
      if not line.startswith("#ifdef"):
        continue

      match = IncludeItem.DEPENDENCY_PATTERN.search(line)
      self.dependencies.append(match.group(1))

  def isConditional(self):
    return len(self.dependencies) != 0


class SourceFile:
  """A source code file

  Attributes
  ----------
  path : String
  name : String
  includes : List[String]
  """

  def __init__(self, path, name):
    """
    Parameters
    ----------
    path : String
    name : String
    """
    self.path = path
    self.name = name
    self.includes = []

    # Parse include items
    with open(self.path + "/" + self.name, "r") as file:
      dependencies = []

      for line in file:
        # Remove white spaces
        line = line.strip()

        # Record dependencies
        if line.startswith("#if"):
          dependencies.append(line)

        if line.startswith("#endif"):
          dependencies.pop()

        # Add an include item
        if line.startswith("#include"):
          self.includes.append(IncludeItem(line, dependencies))


class SourceGroup:
  """hpp and/or cpp file(s) that form a library or executable

  Attributes
  ----------
  path : String
  name : String
  files : Dict[String, SourceFile]
  """

  def __init__(self, path, name):
    """
    Parameters
    ----------
    path : String
    name : String
    """
    self.path = path
    self.name = name
    self.files = {}

  def addExtension(self, ext):
    """Add a file with the specified extension (e.g. .hpp, .cpp) into this group

    Parameters
    ----------
    ext : String
    """
    self.files[ext] = SourceFile(self.path, self.name + ext)

  def getFileNames(self):
    return [self.name + ext for ext in self.files]

  def getFilePaths(self):
    return [self.path + "/" + self.name + ext for ext in self.files]


################################################################################
######################### Directory / Top-level Wrapper ########################
################################################################################

class Directory:
  """Directory node in the document tree

  Attributes
  ----------
  path : String
  directories : Dict[String, Directory]
  sources : Dict[String, SourceGroup]
  """

  def __init__(self, path):
    """
    Parameters
    ----------
    path : String
    """
    self.path = path
    self.directories = {}
    self.sources = {}

    # Parse directories.
    for filename in os.listdir(path):
      fullpath = path + "/" + filename
      if (not os.path.isdir(fullpath)
          or filename.startswith(".")
          or fullpath in EXCLUDED_DIRECTORIES):
        continue

      self.directories[filename] = Directory(fullpath)

    # Parse source code files.
    for filename in os.listdir(path):
      fullpath = path + "/" + filename

      if (os.path.isdir(fullpath)
          or filename.startswith(".")
          or fullpath in EXCLUDED_FILES):
        continue

      name, ext = os.path.splitext(filename)

      if ext not in SOURCE_CODE_FILE_EXTENSIONS:
        continue

      self.addSourceCodeFile(name, ext)

    # Parse CMakeLists.txt file.
    self.cmakelists = CMakeLists(path)

  def addSourceCodeFile(self, name, ext):
    """Parse and add a source code file

    Parameters
    ----------
    path : String
    ext : String
    """
    if name not in self.sources:
      self.sources[name] = SourceGroup(self.path, name)

    self.sources[name].addExtension(ext)

  def getChildren(self):
    """Get child directories.

    Returns
    ----------
    Dict[String, Directory]
    """
    return self.directories.values()

  @TopDownVisitor
  def autofixTargets(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.autofixTargets(CMakeLocalContext(self.sources))

  @TopDownVisitor
  def collectReferences(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.collectReferences(global_ctx)

  @TopDownVisitor
  def addMissingTargets(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.addMissingTargets(CMakeLocalContext(self.sources), global_ctx)

  @TopDownVisitor
  def buildFileIndex(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    for name in self.sources:
      group = self.sources[name]
      for ext in group.files:
        filepath = self.path + "/" + name + ext
        global_ctx.file_index[filepath] = group.files[ext]

  @TopDownVisitor
  def buildTargetIndex(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.buildTargetIndex(global_ctx)

  @TopDownVisitor
  def collectDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.collectDependencies(global_ctx)

  @TopDownVisitor
  def collectTests(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.collectTests(global_ctx)

  @TopDownVisitor
  def addMissingDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.addMissingDependencies(global_ctx)

  @TopDownVisitor
  def autofixDependencies(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.autofixDependencies(global_ctx)

  @BottomUpVisitor
  def addMissingSubdirectories(self, global_ctx):
    """
    Parameters
    ----------
    global_ctx : CMakeGlobalContext
    """
    self.cmakelists.addMissingSubdirectories(global_ctx)

  @TopDownVisitor
  def layoutCMakeLists(self):
    """Rearrange the commands in cmakelists.
    """
    self.cmakelists.layout()

  def writeCMakeLists(self):
    # Recursively process subdirectories.
    for name in self.directories:
      self.directories[name].writeCMakeLists()

    self.cmakelists.writeToFile()


class DocumentRoot:
  """The document tree root

  Attributes
  ----------
  path : String
  directories : Dict[String, Directory]
  """

  def __init__(self):
    """Constructor
    """
    self.root = Directory(".")

  def autofixCMakeLists(self):
    # Create a global context for storing intermediate information.
    global_ctx = CMakeGlobalContext(self)

    # Fix existing targets.
    self.root.autofixTargets(global_ctx)

    # Collect referenced files.
    self.root.collectReferences(global_ctx)

    # Add missing targets.
    self.root.addMissingTargets(global_ctx)

    # Collect dependencies.
    self.root.buildFileIndex(global_ctx)
    self.root.buildTargetIndex(global_ctx)
    self.root.collectDependencies(global_ctx)

    # Collect tests.
    self.root.collectTests(global_ctx)

    # Add missing link dependencies.
    self.root.addMissingDependencies(global_ctx)

    # Fix link dependencies.
    self.root.autofixDependencies(global_ctx)

    # Add missing subdirectories.
    self.root.addMissingSubdirectories(global_ctx)

    # Rearrange the order of cmake commands.
    self.root.layoutCMakeLists()

  def writeCMakeLists(self):
    self.root.writeCMakeLists()


################################################################################
################################# Entry Point ##################################
################################################################################

if __name__ == "__main__":
  document = DocumentRoot()
  document.autofixCMakeLists()
  document.writeCMakeLists()
