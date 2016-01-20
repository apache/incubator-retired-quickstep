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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_HPP_

#include <cstddef>

#include "query_optimizer/OptimizerContext.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabase;
class ParseStatement;
class QueryHandle;
class StorageManager;

namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Main class of the optimizer that creates and optimizes all types of query
 *        plans.
 */
class Optimizer {
 public:
  /**
   * @brief Constructor.
   *
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param query_id The query id. Used to identify a query and create distinct
   *        names for temporary relations.
   * @param database The database that the query is executed on.
   * @param storage_manager The storage manager for the database.
   * @param bus A pointer to the TMB.
   */
  // TODO(zuyu): Remove passing 'bus' once WorkOrder serialization is done.
  Optimizer(const tmb::client_id foreman_client_id,
            const std::size_t query_id,
            CatalogDatabase *database,
            StorageManager *storage_manager,
            tmb::MessageBus *bus)
      : optimizer_context_(foreman_client_id, query_id, database, storage_manager, bus) {}

  /**
   * @brief Destructor.
   */
  ~Optimizer() {}

  /**
   * @brief Fills an execution plan as a list of operators,
   *        and the query result relation in
   *        \p query_handle for \p parse_statement.
   * @note Does not take ownership of \p query_handle.
   *
   * @param parse_statement The parse tree of the input query.
   * @param query_handle The generated query handle to output.
   */
  void generateQueryHandle(const ParseStatement &parse_statement,
                           QueryHandle *query_handle);

  /**
   * @return True if the catalog will be permanently changed after executing the
   *         query and needs to be saved.
   */
  bool isCatalogChanged() const {
    return optimizer_context_.is_catalog_changed();
  }

 private:
  OptimizerContext optimizer_context_;

  DISALLOW_COPY_AND_ASSIGN(Optimizer);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_OPTIMIZER_HPP_ */
