/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
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
