/*
 * StreamCoordinator.cpp
 *
 *  Created on: Mar 30, 2016
 *      Author: root
 */

#include "query_execution/StreamCoordinatorThread.hpp"
#include "query_execution/Foreman.hpp"
#include <cstddef>
#include <memory>	
#include <unordered_map>
#include <chrono>
#include <algorithm> 
#include <vector>
#include "relational_operators/RelationalOperator.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"
#include "query_execution/QueryExecutionMessages.pb.h"
#include "query_execution/QueryExecutionUtil.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "types/Type.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "glog/logging.h"
#include "gtest/gtest_prod.h"

#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/tagged_message.h"
namespace quickstep {


void StreamCoordinatorThread::run()
{
    
    for(;;)
    {
    
    if(query_dag_== NULL) {
    	sleep(1);	
    }
	initializeTimer();
	if(op_timer_heap_.size()==0) sleep(1);
	processTimerEvents();
	sleep(sleep_time_/1000);	
    
    }

}

void StreamCoordinatorThread::processTimerEvents()
{

	auto now = clock::now();
	  while (!op_timer_heap_.empty() && op_timer_heap_.front().fire_time() < now) {
	    // Move the top entry to the end.
	    std::pop_heap(op_timer_heap_.begin(), op_timer_heap_.end());

	    // Check if current op_index has completed.
	    dag_node_index op_index =  op_timer_heap_.back().op_index();

	    	// Here you notify foreman of this new operator
	    	// Notify Foreman for the avaiable work order on the blob.
	    	  serialization::WorkOrdersAvailableMessage message_proto;
	    	  message_proto.set_operator_index(op_index);

	    	  // NOTE(zuyu): Using the heap memory to serialize proto as a c-like string.
	    	  const size_t message_proto_length = message_proto.ByteSize();
	    	  char *message_proto_bytes = static_cast<char*>(std::malloc(message_proto_length));
	    	  CHECK(message_proto.SerializeToArray(message_proto_bytes, message_proto_length));

	    	  tmb::TaggedMessage tagged_message(static_cast<const void *>(message_proto_bytes),
	    	                                    message_proto_length,
	    	                                    kStreamCoordinatorMessage);
	    	  std::free(message_proto_bytes);

	    	  // Send new work order available message to Foreman.
     
	    	  const tmb::MessageBus::SendStatus send_status =
	    	      QueryExecutionUtil::SendTMBMessage(
	    	          bus_,
	    	          stream_coordinator_client_id_,
	    	          tmb_client_id_,
	    	          std::move(tagged_message));
	    	  CHECK(send_status == tmb::MessageBus::SendStatus::kOK) << "Message could not "
	    	      "be sent from thread with TMB client ID "
	    	      << ClientIDMap::Instance()->getValue() << " to Foreman with TMB client "
	    	      "ID " << tmb_client_id_;

	      // Update fire point.
	        op_timer_heap_.back().updateFireTime();
            if(sleep_time_>op_timer_heap_.back().time_period().count())
            	sleep_time_=op_timer_heap_.back().time_period().count();

	      // Re-insert the operator into the heap.
	        std::push_heap(op_timer_heap_.begin(), op_timer_heap_.end());

}
}


void StreamCoordinatorThread::initializeTimer()
{
	 // Query the operators to see if they need timed getAllWorkOrders() calls.
	  for (dag_node_index i = 0; i < query_dag_->size(); ++i) {
	    RelationalOperator *op = query_dag_->getNodePayloadMutable(i);
	    std::pair<std::chrono::milliseconds, bool> timer =
	        op->registerTimeWorkOrderRequest();
	    if (timer.second) {
	      // Add the operator to the timer heap.
	      op_timer_heap_.emplace_back(i, timer.first);
	    }
	  }
	  // Initialize the timer heap.
	  std::make_heap(op_timer_heap_.begin(), op_timer_heap_.end());

}

}

