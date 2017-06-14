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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_HPP_

#include <memory>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class Physical;
typedef std::shared_ptr<const Physical> PhysicalPtr;

/**
 * @brief Base class for physical plan nodes.
 */
class Physical : public OptimizerTree<Physical> {
 public:
  /**
   * @brief Destructor.
   */
  ~Physical() override {}

  /**
   * @return The physical node type.
   */
  virtual PhysicalType getPhysicalType() const = 0;

  /**
   * @brief Gets the output attributes of this node.
   *
   * @return The output AttributeReferences by this node.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const = 0;

  /**
   * @brief Returns AttributeReferences referenced by the input expressions of
   *        this physical node.
   *
   * @return A vector of AttributeReference referenced by the input expressions
   *         of this physical node.
   */
  virtual std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const = 0;

  /**
   * @brief Creates a new copy in \p output if there are some project
   *        expressions that are not in \p referenced_expressions.
   *        Otherwise, keeps \p output unchanged.
   *
   * @param referenced_attributes The attributes to be kept in the project
   *                              expression list.
   * @param output A new copy if some project expressions should be removed.
   * @return True if a new copy is generated (i.e. \p output is changed).
   */
  virtual bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const = 0;

  /**
   * @brief Creates a copy with the partition scheme header replaced by \p
   *        partition_scheme_header.
   *
   * @param partition_scheme_header The partition scheme header to be
   *        substituted for the existing one, if any. It takes ownership of
   *        'partition_scheme_header'.
   *
   * @return A copy with \p partition_scheme_header as the partition scheme
   *         header.
   */
  virtual PhysicalPtr copyWithNewOutputPartitionSchemeHeader(
      PartitionSchemeHeader *partition_scheme_header) const {
    std::unique_ptr<PartitionSchemeHeader> new_partition_scheme_header(partition_scheme_header);
    LOG(FATAL) << "copyWithNewOutputPartitionSchemeHeader is not implemented for " << getName();
  }

  /**
   * @brief Get the partition scheme of the physical plan node.
   *
   * @return A const pointer to the partition scheme of the node.
   **/
  const PartitionSchemeHeader* getOutputPartitionSchemeHeader() const {
    return partition_scheme_header_.get();
  }

  /**
   * @brief Clone a copy of the partition scheme header.
   *
   * @return A copy of the partition scheme header. Caller should take ownership
   *         of the returned object.
   **/
  PartitionSchemeHeader* cloneOutputPartitionSchemeHeader() const {
    if (partition_scheme_header_) {
      return new PartitionSchemeHeader(*partition_scheme_header_);
    }

    return nullptr;
  }

 protected:
  /**
   * @brief Constructor.
   *
   * @param partition_scheme_header The partition scheme header of the relation.
   *        The constructor takes ownership of 'partition_scheme_header'.
   */
  explicit Physical(PartitionSchemeHeader *partition_scheme_header = nullptr)
      : partition_scheme_header_(partition_scheme_header) {}

  std::unique_ptr<PartitionSchemeHeader> partition_scheme_header_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Physical);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_PHYSICAL_HPP_ */
