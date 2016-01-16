/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_CONTEXT_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_CONTEXT_HPP_

#include <cstddef>
#include <cstdlib>
#include <string>

#include "catalog/CatalogTypedefs.hpp"
#include "query_optimizer/expressions/ExprId.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabase;
class CatalogRelation;
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
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param query_id The query ID.
   * @param catalog_database The catalog database where this query is executed.
   * @param storage_manager The storage manager to use for allocating storage
   *        blocks.
   * @param bus A pointer to the TMB.
   */
  OptimizerContext(const tmb::client_id foreman_client_id,
                   const std::size_t query_id,
                   CatalogDatabase *catalog_database,
                   StorageManager *storage_manager,
                   tmb::MessageBus *bus)
      : foreman_client_id_(foreman_client_id),
        query_id_(query_id),
        current_expr_id_(-1),
        catalog_database_(catalog_database),
        storage_manager_(storage_manager),
        bus_(bus),
        is_catalog_changed_(false) {}

  /**
   * @brief Destructor.
   */
  ~OptimizerContext() {}

  /**
   * @brief Get the TMB client ID of Foreman thread.
   *
   * @return TMB client ID of foreman thread.
   **/
  tmb::client_id getForemanClientID() const {
    return foreman_client_id_;
  }

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
   * @return Query ID.
   */
  std::size_t query_id() const { return query_id_; }

  /**
   * @brief Get a pointer to the TMB.
   *
   * @return A pointer to the TMB.
   **/
  tmb::MessageBus* getMessageBus() {
    return bus_;
  }

  /**
   * @brief Gets the next ExprId.
   *
   * @return A new ExprId.
   */
  expressions::ExprId nextExprId() {
    return ++current_expr_id_;
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
  const tmb::client_id foreman_client_id_;

  const std::size_t query_id_;

  expressions::ExprId current_expr_id_;

  CatalogDatabase *catalog_database_;
  StorageManager *storage_manager_;

  // TODO(zuyu): Remove 'bus_' once WorkOrder serialization is done.
  tmb::MessageBus *bus_;

  bool is_catalog_changed_;

  DISALLOW_COPY_AND_ASSIGN(OptimizerContext);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_CONTEXT_HPP_ */
