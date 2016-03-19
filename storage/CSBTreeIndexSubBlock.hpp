/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_STORAGE_CSBTREE_INDEX_SUB_BLOCK_HPP_
#define QUICKSTEP_STORAGE_CSBTREE_INDEX_SUB_BLOCK_HPP_

#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/predicate/PredicateCost.hpp"
#include "storage/IndexSubBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageConstants.hpp"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"

namespace quickstep {

class CSBTreeIndexSubBlock;
class CatalogRelationSchema;
class ComparisonPredicate;
class IndexSubBlockDescription;
class TupleIdSequence;
class TupleStorageSubBlock;
class Type;
class TypedValue;

QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(CSBTreeIndexSubBlock);

namespace csbtree_internal {
class CompositeEntryReference;
class CompressedEntryReference;
class EntryReference;
class PredicateEvaluationForwarder;

// Hack which provides the UncheckedComparator interface, but compares
// composite keys.
// TODO(chasseur): Template this to avoid virtual calls.
class CompositeKeyLessComparator : public UncheckedComparator {
 public:
  CompositeKeyLessComparator(const CSBTreeIndexSubBlock &owner,
                             const CatalogRelationSchema &relation);

  ~CompositeKeyLessComparator() {
  }

  bool compareTypedValues(const TypedValue &left, const TypedValue &right) const {
    FATAL_ERROR("Can not use CompositeKeyLessComparator to compare TypedValue.");
  }

  inline bool compareDataPtrs(const void *left, const void *right) const {
    return compareDataPtrsInl(left, right);
  }

  bool compareDataPtrsInl(const void *left, const void *right) const;

  bool compareTypedValueWithDataPtr(const TypedValue &left, const void *right) const {
    FATAL_ERROR("Can not use CompositeKeyLessComparator to compare TypedValue.");
  }

  bool compareDataPtrWithTypedValue(const void *left, const TypedValue &right) const {
    FATAL_ERROR("Can not use CompositeKeyLessComparator to compare TypedValue.");
  }

 private:
  const CSBTreeIndexSubBlock &owner_;
  PtrVector<UncheckedComparator> attribute_comparators_;
};

}  // namespace csbtree_internal

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief Exception thrown when the key for a CSBTreeIndexSubBlock is too large
 *        to fit more than one key in a node of the CSB+-tree.
 **/
class CSBTreeKeyTooLarge : public std::exception {
 public:
  virtual const char* what() const throw() {
    return "CSBTreeKeyTooLarge: Attempted to create a CSBTreeIndexSubBlock "
           "with an index key which is too large to fit more than 1 key in "
           "a node.";
  }
};

/**
 * @brief An IndexSubBlock which implements a full CSB+-tree with linear scan
 *        intra-node search.
 * @warning This IndexSubBlock only supports fixed-length attributes, and the
 *          total key length must be small enough to fit at least 2 keys in a
 *          node.
 **/
class CSBTreeIndexSubBlock : public IndexSubBlock {
 public:
  CSBTreeIndexSubBlock(const TupleStorageSubBlock &tuple_store,
                       const IndexSubBlockDescription &description,
                       const bool new_block,
                       void *sub_block_memory,
                       const std::size_t sub_block_memory_size);

  ~CSBTreeIndexSubBlock() override {
  }

  /**
   * @brief Determine whether an IndexSubBlockDescription is valid for this
   *        type of IndexSubBlock.
   *
   * @param relation The relation an index described by description would
   *        belong to.
   * @param description A description of the parameters for this type of
   *        IndexSubBlock, which will be checked for validity.
   * @return Whether description is well-formed and valid for this type of
   *         IndexSubBlock belonging to relation (i.e. whether an IndexSubBlock
   *         of this type, belonging to relation, can be constructed according
   *         to description).
   **/
  static bool DescriptionIsValid(const CatalogRelationSchema &relation,
                                 const IndexSubBlockDescription &description);

