/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_SUM_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_SUM_HPP_

#include <string>
#include <vector>

#include "expressions/aggregation/AggregateFunction.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregationHandle;
class Type;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief AggregateFunction representing SQL SUM.
 **/
class AggregateFunctionSum : public AggregateFunction {
 public:
  static const AggregateFunctionSum& Instance() {
    static AggregateFunctionSum instance;
    return instance;
  }

  std::string getName() const override {
    return "SUM";
  }

  bool canApplyToTypes(
      const std::vector<const Type*> &argument_types) const override;

  const Type* resultTypeForArgumentTypes(
      const std::vector<const Type*> &argument_types) const override;

  AggregationHandle* createHandle(
      const std::vector<const Type*> &argument_types) const override;

 private:
  AggregateFunctionSum()
      : AggregateFunction(AggregationID::kSum) {
  }

  DISALLOW_COPY_AND_ASSIGN(AggregateFunctionSum);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_SUM_HPP_
