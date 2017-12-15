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

THIRD_PARTY_DIR=`pwd`
if [ "${PWD##*/}" != "third_party" ]; then
  echo "ERROR: This script can be run only from the third party directory"
  exit 1
fi

THIRD_PARTY_SRC_DIR=${THIRD_PARTY_DIR}/src
cd ${THIRD_PARTY_SRC_DIR}

third_party_dir_names=("benchmark"
                       "gflags"
                       "googletest"
                       "linenoise"
                       "re2"
                       "gperftools"
                       "glog"
                       )

for ((lib_index=0; lib_index < ${#third_party_dir_names[*]}; lib_index++))
do
  rm -rf ${third_party_dir_names[lib_index]}/
done
