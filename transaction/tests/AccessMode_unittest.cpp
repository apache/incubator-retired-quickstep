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

#include "transaction/AccessMode.hpp"

#include "gtest/gtest.h"

namespace quickstep {
namespace transaction {

class AccessModeTest : public ::testing::Test {
 protected:
  AccessModeTest()
      : nl_mode_(AccessMode::NoLockMode()),
        is_mode_(AccessMode::IsLockMode()),
        ix_mode_(AccessMode::IxLockMode()),
        s_mode_(AccessMode::SLockMode()),
        six_mode_(AccessMode::SixLockMode()),
        x_mode_(AccessMode::XLockMode()) {
  }

  const AccessMode nl_mode_;
  const AccessMode is_mode_;
  const AccessMode ix_mode_;
  const AccessMode s_mode_;
  const AccessMode six_mode_;
  const AccessMode x_mode_;
};

TEST_F(AccessModeTest, ModeCompatibilty) {
  EXPECT_TRUE(nl_mode_.isCompatible(nl_mode_));
  EXPECT_TRUE(nl_mode_.isCompatible(is_mode_));
  EXPECT_TRUE(nl_mode_.isCompatible(ix_mode_));
  EXPECT_TRUE(nl_mode_.isCompatible(s_mode_));
  EXPECT_TRUE(nl_mode_.isCompatible(six_mode_));
  EXPECT_TRUE(nl_mode_.isCompatible(x_mode_));

  EXPECT_TRUE(is_mode_.isCompatible(nl_mode_));
  EXPECT_TRUE(is_mode_.isCompatible(is_mode_));
  EXPECT_TRUE(is_mode_.isCompatible(ix_mode_));
  EXPECT_TRUE(is_mode_.isCompatible(s_mode_));
  EXPECT_TRUE(is_mode_.isCompatible(six_mode_));
  EXPECT_FALSE(is_mode_.isCompatible(x_mode_));

  EXPECT_TRUE(ix_mode_.isCompatible(nl_mode_));
  EXPECT_TRUE(ix_mode_.isCompatible(is_mode_));
  EXPECT_TRUE(ix_mode_.isCompatible(ix_mode_));
  EXPECT_FALSE(ix_mode_.isCompatible(s_mode_));
  EXPECT_FALSE(ix_mode_.isCompatible(six_mode_));
  EXPECT_FALSE(ix_mode_.isCompatible(x_mode_));

  EXPECT_TRUE(s_mode_.isCompatible(nl_mode_));
  EXPECT_TRUE(s_mode_.isCompatible(is_mode_));
  EXPECT_FALSE(s_mode_.isCompatible(ix_mode_));
  EXPECT_TRUE(s_mode_.isCompatible(s_mode_));
  EXPECT_FALSE(s_mode_.isCompatible(six_mode_));
  EXPECT_FALSE(s_mode_.isCompatible(x_mode_));

  EXPECT_TRUE(six_mode_.isCompatible(nl_mode_));
  EXPECT_TRUE(six_mode_.isCompatible(is_mode_));
  EXPECT_FALSE(six_mode_.isCompatible(ix_mode_));
  EXPECT_FALSE(six_mode_.isCompatible(s_mode_));
  EXPECT_FALSE(six_mode_.isCompatible(six_mode_));
  EXPECT_FALSE(six_mode_.isCompatible(x_mode_));

  EXPECT_TRUE(x_mode_.isCompatible(nl_mode_));
  EXPECT_FALSE(x_mode_.isCompatible(is_mode_));
  EXPECT_FALSE(x_mode_.isCompatible(ix_mode_));
  EXPECT_FALSE(x_mode_.isCompatible(s_mode_));
  EXPECT_FALSE(x_mode_.isCompatible(six_mode_));
  EXPECT_FALSE(x_mode_.isCompatible(x_mode_));
}

TEST_F(AccessModeTest, ModeQueryChecks) {
  EXPECT_FALSE(nl_mode_.isIntentionShareLock());
  EXPECT_FALSE(nl_mode_.isIntentionExclusiveLock());
  EXPECT_FALSE(nl_mode_.isShareLock());
  EXPECT_FALSE(nl_mode_.isShareAndIntentionExclusiveLock());
  EXPECT_FALSE(nl_mode_.isExclusiveLock());

  EXPECT_TRUE(is_mode_.isIntentionShareLock());
  EXPECT_FALSE(is_mode_.isIntentionExclusiveLock());
  EXPECT_FALSE(is_mode_.isShareLock());
  EXPECT_FALSE(is_mode_.isShareAndIntentionExclusiveLock());
  EXPECT_FALSE(is_mode_.isExclusiveLock());

  EXPECT_FALSE(ix_mode_.isIntentionShareLock());
  EXPECT_TRUE(ix_mode_.isIntentionExclusiveLock());
  EXPECT_FALSE(ix_mode_.isShareLock());
  EXPECT_FALSE(ix_mode_.isShareAndIntentionExclusiveLock());
  EXPECT_FALSE(ix_mode_.isExclusiveLock());

  EXPECT_FALSE(s_mode_.isIntentionShareLock());
  EXPECT_FALSE(s_mode_.isIntentionExclusiveLock());
  EXPECT_TRUE(s_mode_.isShareLock());
  EXPECT_FALSE(s_mode_.isShareAndIntentionExclusiveLock());
  EXPECT_FALSE(s_mode_.isExclusiveLock());

  EXPECT_FALSE(six_mode_.isIntentionShareLock());
  EXPECT_FALSE(six_mode_.isIntentionExclusiveLock());
  EXPECT_FALSE(six_mode_.isShareLock());
  EXPECT_TRUE(six_mode_.isShareAndIntentionExclusiveLock());
  EXPECT_FALSE(six_mode_.isExclusiveLock());

  EXPECT_FALSE(x_mode_.isIntentionShareLock());
  EXPECT_FALSE(x_mode_.isIntentionExclusiveLock());
  EXPECT_FALSE(x_mode_.isShareLock());
  EXPECT_FALSE(x_mode_.isShareAndIntentionExclusiveLock());
  EXPECT_TRUE(x_mode_.isExclusiveLock());
}

TEST_F(AccessModeTest, Equality) {
  EXPECT_EQ(nl_mode_, nl_mode_);
  EXPECT_NE(nl_mode_, is_mode_);
  EXPECT_NE(nl_mode_, ix_mode_);
  EXPECT_NE(nl_mode_, s_mode_);
  EXPECT_NE(nl_mode_, six_mode_);
  EXPECT_NE(nl_mode_, x_mode_);

  EXPECT_NE(is_mode_, nl_mode_);
  EXPECT_EQ(is_mode_, is_mode_);
  EXPECT_NE(is_mode_, ix_mode_);
  EXPECT_NE(is_mode_, s_mode_);
  EXPECT_NE(is_mode_, six_mode_);
  EXPECT_NE(is_mode_, x_mode_);

  EXPECT_NE(ix_mode_, nl_mode_);
  EXPECT_NE(ix_mode_, is_mode_);
  EXPECT_EQ(ix_mode_, ix_mode_);
  EXPECT_NE(ix_mode_, s_mode_);
  EXPECT_NE(ix_mode_, six_mode_);
  EXPECT_NE(ix_mode_, x_mode_);

  EXPECT_NE(s_mode_, nl_mode_);
  EXPECT_NE(s_mode_, is_mode_);
  EXPECT_NE(s_mode_, ix_mode_);
  EXPECT_EQ(s_mode_, s_mode_);
  EXPECT_NE(s_mode_, six_mode_);
  EXPECT_NE(s_mode_, x_mode_);

  EXPECT_NE(six_mode_, nl_mode_);
  EXPECT_NE(six_mode_, is_mode_);
  EXPECT_NE(six_mode_, ix_mode_);
  EXPECT_NE(six_mode_, s_mode_);
  EXPECT_EQ(six_mode_, six_mode_);
  EXPECT_NE(six_mode_, x_mode_);

  EXPECT_NE(x_mode_, nl_mode_);
  EXPECT_NE(x_mode_, is_mode_);
  EXPECT_NE(x_mode_, ix_mode_);
  EXPECT_NE(x_mode_, s_mode_);
  EXPECT_NE(x_mode_, six_mode_);
  EXPECT_EQ(x_mode_, x_mode_);
}

}  // namespace transaction
}  // namespace quickstep
