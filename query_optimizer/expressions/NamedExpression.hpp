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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_NAMED_EXPRESSION_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_NAMED_EXPRESSION_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Scalar.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class NamedExpression;
typedef std::shared_ptr<const NamedExpression> NamedExpressionPtr;

/**
 * @brief Expression with an expression ID and name, including Alias and
 *        AttributeReference. A NamedExpression can be referenced by other
 *        expressions.
 */
class NamedExpression : public Scalar {
 public:
  ~NamedExpression() override {}

  /**
   * @return The expression ID.
   */
  inline ExprId id() const { return id_; }

  /**
   * @return The (internal) attribute name.
   */
  inline const std::string& attribute_name() const { return attribute_name_; }

  /**
   * @return The attribute alias.
   */
  inline const std::string& attribute_alias() const { return attribute_alias_; }

  /**
   * @return The relation name. May be empty.
   */
  inline const std::string& relation_name() const { return relation_name_; }

  /**
   * @brief Compares this named expression with \p other. Two named expressions
   *        are equal if they have the same ExprId and are both Alias or
   *        AttributeReference.
   *
   * @param other Another named expression to compare with.
   * @return True if the named expression is equal to \p other.
   */
  inline bool equals(const NamedExpressionPtr &other) const {
    return getExpressionType() == other->getExpressionType() &&
           id_ == other->id();
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param id ExprId assigned to this expression.
   * @param attribute_name The (internal) attribute name.
   * @param attribute_alias The attribute name for display.
   * @param relation_name A relation name indicating where it comes from. Can be
   *                      empty. This is only for the printing purpose only.
   *                      When this named expression is converted to a
   *                      CatalogAttribute, the relation name may not be
   *                      \p relation_name.
   */
  NamedExpression(const ExprId id,
                  const std::string &attribute_name,
                  const std::string &attribute_alias,
                  const std::string &relation_name)
      : id_(id),
        attribute_name_(attribute_name),
        attribute_alias_(attribute_alias),
        relation_name_(relation_name) {}

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  ExprId id_;
  std::string attribute_name_;
  std::string attribute_alias_;
  std::string relation_name_;

  DISALLOW_COPY_AND_ASSIGN(NamedExpression);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_NAMED_EXPRESSION_HPP_ */
