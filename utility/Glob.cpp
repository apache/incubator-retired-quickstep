/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
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

  const int ret = glob(pattern.c_str(), 0, nullptr, &glob_result);
  std::size_t num_matches = 0;
  if (ret == GLOB_ABORTED) {
    LOG(ERROR) << "An error occurred during glob.";
  } else if (ret == GLOB_NOMATCH) {
    LOG(WARNING) << "Glob function made zero matches.";
  } else if (ret == GLOB_NOSPACE) {
    LOG(ERROR) << "Glob function failed to allocate memory.";
  } else {
    // There should be no errors at this point.
    DCHECK_EQ(ret, 0);
    num_matches = glob_result.gl_pathc;
  }

  for (std::size_t i = 0; i < num_matches; ++i) {
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
