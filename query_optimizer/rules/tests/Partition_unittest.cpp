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

#include "query_optimizer/rules/Partition.hpp"

#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/physical/PartitionSchemeHeader.hpp"
#include "query_optimizer/rules/tests/PhysicalRuleTest.hpp"
#include "utility/Macros.hpp"

#include "gflags/gflags.h"
#include "gtest/gtest.h"

using std::make_unique;
using std::move;
using std::size_t;

namespace quickstep {
namespace optimizer {

DECLARE_uint64(num_repartitions);

namespace E = expressions;
namespace P = physical;

class PartitionTest : public PhysicalRuleTest {
 protected:
  PartitionTest() {}

  ~PartitionTest() override {}

  void SetUp() override {
    PhysicalRuleTest::SetUp();
  }

  void setupRule(std::unique_ptr<Rule<P::Physical>> *rule) override {
    *rule = make_unique<Partition>(optimizer_context());
  }

  void needsRepartitionForHashJoin() {
    Partition::needsRepartitionForHashJoin(
        left_partition_scheme_header_.get(), { relation_attribute_reference_0_0_ },
        right_partition_scheme_header_.get(), { relation_attribute_reference_1_0_ },
        &left_needs_repartition_, &right_needs_repartition_, &num_partitions_);
  }

  void useLeftHashPartitionMatchedJoinAttributes(const size_t num_partition) {
    P::PartitionSchemeHeader::PartitionExprIds left_partition_expr_ids;
    left_partition_expr_ids.push_back({ relation_attribute_reference_0_0_->id() });
    left_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        P::PartitionSchemeHeader::PartitionType::kHash,
        num_partition, move(left_partition_expr_ids));
  }

  void useLeftHashPartitionNotMatchedJoinAttributes(const size_t num_partition) {
    P::PartitionSchemeHeader::PartitionExprIds left_partition_expr_ids;
    left_partition_expr_ids.push_back({ relation_attribute_reference_0_1_->id() });
    left_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        P::PartitionSchemeHeader::PartitionType::kHash,
        num_partition, move(left_partition_expr_ids));
  }

  void useRightHashPartitionMatchedJoinAttributes(const size_t num_partition) {
    P::PartitionSchemeHeader::PartitionExprIds right_partition_expr_ids;
    right_partition_expr_ids.push_back({ relation_attribute_reference_1_0_->id() });
    right_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        P::PartitionSchemeHeader::PartitionType::kHash,
        num_partition, move(right_partition_expr_ids));
  }

  void useRightHashPartitionNotMatchedJoinAttributes(const size_t num_partition) {
    P::PartitionSchemeHeader::PartitionExprIds right_partition_expr_ids;
    right_partition_expr_ids.push_back({ relation_attribute_reference_1_1_->id() });
    right_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        P::PartitionSchemeHeader::PartitionType::kHash,
        num_partition, move(right_partition_expr_ids));
  }

  std::unique_ptr<P::PartitionSchemeHeader> left_partition_scheme_header_, right_partition_scheme_header_;
  bool left_needs_repartition_, right_needs_repartition_;
  size_t num_partitions_;

 private:
  DISALLOW_COPY_AND_ASSIGN(PartitionTest);
};

TEST_F(PartitionTest, HashJoinLeftHashPartitionMatchedJoinAttributesRightNoPartitionTest) {
  const size_t kLeftNumPartitions = 64u;
  useLeftHashPartitionMatchedJoinAttributes(kLeftNumPartitions);

  needsRepartitionForHashJoin();

  EXPECT_FALSE(left_needs_repartition_);
  EXPECT_FALSE(right_needs_repartition_);
  EXPECT_EQ(kLeftNumPartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionNotMatchedJoinAttributesRightNoPartitionTest) {
  useLeftHashPartitionNotMatchedJoinAttributes(16u);

  needsRepartitionForHashJoin();

  EXPECT_TRUE(left_needs_repartition_);
  EXPECT_TRUE(right_needs_repartition_);
  EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftNonHashPartitionRightNoPartitionTest) {
  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    left_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 8u, P::PartitionSchemeHeader::PartitionExprIds());

    needsRepartitionForHashJoin();

    EXPECT_TRUE(left_needs_repartition_);
    EXPECT_TRUE(right_needs_repartition_);
    EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
  }
}

