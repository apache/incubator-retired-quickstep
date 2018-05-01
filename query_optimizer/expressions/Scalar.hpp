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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_HPP_

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "query_optimizer/expressions/Expression.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "utility/HashError.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class Scalar;

namespace optimizer {
namespace expressions {

/** \addtogroup OptimizerExpressions
 *  @{
 */

class Scalar;
typedef std::shared_ptr<const Scalar> ScalarPtr;

/**
 * @brief Scalar expression that returns a single value based on input values
 *        that are not across multiple rows in an input relation.
 */
class Scalar : public Expression {
 public:
  ~Scalar() override {}

  /**
   * @brief Converts this expression to an expression for evaluation (in
   *        quickstep execution expressions system under /expressions)
   *        with each attribute reference being replaced by a CatalogAttribute
   *        reference provided in \p substitution_map.
   *
   * @param substitution_map Map from ExprId to CatalogAttribute for use in
   *                         substituting CatalogAttribute for
   *                         AttributeReference.
   * @param left_expr_ids The ExprIds from the left hand side.
   * @param right_expr_ids The ExprIds from the right hand side.
   * @return Concretized expression for evaluation.
   */
  virtual ::quickstep::Scalar* concretize(
      const std::unordered_map<ExprId, const CatalogAttribute*> &substitution_map,
      const std::unordered_set<ExprId> &left_expr_ids = std::unordered_set<ExprId>(),
      const std::unordered_set<ExprId> &right_expr_ids = std::unordered_set<ExprId>()) const = 0;

  /**
   * @brief Check whether this scalar is semantically equivalent to \p other.
   *
   * @note The fact that two scalars are semantically equal brings more
   *       optimization opportunities, e.g. common subexpression elimination.
   *       Meanwhile, it is always safe to assume that two scalars are not equal.
   *
   * @return True if this scalar equals \p other; false otherwise.
   */
  virtual bool equals(const ScalarPtr &other) const {
    return false;
  }

  /**
   * @brief Get a hash of this scalar.
   *
   * @return A hash of this scalar.
   */
  std::size_t hash() const {
    if (hash_cache_ == nullptr) {
      hash_cache_ = std::make_unique<std::size_t>(computeHash());
    }
    return *hash_cache_;
  }

 protected:
  Scalar() {}

  /**
   * @brief Compute a hash of this scalar.
   *
   * @note Override this method to actually compute the hash. Note that the
   *       public method hash() is a caching wrapper for this method.
   *
   * @return A hash of this scalar.
   */
  virtual std::size_t computeHash() const {
    throw HashNotSupported("Unsupported computeHash() in " + getName());
  }

 private:
  mutable std::unique_ptr<std::size_t> hash_cache_;

  DISALLOW_COPY_AND_ASSIGN(Scalar);
};

/** @} */

}  // namespace expressions
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_EXPRESSIONS_SCALAR_HPP_ */
