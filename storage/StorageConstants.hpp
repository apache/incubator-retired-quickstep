/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_STORAGE_STORAGE_CONSTANTS_HPP_
#define QUICKSTEP_STORAGE_STORAGE_CONSTANTS_HPP_

#include <cstddef>

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

// Size of a memory slot managed by the StorageManager. This is the smallest
// quantum of allocation for StorageBlocks and StorageBlobs. 2 MB is the large
// page size on x86.
//
// TODO(chasseur): Possibly change this for non-x86 architectures with
// different large page sizes.
const std::size_t kSlotSizeBytes = 0x200000;

// The assumed size of a cache-line. 64 bytes is correct for pretty much any
// modern x86 CPU, and many other architectures as well. This constant may be
// used to try and align memory nicely for performance, but should not be
// relied upon for correctness.
const std::size_t kCacheLineBytes = 64;

// The size, in bytes, of a node in a CSBTreeIndexSubBlock. Should always be
// a power of two.
//
// TODO(chasseur): This is probably undersized, given the efficiency of
// hardware prefetching on modern CPUs. Experiments should be conducted to
// tune this properly (experiments might also look at doing a binary search
// rather than a linear scan for intra-node key search for larger nodes).
const std::size_t kCSBTreeNodeSizeBytes = kCacheLineBytes;

// The target load factor for HashTable implementations. In general, a
// HashTable for N entries will have approximately kHashTableLoadFactor * N
// buckets. This constant controls a time-space tradeoff, with lower values
// leading to more efficient packing in memory at the expense of increased
// frequency of collisions (which incur additional runtime cost for probing
// and resolution).
const double kHashTableLoadFactor = 2.0f;

// For a resizable LinearOpenAddressingHashTable, this constant sets the
// number of overflow buckets. For a non-resizable
// LinearOpenAddressingHashTable, this is the maximum number of overflow
// buckets (the actual number may be less if a particularly small chunk of
// memory is used). This should not be set too high, as a long overflow chain
// is a symptom of a badly overfilled HashTable, and it is better to simply
// resize and redistribute entries into regular buckets rather than to
// frequently scan over a long chain of buckets.
const std::size_t kLinearOpenAddressingHashTableNumOverflowBuckets = 64;

// For a fixed-size LinearOpenAddressingHashTable, approximately
// kFixedSizeLinearOpenAddressingHashTableOverflowFactor of the total buckets
// in the table OR kLinearOpenAddressingHashTableNumOverflowBuckets (whichever
// is smaller) will be overflow buckets, while the rest will be ordinary
// buckets.
const double kFixedSizeLinearOpenAddressingHashTableOverflowFactor = 0.0625f;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_STORAGE_CONSTANTS_HPP_
