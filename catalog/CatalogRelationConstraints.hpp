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

#ifndef QUICKSTEP_CATALOG_CATALOG_RELATION_CONSTRAINTS_HPP_
#define QUICKSTEP_CATALOG_CATALOG_RELATION_CONSTRAINTS_HPP_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <set>
#include <utility>

#include "catalog/Catalog.pb.h"
#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

/**
 * @brief Constraints on a catalog relation.
 **/
class CatalogRelationConstraints {
 public:
  /**
   * @brief Constructor.
   **/
  CatalogRelationConstraints() {}

  /**
   * @brief Reconstruct a CatalogRelationConstraints object from its serialized
   *        Protocol Buffer form.
   *
   * @param proto The Protocol Buffer serialization of a CatalogRelationConstraints
   *        object, previously produced by getProto().
   **/
  explicit CatalogRelationConstraints(const serialization::CatalogRelationConstraints &proto);

  /**
   * @brief Serialize the CatalogRelationConstraints object as Protocol Buffer.
   *
   * @return The Protocol Buffer representation of the CatalogRelationConstraints
   *         object.
   **/
  serialization::CatalogRelationConstraints getProto() const;

  static bool ProtoIsValid(const serialization::CatalogRelationConstraints &proto,
                           const std::size_t num_attributes);

  bool hasPrimaryKey() const {
    return (primary_key_ != nullptr);
  }

  const std::set<attribute_id>* getPrimaryKey() const {
    return primary_key_.get();
  }

  template <typename IterableT>
  void setPrimaryKey(IterableT &&primary_key) {
    CHECK(!primary_key.empty());
    primary_key_.reset(
        new std::set<attribute_id>(primary_key.begin(), primary_key.end()));
  }

  void removePrimaryKey() {
    primary_key_.reset();
  }

  bool impliesUniqueAttributes(const std::set<attribute_id> &attributes) const {
    if (primary_key_ == nullptr) {
      return false;
    }

    std::vector<attribute_id> attr_intersection;
    std::set_intersection(primary_key_->begin(), primary_key_->end(),
                          attributes.begin(), attributes.end(),
                          std::back_inserter(attr_intersection));
    return (attr_intersection.size() == primary_key_->size());
  }

 private:
  std::unique_ptr<std::set<attribute_id>> primary_key_;

  DISALLOW_COPY_AND_ASSIGN(CatalogRelationConstraints);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_RELATION_CONSTRAINTS_HPP_
