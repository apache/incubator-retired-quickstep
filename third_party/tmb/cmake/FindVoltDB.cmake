# Module to find VoltDB C++ client library.

find_path(VOLTDB_INCLUDE_DIR InvocationResponse.hpp)

find_library(VOLTDB_LIBRARY NAMES voltdbcpp libvoltdbcpp)

# Extra libraries needed, bundled with VoltDB C++.
get_filename_component(VOLTDB_LIB_DIR ${VOLTDB_LIBRARY} PATH)
find_library(VOLTDB_EVENT_LIBRARY NAMES event libevent
                                  HINTS ${VOLTDB_LIB_DIR})
find_library(VOLTDB_EVENT_PTHREADS_LIBRARY
             NAMES event_pthreads libevent_pthreads
             HINTS ${VOLTDB_LIB_DIR})

set(VOLTDB_LIBRARIES ${VOLTDB_LIBRARY}
                     ${VOLTDB_EVENT_LIBRARY}
                     ${VOLTDB_EVENT_PTHREADS_LIBRARY}
                     rt)
set(VOLTDB_INCLUDE_DIRS ${VOLTDB_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VoltDB DEFAULT_MSG
                                  VOLTDB_LIBRARY
                                  VOLTDB_EVENT_LIBRARY
                                  VOLTDB_EVENT_PTHREADS_LIBRARY
                                  VOLTDB_INCLUDE_DIR)

mark_as_advanced(VOLTDB_INCLUDE_DIR
                 VOLTDB_LIBRARY
                 VOLTDB_EVENT_LIBRARY
                 VOLTDB_EVENT_PTHREADS_LIBRARY)
