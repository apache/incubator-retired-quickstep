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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_WINDOW_AGGREGATE_FUNCTION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_WINDOW_AGGREGATE_FUNCTION_HPP_

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

struct WindowFrameInfo {
  /**
   * @brief Cosntructor.
   *
   * @param is_row_in True if this window frame is defined by ROWS, false if
   *                  defined by RANGE.
   * @param num_preceding_in The number of preceding tuples the window frame
   *                         will cover, -1 means UNBOUNDED.
   * @param num_following_in The number of following tuples the window frame
   *                         will cover, -1 means UNBOUNDED.
   **/
  WindowFrameInfo(const bool is_row_in,
                  const int num_preceding_in,
                  const int num_following_in)
      : is_row(is_row_in),
        num_preceding(num_preceding_in),
        num_following(num_following_in) {}

  const bool is_row;
  const int num_preceding;
  const int num_following;
};

struct WindowInfo {
  /**
   * @brief Constructor.
   *
   * @param partition_by_attributes_in The partition keys for the window.
   * @param order_by_attributes_in The order keys for the window.
   * @param order_by_directions_in The order direction for order key.
   * @param nulls_first_in The nulls' position for order key.
   * @param frame_info_in The window frame information for the window. Null
   *        means there is no explicit window frame definition.
   **/
  WindowInfo(const std::vector<AttributeReferencePtr> &partition_by_attributes_in,
             const std::vector<AttributeReferencePtr> &order_by_attributes_in,
             const std::vector<bool> &order_by_directions_in,
             const std::vector<bool> &nulls_first_in,
             const WindowFrameInfo *frame_info_in)
      : partition_by_attributes(partition_by_attributes_in),
        order_by_attributes(order_by_attributes_in),
        order_by_directions(order_by_directions_in),
        nulls_first(nulls_first_in),
        frame_info(frame_info_in) {}

  const std::vector<AttributeReferencePtr> partition_by_attributes;
  const std::vector<AttributeReferencePtr> order_by_attributes;
  const std::vector<bool> order_by_directions;
  const std::vector<bool> nulls_first;
  const WindowFrameInfo *frame_info;
};

class WindowAggregateFunction;
typedef std::shared_ptr<const WindowAggregateFunction> WindowAggregateFunctionPtr;

/**
 * @brief Represents a window aggregate function and its arguments in the
 *        optimizer. This class wraps some of the functionality from
 *        quickstep::AggregateFunction and represents a particular instance
 *        of an aggregate during query optimization.
 **/
class WindowAggregateFunction : public Expression {
 public:
  /**
   * @brief Destructor.
   */
  ~WindowAggregateFunction() override {}

  ExpressionType getExpressionType() const override {
    return ExpressionType::kWindowAggregateFunction;
  }

  std::string getName() const override {
    return "WindowAggregateFunction";
  }

  const Type& getValueType() const override;

  bool isConstant() const override {
    // Window aggregate function is never considered as a constant expression.
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
   * @return The WindowAggregateFunction singleton (from the expression system)
   *         for this node.
   **/
  inline const ::quickstep::AggregateFunction& window_aggregate() const {
    return window_aggregate_;
  }

  /**
   * @return The list of scalar arguments to this aggregate.
   **/
  inline const std::vector<ScalarPtr>& arguments() const {
    return arguments_;
  }

  /**
   * @return The window info of this window aggregate function.
   **/
  inline const WindowInfo window_info() const {
    return window_info_;
  }

  /**
   * @return The name of the window.
   **/
  inline const std::string window_name() const {
    return window_name_;
  }

  /**
   * @return Whether this is a DISTINCT aggregation.
   **/
  inline bool is_distinct() const {
    return is_distinct_;
  }

  /**
   * @brief Create a new WindowAggregateFunction by directly defined window.
   *
   * @warning It is an error to call this with arguments that the given
   *          aggregate can not apply to.
   *
   * @param aggregate The underlying WindowAggregateFunction from the expression
   *        system.
   * @param arguments A list of arguments to the window aggregate function.
   * @param window_info The window info of the window aggregate function.
   * @param is_distinct Whether this is a DISTINCT aggregation.
   * @return A new AggregateFunctionPtr.
   **/
  static WindowAggregateFunctionPtr Create(const ::quickstep::AggregateFunction &window_aggregate,
                                           const std::vector<ScalarPtr> &arguments,
                                           const WindowInfo &window_info,
                                           const std::string &window_name,
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
   * @param window_aggregate The actual AggregateFunction to use.
   * @param arguments A list of arguments to the window aggregate function.
   * @param window_info The window info of the window aggregate function.
   * @param is_distinct Indicates whether this is a DISTINCT aggregation.
   */
  WindowAggregateFunction(const ::quickstep::AggregateFunction &window_aggregate,
                          const std::vector<ScalarPtr> &arguments,
                          const WindowInfo &window_info,
                          const std::string &window_name,
                          const bool is_distinct)
      : window_aggregate_(window_aggregate),
        arguments_(arguments),
        window_info_(window_info),
        window_name_(window_name),
        is_distinct_(is_distinct) {
    for (const ScalarPtr &child : arguments_) {
      addChild(child);
    }
  }

  // TODO(Shixuan): Currently this class uses AggregationFunction as
  // window_aggregate_. If it really needs to be seperated from the
  // AggregationFunction, a new class for WindowAggregationFunction should be
  // created as quickstep::WindowAggregateFunction.
  const ::quickstep::AggregateFunction &window_aggregate_;
  std::vector<ScalarPtr> arguments_;
  const WindowInfo window_info_;
  const std::string window_name_;
  const bool is_distinct_;

  DISALLOW_COPY_AND_ASSIGN(WindowAggregateFunction);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_WINDOW_AGGREGATE_FUNCTION_HPP_
