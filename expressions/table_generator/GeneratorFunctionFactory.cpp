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

#include "expressions/table_generator/GeneratorFunctionFactory.hpp"

#include <string>
#include <utility>
#include <vector>

#include "expressions/table_generator/GenerateSeries.hpp"
#include "expressions/table_generator/GeneratorFunction.hpp"
#include "expressions/table_generator/GeneratorFunction.pb.h"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

GeneratorFunctionFactory::GeneratorFunctionFactory() {
#define REGISTER_GENERATOR_FUNCTION_(FuncClass) \
  func_map_.emplace(FuncClass::Instance().getName(), &FuncClass::Instance())

  // Register all generator functions here.
  REGISTER_GENERATOR_FUNCTION_(GenerateSeries);

#undef REGISTER_GENERATOR_FUNCTION_
}

const GeneratorFunctionFactory& GeneratorFunctionFactory::Instance() {
  static GeneratorFunctionFactory instance;
  return instance;
}

const GeneratorFunction* GeneratorFunctionFactory::getByName(const std::string &name) const {
  const auto it = func_map_.find(name);
  if (it != func_map_.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}

GeneratorFunctionHandle* GeneratorFunctionFactory::reconstructFromProto(
    const serialization::GeneratorFunctionHandle &proto) const {
  const GeneratorFunction *func_template = getByName(proto.function_name());
  CHECK(func_template != nullptr)
      << "Generator function " << proto.function_name() << " not found";

  std::vector<TypedValue> args;
  for (const auto &arg_proto : proto.args()) {
    args.emplace_back(TypedValue::ReconstructFromProto(arg_proto));
  }

  return func_template->createHandle(args);
}

}  // namespace quickstep
