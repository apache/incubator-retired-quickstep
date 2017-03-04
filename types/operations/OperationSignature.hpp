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

#ifndef QUICKSTEP_TYPES_OPERATIONS_OPERATION_SIGNATURE_HPP_
#define QUICKSTEP_TYPES_OPERATIONS_OPERATION_SIGNATURE_HPP_

#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "types/TypeID.hpp"
#include "types/operations/Operation.pb.h"

#include "utility/HashPair.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Types
 *  @{
 */

class OperationSignature;
typedef std::shared_ptr<const OperationSignature> OperationSignaturePtr;

class OperationSignature {
 public:
  serialization::OperationSignature getProto() const;

  static OperationSignaturePtr ReconstructFromProto(
      const serialization::OperationSignature &proto);

  inline const std::string& getName() const {
    return operation_name_;
  }

  inline std::size_t getArity() const {
    return argument_type_ids_.size();
  }

  inline std::size_t getNonStaticArity() const {
    return argument_type_ids_.size() - num_static_arguments_;
  }

  inline const std::vector<TypeID>& getArgumentTypeIDs() const {
    return argument_type_ids_;
  }

  inline std::size_t getNumStaticArguments() const {
    return num_static_arguments_;
  }

  inline bool operator==(const OperationSignature &r) const {
    return operation_name_ == r.operation_name_
        && argument_type_ids_ == r.argument_type_ids_
        && num_static_arguments_ == r.num_static_arguments_;
  }

  inline bool operator!=(const OperationSignature &r) const {
    return !(*this == r);
  }

  inline bool operator<(const OperationSignature &r) const {
    int cmp_code = operation_name_.compare(r.operation_name_);
    if (cmp_code != 0) {
      return cmp_code < 0;
    }
    cmp_code = static_cast<int>(getArity() - r.getArity());
    if (cmp_code != 0) {
      return cmp_code < 0;
    }
    cmp_code = static_cast<int>(num_static_arguments_ - r.num_static_arguments_);
    if (cmp_code != 0) {
      return cmp_code > 0;
    }
    for (std::size_t i = 0; i < getArity(); ++i) {
      const auto l_tid =
          static_cast<std::underlying_type_t<TypeID>>(argument_type_ids_.at(i));
      const auto r_tid =
          static_cast<std::underlying_type_t<TypeID>>(r.argument_type_ids_.at(i));
      if (l_tid != r_tid) {
        return l_tid < r_tid;
      }
    }
    return false;
  }

  inline std::size_t hash() const {
    std::size_t hash_code = std::hash<std::string>()(operation_name_);
    for (const TypeID tid : argument_type_ids_) {
      hash_code = CombineHashes(hash_code, static_cast<std::size_t>(tid));
    }
    hash_code = CombineHashes(hash_code, num_static_arguments_);
    return hash_code;
  }

  std::string toString() const;

  static OperationSignaturePtr Create(
      const std::string &operation_name,
      const std::vector<TypeID> &argument_type_ids,
      const std::size_t num_static_arguments) {
    return OperationSignaturePtr(
        new OperationSignature(operation_name,
                               argument_type_ids,
                               num_static_arguments));
  }

  static OperationSignaturePtr Create(
      const std::string &operation_name,
      const std::vector<TypeID> &regular_argument_type_ids,
      const std::vector<TypeID> &static_argument_type_ids) {
    std::vector<TypeID> argument_type_ids = regular_argument_type_ids;
    argument_type_ids.insert(argument_type_ids.end(),
                             static_argument_type_ids.begin(),
                             static_argument_type_ids.end());
    return OperationSignaturePtr(
        new OperationSignature(operation_name,
                               argument_type_ids,
                               static_argument_type_ids.size()));
  }

 private:
  OperationSignature(const std::string &operation_name,
                     const std::vector<TypeID> &argument_type_ids,
                     const std::size_t num_static_arguments)
      : operation_name_(operation_name),
        argument_type_ids_(argument_type_ids),
        num_static_arguments_(num_static_arguments) {
    DCHECK_GE(argument_type_ids_.size(), num_static_arguments_);
  }

  const std::string operation_name_;
  const std::vector<TypeID> argument_type_ids_;
  const std::size_t num_static_arguments_;

  DISALLOW_COPY_AND_ASSIGN(OperationSignature);
};

/**
 * @brief Implements the equal function for operation signatures.
 */
struct OperationSignatureEqual {
  inline bool operator()(const OperationSignaturePtr &lhs,
                         const OperationSignaturePtr &rhs) const {
    return *lhs == *rhs;
  }
};

/**
 * @brief Implements the hash function for operation signatures.
 */
struct OperationSignatureHash {
  inline std::size_t operator()(const OperationSignaturePtr &op_sig) const {
    return op_sig->hash();
  }
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_TYPES_OPERATIONS_OPERATION_SIGNATURE_HPP_
