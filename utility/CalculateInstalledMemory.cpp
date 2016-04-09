/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "utility/CalculateInstalledMemory.hpp"
#include "utility/UtilityConfig.h"

#if defined(QUICKSTEP_HAVE_SYSCONF)
#include <unistd.h>
#elif defined(QUICKSTEP_HAVE_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "glog/logging.h"

namespace quickstep {
namespace utility {
namespace system {

bool calculateTotalMemoryInBytes(std::uint64_t *total_memory) {
#if defined(QUICKSTEP_HAVE_SYSCONF)
  const std::uint64_t num_pages = static_cast<std::uint64_t>(sysconf(_SC_PHYS_PAGES));
  const std::uint64_t page_size = static_cast<std::uint64_t>(sysconf(_SC_PAGE_SIZE));
  if (num_pages > 0 &&  page_size > 0) {
    *total_memory = num_pages * page_size;
    LOG(INFO) << "Total installed memory is " << *total_memory << " bytes\n";
    return true;
  }
  LOG(INFO) << "Could not compute the total available memory using sysconf\n";
  return false;
#elif defined(QUICKSTEP_HAVE_WINDOWS)
  MEMORYSTATUSEX mem_status;
  mem_status.dwLength = sizeof(mem_status);
  GlobalMemoryStatusEx(&mem_status);
  if (mem_status.ullTotalPhys > 0) {
    *total_memory = static_cast<std::uint64_t>(mem_status.ullTotalPhys);
    LOG(INFO) << "Total installed memory is " << *total_memory << " bytes\n";
    return true;
  }
  LOG(INFO) << "Could not compute the total installed memory using GlobalMemoryStatusEx\n";
  return false;
#else
// TODO(jmp): Expand to find other ways to calculate the installed memory.
#error "No implementation available to calculate the total installed memory."
#endif
}

}  // namespace system
}  // namespace utility
}  // namespace quickstep
