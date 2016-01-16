# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# Copyright (c) 2015, Pivotal Software, Inc.
# All rights reserved.
# See file CREDITS.txt for details.

# Module to find LibNuma
find_path(LIBNUMA_INCLUDE_DIR NAMES numa.h numaif.h)
find_library(LIBNUMA_LIBRARY NAMES numa)

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibNuma DEFAULT_MSG
                                  LIBNUMA_LIBRARY
                                  LIBNUMA_INCLUDE_DIR)

mark_as_advanced(LIBNUMA_INCLUDE_DIR LIBNUMA_LIBRARY)

