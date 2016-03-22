/**
 * This file copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/PrintToScreenOperator.hpp"

#include <cstdio>

#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "cli/PrintToScreen.hpp"

#include "glog/logging.h"

namespace quickstep {

bool PrintToScreenOperator::getAllWorkOrders(
		WorkOrdersContainer *container,
	    QueryContext *query_context,
		StorageManager *storage_manager,
		const tmb::client_id foreman_client_id,
		tmb::MessageBus *bus)
{

  if (input_is_stored_) {
    // Single work order to print the entire relation.
    container->addNormalWorkOrder(new PrintToScreenWorkOrder(input_relation_),
                                  op_index_);
    return true;
  } else {
    if (!done_generating_) {
      if (!done_printing_header_) {
        // Print header.
        PrintToScreen printer(input_relation_, nullptr);
        printer.printHeader(stdout);
        done_printing_header_ = true;
      }

      // Print the newly streamed blocks.
      while (num_workorders_generated_ < input_relation_blocks_.size()) {
        container->addNormalWorkOrder(
            new PrintToScreenWorkOrder(
              input_relation_,
              input_relation_blocks_[num_workorders_generated_],
              storage_manager,
              &num_workorders_completed_),
            op_index_);
        ++num_workorders_generated_;
      }

      if (done_feeding_input_relation_ &&
          num_workorders_generated_ ==
              num_workorders_completed_.load(std::memory_order_acquire)) {
        // All blocks printed. Print footer and return done.
        PrintToScreen printer(input_relation_, nullptr);
        printer.printHeader(stdout);
        done_generating_ = true;
      }
    }
    return done_generating_;
  }
}

void PrintToScreenWorkOrder::execute() {
  PrintToScreen printer(input_relation_, storage_manager_);
  switch (type_) {
    case kPrintRelation: {
      printer.printRelation(stdout);
      break;
    }
    case kPrintBlock: {
      printer.printBlock(input_block_, stdout);
      num_workorders_completed_->fetch_add(1, std::memory_order_release);
      break;
    }
  }
}

}  // namespace quickstep