  /**
   * @brief Estimate the average number of bytes (including any applicable
   *        overhead) used to index a single tuple in this type of
   *        IndexSubBlock. Used by StorageBlockLayout::finalize() to divide
   *        block memory amongst sub-blocks.
   * @warning description must be valid. DescriptionIsValid() should be called
   *          first if necessary.
   *
   * @param relation The relation tuples belong to.
   * @param description A description of the parameters for this type of
   *        IndexSubBlock.
   * @return The average/ammortized number of bytes used to index a single
   *         tuple of relation in an IndexSubBlock of this type described by
   *         description.
   **/
  static std::size_t EstimateBytesPerTuple(const CatalogRelationSchema &relation,
                                           const IndexSubBlockDescription &description);

  IndexSubBlockType getIndexSubBlockType() const override {
    return kCSBTree;
  }

  bool supportsAdHocAdd() const override {
    return initialized_;
  }

  bool supportsAdHocRemove() const override {
    return true;
  }

  bool addEntry(const tuple_id tuple) override;

  bool bulkAddEntries(const TupleIdSequence &tuples) override;

  void removeEntry(const tuple_id tuple) override;

  void bulkRemoveEntries(const TupleIdSequence &tuples) override;

  predicate_cost_t estimatePredicateEvaluationCost(
      const ComparisonPredicate &predicate) const override;

  /**
   * @note Currently this version only supports simple comparisons of a literal
   *       value with a non-composite key.
   **/
  TupleIdSequence* getMatchesForPredicate(const ComparisonPredicate &predicate,
                                          const TupleIdSequence *filter) const override;

  bool rebuild() override;

 private:
  // Information stored at the beginning of every node in the tree.
  struct NodeHeader {
    std::uint16_t num_keys;
    bool is_leaf;
    // 'node_group_reference' is the child for internal nodes, the right
    // sibling group for the last node in a group of leaf nodes, and
    // kNodeGroupNextLeaf for other leaf nodes. The very rightmost node
    // at the leaf level has this set to kNodeGroupNone.
    int node_group_reference;
  };

  // Used by internalInsertHelper() and leafInsertHelper() to communicate
  // information about node and group splits to higher levels in the tree.
  struct InsertReturnValue {
    InsertReturnValue()
        : split_node_least_key(NULL),
          new_node_group_id(kNodeGroupNone),
          left_split_group_smaller(false) {
    }

    const void *split_node_least_key;
    int new_node_group_id;
    bool left_split_group_smaller;
  };

  static const int kNodeGroupNone;      // -1
  static const int kNodeGroupNextLeaf;  // -2
  // Used in InsertReturnValue to indicate index is full (needed to allocate
  // new node groups, but not enough were free).
  static const int kNodeGroupFull;      // -3

  // Initialize this block's internal metadata and structure. Usually called by
  // the constructor, unless the key may be compressed, in which case it is
  // called by rebuild().
  bool initialize(const bool new_block);

  // Get the number of the node group containing the root node for the tree.
  inline int getRootNodeGroupNumber() const {
    return *static_cast<const int*>(sub_block_memory_);
  }

  // Set the number of the node group containing the root node for the tree.
  inline void setRootNodeGroupNumber(const int node_group_number) {
    *static_cast<int*>(sub_block_memory_) = node_group_number;
  }

  // Get the location of the node designated by 'node_number' in the group
  // with 'node_group_number'.
  inline void* getNode(const int node_group_number, const std::uint16_t node_number) const {
    DEBUG_ASSERT(node_group_number >= 0);
    return static_cast<char*>(node_groups_start_)
           + node_group_number * node_group_size_bytes_
           + node_number * kCSBTreeNodeSizeBytes;
  }

  // Get the root node of the tree.
  inline void* getRootNode() const {
    return getNode(getRootNodeGroupNumber(), 0);
  }

