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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_TEST_DATABASE_LOADER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_TEST_DATABASE_LOADER_HPP_

#include <string>

#include "catalog/CatalogDatabase.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "storage/StorageManager.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Creates and populate testing schema and data.
 */
class TestDatabaseLoader {
 public:
  /**
   * @brief Constructor.
   *
   * @param storage_path A filesystem directory where the blocks may be
   *                     evicted to during the execution of a test query.
   *                     Can be empty if the test query is not executed
   *                     in the query engine.
   */
  explicit TestDatabaseLoader(const std::string &storage_path = "")
      : thread_id_map_(ClientIDMap::Instance()),
        catalog_database_(nullptr /* parent */,
                          "TestDatabase" /* name */,
                          0 /* id */),
        storage_manager_(storage_path),
        test_relation_(nullptr) {
    bus_.Initialize();

    const tmb::client_id worker_thread_client_id = bus_.Connect();
    bus_.RegisterClientAsSender(worker_thread_client_id, kCatalogRelationNewBlockMessage);

    // Refer to InsertDestination::sendBlockFilledMessage for the rationale
    // behind using ClientIDMap.
    thread_id_map_->addValue(worker_thread_client_id);

    scheduler_client_id_ = bus_.Connect();
    bus_.RegisterClientAsReceiver(scheduler_client_id_, kCatalogRelationNewBlockMessage);
  }

  ~TestDatabaseLoader() {
    clear();

    thread_id_map_->removeValue();
  }

  /**
   * @brief Gets the test database.
   *
   * @return The test database.
   */
  CatalogDatabase* catalog_database() { return &catalog_database_; }

  /**
   * @brief Gets the storage manager.
   *
   * @return The storage manager.
   */
  StorageManager* storage_manager() { return &storage_manager_; }

  /**
   * @brief Gets the test relation.
   *
   * @return The test relation.
   */
  CatalogRelation* test_relation() { return test_relation_; }

  /**
   * @brief Creates a CatalogRelation with six columns (one column per type,):
   *        int_col (nullable), long_col, float_col, double_col (nullable),
   *        char_col, vchar_col (nullable).
   *        If \p allow_vchar is false, vchar_col is not added.
   * @warning This can only be called once.
   *
   * @param allow_vchar Whether the VCHAR column should be added.
   * @return The test database.
   */
  CatalogRelation* createTestRelation(bool allow_vchar);

  /**
   * @brief Loads data into the test relation. The test relation has 25 tuples.
   *        Each tuple is
   *           ((-1)^x*x, x^2, sqrt(x), (-1)^x*x*sqrt(x),
   *           concat(string(int_col), string(float_col)).
   *        where 0 <= x <= 24.
   *        Nullable attributes (int_col and doulbe_col) have a NULL value for
   *        every 10 tuples.
   * @warning This can only be called once and the test relation cannot have
   *          vchar_col, since the default layout does not support it.
   */
  void loadTestRelation();

  /**
   * @brief Removes all data and drops all relations from the database.
   */
  void clear();

 private:
  /**
   * @brief Simulate Foreman to add all new blocks to the relation.
   */
  void processCatalogRelationNewBlockMessages();

  ClientIDMap *thread_id_map_;

  MessageBusImpl bus_;
  tmb::client_id scheduler_client_id_;

  CatalogDatabase catalog_database_;
  StorageManager storage_manager_;
  // Owned by catalog_database_.
  CatalogRelation* test_relation_;

  DISALLOW_COPY_AND_ASSIGN(TestDatabaseLoader);
};

/** @} */

}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_TESTS_TEST_CATALOG_HPP_ */
