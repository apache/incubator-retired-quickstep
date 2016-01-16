#!/usr/bin/env python2

# Script to do analyze the dependencies in Quickstep particularly cycles in the
# dependency graph. This script can be used to find:
#   1. Cycles in the dependency graph.
#   2. Strongly connected components in the dependency graph.
#   3. Find shortest path between two targets.
#
# Dependency:
#   pip install networkx
#
# Usage:
#   Find the shortest path between target1 and target2.
#       cyclic_dependency.py --path [target1] [target2]
#   Find strongly connected components in the dependency graph.
#       cyclic_dependency.py --components
#   Find cycles in the graph.
#       cyclic_dependency.py --cycles
#

import itertools
import networkx as nx
from optparse import OptionParser
import os
import pprint
import sys

# Don't scan these directories for quickstep modules.
EXCLUDED_TOP_LEVEL_DIRS = ["build", "third_party"]

# Explicitly ignored dependencies (special headers with no other quickstep
# dependencies).
IGNORED_DEPENDENCIES = frozenset(["quickstep_threading_WinThreadsAPI"])

# States when scanning a CMakeLists.txt file.
CMAKE_SCANNING_NONE = 0
CMAKE_SCANNING_TARGET_LINK_LIBRARIES = 1

# Process a CMake target_link_libraries() command with arguments
# 'target_link_libraries_args', adding all the quickstep libraries specified
# to the target's set of dependencies in 'deps_in_cmake'.
def process_target_link_libraries(directory,
                                  target_link_libraries_args,
                                  deps_in_cmake):
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

# Scan a CMake file, building up the dependency sets for targets from included
# C++ headers and from target_link_libraries() specified in CMakeLists.txt and
# comparing the two for discrepancies.
def process_cmakelists_file(cmakelists_filename, qs_module_dirs):
    cmakelists_file = open(cmakelists_filename, "r")
    directory = os.path.dirname(cmakelists_filename)
    deps_in_cmake = {}
    scan_state = CMAKE_SCANNING_NONE
    stitched_string = ""
    for line in cmakelists_file:
        if scan_state == CMAKE_SCANNING_NONE:
            target_link_libraries_pos = line.find("target_link_libraries(")
            if target_link_libraries_pos != -1:
                scan_state = CMAKE_SCANNING_TARGET_LINK_LIBRARIES
                stitched_string = line[target_link_libraries_pos
                                       + len("target_link_libraries("):]
                closing_paren_pos = stitched_string.find(")")
                if closing_paren_pos != -1:
                    stitched_string = stitched_string[:closing_paren_pos]
                    process_target_link_libraries(directory,
                                                  stitched_string,
                                                  deps_in_cmake)
                    stitched_string = ""
                    scan_state = CMAKE_SCANNING_NONE
        elif scan_state == CMAKE_SCANNING_TARGET_LINK_LIBRARIES:
            closing_paren_pos = line.find(")")
            if closing_paren_pos == -1:
                stitched_string += line
            else:
                stitched_string += line[:closing_paren_pos]
                process_target_link_libraries(directory,
                                              stitched_string,
                                              deps_in_cmake)
                stitched_string = ""
                scan_state = CMAKE_SCANNING_NONE
    return deps_in_cmake

# Create dependency graph in networkx, and returns Digraph() object, node to
# target mapping, and target to node mapping.
def create_graph(deps_in_cmake):
    nodes = set()
    for source, dest_set in deps_in_cmake.iteritems():
        nodes.add(source)
        nodes.update(dest_set)

    nodes_list = list(nodes)
    nodes_map = {}
    for i, n in zip(xrange(len(nodes_list)), nodes_list):
        nodes_map[n] = i

    G = nx.DiGraph()

    for source, dest_set in deps_in_cmake.iteritems():
        source_node = nodes_map[source]
        for dest in dest_set:
            if source == dest: continue
            dest_node = nodes_map[dest]
            G.add_edge(source_node, dest_node)

    return G, nodes_list, nodes_map

# Lists the strongly connected components in the graph.
def find_strongly_connected_components(G, nodes_list):
    components = 0
    for n in nx.strongly_connected_components(G):
        if len(n) > 1:
            components += 1
            # Only output components bigger than 1.
            print [nodes_list[i] for i in n]
    return components

# Lists cycles in the graph truncating to 100 cycles.
def find_cycles(G, nodes_list, truncate):
    cycles = 0
    for n in nx.simple_cycles(G):
        print [nodes_list[i] for i in n]
        cycles += 1
        if cycles >= truncate:
            print "Many cycles found. Truncating to {0} cycles.".format(truncate)
            break
    return cycles

# Find the shortest path from source to target.
def find_path(G, nodes_list, nodes_map, source, target):
    source_node = nodes_map[source]
    target_node = nodes_map[target]
    if nx.has_path(G, source_node, target_node):
        print [nodes_list[i] for i in nx.shortest_path(G,
                source_node,
                target_node)]
    else:
        print 'No path.'

def main():
    if not os.getcwd().endswith("quickstep"):
        print ("WARNING: you don't appear to be running in the root quickstep "
               "source directory. Don't blame me if something goes wrong.")
    qs_module_dirs = []
    for filename in os.listdir("."):
        if (os.path.isdir(filename)
            and not filename.startswith(".")
            and filename not in EXCLUDED_TOP_LEVEL_DIRS):
            qs_module_dirs.append(filename)
    cmakelists_to_process = []
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
    dependencies = {}
    for cmakelists_filename in cmakelists_to_process:
        dependencies.update(process_cmakelists_file(cmakelists_filename,
                                                    qs_module_dirs))
    parser = OptionParser()
    parser.add_option("--components", action='store_true',
            help='List strongly connected components in the dependency graph.')
    parser.add_option("--cycles", action='store_true',
            help='List cycles in the dependency graph.')
    parser.add_option("--path", action='store_true',
            help='Output the shortest path between two targets.')
    parser.add_option("--dependency", action='store_true',
            help='Output the dependencies graph.')
    parser.add_option("--truncate", type=int, default=100,
            help='Truncate cycles to this number. Default: 100.')
    (options, args) = parser.parse_args()

    if options.dependency:
        pprint.pprint(dependencies)

    G, nodes_list, nodes_map = create_graph(dependencies)

    if options.path:
        if len(args) != 2:
            raise ValueError
        find_path(G, nodes_list, nodes_map, args[0], args[1])
        return 0
    elif options.cycles:
        return find_cycles(G, nodes_list, options.truncate)
    else:
        return find_strongly_connected_components(G, nodes_list)

if __name__ == "__main__":
    return_code = main()
    if return_code > 0:
        sys.exit(1)
    else:
        sys.exit(0)

