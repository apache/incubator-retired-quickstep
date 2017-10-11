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
  static bool HasOperation(const std::string &operation_name,
                           const std::size_t arity);

  static bool HasOperation(const OperationSignaturePtr &op_signature);

  static bool CanApplyUnaryOperation(
      const std::string &operation_name,
      const Type &type,
      const std::vector<GenericValue> &static_arguments = {});

  static bool CanApplyBinaryOperation(
      const std::string &operation_name,
      const Type &left, const Type &right,
      const std::vector<GenericValue> &static_arguments = {});

  /**
   * @brief Get the Operation for a specified operation signature.
   */
  static OperationPtr GetOperation(const OperationSignaturePtr &op_signature);


  /**
   * @brief Get the UnaryOperation for a specified operation signature.
   */
  static UnaryOperationPtr GetUnaryOperation(const OperationSignaturePtr &op_signature);

  /**
   * @brief Get the BinaryOperation for a specified operation signature.
   */
  static BinaryOperationPtr GetBinaryOperation(const OperationSignaturePtr &op_signature);

  /**
   * @brief Resolve an operation from its name and arguments.
   */
  static OperationSignaturePtr ResolveOperation(
      const std::string &operation_name,
      const std::shared_ptr<const std::vector<const Type*>> &argument_types,
      const std::shared_ptr<const std::vector<GenericValue>> &static_arguments,
      std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      std::string *message);


  // ---------------------------------------------------------------------------
  // Utility short-cuts.


  static bool CanApplyCastOperation(const Type &source_type, const Type &target_type);

  static UnaryOperationPtr GetCastOperation(const TypeID source_id);


  static bool CanApplyAddOperation(const Type &left, const Type &right);

  static BinaryOperationPtr GetAddOperation(const TypeID left_id,
                                            const TypeID right_id);


  static bool CanApplySubtractOperation(const Type &left, const Type &right);

  static BinaryOperationPtr GetSubtractOperation(const TypeID left_id,
                                                 const TypeID right_id);


  static bool CanApplyMultiplyOperation(const Type &left,  const Type &right);

  static BinaryOperationPtr GetMultiplyOperation(const TypeID left_id,
                                                 const TypeID right_id);

  static bool CanApplyDivideOperation(const Type &left, const Type &right);

  static BinaryOperationPtr GetDivideOperation(const TypeID left_id,
                                               const TypeID right_id);

 private:
  OperationFactory();

  static const OperationFactory& Instance();

  template <typename OperationT>
  void registerOperation();

  template <typename FunctorPackT>
  void registerFunctorPack();

  void registerOperationInternal(const OperationPtr &operation);

  struct PartialSignature {
    PartialSignature(const std::vector<TypeID> *argument_type_ids_in,
                     const std::size_t num_static_arguments_in)
        : argument_type_ids(argument_type_ids_in),
          num_static_arguments(num_static_arguments_in) {
    }
    inline bool operator<(const PartialSignature &rhs) const {
      const PartialSignature &lhs = *this;
      const std::vector<TypeID> &lhs_ids = *lhs.argument_type_ids;
      const std::vector<TypeID> &rhs_ids = *rhs.argument_type_ids;
      int cmp_code = static_cast<int>(lhs_ids.size()) - static_cast<int>(rhs_ids.size());
      if (cmp_code != 0) {
        return cmp_code < 0;
      }
      for (std::size_t i = 0; i < lhs.argument_type_ids->size(); ++i) {
        cmp_code = static_cast<int>(lhs_ids[i]) - static_cast<int>(rhs_ids[i]);
        if (cmp_code != 0) {
          return cmp_code < 0;
        }
      }
      return lhs.num_static_arguments > rhs.num_static_arguments;
    }
    const std::vector<TypeID> *argument_type_ids;
    const std::size_t num_static_arguments;
  };

  using PartialSignatureIndex = std::map<PartialSignature,
                                         OperationSignaturePtr>;
  using StaticArityIndex = std::multimap<std::size_t,
                                         OperationSignaturePtr,
                                         std::greater<std::size_t>>;

  struct SecondaryIndex {
    inline void addSignature(const OperationSignaturePtr &op_sig) {
      partial_signature_index.emplace(
          PartialSignature(&op_sig->getArgumentTypeIDs(),
                           op_sig->getNumStaticArguments()),
          op_sig);
      static_arity_index.emplace(op_sig->getNumStaticArguments(), op_sig);
    }
    PartialSignatureIndex partial_signature_index;
    StaticArityIndex static_arity_index;
  };

  enum class ResolveStatus {
    kSuccess = 0,
    kError,
    kNotFound
  };

  OperationSignaturePtr resolveOperation(
      const std::string &operation_name,
      const std::shared_ptr<const std::vector<const Type*>> &argument_types,
      const std::shared_ptr<const std::vector<GenericValue>> &static_arguments,
      std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      std::string *message) const;

  ResolveStatus resolveOperationWithFullTypeMatch(
      const PartialSignatureIndex &partial_signature_index,
      const std::vector<TypeID> &argument_type_ids,
      const std::vector<const Type*> &argument_types,
      const std::vector<GenericValue> &static_arguments,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      OperationSignaturePtr *resolved_op_signature,
      std::string *message) const;

  ResolveStatus resolveOperationWithPartialTypeMatch(
      const StaticArityIndex &static_arity_index,
      const std::vector<TypeID> &argument_type_ids,
      const std::vector<const Type*> &argument_types,
      const std::vector<GenericValue> &static_arguments,
      std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
      std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
      OperationSignaturePtr *resolved_op_signature,
      std::string *message) const;

  bool canSafelyCoerceTypes(const std::vector<TypeID> &source_type_ids,
                            const std::vector<TypeID> &target_type_ids) const;

  void initializeTypeIDSafeCoercibility();

  bool canApplyOperationTo(const OperationPtr operation,
                           const std::vector<const Type*> &argument_types,
                           const std::vector<GenericValue> &static_arguments,
                           std::string *message) const;

  std::unordered_map<OperationSignaturePtr,
                     OperationPtr,
                     OperationSignatureHash,
                     OperationSignatureEqual> operations_;

  std::unordered_map<std::pair<std::string, std::size_t>,
                     SecondaryIndex> primary_index_;

  std::unordered_set<std::pair<TypeID, TypeID>> type_id_safe_coercibility_;

  DISALLOW_COPY_AND_ASSIGN(OperationFactory);
};



/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_OPERATION_FACTORY_HPP_
