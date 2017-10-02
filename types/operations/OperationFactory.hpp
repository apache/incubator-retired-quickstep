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

#ifndef QUICKSTEP_TYPES_OPERATIONS_OPERATION_FACTORY_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_OPERATION_FACTORY_HPP_

#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>

#include "types/GenericValue.hpp"
#include "types/TypeID.hpp"
#include "types/operations/Operation.hpp"
#include "types/operations/OperationSignature.hpp"
#include "types/operations/binary_operations/BinaryOperation.hpp"
#include "types/operations/unary_operations/UnaryOperation.hpp"
#include "utility/HashPair.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class Type;

/** \addtogroup Types
 *  @{
 */

class OperationFactory {
 public:
  static const OperationFactory& Instance();

  inline bool hasOperation(const std::string &operation_name,
                           const std::size_t arity) const {
    const auto indices_it =
        primary_index_.find(std::make_pair(operation_name, arity));
    return indices_it != primary_index_.end();
  }

  inline OperationPtr getOperation(const OperationSignaturePtr &op_signature) const {
    DCHECK(operations_.find(op_signature) != operations_.end());
    return operations_.at(op_signature);
  }

  inline OperationPtr getOperation(const std::string &operation_name,
                                   const std::vector<TypeID> &argument_type_ids,
                                   const std::size_t num_static_arguments = 0) const {
    return getOperation(
        OperationSignature::Create(
            operation_name, argument_type_ids, num_static_arguments));
  }

  inline UnaryOperationPtr getUnaryOperation(
      const OperationSignaturePtr &op_signature) const {
    const OperationPtr operation = getOperation(op_signature);
    DCHECK(operation->getOperationSuperTypeID() == Operation::kUnaryOperation);
    return std::static_pointer_cast<const UnaryOperation>(operation);
  }

  inline UnaryOperationPtr getUnaryOperation(
      const std::string &operation_name,
      const std::vector<TypeID> &argument_type_ids,
      const std::size_t num_static_arguments = 0) const {
    return getUnaryOperation(
        OperationSignature::Create(
            operation_name, argument_type_ids, num_static_arguments));
  }

  inline BinaryOperationPtr getBinaryOperation(
      const OperationSignaturePtr &op_signature) const {
    const OperationPtr operation = getOperation(op_signature);
    DCHECK(operation->getOperationSuperTypeID() == Operation::kBinaryOperation);
    return std::static_pointer_cast<const BinaryOperation>(operation);
  }

  inline BinaryOperationPtr getBinaryOperation(
      const std::string &operation_name,
      const std::vector<TypeID> &argument_type_ids,
      const std::size_t num_static_arguments = 0) const {
    return getBinaryOperation(
        OperationSignature::Create(
            operation_name, argument_type_ids, num_static_arguments));
  }

  OperationSignaturePtr resolveOperation(
      const std::string &operation_name,
      const std::shared_ptr<const std::vector<const Type*>> &argument_types,
      const std::shared_ptr<const std::vector<GenericValue>> &static_arguments,
      std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      std::string *message) const;

 private:
  OperationFactory();

  template <typename OperationT>
  void registerOperation();

  template <typename FunctorPackT>
  void registerFunctorPack();

  void registerOperationInternal(const OperationPtr &operation);

  using PartialSignature = std::pair<const std::vector<TypeID>*, std::size_t>;

  struct PartialSignatureLess {
    inline bool operator()(const PartialSignature &lhs,
                           const PartialSignature &rhs) const {
      int cmp_code = static_cast<int>(lhs.first->size())
                         - static_cast<int>(lhs.first->size());
      if (cmp_code != 0) {
        return cmp_code < 0;
      }
      for (std::size_t i = 0; i < lhs.first->size(); ++i) {
        cmp_code = static_cast<int>(lhs.first->at(i))
                       - static_cast<int>(rhs.first->at(i));
        if (cmp_code != 0) {
          return cmp_code < 0;
        }
      }
      return lhs.second > rhs.second;
    }
  };

  using PartialSignatureIndex = std::map<PartialSignature,
                                         OperationSignaturePtr,
                                         PartialSignatureLess>;

  enum class ResolveStatus {
    kSuccess = 0,
    kError,
    kNotFound
  };

  ResolveStatus resolveOperationWithFullTypeMatch(
      const PartialSignatureIndex &secondary_index,
      const std::vector<TypeID> &argument_type_ids,
      const std::vector<const Type*> &argument_types,
      const std::vector<GenericValue> &static_arguments,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      OperationSignaturePtr *resolved_op_signature,
      std::string *message) const;

  ResolveStatus resolveOperationWithPartialTypeMatch(
      const PartialSignatureIndex &secondary_index,
      const std::vector<TypeID> &argument_type_ids,
      const std::vector<const Type*> &argument_types,
      const std::vector<GenericValue> &static_arguments,
      std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      OperationSignaturePtr *resolved_op_signature,
      std::string *message) const;

//  ResolveStatus resolveOperationGeneric(
//      const std::set<OperationSignaturePtr> signatures,
//      const std::vector<TypeID> &argument_type_ids,
//      const std::vector<const Type*> &argument_types,
//      const std::vector<GenericValue> &static_arguments,
//      std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
//      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
//      OperationSignaturePtr *op_signature,
//      std::string *message) const;

  bool canApplyOperationTo(const OperationPtr operation,
                           const std::vector<const Type*> &argument_types,
                           const std::vector<GenericValue> &static_arguments,
                           std::string *message) const;

  std::unordered_map<OperationSignaturePtr,
                     OperationPtr,
                     OperationSignatureHash,
                     OperationSignatureEqual> operations_;

  std::unordered_map<std::pair<std::string, std::size_t>,
                     PartialSignatureIndex> primary_index_;

  DISALLOW_COPY_AND_ASSIGN(OperationFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_OPERATION_FACTORY_HPP_
