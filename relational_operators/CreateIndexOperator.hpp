/**
 *   Copyright 2016, Quickstep Research Group, Computer Sciences Department,
 *     University of Wisconsin—Madison.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_CREATE_INDEX_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_CREATE_INDEX_OPERATOR_HPP_

#include <string>

#include "catalog/CatalogRelation.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogRelation;
class QueryContext;
class StorageManager;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which creates an index.
 **/
class CreateIndexOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation to create index upon.
   * @param index_name The index to create.
   * @param index_description The index_description associated with this index.
   **/
  CreateIndexOperator(CatalogRelation *relation,
                      const std::string &index_name,
                      IndexSubBlockDescription &&index_description)  // NOLINT(whitespace/operators)
      : relation_(DCHECK_NOTNULL(relation)),
        index_name_(index_name),
        index_description_(index_description) {}

  ~CreateIndexOperator() override {}

  /**
   * @note no WorkOrder generated for this operator.
   **/
  bool getAllWorkOrders(WorkOrdersContainer *container,
                        QueryContext *query_context,
                        StorageManager *storage_manager,
                        const tmb::client_id scheduler_client_id,
                        tmb::MessageBus *bus) override;

  void updateCatalogOnCompletion() override;

 private:
  CatalogRelation *relation_;
  const std::string &index_name_;
  IndexSubBlockDescription index_description_;

  DISALLOW_COPY_AND_ASSIGN(CreateIndexOperator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_CREATE_INDEX_OPERATOR_HPP_