  // Get the right-sibling of the leaf node '*node', which may be in another
  // group. If '*node' is the very right-most leaf, returns NULL.
  inline void* getRightSiblingOfLeafNode(const void *node) const {
    DEBUG_ASSERT(static_cast<const NodeHeader*>(node)->is_leaf);
    const int sibling_reference = static_cast<const NodeHeader*>(node)->node_group_reference;
    if (sibling_reference == kNodeGroupNextLeaf) {
      return const_cast<char*>(static_cast<const char*>(node) + kCSBTreeNodeSizeBytes);
    } else if (sibling_reference >= 0) {
      return getNode(sibling_reference, 0);
    } else {
      DEBUG_ASSERT(sibling_reference == kNodeGroupNone);
      return NULL;
    }
  }

  // Remove all entries and reset this to an empty index.
  void clearIndex();

  // Makes a copy of a composite key from 'tuple' in 'tuple_store_'. Caller is
  // responsible for freeing the memory allocated.
  void* makeKeyCopy(const tuple_id tuple) const;

  // Returns a pointer to the least key in the sub-tree anchored at '*node'.
  const void* getLeastKey(const void *node) const;

  // Return the first leaf node under *node which may contain *key. If
  // duplicate keys are present, it may be necessary to use
  // getRightSiblingOfLeafNode() to find the actual desired leaf.
  template <typename LiteralLessKeyComparatorT,
            typename KeyLessLiteralComparatorT>
  void* findLeaf(
      const void *node,
      const void *literal,
      const LiteralLessKeyComparatorT &literal_less_key_comparator,
      const KeyLessLiteralComparatorT &key_less_literal_comparator) const;

  // Get the very first leaf node in the tree.
  void* getLeftmostLeaf() const;

  // Attempt to insert the entry (compressed_code, tuple) in the index.
  // 'CodeType' is the compressed type of the code (either uint8_t, uint16_t,
  // or uint32_t).
  template <typename CodeType>
  InsertReturnValue compressedKeyAddEntryHelper(const tuple_id tuple,
                                                const std::uint32_t compressed_code);

  // Insert the entry (*key, tuple) into the appropriate leaf descendent of
  // '*node'. 'node_group_allocation_requirement' is the number of node group
  // splits of ancestors of '*node' and '*node' itself which would be necessary
  // if the node group containing '*node' were split. '*node' will have an
  // entry added if a child splits. If *node's child node group splits, '*node'
  // will be split, and '*split_node_first_key' in the return value will point
  // to the first key in the right split node. If a split occurs and the node
  // group '*node' belongs to is full, then the node group will be split, and
  // 'new_node_group_id' in the return value will be set to the ID of the
  // newly-created right split node group. Also, in cases where a node group
  // split occured and node group splits are asymmetric (i.e.
  // max_keys_internal_ + 1 is odd), left_split_group_smaller will be set to
  // indicate whether the left or right half of the split initially had one
  // less node than the other (the value of left_split_group_smaller should be
  // used by the caller to determine how to split *node's parent). The caller
  // is responsible for updating the ancestors of '*node' to reflect any splits
  // that occur.
  //
  // If node group splits are necessary to insert the new entry, but there are
  // not enough free node groups, then '*node' and its descendents will be
  // unmodified and 'new_node_group_id' in the return value will be set to
  // kNodeGroupFull.
  //
  // Note that this method may fail in some cases where tightly packing the
  // index by calling rebuild() would allow the index to accomodate the new
  // entry.
  template <typename ComparatorT>
  InsertReturnValue internalInsertHelper(const int node_group_allocation_requirement,
                                         const tuple_id tuple,
                                         const void *key,
                                         const ComparatorT &key_comparator,
                                         const NodeHeader *parent_node_header,
                                         void *node);

