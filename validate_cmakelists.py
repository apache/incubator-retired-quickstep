#!/usr/bin/env python

"""Script to do basic sanity checking for target_link_libraries() commands in
CMakeLists.txt files.

Scans C++ sources specified in add_library() commands for includes that look
like they are in the Quickstep source tree, then makes sure that the
corresponding libraries appear in the target_link_libraries() command for the
library.

TODO List / Known Issues & Limitations:
    - Script skips over targets that are built conditionally (i.e. that have
      multiple add_library() commands) and just prints a warning to the user.
    - Script only validates libraries, not executables.
    - Script only checks quickstep includes and libraries, so it will not
      detect missing third party libraries.
"""

#   Copyright 2011-2015 Quickstep Technologies LLC.
#   Copyright 2015-2016 Pivotal Software, Inc.
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import os
import sys

# Don't scan these directories for quickstep modules.
EXCLUDED_TOP_LEVEL_DIRS = ["build", "third_party"]

# Explicitly ignored dependencies (special headers with no other quickstep
# dependencies).
IGNORED_DEPENDENCIES = frozenset(
    ["quickstep_storage_DataExchange.grpc_proto",
     "quickstep_threading_WinThreadsAPI",
     "quickstep_utility_textbasedtest_TextBasedTest",
     "quickstep_utility_textbasedtest_TextBasedTestDriver",
     "quickstep_storage_bitweaving_BitWeavingHIndexSubBlock",
     "quickstep_storage_bitweaving_BitWeavingIndexSubBlock",
     "quickstep_storage_bitweaving_BitWeavingVIndexSubBlock"])

# States when scanning a CMakeLists.txt file.
CMAKE_SCANNING_NONE = 0
CMAKE_SCANNING_LIBRARY = 1
CMAKE_SCANNING_TARGET_LINK_LIBRARIES = 2
CMAKE_SCANNING_IGNORE = 3

def convert_path_to_targetname(include_path):
    """Convert an included header file's path to a quickstep library target in
    cmake.

    Args:
        include_path (str): A header file path taken from a C++ include
            statement.

    Returns:
        str: The target name in CMake that corresponds to the specified header.
    """
    path_components = include_path.split("/")
    for idx in range(len(path_components) - 1):
        path_components[idx] = path_components[idx].replace("_", "")
    if path_components[-1].endswith("_gen.hpp"):
        # Generated header (e.g. parser or lexer).
        path_components[-1] = path_components[-1][:-8]
    elif path_components[-1].endswith(".hpp"):
        # Regular header.
        path_components[-1] = path_components[-1][:-4]
    elif path_components[-1].endswith(".pb.h"):
        # Generated protobuf header.
        path_components[-1] = path_components[-1][:-5] + "_proto"
    return "quickstep_" + "_".join(path_components)

def convert_proto_path_to_targetname(import_path):
    """Convert an imported proto's path to a quickstep library target in CMake.

    Args:
        import_path (str): A proto definition file path taken from a protobuf
            import statement.

    Returns:
        str: The target name in CMake that corresponds to the specified proto
            definition.
    """
    path_components = import_path.split("/")
    for idx in range(len(path_components) - 1):
        path_components[idx] = path_components[idx].replace("_", "")
    if path_components[-1].endswith(".proto"):
        path_components[-1] = path_components[-1][:-6] + "_proto"
    return "quickstep_" + "_".join(path_components)

def get_module_targetname_for_cmakelists(cmakelists_filename):
    """Determine what the name for the all-in-one module target should be based
    on the CMakeLists.txt filename with path.

    Args:
        cmakelists_filename (str): CMakeLists.txt filename with path from
            quickstep root.

    Returns:
        str: The target name in CMake that corresponds to the special
            all-in-one library for the module described by the CMakeLists.txt
            file.
    """
    components = []
    (head, tail) = os.path.split(cmakelists_filename)
    while head != "":
        (head, tail) = os.path.split(head)
        if tail != ".":
            components.append(tail.replace("_", ""))
    components.append("quickstep")
    components.reverse()
    return "_".join(components)

