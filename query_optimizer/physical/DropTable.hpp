/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_DROPTABLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_DROPTABLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/ExpressionUtil.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/PhysicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace physical {

/** \addtogroup OptimizerPhysical
 *  @{
 */

class DropTable;
typedef std::shared_ptr<const DropTable> DropTablePtr;

/**
 * @brief Drops a table.
 */
class DropTable : public Physical {
 public:
  PhysicalType getPhysicalType() const override { return PhysicalType::kDropTable; }

  std::string getName() const override { return "DropTable"; }

  /**
   * @return Gets the catalog relation to be dropped.
   */
  const CatalogRelation* catalog_relation() const { return catalog_relation_; }

  PhysicalPtr copyWithNewChildren(
      const std::vector<PhysicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(catalog_relation_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return {};
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return {};
  }

  bool maybeCopyWithPrunedExpressions(
      const expressions::UnorderedNamedExpressionSet &referenced_expressions,
      PhysicalPtr *output) const override {
    return false;
  }

  /**
   * @brief Creates a DropTable physical node that drops \p catalog_relation.
   *
   * @param catalog_relation The relation to be dropped.
   * @return An immutable DropTable node.
   */
  static DropTablePtr Create(const CatalogRelation *catalog_relation) {
    return DropTablePtr(new DropTable(catalog_relation));
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
  explicit DropTable(const CatalogRelation *catalog_relation)
      : catalog_relation_(catalog_relation) {}

  const CatalogRelation *catalog_relation_;

  DISALLOW_COPY_AND_ASSIGN(DropTable);
};

/** @} */

}  // namespace physical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_PHYSICAL_DROPTABLE_HPP_ */
