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

#include "types/operations/utility/CastUtil.hpp"

#include <vector>

#include "types/TypeID.hpp"
#include "types/TypeRegistrar.hpp"
#include "types/TypeUtil.hpp"
#include "types/operations/unary_operations/CastFunctorOverloads.hpp"
#include "utility/meta/Common.hpp"
#include "utility/meta/TypeList.hpp"

namespace quickstep {

namespace {

template <typename T>
struct GetTypeClass {
  using type = typename TypeIDTrait<T::value>::TypeClass;
};

template <typename TL>
struct HasCast : meta::IsCompleteType<
    CastFunctor<typename TL::template at<0>, typename TL::template at<1>>> {};

template <typename TL>
struct GetSourceTypeID {
  inline TypeID operator()() const {
    return TL::head::kStaticTypeID;
  }
};

}  // namespace

std::vector<TypeID> CastUtil::GetCoercibleSourceTypeIDs(const TypeID target_type_id) {
  return InvokeOnTypeID(
      target_type_id,
      [](auto tid) -> std::vector<TypeID> {
    using TargetType = typename TypeIDTrait<decltype(tid)::value>::TypeClass;
    return TypeIDSequenceAll::template bind_to<meta::TypeList>
                            ::template map<GetTypeClass>
                            ::template cartesian_product<meta::TypeList<TargetType>>
                            ::template filter<HasCast>
                            ::template Instantiate<std::vector<TypeID>, GetSourceTypeID>();
  });
}

}  // namespace quickstep
