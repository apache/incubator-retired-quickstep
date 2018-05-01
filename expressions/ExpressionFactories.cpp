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

#include "expressions/ExpressionFactories.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogDatabaseLite.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/predicate/ComparisonPredicate.hpp"
#include "expressions/predicate/ConjunctionPredicate.hpp"
#include "expressions/predicate/DisjunctionPredicate.hpp"
#include "expressions/predicate/NegationPredicate.hpp"
#include "expressions/predicate/Predicate.hpp"
#include "expressions/predicate/TrivialPredicates.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "expressions/scalar/ScalarAttribute.hpp"
#include "expressions/scalar/ScalarBinaryExpression.hpp"
#include "expressions/scalar/ScalarCaseExpression.hpp"
#include "expressions/scalar/ScalarLiteral.hpp"
#include "expressions/scalar/ScalarSharedExpression.hpp"
#include "expressions/scalar/ScalarUnaryExpression.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/binary_operations/BinaryOperationFactory.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/unary_operations/UnaryOperationFactory.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

namespace S = serialization;

class Type;

Predicate* PredicateFactory::ReconstructFromProto(const serialization::Predicate &proto,
                                                  const CatalogDatabaseLite &database) {
  DCHECK(ProtoIsValid(proto, database))
      << "Attempted to create Predicate from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.predicate_type()) {
    case serialization::Predicate::TRUE:
      return new TruePredicate();
    case serialization::Predicate::FALSE:
      return new FalsePredicate();
    case serialization::Predicate::COMPARISON:
      return new ComparisonPredicate(
          ComparisonFactory::ReconstructFromProto(proto.GetExtension(serialization::ComparisonPredicate::comparison)),
          ScalarFactory::ReconstructFromProto(proto.GetExtension(serialization::ComparisonPredicate::left_operand),
                                              database),
          ScalarFactory::ReconstructFromProto(proto.GetExtension(serialization::ComparisonPredicate::right_operand),
                                              database));
    case serialization::Predicate::NEGATION:
      return NegationPredicate::NegatePredicate(
          ReconstructFromProto(proto.GetExtension(serialization::NegationPredicate::operand), database));
    case serialization::Predicate::CONJUNCTION: {
      ConjunctionPredicate* predicate = new ConjunctionPredicate();
      for (int i = 0;
           i < proto.ExtensionSize(serialization::PredicateWithList::operands);
           ++i) {
        predicate->addPredicate(
            ReconstructFromProto(proto.GetExtension(serialization::PredicateWithList::operands, i), database));
      }
      return predicate;
    }
    case serialization::Predicate::DISJUNCTION: {
      DisjunctionPredicate* predicate = new DisjunctionPredicate();
      for (int i = 0;
           i < proto.ExtensionSize(serialization::PredicateWithList::operands);
           ++i) {
        predicate->addPredicate(
            ReconstructFromProto(proto.GetExtension(serialization::PredicateWithList::operands, i), database));
      }
      return predicate;
    }
    default:
      FATAL_ERROR("Unknown Predicate Type in PredicateFactory::ReconstructFromProto");
  }
}

bool PredicateFactory::ProtoIsValid(const serialization::Predicate &proto,
                                    const CatalogDatabaseLite &database) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the predicate_type is valid, and extensions if any.
  switch (proto.predicate_type()) {
    case serialization::Predicate::TRUE:  // Fall through.
    case serialization::Predicate::FALSE:
      return true;
    case serialization::Predicate::COMPARISON: {
      if (proto.HasExtension(serialization::ComparisonPredicate::comparison)
          && proto.HasExtension(serialization::ComparisonPredicate::left_operand)
          && proto.HasExtension(serialization::ComparisonPredicate::right_operand)) {
        return ComparisonFactory::ProtoIsValid(proto.GetExtension(serialization::ComparisonPredicate::comparison))
               && ScalarFactory::ProtoIsValid(proto.GetExtension(serialization::ComparisonPredicate::left_operand),
                                              database)
               && ScalarFactory::ProtoIsValid(proto.GetExtension(serialization::ComparisonPredicate::right_operand),
                                              database);
      }
      break;
    }
    case serialization::Predicate::NEGATION: {
      if (proto.HasExtension(serialization::NegationPredicate::operand)) {
        return ProtoIsValid(proto.GetExtension(serialization::NegationPredicate::operand), database);
      }
      break;
    }
    case serialization::Predicate::CONJUNCTION:  // Fall through.
    case serialization::Predicate::DISJUNCTION: {
      for (int i = 0;
           i < proto.ExtensionSize(serialization::PredicateWithList::operands);
           ++i) {
        if (!ProtoIsValid(proto.GetExtension(serialization::PredicateWithList::operands, i), database)) {
          return false;
        }
      }

      return true;
    }
    default: {
      break;
    }
  }
  return false;
}

