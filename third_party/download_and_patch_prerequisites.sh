#!/bin/bash

# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements.  See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership.  The ASF licenses this file
# to you under the Apache License, Version 2.0 (the
# "License"); you may not use this file except in compliance
# with the License.  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
# KIND, either express or implied.  See the License for the
# specific language governing permissions and limitations
# under the License.

# Check if the prerequisite bash programs exist on the system.
for cmd in curl tar; do
  echo -n "Checking if ${cmd} is present ..."
  if ! hash ${cmd} 2>/dev/null; then
    echo ""
    echo "ERROR: Program ${cmd} is not installed on the system."
    exit 1
  else
    echo "ok"
  fi
done

THIRD_PARTY_DIR=`pwd`
if [ "${PWD##*/}" != "third_party" ]; then
  echo "ERROR: This script can be run only from the third party directory"
  exit 1
fi

THIRD_PARTY_SRC_DIR=${THIRD_PARTY_DIR}/src
if [ ! -d "$THIRD_PARTY_SRC_DIR" ]; then
  mkdir -p ${THIRD_PARTY_SRC_DIR}
fi

PATCH_DIR=${THIRD_PARTY_DIR}/patches
cd src/

third_party_dir_names=("benchmark"
                       "gflags"
                       "googletest"
                       "linenoise"
                       "re2"
                       "gperftools"
                       "glog"
                       )

third_party_lib_urls=("https://github.com/google/benchmark/archive/v1.3.0.tar.gz"
                      "https://github.com/gflags/gflags/archive/v2.1.2.tar.gz"
                      "https://github.com/google/googletest/archive/release-1.8.0.tar.gz"
                      "https://github.com/antirez/linenoise/archive/1.0.tar.gz"
                      "https://github.com/google/re2/archive/2017-01-01.tar.gz"
                      "https://github.com/gperftools/gperftools/releases/download/gperftools-2.5/gperftools-2.5.tar.gz"
                      "https://github.com/google/glog/archive/v0.3.5.tar.gz"
                      )

downloaded_archive_names=("v1.3.0.tar.gz"
                          "v2.1.2.tar.gz"
                          "release-1.8.0.tar.gz"
                          "1.0.tar.gz"
                          "2017-01-01.tar.gz"
                          "gperftools-2.5.tar.gz"
                          "v0.3.5.tar.gz"
                          )

tar_options=("-xzf"
             "-xzf"
             "-xzf"
             "-xzf"
             "-xzf"
             "-xzf"
             "-xzf"
             )

for ((lib_index=0; lib_index < ${#third_party_dir_names[*]}; lib_index++))
do
  # If the third party directory is not present, create it.
  if [ ! -d ${third_party_dir_names[lib_index]} ]; then
    mkdir ${third_party_dir_names[lib_index]}
  fi

  # Downaload the compressed archive for the third party library.
  curl_cmd="curl -L -O ${third_party_lib_urls[lib_index]}"
  echo "Downloading from ${third_party_lib_urls[lib_index]} ..."
  echo ${curl_cmd}
  eval ${curl_cmd}
  if [ -f ${downloaded_archive_names[lib_index]} ]; then
    echo "File ${downloaded_archive_names[lib_index]} downloaded successfully"

    # Uncompress the archive to its designated directory.
    # The strip-components option will ensure that all the files directly end up
    # in the desired directory, without any intermediate hierarchy level.
    tar_cmd="tar ${tar_options[lib_index]} ${downloaded_archive_names[lib_index]} -C ${third_party_dir_names[lib_index]} --strip-components=1"
    echo ${tar_cmd}
    echo "Extracting from ${downloaded_archive_names[lib_index]} ..."
    eval ${tar_cmd}

    # Delete the compressed archive.
    rm -rf ${downloaded_archive_names[lib_index]}
  else
    echo "Error downloading file ${downloaded_archive_names[lib_index]} from ${third_party_lib_urls[lib_index]}"
  fi
done

# Back to the third_party directory.
cd ${THIRD_PARTY_DIR}

# Apply patches now.

# Apply linenoise patch
cp ${PATCH_DIR}/linenoise/CMakeLists.txt ${THIRD_PARTY_SRC_DIR}/linenoise
patch ${THIRD_PARTY_SRC_DIR}/linenoise/linenoise.h ${PATCH_DIR}/linenoise/linenoise.h.patch
patch ${THIRD_PARTY_SRC_DIR}/linenoise/linenoise.c ${PATCH_DIR}/linenoise/linenoise.c.patch

# Apply gflags patch.
patch ${THIRD_PARTY_SRC_DIR}/gflags/CMakeLists.txt ${PATCH_DIR}/gflags/CMakeLists.patch
patch ${THIRD_PARTY_SRC_DIR}/gflags/src/gflags_reporting.cc ${PATCH_DIR}/gflags/gflags_reporting.cc.patch

# Apply re2 patch.
patch ${THIRD_PARTY_SRC_DIR}/re2/CMakeLists.txt ${PATCH_DIR}/re2/re2CMake.patch

# Apply benchmark patches.
patch ${THIRD_PARTY_SRC_DIR}/benchmark/CMakeLists.txt ${PATCH_DIR}/benchmark/CMakeLists.patch

# Apply glog patches.
patch ${THIRD_PARTY_SRC_DIR}/glog/CMakeLists.txt ${PATCH_DIR}/glog/glogCMakeLists.txt.patch
patch ${THIRD_PARTY_SRC_DIR}/glog/src/utilities.cc ${PATCH_DIR}/glog/utilities.cc.patch
