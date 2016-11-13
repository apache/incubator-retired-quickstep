/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "utility/NetworkUtil.hpp"

#include "utility/UtilityConfig.h"  // For QUICKSTEP_HAVE_WINDOWS.

#ifndef QUICKSTEP_HAVE_WINDOWS
#include <arpa/inet.h>
#include <net/if.h>  // Need to include before <ifaddrs.h> for NetBSD.
#include <netinet/in.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netdb.h>

#include <cstring>
#endif  // QUICKSTEP_HAVE_WINDOWS

#include <string>

#include "gflags/gflags.h"
#include "glog/logging.h"

namespace quickstep {

DEFINE_bool(use_ethernet_ip, false,
            "Use the Ethernet ip address, instead of the loopback.");

std::string GetIpv4Address() {
#ifndef QUICKSTEP_HAVE_WINDOWS
  struct ifaddrs *ifaddrs;
  CHECK_EQ(0, getifaddrs(&ifaddrs));

  char ip_address[NI_MAXHOST] = { '\0' };

  for (struct ifaddrs *ifa = ifaddrs; ifa; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == nullptr ||
        ifa->ifa_addr->sa_family != AF_INET ||
        (ifa->ifa_flags & IFF_UP) == 0) {
      continue;
    }

    if (FLAGS_use_ethernet_ip && !std::strncmp(ifa->ifa_name, "lo", 2)) {
      // NOTE: On Linux it starts with 'eth', while on Mac OS X, 'en'.
      continue;
    }

    struct sockaddr_in *s4 = (struct sockaddr_in *)(ifa->ifa_addr);
    CHECK(inet_ntop(AF_INET, reinterpret_cast<void *>(&(s4->sin_addr)), ip_address, sizeof(ip_address)) != nullptr);
    break;
  }

  freeifaddrs(ifaddrs);

  return ip_address;
#else
  CHECK(!FLAGS_use_ethernet_ip);
  return kLocalIpv4Address;
#endif  // QUICKSTEP_HAVE_WINDOWS
}

}  // namespace quickstep
