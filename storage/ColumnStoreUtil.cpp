/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#include "storage/ColumnStoreUtil.hpp"

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/TupleIdSequence.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "types/operations/comparisons/ComparisonUtil.hpp"
#include "utility/Macros.hpp"

namespace quickstep {
namespace column_store_util {

TupleIdSequence* SortColumnPredicateEvaluator::EvaluatePredicateForUncompressedSortColumn(
    const ComparisonPredicate &predicate,
    const CatalogRelationSchema &relation,
    const attribute_id sort_attribute_id,
    void *sort_attribute_stripe,
    const tuple_id num_tuples) {
  // Determine if the predicate is a comparison of the sort column with a literal.
  if (predicate.isAttributeLiteralComparisonPredicate()) {
    const CatalogAttribute *comparison_attribute = NULL;
    bool left_literal = false;
    if (predicate.getLeftOperand().hasStaticValue()) {
      DEBUG_ASSERT(predicate.getRightOperand().getDataSource() == Scalar::kAttribute);
      comparison_attribute
          = &(static_cast<const ScalarAttribute&>(predicate.getRightOperand()).getAttribute());
      left_literal = true;
    } else {
      DEBUG_ASSERT(predicate.getLeftOperand().getDataSource() == Scalar::kAttribute);
      comparison_attribute
          = &(static_cast<const ScalarAttribute&>(predicate.getLeftOperand()).getAttribute());
      left_literal = false;
    }

    DEBUG_ASSERT(comparison_attribute->getParent().getID() == relation.getID());
    if (comparison_attribute->getID() == sort_attribute_id) {
      const Type &attr_type = comparison_attribute->getType();

      TypedValue comparison_literal;
      const Type *literal_type;
      if (left_literal) {
        comparison_literal = predicate.getLeftOperand().getStaticValue().makeReferenceToThis();
        literal_type = &predicate.getLeftOperand().getType();
      } else {
        comparison_literal = predicate.getRightOperand().getStaticValue().makeReferenceToThis();
        literal_type = &predicate.getRightOperand().getType();
      }
      const bool same_types = literal_type->isSubsumedBy(attr_type);

      // Find the bounds on the range of matching tuples.
      tuple_id min_match = 0;
      tuple_id max_match_bound = num_tuples;
      ColumnStripeIterator begin_it(sort_attribute_stripe,
                                    attr_type.maximumByteLength(),
                                    0);
      ColumnStripeIterator end_it(sort_attribute_stripe,
                                  attr_type.maximumByteLength(),
                                  num_tuples);

      switch (predicate.getComparison().getComparisonID()) {
        case ComparisonID::kEqual:
        // Note: There is a special branch below for kNotEqual which takes the
        // complement of the matched range.
        case ComparisonID::kNotEqual: {
          ColumnStripeIterator min_match_it;
          if (same_types) {
            min_match_it = GetBoundForUntypedValue<
                ColumnStripeIterator,
                LowerBoundFunctor>(attr_type,
                                   begin_it,
                                   end_it,
                                   comparison_literal.getDataPtr());
            min_match = min_match_it.getTuplePosition();
            max_match_bound = GetBoundForUntypedValue<
                ColumnStripeIterator,
                UpperBoundFunctor>(attr_type,
                                   min_match_it,
                                   end_it,
                                   comparison_literal.getDataPtr()).getTuplePosition();
          } else {
            min_match_it = GetBoundForDifferentTypedValue<
                ColumnStripeIterator,
                LowerBoundFunctor>(attr_type,
                                   begin_it,
                                   end_it,
                                   comparison_literal,
                                   *literal_type);
            min_match = min_match_it.getTuplePosition();
            max_match_bound = GetBoundForDifferentTypedValue<
                ColumnStripeIterator,
                UpperBoundFunctor>(attr_type,
                                   min_match_it,
                                   end_it,
                                   comparison_literal,
                                   *literal_type).getTuplePosition();
          }
          break;
        }
        case ComparisonID::kLess:
          if (left_literal) {
            if (same_types) {
              min_match = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              min_match = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          } else {
            if (same_types) {
              max_match_bound = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              max_match_bound = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          }
          break;
        case ComparisonID::kLessOrEqual:
          if (left_literal) {
            if (same_types) {
              min_match = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              min_match = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          } else {
            if (same_types) {
              max_match_bound = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              max_match_bound = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          }
          break;
        case ComparisonID::kGreater:
          if (left_literal) {
            if (same_types) {
              max_match_bound = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              max_match_bound = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          } else {
            if (same_types) {
              min_match = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              min_match = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          }
          break;
        case ComparisonID::kGreaterOrEqual:
          if (left_literal) {
            if (same_types) {
              max_match_bound = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              max_match_bound = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  UpperBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          } else {
            if (same_types) {
              min_match = GetBoundForUntypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal.getDataPtr()).getTuplePosition();
            } else {
              min_match = GetBoundForDifferentTypedValue<
                  ColumnStripeIterator,
                  LowerBoundFunctor>(attr_type,
                                     begin_it,
                                     end_it,
                                     comparison_literal,
                                     *literal_type).getTuplePosition();
            }
          }
          break;
        default:
          FATAL_ERROR("Unknown Comparison in SortColumnPredicateEvaluator::"
                      "EvaluatePredicateForUncompressedSortColumn()");
      }

      // Create and return the sequence of matches.
      TupleIdSequence *matches = new TupleIdSequence(num_tuples);
      if (predicate.getComparison().getComparisonID() == ComparisonID::kNotEqual) {
        // Special case: return all tuples NOT in the range for kEqual.
        matches->setRange(0, min_match, true);
        matches->setRange(max_match_bound, num_tuples - max_match_bound, true);
      } else {
        matches->setRange(min_match, max_match_bound - min_match, true);
      }

      return matches;
    } else {
      return NULL;
    }
  } else {
    // Can not evaluate a non-comparison predicate, so pass through.
    return NULL;
  }
}

}  // namespace column_store_util
}  // namespace quickstep
