/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_ID_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_ID_HPP_

namespace quickstep {

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief The possible types of aggregations.
 **/
enum class AggregationID {
  kAvg = 0,
  kCount,
  kMax,
  kMin,
  kSum
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATION_ID_HPP_
