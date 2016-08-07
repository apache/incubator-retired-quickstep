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

#include "catalog/IndexScheme.hpp"

#include <memory>
#include <string>

#include "catalog/Catalog.pb.h"
#include "storage/IndexSubBlockDescriptionFactory.hpp"
#include "storage/StorageBlockLayout.pb.h"

#include "glog/logging.h"

namespace quickstep {

bool IndexScheme::ProtoIsValid(const serialization::IndexScheme &proto) {
  // Check that proto is fully initialized.
  if (!proto.IsInitialized()) {
    return false;
  }

  // Check that each index entry contains initialized valid index description.
  for (int i = 0; i < proto.index_entries_size(); ++i) {
    const serialization::IndexScheme_IndexEntry &index_entry = proto.index_entries(i);
    if (!IndexSubBlockDescriptionFactory::ProtoIsValid(index_entry.index_description())) {
      return false;
    }
  }
  return true;
}

IndexScheme* IndexScheme::ReconstructFromProto(const serialization::IndexScheme &proto) {
  DCHECK(ProtoIsValid(proto))
    << "Attempted to create IndexScheme from an invalid proto description:\n"
    << proto.DebugString();

  std::unique_ptr<IndexScheme> index_scheme(new IndexScheme());

  for (int index_num = 0; index_num < proto.index_entries_size(); ++index_num) {
    const serialization::IndexScheme_IndexEntry &index_entry = proto.index_entries(index_num);
    // Make sure that index with same name does not already exist.
    DCHECK(index_scheme->index_map_.find(index_entry.index_name())
           == index_scheme->index_map_.end())
        << "Attempted to create IndexScheme from proto with duplicate index names.";
    // Store the index_name and corresponding index description in map.
    index_scheme->index_map_.emplace(index_entry.index_name(), index_entry.index_description());
  }
  return index_scheme.release();
}

serialization::IndexScheme IndexScheme::getProto() const {
  serialization::IndexScheme proto;
  // Set the entries of the index scheme.
  for (auto cit = index_map_.cbegin(); cit != index_map_.cend(); ++cit) {
    // Create an index entry.
    serialization::IndexScheme_IndexEntry *index_entry = proto.add_index_entries();

    // Populate the details of the index entry.
    index_entry->set_index_name(cit->first);
    index_entry->mutable_index_description()->MergeFrom(cit->second);
  }
  return proto;
}

}  // namespace quickstep
