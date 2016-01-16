# Module to find SQLite 3.

find_package(PkgConfig)
pkg_check_modules(PC_SQLITE3 QUIET sqlite3)
set(SQLITE3_DEFINITIONS ${PC_SQLITE3_CFLAGS_OTHER})

find_path(SQLITE3_INCLUDE_DIR sqlite3.h
          HINTS ${PC_SQLITE3_INCLUDEDIR} ${PC_SQLITE3_INCLUDE_DIRS})

find_library(SQLITE3_LIBRARY NAMES sqlite3 libsqlite3
             HINTS ${PC_SQLITE3_LIBDIR} ${PC_SQLITE3_LIBRARY_DIRS})

set(SQLITE3_LIBRARIES ${SQLITE3_LIBRARY})
set(SQLITE3_INCLUDE_DIRS ${SQLITE3_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SQLite3 DEFAULT_MSG
                                  SQLITE3_LIBRARY SQLITE3_INCLUDE_DIR)

mark_as_advanced(SQLITE3_INCLUDE_DIR SQLITE3_LIBRARY)
