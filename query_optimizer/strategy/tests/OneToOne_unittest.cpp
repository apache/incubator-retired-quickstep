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

#include "query_optimizer/strategy/OneToOne.hpp"

#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/logical/TopLevelPlan.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/physical/TopLevelPlan.hpp"
#include "query_optimizer/strategy/tests/StrategyTest.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

class OneToOneTest : public StrategyTest {
 protected:
  OneToOneTest() : StrategyTest() {}

  ~OneToOneTest() override {}

  void setupStrategy(std::unique_ptr<Strategy> *strategy) override {
    strategy->reset(new OneToOne(physical_generator()));
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(OneToOneTest);
};

TEST_F(OneToOneTest, TopLevelPlanTest) {
  input_logical_ = L::TopLevelPlan::Create(logical_project_0_);
  expected_physical_ = P::TopLevelPlan::Create(physical_project_0_);
  EXPECT_CORRECT_PHYSICAL();
}

TEST_F(OneToOneTest, TableReferenceTest) {
  const L::TableReferencePtr table_reference =
      createTableReference("one_to_one_test_relation");
  input_logical_ = table_reference;
  expected_physical_ = P::TableReference::Create(table_reference->catalog_relation(),
                                                 table_reference->relation_alias(),
                                                 table_reference->attribute_list());
  EXPECT_CORRECT_PHYSICAL();
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
