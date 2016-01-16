/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_OPTIMIZER_QUERY_PROCESSOR_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_QUERY_PROCESSOR_HPP_

#include <cstddef>
#include <exception>
#include <memory>
#include <string>

#include "catalog/Catalog.hpp"
#include "storage/StorageManager.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

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
   * @param storage_path The filesystem directory where blocks are stored on
   *        disk.
   * @param foreman_client_id The TMB client ID of the Foreman thread.
   * @param bus A pointer to the TMB.
   **/
  QueryProcessor(const std::string &catalog_filename,
                 const std::string &storage_path,
                 const tmb::client_id foreman_client_id,
                 tmb::MessageBus *bus)
      : catalog_filename_(catalog_filename),
        foreman_client_id_(foreman_client_id),
        bus_(bus),
        catalog_altered_(false),
        query_id_(0) {
    loadCatalog();
    storage_manager_.reset(new StorageManager(storage_path));
  }

  /**
   * @brief Destructor.
   **/
  ~QueryProcessor() {}

  /**
   * @brief Create a query handle for the given parsed SQL statement. This
   *        includes that the optimizer creates a QueryPlan inside the handle.
   *
   * @param statement The parsed SQL statement to generate a query handle for.
   * @return A query handle for statement.
   **/
  QueryHandle* generateQueryHandle(const ParseStatement &statement);

  /**
   * @brief Save the catalog back to disk.
   **/
  void saveCatalog();

  /**
   * @brief Get the default database in the Catalog held by this
   *        QueryProcessor.
   **/
  CatalogDatabase* getDefaultDatabase() const {
    return catalog_->getDatabaseByNameMutable("default");
  }

  /**
   * @brief Get the StorageManager held by this QueryProcessor.
   **/
  StorageManager* getStorageManager() const {
    return storage_manager_.get();
  }

 private:
  void loadCatalog();  // If it exists, free catalog_ before calling this

  std::string catalog_filename_;

  const tmb::client_id foreman_client_id_;
  // TODO(zuyu): Remove 'bus_' once WorkOrder serialization is done.
  tmb::MessageBus *bus_;

  std::unique_ptr<Catalog> catalog_;
  std::unique_ptr<StorageManager> storage_manager_;
  bool catalog_altered_;

  std::size_t query_id_;

  DISALLOW_COPY_AND_ASSIGN(QueryProcessor);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_OPTIMIZER_QUERY_PROCESSOR_HPP_
