/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
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

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_TYPEDEFS_HPP_
