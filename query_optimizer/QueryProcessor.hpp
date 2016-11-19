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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_QUERY_PROCESSOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_QUERY_PROCESSOR_HPP_

#include <cstddef>
#include <exception>
#include <memory>
#include <string>
#include <utility>

#include "catalog/Catalog.hpp"
#include "query_optimizer/Optimizer.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogDatabase;
class ParseStatement;
class QueryHandle;

/** \addtogroup QueryOptimizer
 *  @{
 */

/**
 * @brief Exception thrown when unable to read the catalog from disk.
 **/
class UnableToReadCatalog : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param filename The catalog filename provided.
   **/
  explicit UnableToReadCatalog(const std::string &filename)
      : message_("UnableToReadCatalog: Unable to read catalog from file ") {
    message_.append(filename);
  }

  ~UnableToReadCatalog() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when unable to write the catalog to disk.
 **/
class UnableToWriteCatalog : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param filename The catalog filename provided.
   **/
  explicit UnableToWriteCatalog(const std::string &filename)
      : message_("UnableToWriteCatalog: Unable to write catalog to file ") {
    message_.append(filename);
  }

  ~UnableToWriteCatalog() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/**
 * @brief Exception thrown when there is a Proto parse error when loading the
 *        catalog.
 **/
class CatalogNotProto : public std::exception {
 public:
  /**
   * @brief Constructor.
   *
   * @param filename The catalog filename provided.
   **/
  explicit CatalogNotProto(const std::string &filename)
      : message_("CatalogNotProto: The file ") {
    message_.append(filename).append(" is not valid Proto");
  }

  ~CatalogNotProto() throw() {
  }

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};


/**
 * @brief An object which manages the state of quickstep and the execution of
 *        queries.
 **/
class QueryProcessor {
 public:
  /**
   * @brief Constructor.
   *
   * @param catalog_filename The file to read the serialized catalog from.
   **/
  explicit QueryProcessor(std::string &&catalog_filename)
      : catalog_filename_(std::move(catalog_filename)),
        catalog_altered_(false),
        query_id_(0) {
    loadCatalog();
  }

  /**
   * @brief Destructor.
   **/
  ~QueryProcessor() {}

  /**
   * @brief Get the next query id.
   **/
  std::size_t query_id() const {
    return query_id_;
  }

  /**
   * @brief Fill a query handle for the given parsed SQL statement. This
   *        includes that the optimizer creates a QueryPlan inside the handle.
   *
   * @param statement The parsed SQL statement to generate a query handle for.
   * @param query_handle The generated query handle to output.
   **/
  void generateQueryHandle(const ParseStatement &statement,
                           QueryHandle *query_handle);

  /**
   * @brief Save the catalog back to disk.
   **/
  void saveCatalog();

  /**
   * @brief Set \p catalog_altered_ to true to indicate that the catalog
   *        has been altered.
   */
  void markCatalogAltered() {
    catalog_altered_ = true;
  }

  /**
   * @brief Get the default database in the Catalog held by this
   *        QueryProcessor.
   **/
  CatalogDatabase* getDefaultDatabase() const {
    return catalog_->getDatabaseByNameMutable("default");
  }

 private:
  void loadCatalog();  // If it exists, free catalog_ before calling this

  optimizer::Optimizer optimizer_;

  const std::string catalog_filename_;

  std::unique_ptr<Catalog> catalog_;

  bool catalog_altered_;

  std::size_t query_id_;

  DISALLOW_COPY_AND_ASSIGN(QueryProcessor);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_PROCESSOR_HPP_
