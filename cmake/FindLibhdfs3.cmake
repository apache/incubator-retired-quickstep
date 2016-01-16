# This file copyright (c) 2011-2015, Quickstep Technologies LLC.
# All rights reserved.
# See file CREDITS.txt for details.

# Module to find the Pivotal libhdfs3.

find_path(LIBHDFS3_INCLUDE_DIR hdfs/hdfs.h)

find_library(LIBHDFS3_LIBRARY NAMES hdfs3 libhdfs3)

# Linking against libhdfs3 also requires linking against gsasl and kerberos.
find_package(GSasl REQUIRED)
find_package(Kerberos REQUIRED)

set(LIBHDFS3_LIBRARIES ${LIBHDFS3_LIBRARY}
                       ${GSASL_LIBRARY}
                       ${KERBEROS_LIBRARY})
set(LIBHDFS3_INCLUDE_DIRS ${LIBHDFS3_INCLUDE_DIR}
                          ${GSASL_INCLUDE_DIR}
                          ${KERBEROS_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Libhdfs3 DEFAULT_MSG
                                  LIBHDFS3_LIBRARY LIBHDFS3_INCLUDE_DIR)

mark_as_advanced(LIBHDFS3_INCLUDE_DIR LIBHDFS3_LIBRARY)
