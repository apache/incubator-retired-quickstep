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

#include "types/operations/OperationFactory.hpp"

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "types/GenericValue.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/TypeUtil.hpp"
#include "types/operations/Operation.hpp"
#include "types/operations/OperationSignature.hpp"
#include "types/operations/binary_operations/ArithmeticBinaryFunctors.hpp"
#include "types/operations/binary_operations/AsciiStringBinaryFunctors.hpp"
#include "types/operations/binary_operations/BinaryOperationSynthesizer.hpp"
#include "types/operations/binary_operations/CMathBinaryFunctors.hpp"
#include "types/operations/unary_operations/ArithmeticUnaryFunctors.hpp"
#include "types/operations/unary_operations/AsciiStringUnaryFunctors.hpp"
#include "types/operations/unary_operations/CMathUnaryFunctors.hpp"
#include "types/operations/unary_operations/CastOperation.hpp"
#include "types/operations/unary_operations/DateExtractOperation.hpp"
#include "types/operations/unary_operations/SubstringOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationSynthesizer.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

namespace quickstep {

namespace {

struct FunctorPackDispatcher {
  template <typename FunctorT>
  inline static std::list<OperationPtr> Generate(
      std::enable_if_t<FunctorT::kOperationSuperTypeID == Operation::kUnaryOperation>* = 0) {
    return { std::make_shared<const UnaryOperationSynthesizer<FunctorT>>() };
  }

  template <typename FunctorT>
  inline static std::list<OperationPtr> Generate(
      std::enable_if_t<FunctorT::kOperationSuperTypeID == Operation::kBinaryOperation>* = 0) {
    return { std::make_shared<const BinaryOperationSynthesizer<FunctorT>>() };
  }