def get_dependency_set_from_cpp_src(src_filename, qs_module_dirs):
    """Read the C++ source file at 'src_filename' and return a set of all
    quickstep libraries it includes headers for.

    Args:
        src_filename (str): A path to a C++ source file (may be header or
            implementation).
        qs_module_dirs (List[str]): List of directories for top-level quickstep
            modules

    Returns:
        Set[str]: A set of CMake target names for the quickstep library targets
            that the C++ file includes.
    """
    dependency_set = set()
    with open(src_filename, "r") as src_file:
        for line in src_file:
            if line.startswith("#include \""):
                include_filename = line[len("#include \""):]
                include_filename = (
                    include_filename[:include_filename.find("\"")])
                # Skip over CMake-generated config headers and -inl companion
                # headers.
                if not (include_filename.endswith("Config.h")
                        or include_filename.endswith("-inl.hpp")):
                    for module_dir in qs_module_dirs:
                        if include_filename.startswith(module_dir):
                            dependency_set.add(
                                convert_path_to_targetname(include_filename))
                            break
    return dependency_set

def get_dependency_set_from_proto_src(src_filename, qs_module_dirs):
    """Read the protobuf definition file at 'src_filename' and return a set of
    all other Quickstep proto libraries it imports.

    Args:
        src_filename (str): A path to a proto definition file.
        qs_module_dirs (List[str]): List of directories for top-level quickstep
            modules

    Returns:
        Set[str]: A set of CMake target names for the quickstep library targets
            that the proto file imports.
    """
    dependency_set = set()
    with open(src_filename, "r") as src_file:
        for line in src_file:
            if line.startswith("import \""):
                import_filename = line[len("import \""):]
                import_filename = import_filename[:import_filename.find("\"")]
                for module_dir in qs_module_dirs:
                    if import_filename.startswith(module_dir):
                        dependency_set.add(
                            convert_proto_path_to_targetname(import_filename))
                        break
    return dependency_set

def process_add_library(qs_module_dirs,
                        directory,
                        add_library_args,
                        deps_from_includes,
                        skipped_targets,
                        generated_targets):
    """Process a CMake add_library() command while scanning a CMakeLists.txt
    file.

    Args:
        qs_module_dirs (List[str]): List of directories for top-level quickstep
            modules
        directory (str): The directory that the CMakeLists.txt file we are
            currently scanning resides in.
        add_library_args (str): The arguments to an add_library() command in
            CMakeLists.txt
        deps_from_includes (Map[str, Set[str]]): A map from a CMake target name
            to the set of other CMake targets it depends on, deduced based on
            what headers the C++/proto sources for the target include. A new
            entry will be added to this map for the target specified by the
            add_library() command.
        skipped_targets (Set[str]): A set of CMake target names that have been
            skipped for dependency checking because multiple add_library()
            commands specified the same target name. This probably means that
            the target in question is built differently depending on some
            configuration options or platform checks.
        generated_targets (Set[str]): A set of CMake target names that appear
            to be built from dynamically-generated source code that we can't
            scan. Note, however, that we can and do scan proto definitions and
            flex/bison sources for dependencies. An entry will be added to this
            set of the given add_library() command references unscannable
            generated sources.
    """
    components = add_library_args.split()
    if components[0].startswith("quickstep"):
        if components[0] in deps_from_includes:
            skipped_targets.add(components[0])
            deps_from_includes[components[0]] = set()
            return
        deps = set()
        for src_filename in components[1:]:
            if src_filename.startswith("${"):
                if (src_filename.endswith("proto_srcs}")
                        or src_filename.endswith("proto_hdrs}")):
                    # Scan protobuf definition instead of C++ source.
                    #
                    # src_filename has the form module_File_proto_srcs, so we
                    # split it by '_' and get the third-from-last part (i.e.
                    # the base filename without extension).
                    src_filename = src_filename.split("_")[-3] + ".proto"
                    full_src_filename = os.path.join(directory, src_filename)
                    deps.update(
                        get_dependency_set_from_proto_src(full_src_filename,
                                                          qs_module_dirs))
                    continue
                elif src_filename.startswith("${BISON_"):
                    # Scan Bison parser source.
                    src_filename = (
                        src_filename[len("${BISON_"):-len("_OUTPUTS}")]
                        + ".ypp")
                elif src_filename.startswith("${FLEX_"):
                    # Scan Flex lexer source.
                    src_filename = (
                        src_filename[len("${FLEX_"):-len("_OUTPUTS}")]
                        + ".lpp")
                else:
                    generated_targets.add(components[0])
                    return
            elif src_filename.startswith("\"${CMAKE_CURRENT_SOURCE_DIR}/"):
                src_filename = src_filename[
                    len("\"${CMAKE_CURRENT_SOURCE_DIR}/"):-1]
            full_src_filename = os.path.join(directory, src_filename)
            deps.update(get_dependency_set_from_cpp_src(full_src_filename,
                                                        qs_module_dirs))
        deps_from_includes[components[0]] = deps

