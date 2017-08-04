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

#include "utility/BulkIoConfiguration.hpp"

#include "glog/logging.h"

namespace quickstep {

void BulkIoConfiguration::initializeDefaultParameters(const BulkIoFormat format) {
  switch (format) {
    case BulkIoFormat::kCsv: {
      delimiter_ = ',';
      escape_strings_ = false;
      header_ = true;
      quote_ = '"';
      null_string_ = "";
      break;
    }
    case BulkIoFormat::kText: {
      delimiter_ = '\t';
      escape_strings_ = true;
      header_ = false;
      quote_ = 0;
      null_string_ = "\\N";
      break;
    }
    default:
      LOG(FATAL) << "Unexpected format in "
                 << "BulkIoConfiguration::initializeDefaultParameters()";
  }
}

}  // namespace quickstep
