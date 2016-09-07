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

#include "utility/lip_filter/LIPFilterDeployment.hpp"

#include <memory>
#include <vector>

#include "types/TypeFactory.hpp"
#include "utility/lip_filter/LIPFilter.hpp"
#include "utility/lip_filter/LIPFilter.pb.h"
#include "utility/lip_filter/LIPFilterBuilder.hpp"
#include "utility/lip_filter/LIPFilterAdaptiveProber.hpp"

#include "glog/logging.h"

namespace quickstep {

LIPFilterDeployment::LIPFilterDeployment(
    const serialization::LIPFilterDeployment &proto,
    const std::vector<std::unique_ptr<LIPFilter>> &lip_filters) {
  switch (proto.action_type()) {
    case serialization::LIPFilterActionType::BUILD:
      action_type_ = LIPFilterActionType::kBuild;
      break;
    case serialization::LIPFilterActionType::PROBE:
      action_type_ = LIPFilterActionType::kProbe;
      break;
    default:
      LOG(FATAL) << "Unsupported LIPFilterActionType: "
                 << serialization::LIPFilterActionType_Name(proto.action_type());
  }

  for (int i = 0; i < proto.entries_size(); ++i) {
    const auto &entry_proto = proto.entries(i);
    lip_filters_.emplace_back(lip_filters.at(entry_proto.lip_filter_id()).get());
    attr_ids_.emplace_back(entry_proto.attribute_id());
    attr_types_.emplace_back(&TypeFactory::ReconstructFromProto(entry_proto.attribute_type()));
  }
}

bool LIPFilterDeployment::ProtoIsValid(
    const serialization::LIPFilterDeployment &proto) {
  if (proto.action_type() != serialization::LIPFilterActionType::BUILD &&
      proto.action_type() != serialization::LIPFilterActionType::PROBE) {
    LOG(FATAL) << "Unsupported LIPFilterActionType: "
               << serialization::LIPFilterActionType_Name(proto.action_type());
  }
  if (proto.entries_size() == 0) {
    return false;
  }
  for (int i = 0; i < proto.entries_size(); ++i) {
    const auto &entry_proto = proto.entries(i);
    if (!TypeFactory::ProtoIsValid(entry_proto.attribute_type())) {
      return false;
    }
  }
  return true;
}

LIPFilterBuilder* LIPFilterDeployment::createLIPFilterBuilder() const {
  DCHECK(action_type_ == LIPFilterActionType::kBuild);
  return new LIPFilterBuilder(lip_filters_, attr_ids_, attr_types_);
}

LIPFilterAdaptiveProber* LIPFilterDeployment::createLIPFilterAdaptiveProber() const {
  DCHECK(action_type_ == LIPFilterActionType::kProbe);
  return new LIPFilterAdaptiveProber(lip_filters_, attr_ids_, attr_types_);
}

}  // namespace quickstep