def process_target_link_libraries(target_link_libraries_args,
                                  deps_in_cmake):
    """Process a CMake target_link_libraries() while scanning a CMakeLists.txt
    file.

    Args:
        target_link_libraries_args (str): The arguments to a
            target_link_libraries() command in CMakeLists.txt
        deps_in_cmake (Map[str, Set[str]]): A map of CMake target names to
            their sets of dependencies (also CMake target names) specified by
            target_link_libraries() commands. If the target being processed
            already has an entry in the map, its set will be expanded with any
            additional dependencies, otherwise a new entry will be created with
            all the dependencies from the current target_link_libraries()
            command. This way, if multiple target_link_libraries() commands are
            processed for the same target, we will build up the union of all
            dependencies for it (just like CMake does).
    """
    components = target_link_libraries_args.split()
    if components[0].startswith("quickstep"):
        deps = set()
        # Intentionally count the first part for self-includes
        for component in components:
            if component.startswith("quickstep"):
                deps.add(component)
        if components[0] in deps_in_cmake:
            deps_in_cmake[components[0]].update(deps)
        else:
            deps_in_cmake[components[0]] = deps

def process_cmakelists_file(cmakelists_filename, qs_module_dirs):
    """Scan a CMakeLists.txt file and report any mistakes (missing or
    superfluous dependencies in target_link_libraries() commands).

    This function will deduce what other libraries a given library target
    should depend on based on what headers are included in its source code. It
    will then collect the set of link dependencies actually specified in
    target_link_libraries() commands, and will print warnings about libraries
    that appear in one set but not the other.

    Args:
        cmakelists_filename (str): The path to a CMakeLists.txt file to scan
            and validate.
        qs_module_dirs (List[str]): List of directories for top-level quickstep
            modules.

    Returns:
        Tuple[Set[str], Set[str], Set[str]]: First element is the set of
            targets that failed validation because they had missing and/or
            superfluous dependencies. Second element is the set of targets
            that were skipped over because they had multiple add_library()
            commands (probably because they are built differently depending on
            configuration options or platform checks). Third element is the
            set of targets that were skipped because they appear to be built
            from dynamically-generated source code (although proto definitions
            and flex/bison sources are detected and scannned for dependencies).
    """
    directory = os.path.dirname(cmakelists_filename)
    module_targetname = get_module_targetname_for_cmakelists(
        cmakelists_filename)
    deps_from_includes = {}
    deps_in_cmake = {}
    validation_failed_targets = set()
    skipped_targets = set()
    generated_targets = set()
    scan_state = CMAKE_SCANNING_NONE
    previous_state = CMAKE_SCANNING_NONE
    stitched_string = ""
    with open(cmakelists_filename, "r") as cmakelists_file:
        for line in cmakelists_file:
            if ("CMAKE_VALIDATE_IGNORE_BEGIN" in line and
                scan_state != CMAKE_SCANNING_IGNORE):
                previous_state = scan_state
                scan_state = CMAKE_SCANNING_IGNORE
                continue

            if scan_state == CMAKE_SCANNING_IGNORE:
                if "CMAKE_VALIDATE_IGNORE_END" in line:
                    scan_state = previous_state
                elif "CMAKE_VALIDATE_IGNORE_BEGIN" in line:
                    print("Nested IGNORE_BEGIN directives found in: "
                        + cmakelists_filename + ", exiting")
                    exit(-1)
                else:
                    continue
            elif scan_state == CMAKE_SCANNING_NONE:
                add_library_pos = line.find("add_library(")
                if add_library_pos != -1:
                    scan_state = CMAKE_SCANNING_LIBRARY
                    stitched_string = (
                        line[add_library_pos + len("add_library("):])
                    closing_paren_pos = stitched_string.find(")")
                    if closing_paren_pos != -1:
                        stitched_string = stitched_string[:closing_paren_pos]
                        process_add_library(qs_module_dirs,
                                            directory,
                                            stitched_string,
                                            deps_from_includes,
                                            skipped_targets,
                                            generated_targets)
                        stitched_string = ""
                        scan_state = CMAKE_SCANNING_NONE
                else:
                    target_link_libraries_pos = line.find(
                        "target_link_libraries(")
                    if target_link_libraries_pos != -1:
                        scan_state = CMAKE_SCANNING_TARGET_LINK_LIBRARIES
                        stitched_string = (
                            line[target_link_libraries_pos
                                 + len("target_link_libraries("):])
                        closing_paren_pos = stitched_string.find(")")
                        if closing_paren_pos != -1:
                            stitched_string = (
                                stitched_string[:closing_paren_pos])
                            process_target_link_libraries(stitched_string,
                                                          deps_in_cmake)
                            stitched_string = ""
                            scan_state = CMAKE_SCANNING_NONE
            elif scan_state == CMAKE_SCANNING_LIBRARY:
                closing_paren_pos = line.find(")")
                if closing_paren_pos == -1:
                    stitched_string += line
                else:
                    stitched_string += line[:closing_paren_pos]
                    process_add_library(qs_module_dirs,
                                        directory,
                                        stitched_string,
                                        deps_from_includes,
                                        skipped_targets,
                                        generated_targets)
                    stitched_string = ""
                    scan_state = CMAKE_SCANNING_NONE
            elif scan_state == CMAKE_SCANNING_TARGET_LINK_LIBRARIES:
                closing_paren_pos = line.find(")")
                if closing_paren_pos == -1:
                    stitched_string += line
                else:
                    stitched_string += line[:closing_paren_pos]
                    process_target_link_libraries(stitched_string,
                                                  deps_in_cmake)
                    stitched_string = ""
                    scan_state = CMAKE_SCANNING_NONE
    # After scanning, report any missing dependencies.
    for target, include_deps in iter(deps_from_includes.items()):
        if target in skipped_targets:
            pass
        elif len(include_deps) != 0:
            if target not in deps_in_cmake:
                if not (target in include_deps and len(include_deps) == 1):
                    validation_failed_targets.add(target)
                    print("Missing target_link_libraries() for " + target + ":")
                    for dep in sorted(include_deps):
                        print("\t" + dep)
            else:
                missing_deps = (include_deps
                                - deps_in_cmake[target]
                                - IGNORED_DEPENDENCIES)
                if len(missing_deps) != 0:
                    validation_failed_targets.add(target)
                    print("Missing target_link_libraries() for " + target + ":")
                    for dep in sorted(missing_deps):
                        print("\t" + dep)
        elif target == module_targetname:
            # Special case hack for module all-in-one library
            missing_deps = (frozenset(deps_from_includes.keys())
                            - deps_in_cmake[target])
            # Filter out test-only libraries.
            true_missing_deps = set()
            for dep in missing_deps:
                if not dep.startswith(module_targetname + "_tests"):
                    true_missing_deps.add(dep)
            if len(true_missing_deps) != 0:
                validation_failed_targets.add(target)
                print("Missing target_link_libraries() for " + target + ":")
                for dep in sorted(true_missing_deps):
                    print("\t" + dep)
    # Also report possibly superfluous extra dependencies.
    for target, cmake_deps in iter(deps_in_cmake.items()):
        if (target not in skipped_targets) and (target in deps_from_includes):
            extra_deps = cmake_deps - deps_from_includes[target]
            if target in extra_deps:
                extra_deps.remove(target)
            if len(extra_deps) != 0 and target != module_targetname:
                validation_failed_targets.add(target)
                print("Possibly superfluous target_link_libraries() for "
                       + target + ":")
                for dep in sorted(extra_deps):
                    print("\t" + dep)
    return (validation_failed_targets, skipped_targets, generated_targets)