  // Insert the entry (*key, tuple) into '*node', which is a child of the node
  // at '*parent_node_header'. If '*node' is full and it is possible to split,
  // then '*node' will be split and the new entry will be inserted into the
  // appropriate node, and '*split_node_first_key' in the return value will
  // point to the first key in the right split node. If the node group which
  // '*node' belongs to is also full, and there are enough free nodes to
  // satisfy 'node_group_allocation_requirement', then the node group is split
  // to make room for the additional node, and 'new_node_group_id' in the
  // return value is set to the newly-created right split node group's ID.
  // Also, in cases where a node group split occured and node group splits are
  // asymmetric (i.e. max_keys_internal_ + 1 is odd), left_split_group_smaller
  // will be set to indicate whether the left or right half of the split
  // initially had one less node than the other (the value of
  // left_split_group_smaller should be used by the caller to determine how to
  // split *node's parent). The caller is responsible for updating the
  // ancestors of '*node' to reflect any splits that occur.
  //
  // If node group splits are necessary to insert the new entry, but there are
  // not enough free node groups, then '*node' will be unmodified and
  // 'new_node_group_id' in the return value will be set to kNodeGroupFull.
  //
  // Note that this method may fail in some cases where tightly packing the
  // index by calling rebuild() would allow the index to accomodate the new
  // entry.
  template <typename ComparatorT>
  InsertReturnValue leafInsertHelper(const int node_group_allocation_requirement,
                                     const tuple_id tuple,
                                     const void *key,
                                     const ComparatorT &key_comparator,
                                     const NodeHeader *parent_node_header,
                                     void *node);

  // Insert the entry (*key, tuple) into the appropriate position in the tree,
  // starting from the root node. Essentially just calls internalInsertHelper()
  // or leafInsertHelper() as appropriate on the root node.
  template <typename ComparatorT>
  inline InsertReturnValue rootInsertHelper(const tuple_id tuple,
                                            const void *key,
                                            const ComparatorT &key_comparator);

  // Split the child node group of '*parent_node_header' by invoking
  // splitNodeGroup(). '**node' is the node whose split necessitated splitting
  // the group. If node group splits are asymmetric, the split will be done
  // such that after '**node' is split the split groups will be balanced. The
  // 'new_node_group_id' and 'left_split_group_smaller' fields of
  // '*caller_return_value' will be filled in by this method. '*node' will be
  // adjusted to point to the location of the node after the group is split.
  // Returns the location of the end of the group which contains '**node' after
  // the split or, in the special case where '**node' should be split across
  // the two groups by calling splitNodeAcrossGroups(), returns NULL.
  const void* splitNodeGroupHelper(const NodeHeader *parent_node_header,
                                   void **node,
                                   InsertReturnValue *caller_return_value);

  // Split the child node group of '*parent_node_header' in half. The node
  // group must be full, and there must be an available free node group in
  // 'node_group_used_bitmap_'. In the case where group splits are asymmetric
  // (i.e. the number of nodes is odd), the left split group will have one less
  // node than the right split group if 'left_smaller' is true, otherwise the
  // right split group will have one less node than the left. If
  // 'will_split_node_across_groups' is true, then left_smaller must be false,
  // and the nodes of the right split group will be shifted right by one,
  // leaving the first node of the right split group uninitialized (the caller
  // should subsequently overwrite it by splitting the last node of the left
  // split group across the two groups). Returns the number of the right split
  // node group. Does not actually split the parent node (caller should do so).
  int splitNodeGroup(const NodeHeader *parent_node_header,
                     const bool left_smaller,
                     const bool will_split_node_across_groups);

  // Split '*node' in half. '*node' itself must be full, and the node group
  // '*node' belongs to must be non-full. '*group_end' is the current end of
  // nodes in the group. If 'right_child_node_group' is kNodeGroupNone, then
  // '*node' must be a leaf, otherwise '*node' must be an internal node and the
  // child node group of the right split node is set to
  // 'right_child_node_group'. Returns a pointer to the least key in the
  // subtree rooted at the right split node. In the case where node splits are
  // asymmetric (i.e. the number of keys is odd), the left split node will have
  // one less key than the right split node if 'left_smaller' is true,
  // otherwise the right split node will have one less key than the left.
  //
  // There is a special case when '*node' is an internal node and
  // 'child_was_split_across_groups' is true. In this case, both halves of the
  // split are completely constructed based on the values initially in '*node',
  // and the caller should NOT subsequently insert a key into one of the split
  // nodes.
  const void* splitNodeInGroup(void *node,
                               const void *group_end,
                               const int right_child_node_group,
                               const bool left_smaller,
                               const bool child_was_split_across_groups);

