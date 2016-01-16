# Module to find Zookeeper C Binding.

find_path(ZOOKEEPER_INCLUDE_DIR zookeeper/zookeeper.h)

find_library(ZOOKEEPER_LIBRARY NAMES zookeeper_mt libzookeeper_mt)

set(ZOOKEEPER_LIBRARIES ${ZOOKEEPER_LIBRARY})
set(ZOOKEEPER_INCLUDE_DIRS ${ZOOKEEPER_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Zookeeper DEFAULT_MSG
                                  ZOOKEEPER_LIBRARY ZOOKEEPER_INCLUDE_DIR)

mark_as_advanced(ZOOKEEPER_INCLUDE_DIR ZOOKEEPER_LIBRARY)
