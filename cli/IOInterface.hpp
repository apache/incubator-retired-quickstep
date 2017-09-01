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

#ifndef QUICKSTEP_CLI_IO_INTERFACE_HPP_
#define QUICKSTEP_CLI_IO_INTERFACE_HPP_

#include <cstdio>
#include <string>

#include "utility/Macros.hpp"

namespace quickstep {

/**
 * @brief An individual IO interaction with Quickstep.
 */
class IOHandle {
 public:
  IOHandle() {}

  /**
   * @note Destructor should handle clean up of any IO state.
   */
  virtual ~IOHandle() {}

  /**
   * @return A file handle for standard output.
   */
  virtual FILE *out() = 0;

  /**
   * @return A file handle for error output.
   */
  virtual FILE *err() = 0;

  virtual std::string getCommand() const = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(IOHandle);
};

/**
 * @brief Virtual base defines a generic, file-based interface around IO.
 *        One IO interaction (eg a SQL query) will be assigned an IOHandle.
 *        On destruction of the IOHandle, the IO interaction has finished.
 */
class IOInterface {
 public:
  /**
   * @note Destructing the IOInterface should close any outstanding IO state
   *       (e.g. an open port).
   */
  virtual ~IOInterface() {}

  /**
   * @brief Retrieves the next IOHandle. Blocks if no IO ready.
   *
   * @return An IOHandle.
   */
  virtual IOHandle* getNextIOHandle() = 0;

 protected:
  IOInterface() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(IOInterface);
};

}  // namespace quickstep

#endif  // QUICKSTEP_CLI_IO_INTERFACE_HPP_
