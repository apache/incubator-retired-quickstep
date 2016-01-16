/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

TEST_F(AggregationOperationStateProtoTest, InvalidAggregateSizeTest) {
  serialization::AggregationOperationState proto;
  proto.set_relation_id(rel_id_);
  proto.set_estimated_num_entries(0);
  EXPECT_FALSE(AggregationOperationState::ProtoIsValid(proto, *database_.get()));
}

}  // namespace quickstep

