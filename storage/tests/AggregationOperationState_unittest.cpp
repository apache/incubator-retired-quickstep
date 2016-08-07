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

#include <cstddef>
#include <memory>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/AggregationOperationState.hpp"
#include "storage/AggregationOperationState.pb.h"

#include "gtest/gtest.h"

using std::unique_ptr;

namespace quickstep {

// NOTE(zuyu): This unittest only covers invalid cases for serializing
// the AggregationOperationState. The valid form should be covered
// by either AggregationOperator_unittest or
// quickstep_queryoptimizer_tests_executiongenerator_select.
class AggregationOperationStateProtoTest : public ::testing::Test {
 protected:
  static const relation_id kInvalidTableId = 100;

  virtual void SetUp() {
    database_.reset(new CatalogDatabase(nullptr, "db"));
    rel_id_ = database_->addRelation(new CatalogRelation(nullptr, "rel"));
  }

  unique_ptr<CatalogDatabase> database_;
  relation_id rel_id_;
};

TEST_F(AggregationOperationStateProtoTest, UninitializationTest) {
  serialization::AggregationOperationState proto;
  EXPECT_FALSE(AggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

TEST_F(AggregationOperationStateProtoTest, InvalidRelationIdTest) {
  serialization::AggregationOperationState proto;
  proto.set_relation_id(kInvalidTableId);
  proto.set_estimated_num_entries(0);
  EXPECT_FALSE(AggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

TEST_F(AggregationOperationStateProtoTest, ZeroAggregateSizeTest) {
  serialization::AggregationOperationState proto;
  proto.set_relation_id(rel_id_);
  proto.set_estimated_num_entries(0);
  EXPECT_TRUE(AggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

}  // namespace quickstep

