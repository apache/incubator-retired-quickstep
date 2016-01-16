/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "utility/Glob.hpp"

#include "utility/UtilityConfig.h"

#if defined(QUICKSTEP_HAVE_GLOB)
#include <glob.h>
#elif defined(QUICKSTEP_HAVE_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <cstddef>
#include <functional>
#include <string>
#include <vector>

#include "glog/logging.h"

namespace quickstep {
namespace utility {
namespace file {

std::size_t GlobForEach(const std::string &pattern,
                        std::function<void(const char *)> functor) {
#if defined(QUICKSTEP_HAVE_GLOB)
  glob_t glob_result;

  int ret = glob(pattern.c_str(), 0, nullptr, &glob_result);
  LOG_IF(ERROR, ret != 0) << "glob() returned non-zero";
  std::size_t num_matches = glob_result.gl_pathc;

  for (std::size_t i = 0; i < glob_result.gl_pathc; ++i) {
    functor(glob_result.gl_pathv[i]);
  }

  globfree(&glob_result);
  return num_matches;

#elif defined(QUICKSTEP_HAVE_WINDOWS)
  std::size_t num_matches = 0;
  WIN32_FIND_DATA find_data;
  HANDLE find_handle = INVALID_HANDLE_VALUE;
  DWORD error_code = 0;

  find_handle = FindFirstFile(pattern.c_str(), &find_data);
  if (find_handle == INVALID_HANDLE_VALUE) {
    error_code = GetLastError();
    if (error_code != ERROR_FILE_NOT_FOUND) {
      LOG(ERROR) << "Failed to retrieve blockfiles with error_code: " << error_code;
    }
    return 0;
  }

  do {
    functor(find_data.cFileName);
    ++num_matches;
  } while (FindNextFile(find_handle, &find_data) != 0);

  error_code = GetLastError();
  if (error_code != ERROR_NO_MORE_FILES) {
    LOG(ERROR) << "Failed to FindNextFile with error_code: " << error_code;
  }

  if (FindClose(find_handle) == 0) {
    LOG(ERROR) << "Failed to close the file with error_code: " << GetLastError();
  }

  return num_matches;

#else
#error "No implementation for Glob in this platform."
#endif
}

std::vector<std::string> GlobExpand(const std::string &pattern) {
  std::vector<std::string> matches;
  GlobForEach(pattern, [&matches](const char *path) {
    matches.emplace_back(path);
  });
  return matches;
}

}  // namespace file
}  // namespace utility
}  // namespace quickstep
