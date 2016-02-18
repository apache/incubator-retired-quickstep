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

#ifndef QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HANDLE_HPP_
#define QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HANDLE_HPP_

#include <memory>
#include <sstream>

#include "expressions/table_generator/GeneratorFunction.pb.h"
#include "types/containers/ColumnVectorsValueAccessor.hpp"
#include "types/Type.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

class GeneratorFunctionHandle;
typedef std::shared_ptr<const GeneratorFunctionHandle> GeneratorFunctionHandlePtr;

/**
 * @brief Abstract representation of a concretized generator function.
 **/
class GeneratorFunctionHandle {
 public:
  /**
   * @brief Get the human-readable name of this GeneratorFunction.
   *
   * @return The human-readable name of this GeneratorFunction.
   **/
  virtual std::string getName() const = 0;
 
  virtual int getNumberOfOutputColumns() const = 0;
 
  virtual std::string getOutputColumnName(int index) const {
    std::ostringstream oss;
    oss << "attr" << (index+1);
    return oss.str();
  }
 
  virtual const Type &getOutputColumnType(int index) const = 0;

  virtual void populateColumns(ColumnVectorsValueAccessor *results) const = 0;

  serialization::GeneratorFunctionHandle getProto() const {
    serialization::GeneratorFunctionHandle proto;
    proto.set_function_name(getName());

    for (const TypedValue &arg : orig_args_) {
      proto.add_args()->CopyFrom(arg.getProto());
    }
    return proto;
  }

 protected:
  GeneratorFunctionHandle(const std::vector<const TypedValue> &orig_args)
      : orig_args_(orig_args) {
  }

 private:
  std::vector<const TypedValue> orig_args_;

  DISALLOW_COPY_AND_ASSIGN(GeneratorFunctionHandle);
};

/** @} */

}  // namespace quickstep

#endif /* QUICKSTEP_EXPRESSIONS_TABLE_GENERATOR_GENERATOR_FUNCTION_HANDLE_HPP_ */
