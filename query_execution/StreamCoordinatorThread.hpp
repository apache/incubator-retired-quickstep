/*
 * StreamCoordinator.hpp
 *
 *  Created on: Mar 30, 2016
 *      Author: root
 */

#ifndef STREAMCOORDINATORTHREAD_HPP_
#define STREAMCOORDINATORTHREAD_HPP_

#include <cstddef>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <vector>
#include "threading/Thread.hpp"
#include "query_execution/Foreman.hpp"
#include "relational_operators/RelationalOperator.hpp"
#include "query_execution/QueryExecutionTypedefs.hpp"
#include "utility/DAG.hpp"
#include "utility/Macros.hpp"
#include "threading/ThreadIDBasedMap.hpp"
#include "glog/logging.h"
#include "gtest/gtest_prod.h"

#include "tmb/message_bus.h"

namespace quickstep {


class StreamCoordinatorThread: public Thread {
public:

    StreamCoordinatorThread(DAG<RelationalOperator, bool>* query_dag, tmb::MessageBus *bus, tmb::client_id tmb_client_id)
     :query_dag_(query_dag),
     bus_(bus),
     tmb_client_id_(tmb_client_id)
      { 
      stream_coordinator_client_id_ = bus_->Connect();
      bus_->RegisterClientAsSender(stream_coordinator_client_id_, kStreamCoordinatorMessage);
  }

    ~StreamCoordinatorThread() override {}

protected:
 
  void run() override;    

private:
	  /**
	   * @brief Process pending timer events and update the timer to future events.
	   **/

    typedef std::chrono::steady_clock clock;
    typedef DAG<RelationalOperator, bool>::size_type_nodes dag_node_index;
    class RelationalOperatorTimer {
    	       public:
    	        RelationalOperatorTimer(const dag_node_index op_index,
    	                                const std::chrono::milliseconds &time_period)
    	            : fire_time_(clock::now()),
    	              time_period_(time_period),
    	              op_index_(op_index) {}

    	        RelationalOperatorTimer(const RelationalOperatorTimer &copy) = default;

    	        RelationalOperatorTimer &operator=(const RelationalOperatorTimer &copy) =
    	            default;

    	        // Update the fire point to one time   period in the future.
    	        void updateFireTime() {
    	          fire_time_ = clock::now() + time_period_;
    	        }

    	        bool operator<(const RelationalOperatorTimer &right) const {
    	          // To create a min-heap, implement greater than comparison here.
    	          return fire_time_ > right.fire_time_;
    	        }

    	        const std::chrono::time_point<clock> fire_time() const {
    	          return fire_time_;
    	        }

                const std::chrono::milliseconds time_period() const {
                  return time_period_;
                }

    	        const dag_node_index op_index() const { return op_index_; }

    	       private:
    	        std::chrono::time_point<clock> fire_time_;
    	        std::chrono::milliseconds time_period_;
    	        dag_node_index op_index_;
    	      };

    std::vector<RelationalOperatorTimer> op_timer_heap_;
    DAG<RelationalOperator, bool>* query_dag_;
    tmb::MessageBus *bus_;
    tmb::client_id tmb_client_id_;
    tmb::client_id stream_coordinator_client_id_;
    uint sleep_time_= 1000;
	void processTimerEvents();
	void initializeTimer();
    DISALLOW_COPY_AND_ASSIGN(StreamCoordinatorThread);

};

}  // namespace quickstep

#endif /* STREAMCOORDINATORTHREAD_HPP_ */

