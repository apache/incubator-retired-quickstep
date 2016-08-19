/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_UTILITY_EVENT_PROFILER_HPP_
#define QUICKSTEP_UTILITY_EVENT_PROFILER_HPP_

#include <chrono>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <map>
#include <ostream>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include "threading/Mutex.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

using clock = std::chrono::steady_clock;

template <typename TagT, typename ...PayloadT>
class EventProfiler {

 public:
  EventProfiler()
      : zero_time_(clock::now()) {
  }

  struct EventInfo {
    clock::time_point start_time;
    clock::time_point end_time;
    bool is_finished;
    std::tuple<PayloadT...> payload;

    explicit EventInfo(const clock::time_point &start_time_in)
        : start_time(start_time_in),
          is_finished(false) {
    }

    EventInfo()
        : start_time(clock::now()),
          is_finished(false) {
    }

    inline void setPayload(PayloadT &&...in_payload) {
      payload = std::make_tuple(in_payload...);
    }

    inline void endEvent() {
      end_time = clock::now();
      is_finished = true;
    }
  };

  struct EventContainer {
    EventContainer()
        : context(0) {}

    inline void startEvent(const TagT &tag) {
      events[tag].emplace_back(clock::now());
    }

    inline void endEvent(const TagT &tag) {
      auto &event_info = events.at(tag).back();
      event_info.is_finished = true;
      event_info.end_time = clock::now();
    }

    inline std::vector<EventInfo> *getEventLine(const TagT &tag) {
      return &events[tag];
    }

    inline void setContext(int context_in) {
      context = context_in;
    }

    inline int getContext() const {
      return context;
    }

    std::map<TagT, std::vector<EventInfo>> events;
    int context;
  };

  EventContainer *getContainer() {
    MutexLock lock(mutex_);
    return &thread_map_[std::this_thread::get_id()];
  }

  void writeToStream(std::ostream &os) const {
    time_t rawtime;
    time(&rawtime);
    char event_id[32];
    strftime(event_id, sizeof event_id, "%Y-%m-%d %H:%M:%S", localtime(&rawtime));

    int thread_id = 0;
    for (const auto &thread_ctx : thread_map_) {
      for (const auto &event_group : thread_ctx.second.events) {
        for (const auto &event_info : event_group.second) {
          CHECK(event_info.is_finished) << "Unfinished profiling event";

          os << std::setprecision(12)
             << event_id << ","
             << thread_id << "," << event_group.first << ",";

          PrintTuple(os, event_info.payload, ",");

          os << std::chrono::duration<double>(event_info.start_time - zero_time_).count()
             << ","
             << std::chrono::duration<double>(event_info.end_time - zero_time_).count()
             << "\n";
        }
      }
      ++thread_id;
    }
  }

  void clear() {
    zero_time_ = clock::now();
    thread_map_.clear();
  }

  const std::map<std::thread::id, EventContainer> &containers() {
    return thread_map_;
  }

  const clock::time_point &zero_time() {
    return zero_time_;
  }

 private:
  template<class Tuple, std::size_t N>
  struct TuplePrinter {
    static void Print(std::ostream &os, const Tuple &t, const std::string &sep) {
      TuplePrinter<Tuple, N-1>::Print(os, t, sep);
      os << std::get<N-1>(t) << sep;
    }
  };

  template<class Tuple>
  struct TuplePrinter<Tuple, 1> {
    static void Print(std::ostream &os, const Tuple &t, const std::string &sep) {
      os << std::get<0>(t) << sep;
    }
  };

  template<class... Args>
  static void PrintTuple(std::ostream &os, const std::tuple<Args...>& t, const std::string &sep) {
    TuplePrinter<decltype(t), sizeof...(Args)>::Print(os, t, sep);
  }

  clock::time_point zero_time_;
  std::map<std::thread::id, EventContainer> thread_map_;
  Mutex mutex_;
};

extern EventProfiler<std::string, std::size_t, std::size_t> simple_profiler;
extern EventProfiler<std::size_t> relop_profiler;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_EVENT_PROFILER_HPP_
