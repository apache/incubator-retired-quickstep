/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_BINARY_JOIN_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_BINARY_JOIN_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Join.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class BinaryJoin;
typedef std::shared_ptr<const BinaryJoin> BinaryJoinPtr;

/**
 * @brief Base class for binary join operators that take exactly two operators
 *        as input.
 */
class BinaryJoin : public Join {
 public:
  /**
   * @return Destructor.
   */
  ~BinaryJoin() override {}

  /**
   * @return The left logical join operator.
   */
  const LogicalPtr& left() const { return left_; }

  /**
   * @return The right logical join operator.
   */
  const LogicalPtr& right() const { return right_; }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override;

 protected:
  /**
   * @brief Constructor.
   *
   * @param left The left join operator.
   * @param right The right join operator.
   */
  BinaryJoin(const LogicalPtr &left,
             const LogicalPtr &right)
      : left_(left), right_(right) {
    addChild(left_);
    addChild(right_);
  }

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  LogicalPtr left_;
  LogicalPtr right_;

  DISALLOW_COPY_AND_ASSIGN(BinaryJoin);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_BINARY_JOIN_HPP_ */
