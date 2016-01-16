#!/usr/bin/env python2

import os
import subprocess
import sys

EXCLUDED_PREFIXES = ['./.', './third_party', './build', './parser/preprocessed']

call_args = ['/usr/bin/env', 'python2', './third_party/cpplint/cpplint.py']

print "Running cpplint on entire source tree. This may take a minute or two..."

for (dirpath, dirnames, filenames) in os.walk('.'):
    filtered = False
    for prefix in EXCLUDED_PREFIXES:
        if dirpath.startswith(prefix):
            filtered = True
    if not filtered:
        for filename in filenames:
            if filename.endswith('.hpp') or filename.endswith('.cpp'):
                call_args.append(dirpath + '/' + filename)

sys.exit(subprocess.call(call_args))
