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

#ifndef QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_
#define QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_

#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/Expressions.pb.h"
#include "expressions/scalar/Scalar.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "types/TypedValue.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogAttribute;
class ColumnVector;
class ValueAccessor;

struct SubBlocksReference;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Scalars which are attribute values from tuples.
 **/
class ScalarAttribute : public Scalar {
 public:
  /**
   * @brief Constructor.
   *
   * @param attribute The attribute to use.
   **/
  explicit ScalarAttribute(const CatalogAttribute &attribute);

  serialization::Scalar getProto() const override;

  Scalar* clone() const override;

  ScalarDataSource getDataSource() const override {
    return kAttribute;
  }

  TypedValue getValueForSingleTuple(const ValueAccessor &accessor,
                                    const tuple_id tuple) const override;

  TypedValue getValueForJoinedTuples(
      const ValueAccessor &left_accessor,
      const relation_id left_relation_id,
      const tuple_id left_tuple_id,
      const ValueAccessor &right_accessor,
      const relation_id right_relation_id,
      const tuple_id right_tuple_id) const override;

  attribute_id getAttributeIdForValueAccessor() const override;

  relation_id getRelationIdForValueAccessor() const override;

  ColumnVector* getAllValues(ValueAccessor *accessor,
                             const SubBlocksReference *sub_blocks_ref) const override;

  ColumnVector* getAllValuesForJoin(
      const relation_id left_relation_id,
      ValueAccessor *left_accessor,
      const relation_id right_relation_id,
      ValueAccessor *right_accessor,
      const std::vector<std::pair<tuple_id, tuple_id>> &joined_tuple_ids) const override;

  const CatalogAttribute& getAttribute() const {
    return attribute_;
  }

  void getDependencyAttributes(std::vector<const CatalogAttribute*> attrs) const override {
    attrs.push_back(&attribute_);
  }
  
 protected:
  const CatalogAttribute &attribute_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ScalarAttribute);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_EXPRESSIONS_SCALAR_SCALAR_ATTRIBUTE_HPP_