Scalar* ScalarFactory::ReconstructFromProto(const serialization::Scalar &proto,
                                            const CatalogDatabaseLite &database) {
  DCHECK(ProtoIsValid(proto, database))
      << "Attempted to create Scalar from an invalid proto description:\n"
      << proto.DebugString();

  switch (proto.data_source()) {
    case serialization::Scalar::LITERAL:
      return new ScalarLiteral(
          TypedValue::ReconstructFromProto(proto.GetExtension(serialization::ScalarLiteral::literal)),
          TypeFactory::ReconstructFromProto(proto.GetExtension(serialization::ScalarLiteral::literal_type)));
    case serialization::Scalar::ATTRIBUTE: {
      const relation_id rel_id = proto.GetExtension(serialization::ScalarAttribute::relation_id);

      Scalar::JoinSide join_side;
      switch (proto.GetExtension(S::ScalarAttribute::join_side)) {
        case S::ScalarAttribute::NONE:
          join_side = Scalar::kNone;
          break;
        case S::ScalarAttribute::LEFT_SIDE:
          join_side = Scalar::kLeftSide;
          break;
        case S::ScalarAttribute::RIGHT_SIDE:
          join_side = Scalar::kRightSide;
          break;
      }

      DCHECK(database.hasRelationWithId(rel_id));
      return new ScalarAttribute(*database.getRelationSchemaById(rel_id).getAttributeById(
          proto.GetExtension(serialization::ScalarAttribute::attribute_id)), join_side);
    }
    case serialization::Scalar::UNARY_EXPRESSION: {
      return new ScalarUnaryExpression(
          UnaryOperationFactory::ReconstructFromProto(
              proto.GetExtension(serialization::ScalarUnaryExpression::operation)),
          ReconstructFromProto(proto.GetExtension(serialization::ScalarUnaryExpression::operand), database));
    }
    case serialization::Scalar::BINARY_EXPRESSION: {
      return new ScalarBinaryExpression(
          BinaryOperationFactory::ReconstructFromProto(
              proto.GetExtension(serialization::ScalarBinaryExpression::operation)),
          ReconstructFromProto(proto.GetExtension(serialization::ScalarBinaryExpression::left_operand), database),
          ReconstructFromProto(proto.GetExtension(serialization::ScalarBinaryExpression::right_operand), database));
    }
    case serialization::Scalar::CASE_EXPRESSION: {
      const Type &result_type = TypeFactory::ReconstructFromProto(
          proto.GetExtension(serialization::ScalarCaseExpression::result_type));

      std::vector<std::unique_ptr<Predicate>> when_predicates;
      for (int when_pred_num = 0;
           when_pred_num < proto.ExtensionSize(serialization::ScalarCaseExpression::when_predicate);
           ++when_pred_num) {
        when_predicates.emplace_back(PredicateFactory::ReconstructFromProto(
            proto.GetExtension(serialization::ScalarCaseExpression::when_predicate, when_pred_num),
            database));
      }

      std::vector<std::unique_ptr<Scalar>> result_expressions;
      for (int result_expr_num = 0;
           result_expr_num < proto.ExtensionSize(serialization::ScalarCaseExpression::result_expression);
           ++result_expr_num) {
        result_expressions.emplace_back(ReconstructFromProto(
            proto.GetExtension(serialization::ScalarCaseExpression::result_expression, result_expr_num),
            database));
      }

      std::unique_ptr<Scalar> else_result_expression(ReconstructFromProto(
          proto.GetExtension(serialization::ScalarCaseExpression::else_result_expression),
          database));

      return new ScalarCaseExpression(result_type,
                                      std::move(when_predicates),
                                      std::move(result_expressions),
                                      else_result_expression.release());
    }
    case serialization::Scalar::SHARED_EXPRESSION: {
      return new ScalarSharedExpression(
          proto.GetExtension(serialization::ScalarSharedExpression::share_id),
          ReconstructFromProto(proto.GetExtension(serialization::ScalarSharedExpression::operand), database));
    }
    default:
      FATAL_ERROR("Unknown Scalar data source in ScalarFactory::ReconstructFromProto");
  }
}

