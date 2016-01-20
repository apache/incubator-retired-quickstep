#   Copyright 2014-2015 Quickstep Technologies LLC.
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
