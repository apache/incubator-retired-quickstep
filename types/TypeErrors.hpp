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

#ifndef QUICKSTEP_TYPES_TYPE_ERRORS_HPP_
#define QUICKSTEP_TYPES_TYPE_ERRORS_HPP_

#include <cstdarg>
#include <exception>
#include <string>

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief Exception thrown when attempting to apply an Operation to arguments
 *        of unsupported Types.
 **/
class OperationInapplicableToType : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param op The name of the Operation which failed to apply.
   * @param num_types The number of arguments to op.
   * @param ... Pointers to c-string type names of the arguments supplied to op.
   **/
  OperationInapplicableToType(const std::string &op, const int num_types, ...)
      : message_("OperationInapplicableToType: Operation ") {
    message_.append(op);
    message_.append(" can not be applied to type");
    if (num_types == 1) {
      message_.append(" ");
    } else {
      message_.append("s ");
    }

    std::va_list types;
    va_start(types, num_types);
    for (int i = 0; i < num_types; ++i) {
      const char *type_name = va_arg(types, const char*);
      message_.append(type_name);
      if (i != num_types - 1) {
        message_.append(", ");
      }
    }
    va_end(types);
  }

  ~OperationInapplicableToType() throw() {}

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_TYPE_ERRORS_HPP_