TEST_F(PartitionTest, HashJoinLeftNoPartitionRightHashPartitionMatchedJoinAttributesTest) {
  const size_t kRightNumPartitions = 64u;
  useRightHashPartitionMatchedJoinAttributes(kRightNumPartitions);

  needsRepartitionForHashJoin();

  EXPECT_TRUE(left_needs_repartition_);
  EXPECT_FALSE(right_needs_repartition_);
  EXPECT_EQ(kRightNumPartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionMatchedJoinAttributesRightHashPartitionMatchedJoinAttributesTest) {
  const size_t kNumPartitions = 64u;
  useLeftHashPartitionMatchedJoinAttributes(kNumPartitions);
  useRightHashPartitionMatchedJoinAttributes(kNumPartitions);

  needsRepartitionForHashJoin();

  EXPECT_FALSE(left_needs_repartition_);
  EXPECT_FALSE(right_needs_repartition_);
  EXPECT_EQ(kNumPartitions, num_partitions_);
}

TEST_F(PartitionTest,
       HashJoinLeftHashPartitionMatchedJoinAttributesRightHashPartitionMatchedJoinAttributesNotEqualNumPartitionsTest) {
  const size_t kLeftNumPartitions = 128u;
  useLeftHashPartitionMatchedJoinAttributes(kLeftNumPartitions);
  useRightHashPartitionMatchedJoinAttributes(64u);

  needsRepartitionForHashJoin();

  EXPECT_FALSE(left_needs_repartition_);
  EXPECT_TRUE(right_needs_repartition_);
  EXPECT_EQ(kLeftNumPartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionNotMatchedJoinAttributesRightHashPartitionMatchedJoinAttributesTest) {
  useLeftHashPartitionNotMatchedJoinAttributes(16u);

  const size_t kRightNumPartitions = 64u;
  useRightHashPartitionMatchedJoinAttributes(kRightNumPartitions);

  needsRepartitionForHashJoin();

  EXPECT_TRUE(left_needs_repartition_);
  EXPECT_FALSE(right_needs_repartition_);
  EXPECT_EQ(kRightNumPartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftNonHashPartitionRightHashPartitionMatchedJoinAttributesTest) {
  const size_t kRightNumPartitions = 64u;
  useRightHashPartitionMatchedJoinAttributes(kRightNumPartitions);

  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    left_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 8u, P::PartitionSchemeHeader::PartitionExprIds());

    needsRepartitionForHashJoin();

    EXPECT_TRUE(left_needs_repartition_);
    EXPECT_FALSE(right_needs_repartition_);
    EXPECT_EQ(kRightNumPartitions, num_partitions_);
  }
}

TEST_F(PartitionTest, HashJoinLeftNoPartitionRightHashPartitionNotMatchedJoinAttributesTest) {
  useRightHashPartitionNotMatchedJoinAttributes(16u);

  needsRepartitionForHashJoin();

  EXPECT_TRUE(left_needs_repartition_);
  EXPECT_TRUE(right_needs_repartition_);
  EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionMatchedJoinAttributesRightHashPartitionNotMatchedJoinAttributesTest) {
  const size_t kLeftNumPartitions = 64u;
  useLeftHashPartitionMatchedJoinAttributes(kLeftNumPartitions);
  useRightHashPartitionNotMatchedJoinAttributes(16u);

  needsRepartitionForHashJoin();

  EXPECT_FALSE(left_needs_repartition_);
  EXPECT_TRUE(right_needs_repartition_);
  EXPECT_EQ(kLeftNumPartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionNotMatchedJoinAttributesRightHashPartitionNotMatchedJoinAttributesTest) {
  useLeftHashPartitionNotMatchedJoinAttributes(64u);
  useRightHashPartitionNotMatchedJoinAttributes(16u);

  needsRepartitionForHashJoin();

  EXPECT_TRUE(left_needs_repartition_);
  EXPECT_TRUE(right_needs_repartition_);
  EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
}

TEST_F(PartitionTest, HashJoinLeftNonHashPartitionRightHashPartitionNotMatchedJoinAttributesTest) {
  useRightHashPartitionNotMatchedJoinAttributes(16u);

  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    left_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 8u, P::PartitionSchemeHeader::PartitionExprIds());

    needsRepartitionForHashJoin();

    EXPECT_TRUE(left_needs_repartition_);
    EXPECT_TRUE(right_needs_repartition_);
    EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
  }
}

TEST_F(PartitionTest, HashJoinLeftNoPartitionRightNonHashPartitionTest) {
  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    right_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 64u, P::PartitionSchemeHeader::PartitionExprIds());

    needsRepartitionForHashJoin();

    EXPECT_TRUE(left_needs_repartition_);
    EXPECT_TRUE(right_needs_repartition_);
    EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
  }
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionMatchedJoinAttributesRightNonHashPartitionTest) {
  const size_t kLeftNumPartitions = 128u;
  useLeftHashPartitionMatchedJoinAttributes(kLeftNumPartitions);

  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    right_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 64u, P::PartitionSchemeHeader::PartitionExprIds());

    needsRepartitionForHashJoin();

    EXPECT_FALSE(left_needs_repartition_);
    EXPECT_TRUE(right_needs_repartition_);
    EXPECT_EQ(kLeftNumPartitions, num_partitions_);
  }
}

TEST_F(PartitionTest, HashJoinLeftHashPartitionNotMatchedJoinAttributesRightNonHashPartitionTest) {
  useLeftHashPartitionNotMatchedJoinAttributes(16u);

  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    right_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 64u, P::PartitionSchemeHeader::PartitionExprIds());

    needsRepartitionForHashJoin();

    EXPECT_TRUE(left_needs_repartition_);
    EXPECT_TRUE(right_needs_repartition_);
    EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
  }
}

TEST_F(PartitionTest, HashJoinLeftNonHashPartitionRightNonHashPartitionTest) {
  for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                     P::PartitionSchemeHeader::PartitionType::kRange }) {
    left_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
        partition_type, 8u, P::PartitionSchemeHeader::PartitionExprIds());

    for (const auto partition_type : { P::PartitionSchemeHeader::PartitionType::kRandom,
                                       P::PartitionSchemeHeader::PartitionType::kRange }) {
      right_partition_scheme_header_ = make_unique<P::PartitionSchemeHeader>(
          partition_type, 64u, P::PartitionSchemeHeader::PartitionExprIds());

      needsRepartitionForHashJoin();

      EXPECT_TRUE(left_needs_repartition_);
      EXPECT_TRUE(right_needs_repartition_);
      EXPECT_EQ(FLAGS_num_repartitions, num_partitions_);
    }
  }
}

}  // namespace optimizer
}  // namespace quickstep
