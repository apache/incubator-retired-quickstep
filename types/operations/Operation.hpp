/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_TYPES_OPERATIONS_OPERATION_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_OPERATION_HPP_

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

/**
 * @brief An operation which can be applied to typed values. Each exact
 *        concrete Operation is a singleton.
 **/
class Operation {
 public:
  /**
   * @brief Categories of intermediate supertypes of Operations.
   **/
  enum OperationSuperTypeID {
    kComparison = 0,
    kUnaryOperation,
    kBinaryOperation,
    kNumOperationSuperTypeIDs  // Not a real OperationSuperTypeID, exists for counting purposes.
  };

  /**
   * @brief Names of operation super-types in the same order as
   *        OperationSuperTypeID.
   * @note Defined out-of-line in Comparison.cpp
   **/
  static const char *kOperationSuperTypeNames[kNumOperationSuperTypeIDs];

  /**
   * @brief Virtual destructor.
   **/
  virtual ~Operation() {
  }

  /**
   * @brief Determine what supertype this Operation belongs to.
   *
   * @return The ID of the supertype this Operation belongs to.
   **/
  inline OperationSuperTypeID getOperationSuperTypeID() const {
    return super_type_id_;
  }

  /**
   * @brief Get the name of this Operation.
   *
   * @return The human-readable name of this Operation.
   **/
  inline const char* getName() const {
    return name_;
  }

  /**
   * @brief Get the short name of this Operation (i.e. a mathematical symbol).
   *
   * @return The short name of this Operation.
   **/
  inline const char* getShortName() const {
    return short_name_;
  }

  /**
   * @brief Determine whether this Operation is exactly the same as another.
   * @note Because all exact operations are singletons, a simple pointer
   *       equality-check is usable here, but this method should be used in
   *       case this behavior might change in the future.
   *
   * @param other The Operation to check for equality.
   * @return Whether this Operation and other are the same.
   **/
  bool equals(const Operation &other) const {
    return (this == &other);
  }

 protected:
  Operation(const OperationSuperTypeID super_type_id,
            const char *name,
            const char *short_name)
      : super_type_id_(super_type_id),
        name_(name),
        short_name_(short_name) {
  }

 private:
  const OperationSuperTypeID super_type_id_;
  const char *name_;
  const char *short_name_;


  DISALLOW_COPY_AND_ASSIGN(Operation);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_OPERATION_HPP_
