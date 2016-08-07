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

#ifndef QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_COPYFROM_HPP_
#define QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_COPYFROM_HPP_

#include <memory>
#include <string>
#include <vector>

#include "query_optimizer/OptimizerTree.hpp"
#include "query_optimizer/expressions/AttributeReference.hpp"
#include "query_optimizer/logical/Logical.hpp"
#include "query_optimizer/logical/LogicalType.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelation;

namespace optimizer {
namespace logical {

/** \addtogroup OptimizerLogical
 *  @{
 */

class CopyFrom;
typedef std::shared_ptr<const CopyFrom> CopyFromPtr;

/**
 * @brief Represents an operation that copies data from a text file to a relation.
 */
class CopyFrom : public Logical {
 public:
  LogicalType getLogicalType() const override { return LogicalType::kCopyFrom; }

  std::string getName() const override { return "CopyFrom"; }

  /**
   * @return The catalog relation to insert the tuples to.
   */
  const CatalogRelation* catalog_relation() const { return catalog_relation_; }

  /**
   * @return The name of the file to read the data from.
   */
  const std::string& file_name() const { return file_name_; }

  /**
   * @return The delimiter used in the text file to separate columns.
   */
  const char column_delimiter() const { return column_delimiter_; }

  /**
   * @return Whether to decode escape sequences in the text file.
   */
  bool escape_strings() const { return escape_strings_; }

  LogicalPtr copyWithNewChildren(
      const std::vector<LogicalPtr> &new_children) const override {
    DCHECK(new_children.empty());
    return Create(catalog_relation_, file_name_, column_delimiter_,
                  escape_strings_);
  }

  std::vector<expressions::AttributeReferencePtr> getOutputAttributes() const override {
    return std::vector<expressions::AttributeReferencePtr>();
  }

  std::vector<expressions::AttributeReferencePtr> getReferencedAttributes() const override {
    return std::vector<expressions::AttributeReferencePtr>();
  }

  /**
   * @brief Creates a CopyFrom logical node.
   *
   * @param catalog_relation The catalog relation to insert the tuples to.
   * @param file_name The name of the file to read the data from.
   * @param column_delimiter The delimiter used in the text file to separate
   *                         columns.
   * @param escape_strings Whether to decode escape sequences in the text file.
   * @return An immutable CopyFrom logical node.
   */
  static CopyFromPtr Create(const CatalogRelation *catalog_relation,
                            const std::string &file_name,
                            const char column_delimiter,
                            bool escape_strings) {
    return CopyFromPtr(new CopyFrom(catalog_relation,
                                    file_name,
                                    column_delimiter,
                                    escape_strings));
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<OptimizerTreeBaseNodePtr> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<OptimizerTreeBaseNodePtr>> *container_child_fields) const override;

 private:
  CopyFrom(const CatalogRelation *catalog_relation,
           const std::string &file_name,
           const char column_delimiter,
           bool escape_strings)
      : catalog_relation_(catalog_relation),
        file_name_(file_name),
        column_delimiter_(column_delimiter),
        escape_strings_(escape_strings) {}

  const CatalogRelation *catalog_relation_;
  std::string file_name_;

  const char column_delimiter_;
  const bool escape_strings_;

  DISALLOW_COPY_AND_ASSIGN(CopyFrom);
};

/** @} */

}  // namespace logical
}  // namespace optimizer
}  // namespace quickstep

#endif /* QUICKSTEP_QUERY_OPTIMIZER_LOGICAL_COPYFROM_HPP_ */
