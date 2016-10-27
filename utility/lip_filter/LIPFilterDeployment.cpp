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

namespace quickstep {

LIPFilterDeployment::LIPFilterDeployment(
    const serialization::LIPFilterDeployment &proto,
    const std::vector<std::unique_ptr<LIPFilter>> &lip_filters) {
  if (proto.build_entries_size() > 0) {
    build_.reset(new LIPFilterDeploymentInfo());
    for (int i = 0; i < proto.build_entries_size(); ++i) {
      const auto &entry_proto = proto.build_entries(i);
      build_->lip_filters_.emplace_back(
          lip_filters.at(entry_proto.lip_filter_id()).get());
      build_->attr_ids_.emplace_back(entry_proto.attribute_id());
      build_->attr_types_.emplace_back(
          &TypeFactory::ReconstructFromProto(entry_proto.attribute_type()));
    }
  }

  if (proto.probe_entries_size() > 0) {
    probe_.reset(new LIPFilterDeploymentInfo());
    for (int i = 0; i < proto.probe_entries_size(); ++i) {
      const auto &entry_proto = proto.probe_entries(i);
      probe_->lip_filters_.emplace_back(
          lip_filters.at(entry_proto.lip_filter_id()).get());
      probe_->attr_ids_.emplace_back(entry_proto.attribute_id());
      probe_->attr_types_.emplace_back(
          &TypeFactory::ReconstructFromProto(entry_proto.attribute_type()));
    }
  }
}

bool LIPFilterDeployment::ProtoIsValid(
    const serialization::LIPFilterDeployment &proto) {
  if (proto.build_entries_size() == 0 && proto.probe_entries_size() == 0) {
    return false;
  }
  for (int i = 0; i < proto.build_entries_size(); ++i) {
    const auto &entry_proto = proto.build_entries(i);
    if (!TypeFactory::ProtoIsValid(entry_proto.attribute_type())) {
      return false;
    }
  }
  for (int i = 0; i < proto.probe_entries_size(); ++i) {
    const auto &entry_proto = proto.probe_entries(i);
    if (!TypeFactory::ProtoIsValid(entry_proto.attribute_type())) {
      return false;
    }
  }
  return true;
}

LIPFilterBuilder* LIPFilterDeployment::createLIPFilterBuilder() const {
  if (build_ == nullptr) {
    return nullptr;
  }

  return new LIPFilterBuilder(build_->lip_filters_,
                              build_->attr_ids_,
                              build_->attr_types_);
}

LIPFilterAdaptiveProber* LIPFilterDeployment::createLIPFilterAdaptiveProber() const {
  if (probe_ == nullptr) {
    return nullptr;
  }

  return new LIPFilterAdaptiveProber(probe_->lip_filters_,
                                     probe_->attr_ids_,
                                     probe_->attr_types_);
}

}  // namespace quickstep
