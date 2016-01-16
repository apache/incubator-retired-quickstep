/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_FILE_MANAGER_LOCAL_HPP_
#define QUICKSTEP_STORAGE_FILE_MANAGER_LOCAL_HPP_

#include "storage/StorageConfig.h"

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_POSIX
#include "storage/FileManagerPosix.hpp"
#elif defined(QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS)
#include "storage/FileManagerWindows.hpp"
#endif

namespace quickstep {

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_POSIX
typedef FileManagerPosix FileManagerLocal;
#elif defined(QUICKSTEP_HAVE_FILE_MANAGER_WINDOWS)
typedef FileManagerWindows FileManagerLocal;
#endif

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_FILE_MANAGER_LOCAL_HPP_
