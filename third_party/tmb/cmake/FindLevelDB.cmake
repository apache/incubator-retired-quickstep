# Module to find LevelDB.

find_path(LEVELDB_INCLUDE_DIR leveldb/db.h)

find_library(LEVELDB_LIBRARY NAMES leveldb libleveldb)

set(LEVELDB_LIBRARIES ${LEVELDB_LIBRARY})
set(LEVELDB_INCLUDE_DIRS ${LEVELDB_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LevelDB DEFAULT_MSG
                                  LEVELDB_LIBRARY LEVELDB_INCLUDE_DIR)

mark_as_advanced(LEVELDB_INCLUDE_DIR LEVELDB_LIBRARY)
