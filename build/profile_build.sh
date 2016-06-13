#!/bin/bash
# This script may be useful for developers to profile the build process itself.
#
# This shell script runs CMake and make, dumping all output into a log file.
# It also logs the CPU usage and memory information during the build.
# All log messages are timestamped to enable profiling.
#
# Dependencies:
# - ts
# sudo apt-get install -y libtime-duration-perl moreutils
# - vmstat and mpstat
# sudo apt-get install -y sysstat
#
# Usage: ./profile_build.sh
# Set the CMake and make command you want to use below.
# If CMakeLists.txt is detected, the script exits because in-source-tree build
# is not supported.
# If CMakeCache.txt is detected, the script skips cmake and runs make only.

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

set -e

CMAKE_COMMAND="cmake \
-D BUILD_SHARED_LIBS=On \
-D USE_TCMALLOC=0 \
-D CMAKE_BUILD_TYPE=Debug \
.. "
MAKE_COMMAND="make VERBOSE=1"
LOG_FILENAME=build.log

# Continuously dump memory usage and cpu load info to files for later analysis
function start_stat_collectors {
  rm -f stats_*.txt
  vmstat -SM 3 | ts "%.s (%H:%M:%S)" > stats_mem.txt 3>&1 &
  PID_vmstat=$!
  mpstat 3 | ts "%.s (%H:%M:%S)" > stats_cpu.txt 2>&1  &
  PID_mpstat=$!
}

function kill_stat_collectors {
  kill $PID_vmstat
  kill $PID_mpstat
  exit
}

function check_directory {
  if [[ -f CMakeLists.txt ]]; then
    echo "Running the build in the source tree is not supported."
    exit 1
  fi
}

function log_repo_version_info {
  git log master... >> $LOG_FILENAME
  git diff master >> $LOG_FILENAME
}

function run_cmake {
  if [[ ! -f CMakeCache.txt ]]; then
    echo "$CMAKE_COMMAND" | tee -a $LOG_FILENAME
    $CMAKE_COMMAND 2>&1 | ts "%.s (%H:%M:%S)" | tee -a $LOG_FILENAME
  else
    echo "CMakeCache.txt detected. Not running CMake again."
  fi
}

function run_make {
  echo "$MAKE_COMMAND" | tee -a $LOG_FILENAME
  $MAKE_COMMAND 2>&1 | ts "%.s (%H:%M:%S)" | tee -a $LOG_FILENAME
}

function print_stats {
  avg_mem=`grep -v r stats_mem.txt | tr -s ' ' | awk -F " " '{s+= $6; c++} END {print s/c/1024}'`
  echo -e "\n\n"
  echo "Average memory used was $avg_mem GB"  | tee -a $LOG_FILENAME

  time_taken=`expr $END_TIME - $START_TIME`
  mins=`expr $time_taken / 60`
  secs=`expr $time_taken % 60`
  echo "Time taken was ${mins}m ${secs}s" | tee -a $LOG_FILENAME
}

check_directory
echo "Starting build in " `pwd`  >> $LOG_FILENAME
start_stat_collectors
trap kill_stat_collectors SIGHUP SIGINT SIGTERM

START_TIME=`date +"%s"`
run_cmake
run_make
END_TIME=`date +"%s"`
kill_stat_collectors
print_stats
