/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_TESTS_TEST_DATABASE_LOADER_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_TESTS_TEST_DATABASE_LOADER_HPP_

#include <cstddef>
#include <memory>
#include <string>

#include "catalog/CatalogDatabase.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"

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
  explicit TestDatabaseLoader(const std::string& storage_path = "")
      : catalog_database_(nullptr /* parent */,
                          "TestDatabase" /* name */,
                          0 /* id */),
        storage_manager_(storage_path),
        test_relation_(nullptr) {}

  ~TestDatabaseLoader() {
    clear();
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
