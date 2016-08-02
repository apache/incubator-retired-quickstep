/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_CONTEXT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_CONTEXT_HPP_

#include "query_optimizer/expressions/ExprId.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class StorageManager;

namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Stores stateful or common info used throughout the optimization of a
 *        query.
 */
class OptimizerContext {
 public:
  static const char *kInternalTemporaryRelationNamePrefix;

  /**
   * @brief Constructor.
   *
   * @param catalog_database The catalog database where this query is executed.
   * @param storage_manager The storage manager to use for allocating storage
   *        blocks.
   */
  OptimizerContext(CatalogDatabase *catalog_database,
                   StorageManager *storage_manager)
      : current_expr_id_(-1),
        catalog_database_(catalog_database),
        storage_manager_(storage_manager),
        has_nested_queries_(false),
        is_catalog_changed_(false) {}

  /**
   * @brief Destructor.
   */
  ~OptimizerContext() {}

  /**
   * @return Const pointer to the catalog database
   *         where the query is executed.
   */
  const CatalogDatabase* catalog_database() const { return catalog_database_; }

  /**
   * @return Mutable pointer to the catalog database
   *         where this query is executed.
   */
  CatalogDatabase* catalog_database() { return catalog_database_; }

  /**
   * @return Const pointer to the storage manager to use for
   *         allocating storage blocks.
   */
  const StorageManager* storage_manager() const { return storage_manager_; }

  /**
   * @return Mutable pointer to the storage manager to use for
   *         allocating storage blocks.
   */
  StorageManager* storage_manager() { return storage_manager_; }

  /**
   * @brief Gets the next ExprId.
   *
   * @return A new ExprId.
   */
  expressions::ExprId nextExprId() {
    return ++current_expr_id_;
  }

  /**
   * @brief Indicate that the query has a nested subquery.
   */
  void set_has_nested_queries() {
    has_nested_queries_ = true;
  }

  /**
   * @return True if the query has a nested subquery.
   */
  bool has_nested_queries() const {
    return has_nested_queries_;
  }

  /**
   * @brief Sets <is_catalog_changed_> to be true to indicate the query will
   *        modify the catalog permanently.
   */
  void set_is_catalog_changed() { is_catalog_changed_ = true; }

  /**
   * @return True if the catalog will be permanently changed after the query.
   */
  bool is_catalog_changed() const { return is_catalog_changed_; }

 private:
  expressions::ExprId current_expr_id_;

  CatalogDatabase *catalog_database_;
  StorageManager *storage_manager_;

  bool has_nested_queries_;

  bool is_catalog_changed_;

  DISALLOW_COPY_AND_ASSIGN(OptimizerContext);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_CONTEXT_HPP_ */
