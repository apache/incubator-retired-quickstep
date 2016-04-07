/**
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

#include "transaction/ResourceId.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class ResourceIdTest : public ::testing::Test {
 protected:
  ResourceIdTest()
      : db1_(1),
        db2_(2),
        db2_rel2_(2, 2),
        db2_rel4_(2, 4),
        db2_rel2_block2_(2, 2, 2),
        db2_rel2_block3_(2, 2, 3),
        db2_rel4_block5_(2, 4, 5),
        db2_rel4_block7_(2, 4, 7),
        db2_rel2_block2_tpl4_(2, 2, 2, 4),
        db2_rel2_block2_tpl7_(2, 2, 2, 7),
        db2_rel4_block5_tpl3_(2, 4, 5, 7),
        db2_rel4_block5_tpl8_(2, 4, 5, 8) {
  }

  const ResourceId db1_;
  const ResourceId db2_;
  const ResourceId db2_rel2_;
  const ResourceId db2_rel4_;
  const ResourceId db2_rel2_block2_;
  const ResourceId db2_rel2_block3_;
  const ResourceId db2_rel4_block5_;
  const ResourceId db2_rel4_block7_;
  const ResourceId db2_rel2_block2_tpl4_;
  const ResourceId db2_rel2_block2_tpl7_;
  const ResourceId db2_rel4_block5_tpl3_;
  const ResourceId db2_rel4_block5_tpl8_;
};

TEST_F(ResourceIdTest, CheckAccessLevels) {
  EXPECT_TRUE(db2_.isDatabaseAccess());
  EXPECT_FALSE(db2_rel4_.isDatabaseAccess());
  EXPECT_FALSE(db2_rel4_block5_.isDatabaseAccess());
  EXPECT_FALSE(db2_rel4_block5_tpl8_.isDatabaseAccess());

  EXPECT_FALSE(db2_.isRelationAccess());
  EXPECT_TRUE(db2_rel4_.isRelationAccess());
  EXPECT_FALSE(db2_rel4_block5_.isRelationAccess());
  EXPECT_FALSE(db2_rel4_block5_tpl8_.isRelationAccess());

  EXPECT_FALSE(db2_.isBlockAccess());
  EXPECT_FALSE(db2_rel4_.isBlockAccess());
  EXPECT_TRUE(db2_rel4_block5_.isBlockAccess());
  EXPECT_FALSE(db2_rel4_block5_tpl8_.isBlockAccess());

  EXPECT_FALSE(db2_.isTupleAccess());
  EXPECT_FALSE(db2_rel4_.isTupleAccess());
  EXPECT_FALSE(db2_rel4_block5_.isTupleAccess());
  EXPECT_TRUE(db2_rel4_block5_tpl8_.isTupleAccess());
}

TEST_F(ResourceIdTest, Equality) {
  // Copy some resource ids.
  const ResourceId db1_copy(db1_);
  const ResourceId db2_rel2_copy(db2_rel2_);
  const ResourceId db2_rel4_block5_copy(db2_rel4_block5_);
  const ResourceId db2_rel4_block5_tpl8_copy(db2_rel4_block5_tpl8_);

  // Self comparison must be equal.
  EXPECT_EQ(db1_, db1_);
  EXPECT_EQ(db2_rel2_, db2_rel2_);
  EXPECT_EQ(db2_rel4_block5_, db2_rel4_block5_);
  EXPECT_EQ(db2_rel4_block5_tpl3_, db2_rel4_block5_tpl3_);

  // If resources are different, than it must have different ids.
  EXPECT_NE(db1_, db2_);
  EXPECT_NE(db2_rel2_, db2_rel4_);
  EXPECT_NE(db2_rel4_block5_, db2_rel4_block7_);
  EXPECT_NE(db2_rel4_block5_tpl3_, db2_rel4_block5_tpl8_);

  // Comparison with a copy must be equal.
  EXPECT_EQ(db1_copy, db1_);
  EXPECT_EQ(db2_rel2_copy, db2_rel2_);
  EXPECT_EQ(db2_rel4_block5_copy, db2_rel4_block5_);
  EXPECT_EQ(db2_rel4_block5_tpl8_copy, db2_rel4_block5_tpl8_);
}

TEST_F(ResourceIdTest, ParentResourceIds) {
  EXPECT_EQ(db2_, db2_rel2_.getParentResourceId());
  EXPECT_EQ(db2_rel2_, db2_rel2_block2_.getParentResourceId());
  EXPECT_EQ(db2_rel2_, db2_rel2_block3_.getParentResourceId());
  EXPECT_EQ(db2_rel2_block2_, db2_rel2_block2_tpl4_.getParentResourceId());
  EXPECT_EQ(db2_rel2_block2_, db2_rel2_block2_tpl7_.getParentResourceId());
}

}  // namespace transaction
}  // namespace quickstep
