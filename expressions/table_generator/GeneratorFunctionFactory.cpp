/**
 *   Copyright 2016 Pivotal Software, Inc.
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

#include "expressions/table_generator/GeneratorFunctionFactory.hpp"

#include <string>

#include "expressions/table_generator/GenerateSeries.hpp"
#include "expressions/table_generator/GenerateSeriesHandle.hpp"
#include "expressions/table_generator/GeneratorFunctionHandle.hpp"
#include "types/TypedValue.hpp"

#include "glog/logging.h"

namespace quickstep {

const GeneratorFunction* GeneratorFunctionFactory::GetByName(const std::string &name) {
  if (name == "generate_series") {
    return &GenerateSeries::Instance();
  } else {
    return nullptr;
  }
}

GeneratorFunctionHandlePtr GeneratorFunctionFactory::ReconstructFromProto(
    const serialization::GeneratorFunctionHandle &proto) {
  const GeneratorFunction *func_template = GetByName(proto.function_name());
  if (func_template == nullptr) {
    LOG(FATAL) << "Generator function " << proto.function_name() << " not found";
  }
 
  std::vector<const TypedValue> args;
  for (const auto& arg_proto : proto.args()) {
    args.emplace_back(std::move(TypedValue::ReconstructFromProto(arg_proto)));
  }
 
  return func_template->concretize(args);
}
 
}  // namespace quickstep
