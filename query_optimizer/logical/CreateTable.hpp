/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATETABLE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATETABLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class CreateTable;
typedef std::shared_ptr<const CreateTable> CreateTablePtr;

/**
 * @brief Represents an operation that creates a new table.
 */
class CreateTable : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kCreateTable; }

  std::string getName() const override { return "CreateTable"; }

  /**
   * @return The name of the relation to be inserted.
   */
  const std::string& relation_name() const { return relation_name_; }

  /**
   * @return Attribute list of the relation.
   */
  const std::vector<expressions::AttributeReferencePtr>& attributes() const {
    return attributes_;
  }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK_EQ(getNumChildren(), new_children.size());
    return Create(relation_name_, attributes_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return attributes_;
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return attributes_;
  }

  /**
   * @brief Creates a CreateTable logical node that represents an operation to
   *        create a new table.
   *
   * @param relation_name The name of the relation to be inserted.
   * @param attributes Schema of the relation.
   * @return An immutable CreateTable node.
   */
  static CreateTablePtr Create(
      const std::string &relation_name,
      const std::vector<expressions::AttributeReferencePtr> &attributes) {
    return CreateTablePtr(new CreateTable(relation_name, attributes));
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
  CreateTable(
      const std::string &relation_name,
      const std::vector<expressions::AttributeReferencePtr> &attributes)
      : relation_name_(relation_name), attributes_(attributes) {}

  std::string relation_name_;
  std::vector<expressions::AttributeReferencePtr> attributes_;

  DISALLOW_COPY_AND_ASSIGN(CreateTable);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_CREATETABLE_HPP_ */
