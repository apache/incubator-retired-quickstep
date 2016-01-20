/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PREDICATE_LITERAL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PREDICATE_LITERAL_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class PredicateLiteral;
typedef std::shared_ptr<const PredicateLiteral> PredicateLiteralPtr;

/**
 * @brief A boolean true or false literal.
 */
class PredicateLiteral : public Predicate {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kPredicateLiteral;
  }

  std::string getName() const override { return "Literal"; }

  bool isConstant() const override { return true; }

  bool is_true() const { return is_true_; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  ::quickstep::Predicate* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable PredicateLiteral.
   *
   * @param is_true Indicates whether the value is true or false.
   * @return An immutable PredicateLiteral with the given boolean value.
   */
  static PredicateLiteralPtr Create(bool is_true) {
    return PredicateLiteralPtr(new PredicateLiteral(is_true));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  explicit PredicateLiteral(bool is_true) : is_true_(is_true) {}

  bool is_true_;

  DISALLOW_COPY_AND_ASSIGN(PredicateLiteral);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_PREDICATE_LITERAL_HPP_ */
