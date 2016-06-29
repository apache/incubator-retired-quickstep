/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#include <cstddef>
#include <memory>

#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/aggregation/AggregateFunction.pb.h"
#include "storage/WindowAggregationOperationState.hpp"
#include "storage/WindowAggregationOperationState.pb.h"

#include "gtest/gtest.h"

using std::unique_ptr;

namespace quickstep {

namespace {
  constexpr relation_id kInvalidTableId = 100;
  constexpr std::int64_t kInvalidNum = -10;
  constexpr std::int64_t kValidNum = 10;
}  // namespace

class WindowAggregationOperationStateProtoTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    database_.reset(new CatalogDatabase(nullptr, "db"));
    rel_id_ = database_->addRelation(new CatalogRelation(nullptr, "rel"));
  }

  unique_ptr<CatalogDatabase> database_;
  relation_id rel_id_;
};

TEST_F(WindowAggregationOperationStateProtoTest, UninitializationTest) {
  serialization::WindowAggregationOperationState proto;
  EXPECT_FALSE(WindowAggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

TEST_F(WindowAggregationOperationStateProtoTest, InvalidRelationIdTest) {
  serialization::WindowAggregationOperationState proto;
  proto.set_relation_id(kInvalidTableId);
  proto.mutable_function()->set_aggregation_id(serialization::AggregateFunction::AVG);
  proto.set_is_row(true);
  proto.set_num_preceding(kValidNum);
  proto.set_num_following(kValidNum);
  EXPECT_FALSE(WindowAggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

TEST_F(WindowAggregationOperationStateProtoTest, InvalidNumTest) {
  serialization::WindowAggregationOperationState proto;
  proto.set_relation_id(rel_id_);
  proto.mutable_function()->set_aggregation_id(serialization::AggregateFunction::AVG);
  proto.set_is_row(true);
  proto.set_num_preceding(kInvalidNum);
  proto.set_num_following(kValidNum);
  EXPECT_FALSE(WindowAggregationOperationState::ProtoIsValid(proto, *database_.get()));

  proto.set_num_preceding(kValidNum);
  proto.set_num_following(kInvalidNum);
  EXPECT_FALSE(WindowAggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

TEST_F(WindowAggregationOperationStateProtoTest, ValidTest) {
  serialization::WindowAggregationOperationState proto;
  proto.set_relation_id(rel_id_);
  proto.mutable_function()->set_aggregation_id(serialization::AggregateFunction::AVG);
  proto.set_is_row(true);
  proto.set_num_preceding(kValidNum);
  proto.set_num_following(kValidNum);
  EXPECT_TRUE(WindowAggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

}  // namespace quickstep
