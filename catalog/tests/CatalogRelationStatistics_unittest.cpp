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

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/Catalog.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationStatistics.hpp"
#include "cli/CommandExecutor.hpp"
#include "cli/DropRelation.hpp"
#include "parser/ParseStatement.hpp"
#include "parser/SqlParserWrapper.hpp"
#include "query_execution/ForemanSingleNode.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/Worker.hpp"
#include "query_execution/WorkerDirectory.hpp"
#include "query_optimizer/QueryHandle.hpp"
#include "query_optimizer/QueryProcessor.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/StorageManager.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

namespace {

constexpr char kStoragePath[] = "./catalog_relation_statistics_test_data/";

constexpr attribute_id kFirstAttributeId = 0;
constexpr attribute_id kSecondAttributeId = 1;

}  // namespace

class CatalogRelationStatisticsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    // Set up the environment for running end-to-end queries.
    quickstep::ClientIDMap::Instance();

    bus_.Initialize();

    main_thread_client_id_ = bus_.Connect();
    bus_.RegisterClientAsSender(main_thread_client_id_, kAdmitRequestMessage);
    bus_.RegisterClientAsSender(main_thread_client_id_, kPoisonMessage);
    bus_.RegisterClientAsReceiver(main_thread_client_id_, kWorkloadCompletionMessage);

    std::string catalog_path(kStoragePath);
    catalog_path.append(kCatalogFilename);

    std::ofstream catalog_file(catalog_path.c_str());
    Catalog catalog;
    catalog.addDatabase(new CatalogDatabase(nullptr, "default"));
    catalog.getProto().SerializeToOstream(&catalog_file);
    catalog_file.close();

    storage_manager_.reset(new StorageManager(kStoragePath));
    query_processor_.reset(new QueryProcessor(std::move(catalog_path)));

    worker_.reset(new Worker(0, &bus_));
    worker_directory_.reset(
        new WorkerDirectory(1, {worker_->getBusClientID()}, {-1}));

    foreman_.reset(
        new ForemanSingleNode(main_thread_client_id_,
                              worker_directory_.get(),
                              &bus_,
                              query_processor_->getDefaultDatabase(),
                              storage_manager_.get()));

    worker_->start();
    foreman_->start();
  }

  virtual void TearDown() {
    for (const auto &relation : *query_processor_->getDefaultDatabase()) {
      DropRelation::Drop(relation,
                         query_processor_->getDefaultDatabase(),
                         storage_manager_.get());
    }

    QueryExecutionUtil::BroadcastPoisonMessage(main_thread_client_id_, &bus_);
    worker_->join();
    foreman_->join();
  }

  void executeQuery(const std::string &query_string) {
    SqlParserWrapper parser_wrapper;
    parser_wrapper.feedNextBuffer(new std::string(query_string));

    ParseResult result = parser_wrapper.getNextStatement();
    DCHECK(result.condition == ParseResult::kSuccess);

    const ParseStatement &statement = *result.parsed_statement;
    std::unique_ptr<QueryHandle> query_handle =
        std::make_unique<QueryHandle>(query_processor_->query_id(),
                                      main_thread_client_id_,
                                      statement.getPriority());
    query_processor_->generateQueryHandle(statement, query_handle.get());

    QueryExecutionUtil::ConstructAndSendAdmitRequestMessage(
        main_thread_client_id_,
        foreman_->getBusClientID(),
        query_handle.release(),
        &bus_);

    QueryExecutionUtil::ReceiveQueryCompletionMessage(main_thread_client_id_, &bus_);
  }

  void executeAnalyze(const std::string &rel_name) {
    SqlParserWrapper parser_wrapper;
    parser_wrapper.feedNextBuffer(new std::string("\\analyze " + rel_name));

    ParseResult result = parser_wrapper.getNextStatement();
    DCHECK(result.condition == ParseResult::kSuccess);

    const ParseStatement &statement = *result.parsed_statement;
    DCHECK(statement.getStatementType() == ParseStatement::kCommand);
    quickstep::cli::executeCommand(statement,
                                   *(query_processor_->getDefaultDatabase()),
                                   main_thread_client_id_,
                                   foreman_->getBusClientID(),
                                   &bus_,
                                   storage_manager_.get(),
                                   query_processor_.get(),
                                   stdout);
  }

  const CatalogRelation *getRelationByName(const std::string &rel_name) const {
    const CatalogRelation *relation =
        query_processor_->getDefaultDatabase()->getRelationByName(rel_name);
    DCHECK(relation != nullptr);
    return relation;
  }

 private:
  MessageBusImpl bus_;
  tmb::client_id main_thread_client_id_;

  std::unique_ptr<StorageManager> storage_manager_;
  std::unique_ptr<QueryProcessor> query_processor_;

  std::unique_ptr<Worker> worker_;
  std::unique_ptr<WorkerDirectory> worker_directory_;
  std::unique_ptr<ForemanSingleNode> foreman_;
};

TEST_F(CatalogRelationStatisticsTest, AnalyzeTest) {
  executeQuery("CREATE TABLE analyzetest(x INT, y DOUBLE);");
  executeQuery("INSERT INTO analyzetest VALUES(0, -0.5);");
  executeQuery("INSERT INTO analyzetest VALUES(1, 0);");
  executeQuery("INSERT INTO analyzetest VALUES(0, 0.5);");
  executeAnalyze("analyzetest");

  const CatalogRelation *relation = getRelationByName("analyzetest");
  const CatalogRelationStatistics &stat = relation->getStatistics();

  EXPECT_EQ(3u, stat.getNumTuples());

  EXPECT_EQ(2u, stat.getNumDistinctValues(kFirstAttributeId));
  EXPECT_EQ(0, stat.getMinValue(kFirstAttributeId).getLiteral<int>());
  EXPECT_EQ(1, stat.getMaxValue(kFirstAttributeId).getLiteral<int>());

  EXPECT_EQ(3u, stat.getNumDistinctValues(kSecondAttributeId));
  EXPECT_EQ(-0.5, stat.getMinValue(kSecondAttributeId).getLiteral<double>());
  EXPECT_EQ(0.5, stat.getMaxValue(kSecondAttributeId).getLiteral<double>());
}

TEST_F(CatalogRelationStatisticsTest, ExactnessTest) {
  executeQuery("CREATE TABLE exactnesstest(x INT);");

  const CatalogRelationStatistics &stat =
      getRelationByName("exactnesstest")->getStatistics();

  EXPECT_FALSE(stat.isExact());

  const std::vector<std::string> queries = {
      "INSERT INTO exactnesstest VALUES(1);",
      "INSERT INTO exactnesstest SELECT i FROM generate_series(2, 10) AS gs(i);",
      "DELETE FROM exactnesstest WHERE x = 5;",
      "UPDATE exactnesstest SET x = 100 WHERE x = 10;"
  };

  for (const std::string &query : queries) {
    executeQuery(query);
    EXPECT_FALSE(stat.isExact());

    executeAnalyze("exactnesstest");
    EXPECT_TRUE(stat.isExact());
  }
}

}  // namespace quickstep
