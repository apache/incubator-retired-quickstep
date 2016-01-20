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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_ATTRIBUTE_REFERENCE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_ATTRIBUTE_REFERENCE_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExpressionType.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Type;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class AttributeReference;
typedef std::shared_ptr<const AttributeReference> AttributeReferencePtr;

/**
 * @brief Reference to the attribute created by an Alias or to an attribute in a
 *        base relation.
 */
class AttributeReference : public NamedExpression {
 public:
  ExpressionType getExpressionType() const override {
    return ExpressionType::kAttributeReference;
  }

  std::string getName() const override { return "AttributeReference"; }

  const Type& getValueType() const override { return type_; }

  bool isConstant() const override { return false; }

  ExpressionPtr copyWithNewChildren(
      const std::vector<ExpressionPtr> &new_children) const override;

  std::vector<AttributeReferencePtr> getReferencedAttributes() const override;

  ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map) const override;

  /**
   * @brief Creates an immutable AttributReference.
   *
   * @param attribute_id The ExprId of the expression this AttributeReference
   *                     references to.
   * @param attribute_name The attribute name. This is only for printing purpose.
   * @param attribute_alias The attribute alias (or display name). This is only
   *                        for printing purpose.
   * @param relation_name The name of the relation where the reference
   *                      expression comes from. This is only for
   *                      printing purpose.
   * @param type The type of the value of the referenced expression.
   * @return An immutable AttributeReference.
   */
  static AttributeReferencePtr Create(ExprId attribute_id,
                                      const std::string &attribute_name,
                                      const std::string &attribute_alias,
                                      const std::string &relation_name,
                                      const Type &type) {
    return AttributeReferencePtr(new AttributeReference(
        attribute_id, attribute_name, attribute_alias, relation_name, type));
  }

 private:
  AttributeReference(attribute_id attribute_id,
                     const std::string &attribute_name,
                     const std::string &attribute_alias,
                     const std::string &relation_name,
                     const Type &type)
      : NamedExpression(attribute_id,
                        attribute_name,
                        attribute_alias,
                        relation_name),
        type_(type) {}

  const Type &type_;

  DISALLOW_COPY_AND_ASSIGN(AttributeReference);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_ATTRIBUTE_REFERENCE_HPP_ */