  // Splits '*node' such that the right half of the split is inserted as the
  // first node in the group designated by 'destination_group_number'. If
  // 'right_child_node_group' is kNodeGroupNone, then '*node' must be a leaf,
  // otherwise '*node' must be an internal node and the child node group of the
  // right split node is set to 'right_child_node_group'. Returns a pointer to
  // the least key in the subtree rooted at the right split node. In the case
  // where node splits are asymmetric (i.e. the number of keys is odd), the
  // left split node will have one less key than the right split node if
  // 'left_smaller' is true, otherwise the right split node will have one less
  // key than the left.
  //
  // This method should ONLY be called after splitNodeGroup(..., false, true)
  // is called. It assumes that '*node' is the last node in its group, and that
  // the destination node group has its existing nodes shifted right by one, so
  // that the right split node can be written directly into the zeroth position
  // in the destination group. This method will behave incorrectly if the
  // caller violates these assumptions.
  //
  // There is a special case when '*node' is an internal node and
  // 'child_was_split_across_groups' is true. In this case, both halves of the
  // split are completely constructed based on the values initially in '*node',
  // and the caller should NOT subsequently insert a key into one of the split
  // nodes.
  const void* splitNodeAcrossGroups(void *node,
                                    const int destination_group_number,
                                    const int right_child_node_group,
                                    const bool left_smaller,
                                    const bool child_was_split_across_groups);

  // Insert the entry (*key, tuple) into '*node'. '*node' must be non-full.
  template <typename ComparatorT>
  void insertEntryInLeaf(const tuple_id tuple,
                         const void *key,
                         const ComparatorT &key_comparator,
                         void *node);

  // Remove the entry (compressed_code, tuple) from the index. 'CodeType' is
  // the compressed type of the code (either uint8_t, uint16_t, or uint32_t).
  template <typename CodeType>
  void compressedKeyRemoveEntryHelper(const tuple_id tuple,
                                      const std::uint32_t compressed_code);

  // Remove the entry (*key, tuple) from '*node'. If the entry does not exist
  // in '*node', but the last key in '*node' matches '*key', then this method
  // will be recursively called with the right-sibling of '*node'.
  template <typename ComparatorT>
  void removeEntryFromLeaf(const tuple_id tuple,
                           const void *key,
                           const ComparatorT &key_comparator,
                           void *node);

  // Helper method for getMatchesForPredicate(). Generates a TupleIdSequence of
  // all tuples which match a predicate of the form 'key comp right_literal'.
  // This version is for uncompressed keys.
  TupleIdSequence* evaluateComparisonPredicateOnUncompressedKey(
      const ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type) const;

  // Helper method for getMatchesForPredicate(). Generates a TupleIdSequence of
  // all tuples which match a predicate of the form 'key comp right_literal'.
  // This version is for compressed keys.
  TupleIdSequence* evaluateComparisonPredicateOnCompressedKey(
      ComparisonID comp,
      const TypedValue &right_literal,
      const Type &right_literal_type) const;

  // Helper method which calls the appropriate predicate-evaluation method
  // below based on 'comp'.
  template <typename LiteralLessKeyComparatorT,
            typename KeyLessLiteralComparatorT>
  TupleIdSequence* evaluatePredicate(
      ComparisonID comp,
      const void *literal,
      const LiteralLessKeyComparatorT &literal_less_key_comparator,
      const KeyLessLiteralComparatorT &key_less_literal_comparator) const;

  // Helper method for evaluateComparisonPredicateOnUncompressedKey() and
  // evaluateComparisonPredicateOnCompressedKey(). Generates a TupleIdSequence
  // of all tuples which have a key equal to '*literal' according to
  // 'literal_less_key_comparator' and 'key_less_literal_comparator'.
  template <typename LiteralLessKeyComparatorT,
            typename KeyLessLiteralComparatorT>
  TupleIdSequence* evaluateEqualPredicate(
      const void *literal,
      const LiteralLessKeyComparatorT &literal_less_key_comparator,
      const KeyLessLiteralComparatorT &key_less_literal_comparator) const;