def main(cmakelists_to_process):
    """Main function for script which scans and analyzes CMakeLists.txt files
    and prints warnings about missing or superfluous dependencies, and about
    targets that could not be automatically scanned and should be manually
    checked.

    Args:
        cmakelists_to_process (List[str]): A list of relative paths of
            CMakeLists.txt files to scan and report on. If empty, this function
            will instead recursively walk the current working directory and
            scan every CMakeLists.txt file that it finds.

    Returns:
        int: The total number of targets that failed validation because of
            missing or superfluous dependencies.
    """
    if not os.getcwd().endswith("quickstep"):
        print("WARNING: you don't appear to be running in the root quickstep "
              "source directory. Don't blame me if something goes wrong.")
    qs_module_dirs = []
    for filename in os.listdir("."):
        if (os.path.isdir(filename)
                and not filename.startswith(".")
                and filename not in EXCLUDED_TOP_LEVEL_DIRS):
            qs_module_dirs.append(filename)
    if len(cmakelists_to_process) == 0:
        for (dirpath, dirnames, filenames) in os.walk('.'):
            skip = False
            for excluded_dir in EXCLUDED_TOP_LEVEL_DIRS:
                if dirpath.startswith(excluded_dir):
                    skip = True
                    break
            if not skip:
                if "CMakeLists.txt" in filenames:
                    cmakelists_to_process.append(
                        os.path.join(dirpath, "CMakeLists.txt"))
    global_validation_failed_targets = set()
    global_skipped_targets = set()
    global_generated_targets = set()
    for cmakelists_filename in cmakelists_to_process:
        (local_validation_failed_targets,
         local_skipped_targets,
         local_generated_targets) = (
             process_cmakelists_file(cmakelists_filename, qs_module_dirs))
        global_validation_failed_targets.update(
            local_validation_failed_targets)
        global_skipped_targets.update(local_skipped_targets)
        global_generated_targets.update(local_generated_targets)
    if len(global_skipped_targets) != 0:
        print("WARNING: The following targets had multiple add_library() "
               + "commands and were NOT checked by this script (they should "
               + "be manually checked):")
        for target in sorted(global_skipped_targets):
            print("\t" + target)
    if len(global_generated_targets) != 0:
        print("INFO: The add_library() commands for the following targets "
               + "appear to reference generated sources, so they were not "
               + "checked):")
        for target in sorted(global_generated_targets):
            print("\t" + target)
    return len(global_validation_failed_targets)

if __name__ == "__main__":
    if main(sys.argv[1:]) > 0:
        sys.exit(1)
    else:
        sys.exit(0)
