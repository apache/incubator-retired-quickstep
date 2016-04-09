/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_AGGREGATE_FUNCTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_AGGREGATE_FUNCTION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregateFunction;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class AggregateFunction;
typedef std::shared_ptr<const AggregateFunction> AggregateFunctionPtr;

/**
 * @brief Represents an aggregate and its arguments in the optimizer. This
 *        class wraps some of the functionality from
 *        quickstep::AggregateFunction and represents a particular instance
 *        of an aggregate during query optimization.
 **/
class AggregateFunction : public Expression {
 public:
  /**
   * @brief Destructor.
   */
  ~AggregateFunction() override {}

  ExpressionType getExpressionType() const override {
    return ExpressionType::kAggregateFunction;
  }

  std::string getName() const override {
    return "AggregateFunction";
  }

  const Type& getValueType() const override;

  bool isConstant() const override {
    // Aggregate function is never considered as a constant expression.
    return false;
  }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  /**
   * @return Whether the type of the return value is nullable.
   **/
  bool isNullable() const;

  /**
   * @return True if this aggregate is a vector aggregate (i.e. with GROUP BY).
   **/
  inline bool is_vector_aggregate() const {
    return is_vector_aggregate_;
  }

  /**
   * @return The AggregateFunction singleton (from the expression system)
   *         for this node.
   **/
  inline const ::quickstep::AggregateFunction& getAggregate() const {
    return aggregate_;
  }

  /**
   * @return The list of scalar arguments to this aggregate.
   **/
  inline const std::vector<ScalarPtr>& getArguments() const {
    return arguments_;
  }

  /*
   * @return Whether this is a DISTINCT aggregation.
   */
  inline bool is_distinct() const {
    return is_distinct_;
  }

  /**
   * @brief Create a new AggregateFunction.
   *
   * @warning It is an error to call this with arguments that the given
   *          aggregate can not apply to.
   *
   * @param aggregate The underlying AggregateFunction from the expression
   *        system.
   * @param arguments A list of arguments to the aggregate function.
   * @param is_vector_aggregate Whether the aggregate has a GROUP BY clause.
   * @param is_distinct Whether this is a DISTINCT aggregation.
   * @return A new AggregateFunctionPtr.
   **/
  static AggregateFunctionPtr Create(const ::quickstep::AggregateFunction &aggregate,
                                     const std::vector<ScalarPtr> &arguments,
                                     const bool is_vector_aggregate,
                                     const bool is_distinct);

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  /**
   * @brief Constructor.
   *
   * @param aggregate The actual AggregateFunction to use.
   * @param arguments The scalar arguments to the aggregate function.
   * @param is_vector_aggregate Indicates whether this aggregation is a vector
   *                            aggregation (i.e. GROUP BY exists).
   * @param is_distinct Indicates whether this is a DISTINCT aggregation.
   */
  AggregateFunction(const ::quickstep::AggregateFunction &aggregate,
                    const std::vector<ScalarPtr> &arguments,
                    const bool is_vector_aggregate,
                    const bool is_distinct)
      : aggregate_(aggregate),
        arguments_(arguments),
        is_vector_aggregate_(is_vector_aggregate),
        is_distinct_(is_distinct) {
    for (const ScalarPtr &child : arguments_) {
      addChild(child);
    }
  }

  const ::quickstep::AggregateFunction &aggregate_;
  std::vector<ScalarPtr> arguments_;
  const bool is_vector_aggregate_;
  const bool is_distinct_;

  DISALLOW_COPY_AND_ASSIGN(AggregateFunction);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_AGGREGATE_FUNCTION_HPP_ */
