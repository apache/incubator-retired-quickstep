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

#include "catalog/IndexScheme.hpp"

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "threading/SpinSharedMutex.hpp"
#include "types/TypedValue.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/LessComparison.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {
  bool IndexScheme::ProtoIsValid(const serialization::IndexScheme &proto) {
    // Check that proto is fully initialized
    if (!proto.IsInitialized()) {
      return false;
    }
    // Check if the number of indices is non-zero.
    if (proto.num_indices() <= 0) {
      return false;
    }
    return true;
  }

  IndexScheme* IndexScheme::DeserializeIndexScheme(const serialization::IndexScheme &proto) {
    DCHECK(ProtoIsValid(proto))
      << "Attempted to create IndexScheme from an invalid proto description:\n"
      << proto.DebugString();

    std::unique_ptr<IndexScheme> index_scheme(new IndexScheme());
    index_scheme->num_indices_ = proto.num_indices();

    for (std::size_t index_num = 0; index_num < index_scheme->num_indices_; ++index_num) {
      serialization::IndexScheme_IndexEntry index_entry = proto.index_entries(index_num);
      std::string index_name = index_entry.index_name();
      std::vector<IndexSubBlockDescription> index_descriptions;
      for (std::size_t i = 0; i < index_entry.index_description_size(); ++i) {
        index_descriptions.emplace_back(index_entry.index_description(i));
      }
      // store the index_name and corresponding list of index descriptions in map
      index_scheme->index_map_[index_name] = index_descriptions;
    }
    return index_scheme.release();
  }

  serialization::IndexScheme IndexScheme::getProto() const {
    serialization::IndexScheme proto;
    // set the number of indices
    proto.set_num_indices(getNumIndices());

    // set the entries of the index scheme
    std::size_t num_indices = proto.num_indices();
    std::size_t index_number = 0;
    std::unordered_map<std::string, std::vector<IndexSubBlockDescription>>::const_iterator itr;

    for (itr = index_map_.cbegin(); itr != index_map_.cend(); ++itr, ++index_number) {
      DEBUG_ASSERT(index_number < num_indices);
      // create an index entry
      proto.add_index_entries();
      serialization::IndexScheme_IndexEntry *index_entry = proto.mutable_index_entries(index_number);

      // populate the details of the index entry
      index_entry->set_index_name(itr->first);
      std::vector<IndexSubBlockDescription> index_descriptions = itr->second;
      std::size_t index_description_num = 0;
      std::vector<IndexSubBlockDescription>::iterator index_description_itr;
      for (index_description_itr = index_descriptions.begin();
           index_description_itr != index_descriptions.end();
           ++index_description_itr) {
        // add each index description to the index entry
        index_entry->add_index_description();
        index_entry->mutable_index_description(index_description_num)->CopyFrom(*index_description_itr);
        ++index_description_num;
      }
    }
    return proto;
  }

}  // namespace quickstep