  template <typename FunctorT>
  inline static std::list<OperationPtr> Generate(
      decltype(FunctorT::template GenerateOperations<FunctorPackDispatcher>())* = 0) {
    return FunctorT::template GenerateOperations<FunctorPackDispatcher>();
  }
};

// TODO(refactor-type): Remove this.
inline static std::shared_ptr<const std::vector<TypedValue>> ToTypedValue(
    const std::vector<GenericValue> &input) {
  std::vector<TypedValue> values;
  for (const auto &item : input) {
    values.emplace_back(item.toTypedValue());
  }
  return std::make_shared<const std::vector<TypedValue>>(std::move(values));
}

}  // namespace

OperationFactory::OperationFactory() {
  registerOperation<CastOperation>();
  registerOperation<DateExtractOperation>();
  registerOperation<SubstringOperation>();

  registerFunctorPack<ArithmeticUnaryFunctorPack>();
  registerFunctorPack<AsciiStringUnaryFunctorPack>();
  registerFunctorPack<CMathUnaryFunctorPack>();

  registerFunctorPack<ArithmeticBinaryFunctorPack>();
  registerFunctorPack<AsciiStringBinaryFunctorPack>();
  registerFunctorPack<CMathBinaryFunctorPack>();

  initializeTypeIDSafeCoercibility();
}

bool OperationFactory::HasOperation(const std::string &operation_name,
                                    const std::size_t arity) {
  const auto &primary_index = Instance().primary_index_;
  const auto indices_it =
      primary_index.find(std::make_pair(operation_name, arity));
  return indices_it != primary_index.end();
}

bool OperationFactory::HasOperation(const OperationSignaturePtr &op_signature) {
  const auto &operations = Instance().operations_;
  return operations.find(op_signature) != operations.end();
}

bool OperationFactory::CanApplyUnaryOperation(
    const std::string &operation_name,
    const Type &type,
    const std::vector<GenericValue> &static_arguments) {
  std::vector<TypeID> argument_type_ids = {type.getTypeID()};
  std::vector<TypedValue> static_tv_arguments;
  for (const auto &value : static_arguments) {
    argument_type_ids.emplace_back(value.getTypeID());
    // TODO(refactor-type): Remove this.
    static_tv_arguments.emplace_back(value.toTypedValue());
  }
  const OperationSignaturePtr op_signature =
      OperationSignature::Create(
          operation_name, argument_type_ids, static_arguments.size());
  if (!HasOperation(op_signature)) {
    return false;
  }
  return GetUnaryOperation(op_signature)->canApplyTo(type, static_tv_arguments);
}

bool OperationFactory::CanApplyBinaryOperation(
    const std::string &operation_name,
    const Type &left, const Type &right,
    const std::vector<GenericValue> &static_arguments) {
  std::vector<TypeID> argument_type_ids = {left.getTypeID(), right.getTypeID()};
  std::vector<TypedValue> static_tv_arguments;
  for (const auto &value : static_arguments) {
    argument_type_ids.emplace_back(value.getTypeID());
    // TODO(refactor-type): Remove this.
    static_tv_arguments.emplace_back(value.toTypedValue());
  }
  // TODO(refactor-type): Handle this.
  DCHECK_EQ(0u, static_arguments.size());
  const OperationSignaturePtr op_signature =
      OperationSignature::Create(
          operation_name, argument_type_ids, static_arguments.size());
  if (!HasOperation(op_signature)) {
    return false;
  }
  return GetBinaryOperation(op_signature)->canApplyTo(left, right, static_tv_arguments);
}

OperationPtr OperationFactory::GetOperation(
    const OperationSignaturePtr &op_signature) {
  DCHECK(HasOperation(op_signature));
  return Instance().operations_.at(op_signature);
}


UnaryOperationPtr OperationFactory::GetUnaryOperation(
    const OperationSignaturePtr &op_signature) {
  const OperationPtr operation = GetOperation(op_signature);
  DCHECK(operation->getOperationSuperTypeID() == Operation::kUnaryOperation);
  return std::static_pointer_cast<const UnaryOperation>(operation);
}

BinaryOperationPtr OperationFactory::GetBinaryOperation(
    const OperationSignaturePtr &op_signature) {
  const OperationPtr operation = GetOperation(op_signature);
  DCHECK(operation->getOperationSuperTypeID() == Operation::kBinaryOperation);
  return std::static_pointer_cast<const BinaryOperation>(operation);
}

OperationSignaturePtr OperationFactory::ResolveOperation(
    const std::string &operation_name,
    const std::shared_ptr<const std::vector<const Type*>> &argument_types,
    const std::shared_ptr<const std::vector<GenericValue>> &static_arguments,
    std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
    std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
    std::string *message) {
  return Instance().resolveOperation(operation_name,
                                     argument_types,
                                     static_arguments,
                                     coerced_argument_types,
                                     coerced_static_arguments,
                                     message);
}


OperationSignaturePtr OperationFactory::resolveOperation(
    const std::string &operation_name,
    const std::shared_ptr<const std::vector<const Type*>> &argument_types,
    const std::shared_ptr<const std::vector<GenericValue>> &static_arguments,
    std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
    std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
    std::string *message) const {
  const std::string lower_case_name = ToLower(operation_name);
  const std::size_t arity = argument_types->size();
  const auto &indices_it =
      primary_index_.find(std::make_pair(lower_case_name, arity));

  if (indices_it == primary_index_.end()) {
    *message = "Unrecognized function " + operation_name
                   + " with " + std::to_string(arity) + " arguments";
  }

  ResolveStatus status;
  OperationSignaturePtr op_signature = nullptr;

  std::vector<TypeID> argument_type_ids;
  for (const auto *type : *argument_types) {
    argument_type_ids.emplace_back(type->getTypeID());
  }

  // First, try full exact matching.
  status = resolveOperationWithFullTypeMatch(indices_it->second.partial_signature_index,
                                             argument_type_ids,
                                             *argument_types,
                                             *static_arguments,
                                             coerced_static_arguments,
                                             &op_signature,
                                             message);
  if (status == ResolveStatus::kSuccess) {
    DCHECK(op_signature != nullptr);
    *coerced_argument_types = argument_types;
    return op_signature;
  } else if (status == ResolveStatus::kError) {
    return nullptr;
  }

  // Otherwise, try partial (non-static arguments) exact matching.
  status = resolveOperationWithPartialTypeMatch(indices_it->second.static_arity_index,
                                                argument_type_ids,
                                                *argument_types,
                                                *static_arguments,
                                                coerced_argument_types,
                                                coerced_static_arguments,
                                                &op_signature,
                                                message);
  if (status == ResolveStatus::kSuccess) {
    DCHECK(op_signature != nullptr);
    return op_signature;
  } else if (status == ResolveStatus::kError) {
    return nullptr;
  }

  // TODO(refactor-type): More informative error message.
  *message = "Unexpected argument types for function " + operation_name;
  return nullptr;
}

OperationFactory::ResolveStatus OperationFactory::resolveOperationWithFullTypeMatch(
    const PartialSignatureIndex &partial_signature_index,
    const std::vector<TypeID> &argument_type_ids,
    const std::vector<const Type*> &argument_types,
    const std::vector<GenericValue> &static_arguments,
    std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
    OperationSignaturePtr *resolved_op_signature,
    std::string *message) const {
  const std::size_t max_num_static_arguments = static_arguments.size();
  auto it = partial_signature_index.lower_bound(
      PartialSignature(&argument_type_ids, max_num_static_arguments));

  if (it != partial_signature_index.end() &&
      *it->first.argument_type_ids == argument_type_ids) {
    const OperationSignaturePtr op_signature = it->second;
    const OperationPtr operation = operations_.at(op_signature);

    *coerced_static_arguments =
        std::make_shared<const std::vector<GenericValue>>(
            static_arguments.begin()
                + (max_num_static_arguments - op_signature->getNumStaticArguments()),
            static_arguments.end());

    if (canApplyOperationTo(operation,
                            argument_types,
                            **coerced_static_arguments,
                            message)) {
      *resolved_op_signature = op_signature;
      return ResolveStatus::kSuccess;
    } else {
      return ResolveStatus::kError;
    }
  }

  return ResolveStatus::kNotFound;
}

OperationFactory::ResolveStatus OperationFactory::resolveOperationWithPartialTypeMatch(
    const StaticArityIndex &static_arity_index,
    const std::vector<TypeID> &argument_type_ids,
    const std::vector<const Type*> &argument_types,
    const std::vector<GenericValue> &static_arguments,
    std::shared_ptr<const std::vector<const Type*>> *coerced_argument_types,
    std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
    OperationSignaturePtr *resolved_op_signature,
    std::string *message) const {
  const std::size_t arity = argument_types.size();
  const std::size_t max_num_static_arguments = static_arguments.size();
  const std::size_t first_static_argument_position = arity - max_num_static_arguments;

  const OperationSignaturePtr *best_match_op_sig = nullptr;
  const std::vector<TypeID> *best_match_type_ids = nullptr;
  std::size_t best_match_num_static_arguments = max_num_static_arguments;

  auto it = static_arity_index.lower_bound(max_num_static_arguments);
  while (it != static_arity_index.end()) {
    if (it->first != best_match_num_static_arguments) {
      if (best_match_op_sig != nullptr) {
        break;
      } else {
        best_match_num_static_arguments = it->first;
      }
    }

    const auto &current_type_ids = it->second->getArgumentTypeIDs();

    if (canSafelyCoerceTypes(argument_type_ids, current_type_ids)) {
      if (best_match_type_ids == nullptr) {
        DCHECK(best_match_op_sig == nullptr);
        best_match_op_sig = &it->second;
        best_match_type_ids = &current_type_ids;
      } else {
        const bool cur_to_best_safe =
            canSafelyCoerceTypes(current_type_ids, *best_match_type_ids);
        const bool best_to_cur_safe =
            canSafelyCoerceTypes(*best_match_type_ids, current_type_ids);
        if (cur_to_best_safe == best_to_cur_safe) {
          *message = "Ambiguous call to overloaded function " +
                     it->second->getName() + ", candidates: " +
                     (*best_match_op_sig)->toString() + " v.s. " +
                     it->second->toString();
          return ResolveStatus::kError;
        }
        if (cur_to_best_safe) {
          best_match_op_sig = &it->second;
          best_match_type_ids = &current_type_ids;
        }
      }
    }

    ++it;
  }

  if (best_match_op_sig == nullptr) {
    return ResolveStatus::kNotFound;
  }

  DCHECK(best_match_type_ids != nullptr);
  DCHECK_EQ(arity, best_match_type_ids->size());
  std::vector<const Type*> coerced_arg_types;
  for (std::size_t i = 0; i < arity; ++i) {
    const Type &source_type = *argument_types[i];
    const TypeID target_type_id = (*best_match_type_ids)[i];
    // TODO(refactor-type): Figure out how to better handle this.
    if (source_type.getTypeID() == target_type_id) {
      coerced_arg_types.emplace_back(&source_type);
    } else if (TypeUtil::GetMemoryLayout(target_type_id) == kCxxInlinePod) {
      coerced_arg_types.emplace_back(
          &TypeFactory::GetType(target_type_id, source_type.isNullable()));
    } else if (target_type_id == kChar && source_type.getTypeID() == kVarChar) {
      coerced_arg_types.emplace_back(
          &CharType::Instance(source_type.isNullable(),
                              source_type.maximumByteLength() - 1));
    } else if (target_type_id == kVarChar && source_type.getTypeID() == kChar) {
      coerced_arg_types.emplace_back(
          &VarCharType::Instance(source_type.isNullable(),
                                 source_type.maximumByteLength() + 1));
    } else if (target_type_id == kText) {
      coerced_arg_types.emplace_back(
          &TextType::Instance(source_type.isNullable()));
    } else {
      LOG(FATAL) << "Unexpected casting";
    }
  }

  std::vector<GenericValue> coerced_static_args;
  for (std::size_t i = arity - best_match_num_static_arguments; i < arity; ++i) {
    const auto &value = static_arguments[i - first_static_argument_position];
    if (coerced_arg_types[i]->equals(*argument_types[i])) {
      coerced_static_args.emplace_back(value);
    } else {
      coerced_static_args.emplace_back(value.coerce(*coerced_arg_types[i]));
    }
  }

  const OperationPtr operation = operations_.at(*best_match_op_sig);
  // TODO(refactor-type): Fix.
  if (canApplyOperationTo(operation,
                          coerced_arg_types,
                          coerced_static_args,
                          message)) {
    *coerced_argument_types =
        std::make_shared<const std::vector<const Type*>>(std::move(coerced_arg_types));
    *coerced_static_arguments =
        std::make_shared<const std::vector<GenericValue>>(std::move(coerced_static_args));
    *resolved_op_signature = *best_match_op_sig;
    return ResolveStatus::kSuccess;
  }

  return ResolveStatus::kNotFound;
}

bool OperationFactory::canSafelyCoerceTypes(const std::vector<TypeID> &source_type_ids,
                                            const std::vector<TypeID> &target_type_ids) const {
  if (source_type_ids.size() != target_type_ids.size()) {
    return false;
  }
  for (std::size_t i = 0; i < source_type_ids.size(); ++i) {
    const TypeID source_id = source_type_ids[i];
    const TypeID target_id = target_type_ids[i];
    if (source_id != target_id) {
      const auto it = type_id_safe_coercibility_.find(
          std::make_pair(source_id, target_id));
      if (it == type_id_safe_coercibility_.end()) {
        return false;
      }
    }
  }
  return true;
}

void OperationFactory::initializeTypeIDSafeCoercibility() {
  // TODO(refactor-type): Figure out how to better handle this.
  const std::vector<TypeID> cxx_inline_pod_type_ids =
      TypeUtil::GetTypeIDVectorOfMemoryLayout<kCxxInlinePod>();

  std::vector<const Type*> cxx_line_pod_types;
  for (const TypeID tid : cxx_inline_pod_type_ids) {
    cxx_line_pod_types.emplace_back(&TypeFactory::GetType(tid, true));
  }

  for (const Type *source_type : cxx_line_pod_types) {
    for (const Type *target_type : cxx_line_pod_types) {
      if (target_type->isSafelyCoercibleFrom(*source_type)) {
        type_id_safe_coercibility_.emplace(
            std::make_pair(source_type->getTypeID(), target_type->getTypeID()));
      }
    }
  }

  for (const TypeID source_id : std::vector<TypeID>({kChar, kVarChar})) {
    for (const TypeID target_id : std::vector<TypeID>({kChar, kVarChar, kText})) {
      type_id_safe_coercibility_.emplace(std::make_pair(source_id, target_id));
    }
  }
  type_id_safe_coercibility_.emplace(std::make_pair(kText, kText));
}

bool OperationFactory::canApplyOperationTo(
    const OperationPtr operation,
    const std::vector<const Type*> &argument_types,
    const std::vector<GenericValue> &static_arguments,
    std::string *message) const {
  switch (operation->getOperationSuperTypeID()) {
    case Operation::kUnaryOperation: {
      const UnaryOperationPtr unary_operation =
          std::static_pointer_cast<const UnaryOperation>(operation);
      return unary_operation->canApplyTo(*argument_types[0],
                                         *ToTypedValue(static_arguments),
                                         message);
    }
    case Operation::kBinaryOperation: {
      const BinaryOperationPtr binary_operation =
          std::static_pointer_cast<const BinaryOperation>(operation);
      return binary_operation->canApplyTo(*argument_types[0],
                                          *argument_types[1],
                                          *ToTypedValue(static_arguments),
                                          message);
    }
    default: {
      const auto operation_id =
         static_cast<std::underlying_type_t<Operation::OperationSuperTypeID>>(
             operation->getOperationSuperTypeID());
      LOG(FATAL) << "Unknown opeation super type id: " << operation_id;
    }
  }
}


const OperationFactory& OperationFactory::Instance() {
  static OperationFactory instance;
  return instance;
}

template <typename OperationT>
void OperationFactory::registerOperation() {
  registerOperationInternal(std::make_shared<const OperationT>());
}

template <typename FunctorPackT>
void OperationFactory::registerFunctorPack() {
  for (const OperationPtr &operation :
           FunctorPackT::template GenerateOperations<FunctorPackDispatcher>()) {
    registerOperationInternal(operation);
  }
}

void OperationFactory::registerOperationInternal(const OperationPtr &operation) {
  DCHECK(operation->getOperationSuperTypeID() == Operation::kUnaryOperation ||
         operation->getOperationSuperTypeID() == Operation::kBinaryOperation);

  for (const OperationSignaturePtr op_sig_orig : operation->getSignatures()) {
    DCHECK(operation->getOperationSuperTypeID() != Operation::kUnaryOperation ||
           op_sig_orig->getNonStaticArity() == 1u);
    DCHECK(operation->getOperationSuperTypeID() != Operation::kBinaryOperation ||
           op_sig_orig->getNonStaticArity() == 2u);

    const OperationSignaturePtr op_sig =
        OperationSignature::Create(ToLower(op_sig_orig->getName()),
                                   op_sig_orig->getArgumentTypeIDs(),
                                   op_sig_orig->getNumStaticArguments());

    // TODO: print error message for collision
    operations_.emplace(op_sig, operation);
    primary_index_[std::make_pair(op_sig->getName(),
                                  op_sig->getArity())].addSignature(op_sig);
  }
}


// ----------------------------------------------------------------------------
// Implemenation of utility short-cuts.

bool OperationFactory::CanApplyCastOperation(const Type &source_type,
                                             const Type &target_type) {
  const GenericValue target_meta_type_value =
      GenericValue::CreateWithLiteral(MetaType::InstanceNonNullable(),
                                      &target_type);
  return CanApplyUnaryOperation("cast", source_type, {target_meta_type_value});
}

UnaryOperationPtr OperationFactory::GetCastOperation(const TypeID source_id) {
  const OperationSignaturePtr op_signature =
      OperationSignature::Create("cast", {source_id, kMetaType}, 1);
  DCHECK(HasOperation(op_signature));
  return GetUnaryOperation(op_signature);
}

bool OperationFactory::CanApplyAddOperation(const Type &left, const Type &right) {
  return CanApplyBinaryOperation("+", left, right);
}

BinaryOperationPtr OperationFactory::GetAddOperation(const TypeID left_id,
                                                     const TypeID right_id) {
  return GetBinaryOperation(OperationSignature::Create("+", {left_id, right_id}, 0));
}

bool OperationFactory::CanApplySubtractOperation(const Type &left, const Type &right) {
  return CanApplyBinaryOperation("-", left, right);
}

BinaryOperationPtr OperationFactory::GetSubtractOperation(const TypeID left_id,
                                                          const TypeID right_id) {
  return GetBinaryOperation(OperationSignature::Create("-", {left_id, right_id}, 0));
}

bool OperationFactory::CanApplyMultiplyOperation(const Type &left, const Type &right) {
  return CanApplyBinaryOperation("*", left, right);
}

BinaryOperationPtr OperationFactory::GetMultiplyOperation(const TypeID left_id,
                                                          const TypeID right_id) {
  return GetBinaryOperation(OperationSignature::Create("*", {left_id, right_id}, 0));
}

bool OperationFactory::CanApplyDivideOperation(const Type &left, const Type &right) {
  return CanApplyBinaryOperation("/", left, right);
}

BinaryOperationPtr OperationFactory::GetDivideOperation(const TypeID left_id,
                                                        const TypeID right_id) {
  return GetBinaryOperation(OperationSignature::Create("/", {left_id, right_id}, 0));
}

}  // namespace quickstep
