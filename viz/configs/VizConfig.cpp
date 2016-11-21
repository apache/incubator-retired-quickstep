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

#include "viz/configs/VizConfig.hpp"

#include <memory>
#include <string>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogRelationSchema.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "viz/VizAnalyzer.hpp"
#include "viz/VizContext.hpp"
#include "viz/VizObject.hpp"

#include "json.hpp"

namespace quickstep {
namespace viz {

using nlohmann::json;

VizConfig::VizConfig(const VizContextPtr &context)
    : context_(context) {
  const VizAnalyzer *analyzer =
      context_->get<VizAnalyzer>("VizAnalyzer");
  relation_ = analyzer->getRelation();
  storage_manager_ = analyzer->getStorageManager();
}

json VizConfig::copyColumn(const attribute_id column_id,
                           const std::string *header) {
  json values = json::array();
  if (header != nullptr) {
    values.push_back(*header);
  }
  for (const block_id bid : relation_->getBlocksSnapshot()) {
    BlockReference block = storage_manager_->getBlock(bid, *relation_);
    std::unique_ptr<ValueAccessor> accessor(
        block->getTupleStorageSubBlock().createValueAccessor());
    const Type &type = relation_->getAttributeById(column_id)->getType();
    InvokeOnValueAccessorNotAdapter(
        accessor.get(),
        [&](auto *accessor) -> void {  // NOLINT(build/c++11)
      while (accessor->next()) {
        values.push_back(
            type.printValueToString(accessor->getTypedValue(column_id)));
      }
    });
  }
  return values;
}

json VizConfig::copySchema(const std::vector<attribute_id> &attr_ids) {
  json schema = json::array();
  for (const attribute_id attr_id : attr_ids) {
    const CatalogAttribute *attr = relation_->getAttributeById(attr_id);
    json attr_info;
    attr_info["name"] = attr->getDisplayName();
    attr_info["type"] = attr->getType().getName();
    schema.push_back(attr_info);
  }
  return schema;
}

json VizConfig::copyTrace() {
  json trace = json::array();
  auto copy_trace_functor = [&trace](const StringValue *str_value) -> void {
    trace.push_back(str_value->value());
  };
  context_->forEach<StringValue>("trace", copy_trace_functor);
  return trace;
}

}  // namespace viz
}  // namespace quickstep
