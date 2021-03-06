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

#ifndef QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_FACTORY_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_FACTORY_HPP_

#include "types/operations/binary_operations/BinaryOperationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class BinaryOperation;
namespace serialization { class BinaryOperation; }

/** \addtogroup Types
 *  @{
 */

/**
 * @brief All-static factory object that provides access to BinaryOperations.
 **/
class BinaryOperationFactory {
 public:
  /**
   * @brief Convenience factory method to get a pointer to a BinaryOperation
   *        from that BinaryOperation's ID.
   *
   * @param id The ID of the desired BinaryOperation.
   * @return The BinaryOperation corresponding to id.
   **/
  static const BinaryOperation& GetBinaryOperation(const BinaryOperationID id);

  /**
   * @brief Get a reference to a BinaryOperation from that BinaryOperation's
   *        serialized Protocol Buffer representation.
   *
   * @param proto A serialized Protocol Buffer representation of a
   *        BinaryOperation, originally generated by getProto().
   * @return The BinaryOperation described by proto.
   **/
  static const BinaryOperation& ReconstructFromProto(const serialization::BinaryOperation &proto);

  /**
   * @brief Check whether a serialization::BinaryOperation is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a
   *        BinaryOperation, originally generated by getProto().
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::BinaryOperation &proto);

 private:
  BinaryOperationFactory();

  DISALLOW_COPY_AND_ASSIGN(BinaryOperationFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_BINARY_OPERATIONS_BINARY_OPERATION_FACTORY_HPP_
