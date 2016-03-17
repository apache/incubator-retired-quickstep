/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_CATALOG_CATALOG_TYPEDEFS_HPP_
#define QUICKSTEP_CATALOG_CATALOG_TYPEDEFS_HPP_

#include <climits>
#include <cstddef>

// In all cases, a negative value indicates an invalid/unset id, except for
// partition_id.

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

typedef int database_id;
typedef int relation_id;
typedef int attribute_id;
typedef std::size_t partition_id;
typedef int numa_node_id;

// This depends on all the above id types being typedefed to int, except for
// partition_id.
const int kCatalogMaxID = INT_MAX;

// Catalog ids use negative values as invalid ids. Mark -1 as constant invalid
// id for the catalog ids.
constexpr int kInvalidCatalogId = -1;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_TYPEDEFS_HPP_
