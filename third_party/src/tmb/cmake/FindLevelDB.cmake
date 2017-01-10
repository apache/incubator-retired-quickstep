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

# Module to find LevelDB.

find_path(LEVELDB_INCLUDE_DIR leveldb/db.h)

find_library(LEVELDB_LIBRARY NAMES leveldb libleveldb)

set(LEVELDB_LIBRARIES ${LEVELDB_LIBRARY})
set(LEVELDB_INCLUDE_DIRS ${LEVELDB_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LevelDB DEFAULT_MSG
                                  LEVELDB_LIBRARY LEVELDB_INCLUDE_DIR)

mark_as_advanced(LEVELDB_INCLUDE_DIR LEVELDB_LIBRARY)
