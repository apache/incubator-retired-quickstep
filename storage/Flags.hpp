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

#ifndef QUICKSTEP_STORAGE_FLAGS_HPP_
#define QUICKSTEP_STORAGE_FLAGS_HPP_

#include "storage/StorageConfig.h"  // For QUICKSTEP_HAVE_FILE_MANAGER_HDFS.

#include "gflags/gflags_declare.h"

namespace quickstep {

/** \addtogroup STORAGE
 *  @{
 */

/**
 * @brief A collection of common flags shared by Quickstep STORAGEs in both the
 * single-node and the distributed version.
 **/

#ifdef QUICKSTEP_HAVE_FILE_MANAGER_HDFS
DECLARE_bool(use_hdfs);

DECLARE_string(hdfs_namenode_host);
DECLARE_int32(hdfs_namenode_port);
DECLARE_int32(hdfs_num_replications);
#endif

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_FLAGS_HPP_
