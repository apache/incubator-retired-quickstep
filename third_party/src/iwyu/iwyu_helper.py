#!/usr/bin/env python2

#
# Helper script written by Quickstep authors to run IWYU in our code base. See
# README.md for more details. This script is not available upstream.
#
# Usage: ./third_party/iwyu/iwyu_helper.py <path-to-hpp-or-cpp-file(s)>
#

import commands
import imp
import itertools
import os.path
import platform
import subprocess
import sys

# Default configuration for IWYU
QUICKSTEP_INCLUDES = [ '.',
                       './build',
                       './build/third_party',
                       './build/third_party/gflags/include',
                       './build/third_party/protobuf/include',
                       './build/third_party/tmb/include',
                       './third_party/benchmark/include',
                       './third_party/glog/src',
                       './third_party/googletest/googletest/include',
                       './third_party/protobuf/src',
                       './third_party/re2',
                       './third_party/tmb/include']
QUICKSTEP_DEFINES = [ '-DQUICKSTEP_DEBUG',
                      '-DQUICKSTEP_ENABLE_VECTOR_COPY_ELISION_SELECTION', ]
CLANG_FLAGS = [ '-std=c++14', '-x', 'c++', ]

# Custom configuration filenames.
CUSTOM_IWYU_MAPPINGS = '.iwyu.imp'
CUSTOM_IWYU_CONFIG = '.iwyu_conf.py'

# Custom configuration sub-sections.
CFG_USER_INCLUDES = 'user-includes'
CFG_SYSTEM_INCLUDES = 'system-includes'
CFG_MAPPINGS = 'mappings'
CFG_ARGS = 'args'
CFG_IGNORE_DEFAULTS = 'ignore-defaults'

# Find the first command from input commands, which is available in the path.
def FindFirstExistingCommand(cmds):
    for cmd in cmds:
        status, _ = commands.getstatusoutput('which ' + cmd)
        if status == 0:
            return cmd
    raise RuntimeError

IWYU_CMD = FindFirstExistingCommand(['iwyu', 'include-what-you-use'])

# Get system include directory for clang (or variants).
def GetIncludes(cmd):
    cmd_args = [cmd] + CLANG_FLAGS + ['-c', '-v', '/dev/null', '-o', '/dev/null']
    # NOTE: IWYU returns exit code 2 instead of 0, so we ignore the exit status
    # for the following command.
    _, output = commands.getstatusoutput(' '.join(cmd_args))
    includes = []
    include_line = False
    for line in output.splitlines():
        if line.startswith('#include <...> search starts here:'):
            include_line = True
            continue
        elif line.startswith('End of search list.'):
            include_line = False
        elif include_line:
            includes.append(line.strip().split()[0])
    return includes

# Get the missing system include directories in IWYU, when compared to clang
# installed in the system.
def FindMissingSystemIncludes():
    iwyu_includes = set(map(os.path.realpath, GetIncludes(IWYU_CMD)))
    clang_includes = set(map(os.path.realpath, GetIncludes('clang')))
    if len(iwyu_includes - clang_includes) > 0:
        print >> sys.stderr, 'IWYU has extra system includes:', \
                ' '.join(list(iwyu_includes - clang_includes))
    return list(clang_includes - iwyu_includes)

# Return mapping arguments in IWYU for a mapping file.
def MappingArg(path):
    return ['-Xiwyu', '--mapping_file={0}'.format(path)]

# Find default (or platform specific) IWYU mapping file (if any).
def FindDefaultMappings():
    default_mapping_path = './third_party/iwyu/{0}.imp'.format(platform.system())
    if os.path.exists(default_mapping_path):
        return [default_mapping_path]
    return []

# Find custom IWYU mapping file (if any).
def FindCustomMappings():
    if os.path.exists(CUSTOM_IWYU_MAPPINGS):
        return [CUSTOM_IWYU_MAPPINGS]
    return []

# Load the custom helper configuration python source file (if any) containing
# the CONFIG dictionary.
def LoadCustomConfig():
    if os.path.exists(CUSTOM_IWYU_CONFIG):
        mod = imp.load_source('config', CUSTOM_IWYU_CONFIG)
        return mod.CONFIG
    return {}

# Returns an empty IWYU configuration.
def GetEmptyConfiguration():
    config = {
        CFG_SYSTEM_INCLUDES: [],
        CFG_USER_INCLUDES: [],
        CFG_MAPPINGS: [],
        CFG_ARGS: [],
    }
    return config

# Returns an default IWYU configuration.
def GetDefaultConfiguration():
    config = {
        CFG_SYSTEM_INCLUDES: FindMissingSystemIncludes(),
        CFG_USER_INCLUDES: QUICKSTEP_INCLUDES,
        CFG_MAPPINGS: FindDefaultMappings() + FindCustomMappings(),
        CFG_ARGS: CLANG_FLAGS + QUICKSTEP_DEFINES,
    }
    return config

# Merge configurations.
def MergeConfig(config, merge):
    if merge.has_key(CFG_SYSTEM_INCLUDES):
        config[CFG_SYSTEM_INCLUDES] += merge[CFG_SYSTEM_INCLUDES]
    if merge.has_key(CFG_USER_INCLUDES):
        config[CFG_USER_INCLUDES] += merge[CFG_USER_INCLUDES]
    if merge.has_key(CFG_MAPPINGS):
        config[CFG_MAPPINGS] += merge[CFG_MAPPINGS]
    if merge.has_key(CFG_ARGS):
        config[CFG_ARGS] += merge[CFG_ARGS]
    return config

# Get the final IWYU configuration to use.
def GetConfiguration():
    m = LoadCustomConfig()
    config = GetDefaultConfiguration()
    if m:
        if m.has_key(CFG_IGNORE_DEFAULTS) and m[CFG_IGNORE_DEFAULTS]:
            # Ignoring default configuration.
            config = MergeConfig(GetEmptyConfiguration(), m)
        else:
            # Adding to default configuration.
            config = MergeConfig(config, m)
    return config

# Run IWYU for the header/source specified in `path' and the specified IWYU
# configuration.
def RunIWYU(path, config):
    args = [IWYU_CMD]
    if config.has_key(CFG_MAPPINGS):
        args += list(itertools.chain.from_iterable(
            map(MappingArg, config[CFG_MAPPINGS])))
    if config.has_key(CFG_ARGS):
        args += config[CFG_ARGS]
    if config.has_key(CFG_SYSTEM_INCLUDES):
        args += list(itertools.chain.from_iterable(
            map(lambda x: ['-isystem', x], config[CFG_SYSTEM_INCLUDES])))
    if config.has_key(CFG_USER_INCLUDES):
        args += list(itertools.chain.from_iterable(
            map(lambda x: ['-I', x], config[CFG_USER_INCLUDES])))
    subprocess.call(args + [path])

if __name__ == '__main__':
    config = GetConfiguration()
    for path in sys.argv[1:]:
        RunIWYU(path, config)
