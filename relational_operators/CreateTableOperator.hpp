/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_CREATE_TABLE_OPERATOR_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_CREATE_TABLE_OPERATOR_HPP_

#include <memory>

#include "catalog/CatalogRelation.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "utility/Macros.hpp"

#include "tmb/id_typedefs.h"

namespace tmb { class MessageBus; }

namespace quickstep {

class CatalogDatabase;
class WorkOrdersContainer;

/** \addtogroup RelationalOperators
 *  @{
 */

/**
 * @brief An operator which creates a relation.
 **/
class CreateTableOperator : public RelationalOperator {
 public:
  /**
   * @brief Constructor.
   *
   * @param relation The relation to add. This CreateTableOperator owns
   *        relation until the WorkOrder it produces is successfully executed,
   *        at which point it is owned by database.
   * @param database The database to add a relation to.
   **/
  CreateTableOperator(CatalogRelation *relation,
                      CatalogDatabase *database)
      : relation_(relation),
        database_(database),
        work_generated_(false) {}

  ~CreateTableOperator() override {}

  /**
   * @note no WorkOrder generated for this operator.
   **/
  bool getAllWorkOrders(WorkOrdersContainer *container,
                        const tmb::client_id foreman_client_id,
                        tmb::MessageBus *bus) override;

 private:
  std::unique_ptr<CatalogRelation> relation_;
  CatalogDatabase *database_;

  bool work_generated_;

  DISALLOW_COPY_AND_ASSIGN(CreateTableOperator);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_CREATE_TABLE_OPERATOR_HPP_
