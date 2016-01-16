/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_TREE_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_TREE_HPP_

#include <cstddef>
#include <memory>
#include <vector>

#include "utility/Macros.hpp"
#include "utility/TreeStringSerializable.hpp"

namespace quickstep {
namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Base class of OptimizerTree. This class is needed so that we may
 *        attach non-child type nodes to a tree in the string representation
 *        as long as the non-child type is a subclass of OptimizerTreeBase.
 *        For example, a Logical node may have a child tree node of an
 *        Expression type.
 */
class OptimizerTreeBase
    : public TreeStringSerializable<std::shared_ptr<const OptimizerTreeBase>> {
 public:
  typedef std::shared_ptr<const OptimizerTreeBase> OptimizerTreeBaseNodePtr;

  /**
   * @brief Destructor.
   */
  ~OptimizerTreeBase() override {}

 protected:
  /**
   * @brief Constructor.
   */
  OptimizerTreeBase() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(OptimizerTreeBase);
};

/**
 * @brief Base class for expressions, logical/physical trees.
 */
template <class NodeType>
class OptimizerTree : public OptimizerTreeBase {
 public:
  typedef std::shared_ptr<const NodeType> OptimizerTreeNodePtr;

  /**
   * @brief Destructor.
   */
  ~OptimizerTree() override {}

  /**
   * @brief Returns the child node list.
   *
   * @return A vector of children.
   */
  const std::vector<OptimizerTreeNodePtr>& children() const {
    return children_;
  }

  /**
   * @brief Returns the number of child nodes.
   *
   * @return The number of child nodes.
   */
  std::size_t getNumChildren() const { return children_.size(); }

  /**
   * @brief Creates a copy with the child nodes replaced by \p new_children.
   *
   * @param new_children The children to be substituted for the existing ones.
   * @return A copy with \p new_children as child nodes.
   */
  virtual OptimizerTreeNodePtr copyWithNewChildren(
      const std::vector<OptimizerTreeNodePtr> &new_children) const = 0;

 protected:
  /**
   * @brief Constructor.
   */
  OptimizerTree() {}

  /**
   * @brief Adds a new child node to this tree node.
   *
   * @param child The node to add as a new child.
   */
  void addChild(const OptimizerTreeNodePtr &child) {
    children_.push_back(child);
  }

 private:
  std::vector<OptimizerTreeNodePtr> children_;

  DISALLOW_COPY_AND_ASSIGN(OptimizerTree);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_TREE_HPP_ */
