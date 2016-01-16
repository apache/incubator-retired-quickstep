/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_FACTORY_HPP_
#define QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_FACTORY_HPP_

#include <string>

#include "expressions/aggregation/AggregationID.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class AggregateFunction;
namespace serialization { class AggregateFunction; }

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief All-static factory class that provides access to the various concrete
 *        implementations of AggregateFunction.
 *
 * AggregateFunctionFactory allows client code to use any AggregateFunction in
 * Quickstep in a generic way without having to know about all the specific
 * subclasses of AggregateFunction. In particular, it is used to deserialize
 * AggregateFunctions used in AggregationOperationState from their protobuf
 * representations (originally created by the optimizer) when deserializing a
 * QueryContext.
 **/
class AggregateFunctionFactory {
 public:
  /**
   * @brief Get a particular AggregateFunction by its ID.
   *
   * @param agg_id The ID of the desired AggregateFunction.
   * @return A reference to the singleton instance of the AggregateFunction
   *         specified by agg_id.
   **/
  static const AggregateFunction& Get(const AggregationID agg_id);

  /**
   * @brief Get a particular AggregateFunction by its name in SQL syntax.
   *
   * @param name The name of the desired AggregateFunction in lower case.
   * @return A pointer to the AggregateFunction specified by name, or NULL if
   *         name does not match any known AggregateFunction.
   **/
  static const AggregateFunction* GetByName(const std::string &name);

  /**
   * @brief Determine if a serialized protobuf representation of an
   *        AggregateFunction is fully-formed and valid.
   *
   * @param proto A serialized protobuf representation of an AggregateFunction
   *        to check for validity.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::AggregateFunction &proto);

  /**
   * @brief Get the AggregateFunction represented by a proto.
   *
   * @warning It is an error to call this method with an invalid proto.
   *          ProtoIsValid() should be called first to check.
   *
   * @param proto A serialized protobuf representation of an AggregateFunction.
   * @return The AggregateFunction represented by proto.
   **/
  static const AggregateFunction& ReconstructFromProto(
      const serialization::AggregateFunction &proto);

 private:
  // Class is all-static and can not be instantiated.
  AggregateFunctionFactory();

  DISALLOW_COPY_AND_ASSIGN(AggregateFunctionFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_AGGREGATION_AGGREGATE_FUNCTION_FACTORY_HPP_
