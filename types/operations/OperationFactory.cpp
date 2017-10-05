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
#include "types/operations/binary_operations/BinaryOperationWrapper.hpp"
#include "types/operations/binary_operations/CMathBinaryFunctors.hpp"
#include "types/operations/unary_operations/ArithmeticUnaryFunctors.hpp"
#include "types/operations/unary_operations/AsciiStringUnaryFunctors.hpp"
#include "types/operations/unary_operations/CMathUnaryFunctors.hpp"
#include "types/operations/unary_operations/CastOperation.hpp"
#include "types/operations/unary_operations/DateExtractOperation.hpp"
#include "types/operations/unary_operations/SubstringOperation.hpp"
#include "types/operations/unary_operations/UnaryOperationWrapper.hpp"
#include "utility/StringUtil.hpp"

namespace quickstep {

namespace {

struct FunctorPackDispatcher {
  template <typename FunctorT>
  inline static std::list<OperationPtr> Generate(
      std::enable_if_t<FunctorT::kOperationSuperTypeID == Operation::kUnaryOperation>* = 0) {
    return { std::make_shared<const UnaryOperationWrapper<FunctorT>>() };
  }

  template <typename FunctorT>
  inline static std::list<OperationPtr> Generate(
      std::enable_if_t<FunctorT::kOperationSuperTypeID == Operation::kBinaryOperation>* = 0) {
    return { std::make_shared<const BinaryOperationWrapper<FunctorT>>() };
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
  const auto &secondary_index = indices_it->second;

  std::vector<TypeID> argument_type_ids;
  for (const auto *type : *argument_types) {
    argument_type_ids.emplace_back(type->getTypeID());
  }

  // First, try full exact matching.
  status = resolveOperationWithFullTypeMatch(secondary_index,
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
  status = resolveOperationWithPartialTypeMatch(secondary_index,
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

  // TODO
  *message = "Unexpected argument types for function " + operation_name;
  return nullptr;
}

OperationFactory::ResolveStatus OperationFactory::resolveOperationWithFullTypeMatch(
    const PartialSignatureIndex &secondary_index,
    const std::vector<TypeID> &argument_type_ids,
    const std::vector<const Type*> &argument_types,
    const std::vector<GenericValue> &static_arguments,
    std::shared_ptr<const std::vector<GenericValue>> *coerced_static_arguments,
    OperationSignaturePtr *resolved_op_signature,
    std::string *message) const {
  const std::size_t max_num_static_arguments = static_arguments.size();
  auto it = secondary_index.lower_bound(
      std::make_pair(&argument_type_ids, max_num_static_arguments));

  if (it != secondary_index.end() && *it->first.first == argument_type_ids) {
    const OperationSignaturePtr op_signature = it->second;
    const OperationPtr operation = getOperation(op_signature);

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
    const PartialSignatureIndex &secondary_index,
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

  auto it = secondary_index.lower_bound(
      std::make_pair(&argument_type_ids, max_num_static_arguments));
  while (it != secondary_index.end()) {
    const std::vector<TypeID> &expected_type_ids = *it->first.first;
    DCHECK_GE(expected_type_ids.size(), it->first.second);
    const std::size_t num_non_static_arguments =
        expected_type_ids.size() - it->first.second;

    if (!std::equal(expected_type_ids.begin(),
                    expected_type_ids.begin() + num_non_static_arguments,
                    argument_type_ids.begin())) {
      break;
    }

    // Coerce static arguments
    std::vector<GenericValue> coerced_static_args;

    bool is_coercible = true;
    for (std::size_t i = num_non_static_arguments; i < arity; ++i) {
      const Type &arg_type = *argument_types.at(i);
      const GenericValue &arg_value =
          static_arguments.at(i - first_static_argument_position);
      const TypeID &expected_type_id = expected_type_ids.at(i);

      if (arg_type.getTypeID() == expected_type_id) {
        coerced_static_args.emplace_back(arg_value);
      } else {
        const Type *expected_type = nullptr;
        if (TypeFactory::TypeRequiresLengthParameter(expected_type_id)) {
          // TODO: refactor type system to make this coercion extensible.
          if (expected_type_id == kChar && arg_type.getTypeID() == kVarChar) {
            expected_type = &TypeFactory::GetType(
                expected_type_id, arg_type.maximumByteLength() - 1);
          } else if (expected_type_id == kVarChar && arg_type.getTypeID() == kChar) {
            expected_type = &TypeFactory::GetType(
                expected_type_id, arg_type.maximumByteLength() + 1);
          }
        } else {
          expected_type = &TypeFactory::GetType(expected_type_id);
        }

        if (expected_type != nullptr && expected_type->isSafelyCoercibleFrom(arg_type)) {
          coerced_static_args.emplace_back(arg_value.coerce(*expected_type));
        } else {
          is_coercible = false;
          break;
        }
      }
    }

    if (is_coercible) {
      std::vector<const Type*> coerced_arg_types(
          argument_types.begin(),
          argument_types.begin() + num_non_static_arguments);
      for (const auto &value : coerced_static_args) {
        coerced_arg_types.emplace_back(&value.getType());
      }

      const OperationPtr operation = getOperation(it->second);
      if (canApplyOperationTo(operation,
                              coerced_arg_types,
                              coerced_static_args,
                              message)) {
        *coerced_argument_types =
            std::make_shared<const std::vector<const Type*>>(std::move(coerced_arg_types));
        *coerced_static_arguments =
            std::make_shared<const std::vector<GenericValue>>(std::move(coerced_static_args));
        *resolved_op_signature = it->second;
        return ResolveStatus::kSuccess;
      }
    }

    ++it;
  }

  return ResolveStatus::kNotFound;
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

    const PartialSignature sig_ref =
        std::make_pair(&op_sig->getArgumentTypeIDs(),
                       op_sig->getNumStaticArguments());
    primary_index_[std::make_pair(op_sig->getName(),
                                  op_sig->getArity())].emplace(sig_ref, op_sig);
  }
}


}  // namespace quickstep