bool ScalarFactory::ProtoIsValid(const serialization::Scalar &proto,
                                 const CatalogDatabaseLite &database) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that the data_source is valid, and extensions if any.
  switch (proto.data_source()) {
    case serialization::Scalar::LITERAL: {
      return proto.HasExtension(serialization::ScalarLiteral::literal)
             && proto.HasExtension(serialization::ScalarLiteral::literal_type)
             && TypedValue::ProtoIsValid(proto.GetExtension(serialization::ScalarLiteral::literal))
             && TypeFactory::ProtoIsValid(proto.GetExtension(serialization::ScalarLiteral::literal_type));
    }
    case serialization::Scalar::ATTRIBUTE: {
      if (proto.HasExtension(serialization::ScalarAttribute::relation_id)
          && proto.HasExtension(serialization::ScalarAttribute::attribute_id)) {
        const relation_id rel_id = proto.GetExtension(serialization::ScalarAttribute::relation_id);
        const attribute_id attr_id = proto.GetExtension(serialization::ScalarAttribute::attribute_id);

        return database.hasRelationWithId(rel_id)
               && database.getRelationSchemaById(rel_id).hasAttributeWithId(attr_id);
      }
      break;
    }
    case serialization::Scalar::UNARY_EXPRESSION: {
      if (proto.HasExtension(serialization::ScalarUnaryExpression::operation)
          && proto.HasExtension(serialization::ScalarUnaryExpression::operand)) {
        return UnaryOperationFactory::ProtoIsValid(proto.GetExtension(serialization::ScalarUnaryExpression::operation))
               && ProtoIsValid(proto.GetExtension(serialization::ScalarUnaryExpression::operand), database);
      }
      break;
    }
    case serialization::Scalar::BINARY_EXPRESSION: {
      if (proto.HasExtension(serialization::ScalarBinaryExpression::operation)
          && proto.HasExtension(serialization::ScalarBinaryExpression::left_operand)
          && proto.HasExtension(serialization::ScalarBinaryExpression::right_operand)) {
        return BinaryOperationFactory::ProtoIsValid(
                   proto.GetExtension(serialization::ScalarBinaryExpression::operation))
               && ProtoIsValid(proto.GetExtension(serialization::ScalarBinaryExpression::left_operand), database)
               && ProtoIsValid(proto.GetExtension(serialization::ScalarBinaryExpression::right_operand), database);
      }
      break;
    }
    case serialization::Scalar::CASE_EXPRESSION: {
      // Check result type.
      if (!(proto.HasExtension(serialization::ScalarCaseExpression::result_type)
            && TypeFactory::ProtoIsValid(proto.GetExtension(
                serialization::ScalarCaseExpression::result_type)))) {
        return false;
      }

      // Check when-predicates and result expressions.
      if (proto.ExtensionSize(serialization::ScalarCaseExpression::when_predicate)
          == proto.ExtensionSize(serialization::ScalarCaseExpression::result_expression)) {
        for (int case_num = 0;
             case_num < proto.ExtensionSize(serialization::ScalarCaseExpression::when_predicate);
             ++case_num) {
          if (!PredicateFactory::ProtoIsValid(
                  proto.GetExtension(
                      serialization::ScalarCaseExpression::when_predicate,
                      case_num),
                  database)) {
            return false;
          }
          if (!ProtoIsValid(
                  proto.GetExtension(serialization::ScalarCaseExpression::result_expression,
                                     case_num),
                  database)) {
            return false;
          }
        }
      } else {
        return false;
      }

      // Check else-result expression.
      if (!(proto.HasExtension(serialization::ScalarCaseExpression::else_result_expression)
            && ProtoIsValid(proto.GetExtension(serialization::ScalarCaseExpression::else_result_expression),
                            database))) {
        return false;
      }

      // Everything checks out.
      return true;
    }
    case serialization::Scalar::SHARED_EXPRESSION: {
      return proto.HasExtension(serialization::ScalarSharedExpression::share_id)
             && proto.HasExtension(serialization::ScalarSharedExpression::operand);
    }
    default: {
      break;
    }
  }

  return false;
}

}  // namespace quickstep