  // Helper method for evaluateComparisonPredicateOnUncompressedKey() and
  // evaluateComparisonPredicateOnCompressedKey(). Generates a TupleIdSequence
  // of all tuples which have a key not equal to '*literal' according to
  // 'literal_less_key_comparator' and 'key_less_literal_comparator'.
  template <typename LiteralLessKeyComparatorT,
            typename KeyLessLiteralComparatorT>
  TupleIdSequence* evaluateNotEqualPredicate(
      const void *literal,
      const LiteralLessKeyComparatorT &literal_less_key_comparator,
      const KeyLessLiteralComparatorT &key_less_literal_comparator) const;

  // Helper method for evaluateComparisonPredicateOnUncompressedKey() and
  // evaluateComparisonPredicateOnCompressedKey(). Generates a TupleIdSequence
  // of all tuples which have a key less than '*literal' according to
  // 'literal_less_key_comparator' and 'key_less_literal_comparator'. If
  // 'include_equal' is true, the sequence will also include tuples whose keys
  // are equal to '*literal'.
  template <typename LiteralLessKeyComparatorT,
            typename KeyLessLiteralComparatorT,
            bool include_equal>
  TupleIdSequence* evaluateLessPredicate(
      const void *literal,
      const LiteralLessKeyComparatorT &literal_less_key_comparator,
      const KeyLessLiteralComparatorT &key_less_literal_comparator) const;

  // Helper method for evaluateComparisonPredicateOnUncompressedKey() and
  // evaluateComparisonPredicateOnCompressedKey(). Generates a TupleIdSequence
  // of all tuples which have a key greater than '*literal' according to
  // 'literal_less_key_comparator' and 'key_less_literal_comparator'. If
  // 'include_equal' is true, the sequence will also include tuples whose keys
  // are equal to '*literal'.
  template <typename LiteralLessKeyComparatorT,
            typename KeyLessLiteralComparatorT,
            bool include_equal>
  TupleIdSequence* evaluateGreaterPredicate(
      const void *literal,
      const LiteralLessKeyComparatorT &literal_less_key_comparator,
      const KeyLessLiteralComparatorT &key_less_literal_comparator) const;

  // Check if there are enough node groups in this CSBTreeIndexSubBlock to
  // build a complete index of all tuple_store_'s tuples.
  bool rebuildSpaceCheck() const;

  // Rebuild the leaf nodes of the index from tuple_store_'s tuples. Requires
  // that the index be empty (i.e. that clearIndex() has been called) and that
  // there are enough node groups in this CSBTreeIndexSubBlock to hold all
  // entries (i.e. that rebuildSpaceCheck() returns true). All node groups used
  // to store leaf nodes will be added to '*used_node_groups', including the
  // initial root. Returns the number of nodes in the rightmost leaf node group
  // (all other leaf node groups will be full).
  std::uint16_t rebuildLeaves(std::vector<int> *used_node_groups);

  // Helper method for rebuildLeaves(). Actually constructs leaf nodes for all
  // of the entries in '*entry_references'. Templated on 'EntryReferenceT' so
  // that it may be used with both EntryReference and CompressedEntryReference.
  template <class EntryReferenceT>
  std::uint16_t buildLeavesFromEntryReferences(
      std::vector<EntryReferenceT> *entry_references,
      std::vector<int> *used_node_groups);

  // Helper method for rebuildLeaves(). Fill in '*entry_references' with an
  // entry for every tuple in 'tuple_store_' with a non-NULL key, and sort
  // '*entry_references' into key order. This version is for the case where the
  // key is non-composite.
  void generateEntryReferencesFromTypedValues(
      std::vector<csbtree_internal::EntryReference> *entry_references) const;

