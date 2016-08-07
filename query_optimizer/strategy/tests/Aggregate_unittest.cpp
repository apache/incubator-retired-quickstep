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

#include "query_optimizer/strategy/Aggregate.hpp"

#include <vector>

#include "expressions/aggregation/AggregateFunctionFactory.hpp"
#include "expressions/aggregation/AggregationID.hpp"
#include "query_optimizer/OptimizerContext.hpp"
#include "query_optimizer/expressions/AggregateFunction.hpp"
#include "query_optimizer/expressions/Alias.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/expressions/NamedExpression.hpp"
#include "query_optimizer/expressions/Predicate.hpp"
#include "query_optimizer/logical/Aggregate.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/Project.hpp"
#include "query_optimizer/logical/TableReference.hpp"
#include "query_optimizer/physical/Aggregate.hpp"
#include "query_optimizer/physical/Physical.hpp"
#include "query_optimizer/physical/Selection.hpp"
#include "query_optimizer/physical/TableReference.hpp"
#include "query_optimizer/strategy/tests/StrategyTest.hpp"
#include "types/TypeID.hpp"
#include "utility/Cast.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {
namespace optimizer {
namespace strategy {

namespace E = ::quickstep::optimizer::expressions;
namespace L = ::quickstep::optimizer::logical;
namespace P = ::quickstep::optimizer::physical;

class AggregateTest : public StrategyTest {
 protected:
  AggregateTest() : StrategyTest() {}

  ~AggregateTest() override {}

  void SetUp() override { StrategyTest::SetUp(); }

  void setupStrategy(std::unique_ptr<Strategy> *strategy) override {
    strategy->reset(new Aggregate(physical_generator()));
  }

  E::AggregateFunctionPtr createSum(const E::ScalarPtr &argument,
                                    const bool is_vector_aggregate,
                                    const bool is_distinct) {
    std::vector<E::ScalarPtr> args(1, argument);

    return E::AggregateFunction::Create(
        AggregateFunctionFactory::Get(AggregationID::kSum),
        args,
        is_vector_aggregate,
        is_distinct);
  }

 private:
  DISALLOW_COPY_AND_ASSIGN(AggregateTest);
};

TEST_F(AggregateTest, BasicTest) {
  for (const bool is_distinct : {true, false}) {
    const E::AggregateFunctionPtr sum = createSum(add_literal_0_,
                                                  true /* is_vector_attribute */,
                                                  is_distinct);
    const E::AliasPtr sum_alias = createAlias(sum,
                                              "sum" /* attribute_name */,
                                              "test_relation" /* relation_name */);
    input_logical_ = L::Aggregate::Create(
        logical_table_reference_0_,
        {alias_0_} /* grouping_expressions */,
        {sum_alias} /* aggregate_expressions */);
    expected_physical_ = P::Aggregate::Create(
        physical_table_reference_0_,
        {alias_0_} /* grouping_expressions */,
        {sum_alias} /* aggregate_expressions */,
        nullptr /* filter_predicate */);
    EXPECT_CORRECT_PHYSICAL();
  }
}

TEST_F(AggregateTest, PullupSelection) {
  for (const bool is_distinct : {true, false}) {
    const E::AggregateFunctionPtr sum = createSum(E::ToRef(alias_add_literal_0_),
                                                  true /* is_vector_attribute */,
                                                  is_distinct);
    const E::AliasPtr sum_alias = createAlias(sum,
                                              "sum" /* attribute_name */,
                                              "test_relation" /* relation_name */);
    const E::AliasPtr alias_on_alias_reference = E::Alias::Create(
        optimizer_context()->nextExprId(),
        E::ToRef(alias_add_literal_0_),
        "alias_on_alias_reference" /* attribute_name */,
        "alias_on_alias_reference" /* attribute_alias */,
        "test_relation" /* relation_name */);

    input_logical_ = L::Aggregate::Create(
        logical_project_on_filter_0_,
        {E::ToRef(alias_0_), alias_on_alias_reference} /* grouping_expressions */,
        {sum_alias} /* aggregate_expressions */);

    const E::AggregateFunctionPtr sum_after_pullup =
        std::static_pointer_cast<const E::AggregateFunction>(
            sum->copyWithNewChildren({alias_add_literal_0_->expression()}));
    const E::AliasPtr sum_alias_after_pullup =
        std::static_pointer_cast<const E::Alias>(sum_alias->copyWithNewChildren({sum_after_pullup}));
    const E::AliasPtr alias_on_alias_reference_after_pullup =
        std::static_pointer_cast<const E::Alias>(alias_on_alias_reference->copyWithNewChildren(
            {alias_add_literal_0_->expression()} /* new_children */));
    expected_physical_ = P::Aggregate::Create(
        physical_table_reference_0_,
        {alias_0_,
         alias_on_alias_reference_after_pullup} /* grouping_expressions */,
        {sum_alias_after_pullup} /* aggregate_expressions */,
        logical_filter_0_->filter_predicate());

    EXPECT_CORRECT_PHYSICAL();
  }
}

}  // namespace strategy
}  // namespace optimizer
}  // namespace quickstep
