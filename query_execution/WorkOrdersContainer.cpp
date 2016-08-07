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

#include "query_execution/WorkOrdersContainer.hpp"

#include <algorithm>
#include <cstddef>
#include <list>
#include <memory>
#include <vector>

#include "relational_operators/WorkOrder.hpp"

#include "glog/logging.h"

using std::unique_ptr;

namespace quickstep {

WorkOrdersContainer::~WorkOrdersContainer() {
  // For each operator ..
  for (std::size_t op = 0; op < num_operators_; ++op) {
    if (hasNormalWorkOrder(op) || hasRebuildWorkOrder(op)) {
      LOG(WARNING) << "Destroying a WorkOrdersContainer that still has pending WorkOrders.";
      break;
    }
  }
}

WorkOrder* WorkOrdersContainer::InternalListContainer::getWorkOrderForNUMANode(
    const int numa_node) {
  for (std::list<unique_ptr<WorkOrder>>::iterator it = workorders_.begin();
       it != workorders_.end();
       ++it) {
    const std::vector<int> &numa_nodes = (*it)->getPreferredNUMANodes();
    if (!numa_nodes.empty()) {
      if (std::find(numa_nodes.begin(), numa_nodes.end(), numa_node) !=
          numa_nodes.end()) {
        WorkOrder *work_order = it->release();
        workorders_.erase(it);
        return work_order;
      }
    }
  }
  return nullptr;
}

void WorkOrdersContainer::OperatorWorkOrdersContainer::addWorkOrder(
    WorkOrder *workorder) {
  const std::vector<int> &numa_nodes = workorder->getPreferredNUMANodes();
  if (!numa_nodes.empty()) {
    if (numa_nodes.size() == 1) {
      // This WorkOrder prefers exactly one NUMA node.
      single_numa_node_workorders_[numa_nodes.front()].addWorkOrder(
          workorder);
    } else {
      // This WorkOrder prefers more than one NUMA node.
      multiple_numa_nodes_workorders_.addWorkOrder(workorder);
    }
  } else {
    numa_agnostic_workorders_.addWorkOrder(workorder);
  }
}

std::size_t
    WorkOrdersContainer::InternalListContainer::getNumWorkOrdersForNUMANode(
    const int numa_node) const {
  std::size_t num_workorders = 0;
  for (const unique_ptr<WorkOrder> &work_order : workorders_) {
    const std::vector<int> &numa_nodes = work_order->getPreferredNUMANodes();
    if (!numa_nodes.empty()) {
      std::vector<int>::const_iterator
          it = std::find(numa_nodes.begin(), numa_nodes.end(), numa_node);
      if (it != numa_nodes.end()) {
        // Found a match.
       ++num_workorders;
      }
    }
  }
  return num_workorders;
}

bool WorkOrdersContainer::InternalListContainer::hasWorkOrderForNUMANode(
    const int numa_node) const {
  for (const unique_ptr<WorkOrder> &work_order : workorders_) {
    const std::vector<int> &numa_nodes = work_order->getPreferredNUMANodes();
    if (!numa_nodes.empty()) {
      std::vector<int>::const_iterator
          it = std::find(numa_nodes.begin(), numa_nodes.end(), numa_node);
      if (it != numa_nodes.end()) {
        // Found a match.
        return true;
      }
    }
  }
  return false;
}

WorkOrder* WorkOrdersContainer::OperatorWorkOrdersContainer::getWorkOrder(
    const bool prefer_single_NUMA_node) {
  // This function tries to get any available WorkOrder.
  WorkOrder *workorder = numa_agnostic_workorders_.getWorkOrder();
  if (workorder == nullptr) {
    if (prefer_single_NUMA_node) {
      workorder = getSingleNUMANodeWorkOrderHelper();
      if (workorder == nullptr) {
        workorder = multiple_numa_nodes_workorders_.getWorkOrder();
      }
    } else {
      workorder = multiple_numa_nodes_workorders_.getWorkOrder();
      if (workorder == nullptr) {
        workorder = getSingleNUMANodeWorkOrderHelper();
      }
    }
  }
  return workorder;
}

WorkOrder* WorkOrdersContainer::OperatorWorkOrdersContainer::
    getSingleNUMANodeWorkOrderHelper() {
  WorkOrder *workorder = nullptr;
  for (PtrVector<InternalQueueContainer>::iterator it =
           single_numa_node_workorders_.begin();
       it != single_numa_node_workorders_.end(); ++it) {
    workorder = it->getWorkOrder();
    if (workorder != nullptr) {
      return workorder;
    }
  }
  return nullptr;
}

}  // namespace quickstep