  // Helper method for rebuildLeaves(). Fill in '*entry_references' with an
  // entry for every tuple in 'tuple_store_' with a non-NULL key, and sort
  // '*entry_references' into key order. This version is for the case where the
  // key is non-composite and compressed.
  void generateEntryReferencesFromCompressedCodes(
      std::vector<csbtree_internal::CompressedEntryReference> *entry_references) const;

  // Helper method for rebuildLeaves(). Fill in '*entry_references' with an
  // entry for every tuple in 'tuple_store_' with a non-NULL key, and sort
  // '*entry_references' into key order. This version is for the case where the
  // key is composite.
  void generateEntryReferencesFromCompositeKeys(
      std::vector<csbtree_internal::CompositeEntryReference> *entry_references) const;

  // Rebuild an internal level of the index, above 'child_node_groups'.
  // 'last_child_num_nodes' is the number of nodes in the rightmost child
  // node groups (all other node child node groups should be full). All node
  // groups in the rebuilt internal level will be added to '*used_node_groups'.
  // Returns the number of nodes in the rightmost node group of the rebuilt
  // level (all other node groups will be full). When this method adds only
  // one node group to '*used_node_groups' and returns 1, rebuilding is
  // finished and the sole element in '*used_node_groups' should be used as the
  // new root.
  std::uint16_t rebuildInternalLevel(const std::vector<int> &child_node_groups,
                                     std::uint16_t last_child_num_nodes,
                                     std::vector<int> *used_node_groups);

  // Moves nodes from the group designated by 'full_node_group_number' to the
  // beginning of the group designated by 'underfull_node_group_number', which
  // contains 'underfull_num_nodes'. Existing nodes in the underfull node group
  // are shifted right. The full node group must be full, and
  // 'underfull_num_nodes' must be less than large_half_num_children_. After
  // rebalancing, the underfull node group will contain exactly
  // large_half_num_children_ nodes, and this method returns the number of
  // nodes remaining in the previously full node group.
  std::uint16_t rebalanceNodeGroups(const int full_node_group_number,
                                    const int underfull_node_group_number,
                                    const std::uint16_t underfull_num_nodes);

  // Create an internal node at '*node' whose child is the node group with
  // 'child_node_group_number' and has 'num_children' children. 'num_children'
  // must be at least 2.
  void makeInternalNode(const int child_node_group_number,
                        const std::uint16_t num_children,
                        void *node);

  // Allocates a new node group and marks it as used.
  int allocateNodeGroup();
  // Deallocates a node group so that it can be reused.
  void deallocateNodeGroup(const int node_group_number);

  bool initialized_;

  bool key_may_be_compressed_;
  bool key_is_compressed_;
  bool key_is_composite_;
  bool key_is_nullable_;

  const Type *key_type_;  // NULL for composite key.

  std::vector<attribute_id> indexed_attribute_ids_;
  std::vector<std::size_t> indexed_attribute_offsets_;

  std::size_t key_length_bytes_;
  std::size_t key_tuple_id_pair_length_bytes_;
  std::unique_ptr<csbtree_internal::CompositeKeyLessComparator> composite_key_comparator_;

  std::uint16_t max_keys_internal_;
  std::uint16_t small_half_num_children_;
  std::uint16_t large_half_num_children_;

  std::uint16_t max_keys_leaf_;
  std::uint16_t small_half_num_keys_leaf_;
  std::uint16_t large_half_num_keys_leaf_;

  void *node_groups_start_;
  std::size_t node_group_size_bytes_;
  std::unique_ptr<BitVector<false>> node_group_used_bitmap_;
  int next_free_node_group_;
  int num_free_node_groups_;

  friend class CSBTreeIndexSubBlockTest;
  friend class CSBTreePrettyPrinter;
  friend class csbtree_internal::CompositeKeyLessComparator;
  friend class csbtree_internal::PredicateEvaluationForwarder;

  DISALLOW_COPY_AND_ASSIGN(CSBTreeIndexSubBlock);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_CSBTREE_INDEX_SUB_BLOCK_HPP_
