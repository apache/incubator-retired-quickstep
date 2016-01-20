/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015 Pivotal Software, Inc.
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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_SORT_MERGE_RUN_OPERATOR_HELPERS_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_SORT_MERGE_RUN_OPERATOR_HELPERS_HPP_

#include <algorithm>
#include <cstddef>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "expressions/scalar/Scalar.hpp"
#include "storage/InsertDestination.hpp"
#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/TupleStorageSubBlock.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/SpinMutex.hpp"
#include "types/Type.hpp"
#include "types/containers/Tuple.hpp"
#include "types/operations/comparisons/Comparison.hpp"
#include "types/operations/comparisons/ComparisonFactory.hpp"
#include "types/operations/comparisons/ComparisonID.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SortConfiguration.hpp"

#include "glog/logging.h"

namespace quickstep {

class CatalogRelationSchema;

namespace merge_run_operator {

/**
 * @addtogroup SortMergeRun
 * @{
 */

/**
 * @brief Structure to hold a run of sorted blocks. Currently, a list of
 * block_ids.
 **/
typedef std::vector<block_id> Run;

/**
 * @brief Class to store the merge tree of sorting process, and produce merge
 * jobs.
 **/
class MergeTree {
 public:
  /**
   * @brief Structure to communicate the merge jobs with the merge operator.
   **/
  struct MergeJob {
    MergeJob(std::size_t _level, bool _is_final_level, std::vector<Run> &&_runs)
        : runs(std::move(_runs)),
          level(_level),
          is_final_level(_is_final_level) {}

    std::vector<Run> runs;
    std::size_t level;
    bool is_final_level;
  };

  /**
   * @brief Constructor for merge tree.
   *
   * @param merge_factor Merge factor of the merge tree.
   **/
  explicit MergeTree(const std::size_t merge_factor)
      : merge_factor_(merge_factor) {}

  /**
   * @brief Initialize the merge tree. Merge tree is dependent on the size of
   * the input runs.
   *
   * @param initial_runs Number of runs in the input. Currently, the number of
   *        blocks in the input relation.
   *
   * @note This can be called after \c initializeForPipeline when the input
   *       runs size is finalized.
   **/
  void initializeTree(const std::size_t initial_runs);

  /**
   * @brief Initialize the merge tree for pipelining. When the number of input
   * blocks are unknown upfront (in the case of pipeline), we initialize a
   * minimal merge tree supporting the first pass of merge alone.
   **/
  void initializeForPipeline();

  /**
   * @brief Check if the final merge for the merge tree is already scheduled,
   * and, if so, update the tree to generate a job to copy the final run into
   * correct output destination.
   *
   * @warning This is only supposed to be invoked when the final size of input
   * relation is computed, and the tree is initialized to this known size.
   **/
  void checkAndFixFinalMerge();

  /**
   * @brief Get merge jobs that can be executed at the moment. In a multi-pass
   * merge, there are situations where the merge tree can only schedule work if
   * the output of executing merge jobs complete.
   *
   * @param jobs The generated merge jobs are written to this vector.
   *
   * @return \c true if the final job was scheduled.
   **/
  bool getMergeJobs(std::vector<MergeJob> *jobs);

  /**
   * @brief Add input blocks to the merge tree.
   *
   * @param blocks Blocks to add as input to merge tree.
   **/
  inline void addInputBlocks(const std::vector<block_id> &blocks) {
    SpinMutexLock lock(pending_mutex_);
    for (const block_id block : blocks) {
      pending_[0].emplace_back(1, block);
    }
  }

  /**
   * @brief Write the output run of a merge job to the merge tree. This run
   * could be input to further merge jobs.
   *
   * @param merge_level Merge level that generated this output run. (It is
   *        0-indexed, and 0 indicates the first pass of the merge.)
   * @param output_run Output run of the merge.
   **/
  inline void writeOutputRun(const std::size_t merge_level, Run *output_run) {
    SpinMutexLock lock(pending_mutex_);
    pending_[merge_level + 1].emplace_back(std::move(*output_run));
  }

 private:
  // Value to signify final level is unknown.
  static constexpr std::size_t kFinalLevelUninitialized =
      std::numeric_limits<std::size_t>::max();

  void getRuns(const std::size_t level,
               const std::size_t num_runs,
               std::vector<Run> *runs) {
    DCHECK(num_runs <= pending_[level].size());
    for (std::size_t i = 0; i < num_runs; ++i) {
      runs->push_back(std::move(pending_[level].back()));
      pending_[level].pop_back();
    }
  }

  // Merge tree is computed bottom up. Level-0 has the initial input runs to the
  // operator (at present each run is a block). Subsequent level are used to
  // store runs obtained by merging the previous level.

  std::vector<std::size_t> runs_scheduled_;  // Runs scheduled in each level.
  std::vector<std::size_t> runs_expected_;   // Runs expected in each level.
  mutable SpinMutex pending_mutex_;  // Mutex to control access to pending_.
  std::vector<std::vector<merge_run_operator::Run>>
      pending_;              // Runs pending scheduling in each level.
  std::size_t num_levels_;   // Number of levels in the merge tree.
  std::size_t final_level_;  // Index (0-based) of the final merge level.
  std::size_t cur_level_;    // Current merge level where jobs are unscheduled.
  const std::size_t merge_factor_;  // Merge factor of the merges.

  DISALLOW_COPY_AND_ASSIGN(MergeTree);
};

/**
 * @brief Run creator. Creates a run by only appending tuples.
 *
 * @warning Assumes the InsertDestination uses StorageBlock that does not
 *          reorder the tuples appended order.
 **/
class RunCreator {
 public:
  /**
   * @brief Constructor.
   *
   * @param run A list of blocks to sort.
   * @param output_destination The InsertDestination that holds the blocks of
   *        the run.
   **/
  RunCreator(Run *run, InsertDestination *output_destination)
      : run_(run), output_destination_(output_destination) {
    createNewBlock();
  }

  /**
   * @brief Destructor.
   **/
  ~RunCreator() { flushBlock(); }

  /**
   * @brief Append tuple to run. Creates a new block if current block is full.
   *
   * @param tuple Tuple to insert into the run.
   *
   * @return true if a new block was created.
   **/
  inline bool appendTuple(const Tuple &tuple) {
    bool new_block = false;
    // TODO(shoban): We should use a non-virtual call to insert tuple to storage
    // block for better performance. Note that we will know the storage
    // implementation apriori.
    while (!storage_block_->insertTupleInBatch(tuple)) {
      output_destination_->returnBlock(std::move(storage_block_), true);
      createNewBlock();
      new_block = true;
    }
    return new_block;
  }

  /**
   * @brief Flush the block to destination.
   *
   * @warning No appends can happen after this.
   **/
  void flushBlock() {
    if (storage_block_.valid()) {
      if (storage_block_->getTupleStorageSubBlock().isEmpty()) {
        // No tuples were inserted. This can be reused by InsertDestination.
        output_destination_->returnBlock(std::move(storage_block_), false);
        // Remove block from run.
        run_->pop_back();
      } else {
        output_destination_->returnBlock(std::move(storage_block_), true);
      }
      storage_block_.release();
    }
    DCHECK(!storage_block_.valid());
  }

 private:
  inline void createNewBlock() {
    storage_block_ = output_destination_->getBlockForInsertion();
    DCHECK(storage_block_->getTupleStorageSubBlock().isInsertOrderPreserving())
        << kTupleStorageSubBlockTypeNames[storage_block_
                                              ->getTupleStorageSubBlock()
                                              .getTupleStorageSubBlockType()]
        << " is not insert order preserving.";
    run_->push_back(storage_block_->getID());
  }

  Run *run_;  // Output run.
  InsertDestination *output_destination_;  // Insert destination to generate
                                           // output blocks.
  MutableBlockReference storage_block_;  // Reference to current block.

  DISALLOW_COPY_AND_ASSIGN(RunCreator);
};

/**
 * @brief Run iterator to iterate through Tuples in the run.
 *
 * @warning Assumes all blocks in the run belong to same TupleStorageSubBlock
 *          type so that the specific ValueAccessor implementation can be
 *          directly used.
 * @warning Assumes no wrappers (like TupleIdSequence or OrderedTupleIdSequence)
 *          are to be applied.
 **/
template <typename ValueAccessorT>
class RunIterator {
 public:
  /**
   * @brief Constructor.
   *
   * @param run Run to iterate on.
   * @param storage_manager Storage manager.
   * @param input_relation Relation that the blocks in run belong to.
   **/
  RunIterator(const Run &run,
              StorageManager *storage_manager,
              const CatalogRelationSchema &input_relation)
      : run_(run),
        run_it_(run_.begin()),
        accessor_(nullptr),
        input_relation_(input_relation),
        storage_manager_(storage_manager) {
    loadAccessor();
  }

  /**
   * @brief Const ValueAccessor to read the tuple at the current position.
   **/
  inline const ValueAccessorT* getValueAccessor() { return accessor_.get(); }

  /**
   * @brief Move to the next tuple.
   *
   * @return \c true if there is a next tuple, \c false otherwise.
   **/
  bool next() {
    while (run_it_ != run_.end()) {
      if (accessor_->next()) {
        return true;
      } else {
        ++run_it_;
        loadAccessor();
      }
    }
    return false;
  }

 private:
  // Loads the current block's ValueAccessor, if we have not reached the end of
  // the run.
  void loadAccessor() {
    if (run_it_ == run_.end()) {
      accessor_.reset();
    } else {
      block_ =
          BlockReference(storage_manager_->getBlock(*run_it_, input_relation_));
      accessor_.reset(static_cast<ValueAccessorT *>(
          block_->getTupleStorageSubBlock().createValueAccessor()));
    }
  }

  const Run &run_;  // Run which iterated upon.
  Run::const_iterator run_it_;  // Run iterator.
  BlockReference block_;  // Reference to current block (i.e., run iterator).
  std::unique_ptr<ValueAccessorT> accessor_;  // ValueAccessor of the current
                                              // block.
  const CatalogRelationSchema &input_relation_;  // Schema of input relation.
  StorageManager *storage_manager_;  // Storage manager.

  DISALLOW_COPY_AND_ASSIGN(RunIterator);
};

/**
 * @brief Merge runs into a single run.
 **/
class RunMerger {
 public:
  /**
   * @brief Constructor.
   *
   * @param sort_config Sort configuration.
   * @param runs Vector of runs. R-valued. Moves and takes ownership of runs.
   * @param top_k Only keep the top-k results of the merge. All the tuples are
   *        kept if \c top_k is 0.
   * @param run_relation Relation that runs belong to.
   * @param output_destination The InsertDestination that stores the merge run.
   * @param level Merge level of this merge.
   * @param storage_manager Storage manager to use.
   **/
  RunMerger(const SortConfiguration &sort_config,
            std::vector<Run> &&runs,
            std::size_t top_k,
            const CatalogRelationSchema &run_relation,
            InsertDestination *output_destination,
            const std::size_t level,
            StorageManager *storage_manager)
      : sort_config_(sort_config),
        input_runs_(std::move(runs)),
        top_k_(top_k),
        output_run_(),
        output_run_creator_(&output_run_, output_destination),
        run_relation_(run_relation),
        level_(level),
        storage_manager_(storage_manager) {}

  /**
   * @brief Merges the runs. This will use a specialized merge implementation
   * depending on the sort configuration.
   **/
  void doMerge();

  /**
   * @brief Get a mutable a pointer to the merged output run.
   **/
  inline Run* getOutputRunMutable() { return &output_run_; }

  /**
   * @brief Get the merge level.
   **/
  inline std::size_t getMergeLevel() const { return level_; }

 private:
  // Generic heap-based merge implementation. Comparator takes care of NULL.
  // This defaults for any number of ORDER BY columns specification.
  // '*first_accessor' is an accessor on the first input block, used only to
  // determine the ValueAccessor type used to access all the blocks in the
  // runs.
  template <bool check_top_k>
  void mergeGeneric(ValueAccessor *first_accessor);

  // Merge implementation specialized for single-column ORDER BY sort
  // specification with NULL FIRST. Since the NULLs (if any) occur in the
  // beginning, we cycle through all the runs first and write out the NULLs,
  // before doing the merge. This makes the comparator have no branches. As
  // above, '*first_accessor' is used only to determine the ValueAccessor type.
  template <bool check_top_k>
  void mergeSingleColumnNullFirst(ValueAccessor *first_accessor);

  // Merge implementation specialized for single-column ORDER BY sort
  // specification with NULL LAST. Since the NULLs (if any) occur in the end of
  // the runs, we use the heap to sort all non-NULL values first and then cycle
  // through the runs to write out the NULLs. As above, '*first_accessor' is
  // used only to determine the ValueAccessor type.
  template <bool check_top_k>
  void mergeSingleColumnNullLast(ValueAccessor *first_accessor);

  // Trivial implementation to copy a run.
  template <bool check_top_k>
  void copyToOutput(const Run &run, ValueAccessor *first_accessor);

  const SortConfiguration &sort_config_;
  std::vector<Run> input_runs_;
  const std::size_t top_k_;
  Run output_run_;
  RunCreator output_run_creator_;
  const CatalogRelationSchema &run_relation_;
  const std::size_t level_;
  StorageManager *storage_manager_;

  DISALLOW_COPY_AND_ASSIGN(RunMerger);
};

// ----------------------------------------------------------------------------
// Implementations of RunMerger merge methods follow.

/**
 * @brief Reference node for each run for use in heap-sort. Holds data of the
 * current tuple at the head of each run, and run-id of this node.
 **/
template <typename ValueAccessorT>
struct GenericHeapNode {
  std::size_t run_id;
  const ValueAccessorT *value_accessor;
};

/**
 * @brief Simple struct to hold the comparators and sort configuration of a
 * single ORDER BY column.
 **/
struct ColumnComparator {
  /**
   * @brief Constructor for ColumnComparator.
   *
   * @param comp_id Comparison type of the column. (kGreater/kLess).
   * @param null_ordering NULL value ordering of this column.
   * @param type Type of this column.
   * @param attr_id Attribute ID of this column in the value-accessor.
   **/
  ColumnComparator(ComparisonID comp_id,
                   const bool null_ordering,
                   const Type &type,
                   const attribute_id attr_id)
      : comparator_(ComparisonFactory::GetComparison(comp_id)
                        .makeUncheckedComparatorForTypes(type, type)),
        equal_(ComparisonFactory::GetComparison(ComparisonID::kEqual)
                   .makeUncheckedComparatorForTypes(type, type)),
        null_ordering_(null_ordering),
        attr_id_(attr_id) {}

  const std::unique_ptr<UncheckedComparator> comparator_;
  const std::unique_ptr<UncheckedComparator> equal_;
  const bool null_ordering_;
  const attribute_id attr_id_;
};

/**
 * @brief Internal data structure for generic comparator to be used in a
 * max-heap to merge sorted runs. This internal structure exists so that we can
 * pass a const reference to this instead of deep copying comparators of all
 * columns.
 **/
class GenericHeapComparatorInternal {
 public:
  /**
   * @brief Constructor.
   *
   * @param sort_config Sort configuration.
   **/
  explicit GenericHeapComparatorInternal(const SortConfiguration &sort_config) {
    DCHECK(sort_config.isValid());

    const PtrVector<Scalar> &order_by = sort_config.getOrderByList();
    const std::vector<bool> &ordering = sort_config.getOrdering();
    const std::vector<bool> &null_ordering = sort_config.getNullOrdering();

    PtrVector<Scalar>::const_iterator order_by_it = order_by.begin();
    std::vector<bool>::const_iterator ordering_it = ordering.begin();
    std::vector<bool>::const_iterator null_ordering_it = null_ordering.begin();

    for (; order_by_it != order_by.end();
         ++order_by_it, ++ordering_it, ++null_ordering_it) {
      const attribute_id attr_id =
          order_by_it->getAttributeIdForValueAccessor();
      const Type &type = order_by_it->getType();
      DCHECK_NE(attr_id, -1);
      if (*ordering_it == kSortAscending) {
        columns_.push_back(new ColumnComparator(
            ComparisonID::kLess, *null_ordering_it, type, attr_id));
      } else {
        columns_.push_back(new ColumnComparator(
            ComparisonID::kGreater, *null_ordering_it, type, attr_id));
      }
    }
  }

  /**
   * @brief Constructor.
   *
   * @param sort_config Sort configuration.
   * @param attr_ids Attribute IDs for ORDER BY columns. This can be used when
   *        ORDER BY columns are ScalarExpression, and they are mapped to
   *        ColumnValueAccessor.
   **/
  GenericHeapComparatorInternal(const SortConfiguration &sort_config,
                                const std::vector<attribute_id> &attr_ids) {
    DCHECK(sort_config.isValid());
    DCHECK(sort_config.getOrderByList().size() == attr_ids.size());

    const PtrVector<Scalar> &order_by = sort_config.getOrderByList();
    const std::vector<bool> &ordering = sort_config.getOrdering();
    const std::vector<bool> &null_ordering = sort_config.getNullOrdering();

    PtrVector<Scalar>::const_iterator order_by_it = order_by.begin();
    std::vector<attribute_id>::const_iterator attr_id_it = attr_ids.begin();
    std::vector<bool>::const_iterator ordering_it = ordering.begin();
    std::vector<bool>::const_iterator null_ordering_it = null_ordering.begin();

    for (; order_by_it != order_by.end();
         ++order_by_it, ++attr_id_it, ++ordering_it, ++null_ordering_it) {
      const Type &type = order_by_it->getType();
      if (*ordering_it == kSortAscending) {
        columns_.push_back(new ColumnComparator(
            ComparisonID::kLess, *null_ordering_it, type, *attr_id_it));
      } else {
        columns_.push_back(new ColumnComparator(
            ComparisonID::kGreater, *null_ordering_it, type, *attr_id_it));
      }
    }
  }

  /**
   * @brief Get the vector of column comparators.
   **/
  inline const PtrVector<ColumnComparator> &getColumnComparators() const {
    return columns_;
  }

 private:
  PtrVector<ColumnComparator> columns_;

  DISALLOW_COPY_AND_ASSIGN(GenericHeapComparatorInternal);
};

/**
 * @brief Generic comparator to be used in max-heap for merging runs.
 **/
template <typename ValueAccessorT>
class GenericHeapComparator {
 public:
  /**
   * @brief Constructor.
   *
   * @param internal Internal generic comparator instance initialized with sort
   *        configuration.
   **/
  explicit GenericHeapComparator(const GenericHeapComparatorInternal &internal)
      : columns_(internal.getColumnComparators()) {}

  /**
   * @brief Comparison operator().
   *
   * @param left Left heap node operand in the comparison.
   * @param right Right heap node operand in the comparison.
   **/
  bool operator()(const GenericHeapNode<ValueAccessorT> &left,
                  const GenericHeapNode<ValueAccessorT> &right) {
    // This needs greater than comparator to implement min-heap using
    // std::make_heap, std::push_heap, std::pop_heap.

    // TODO(quickstep-team): This class is not specialized for
    // nullable/non-nullable attributes, since we do not know at compile time at
    // nullability of ORDER BY expressions. If all the ORDER BY expressions are
    // non-nullable, we can have a specialization.
    for (const ColumnComparator &column : columns_) {
      const void *left_value =
          left.value_accessor->getUntypedValue(column.attr_id_);
      const void *right_value =
          right.value_accessor->getUntypedValue(column.attr_id_);

      if ((left_value != nullptr) && (right_value != nullptr)) {
        if (column.comparator_->compareDataPtrs(left_value, right_value)) {
          return false;
        } else if (!column.equal_->compareDataPtrs(left_value, right_value)) {
          return true;
        }
        // Fallback both are equal.
      } else {
        if ((left_value != nullptr) || (right_value != nullptr)) {
          // Following is same as:
          // if (null_last) return right_value;
          // else return left_value;
          return ((column.null_ordering_ == kSortNullLast) && right_value) ||
                 ((column.null_ordering_ == kSortNullFirst) && left_value);
        }
        // Fallback both are NULL.
      }
    }
    // All ORDER BY columns are equal.
    return false;
  }

 private:
  const PtrVector<ColumnComparator> &columns_;
};

/**
 * @brief Reference node for each run for use in heap-sort. Holds column value
 * of the current tuple at the head of each run, and which run-id of this node.
 **/
struct SingleColumnHeapNode {
  std::size_t run_id;
  const void *value;
};

/**
 * @brief Internal data structure for single column comparator to be used in a
 * max-heap to merge sorted runs. This internal structure exists so that we can
 * pass a const reference to this instead of deep copying comparator.
 **/
class SingleColumnHeapComparatorInternal {
 public:
  /**
   * @brief Constructor.
   *
   * @param sort_config Sort configuration.
   **/
  explicit SingleColumnHeapComparatorInternal(
      const SortConfiguration &sort_config) {
    DCHECK(sort_config.isValid());
    DCHECK_EQ(1u, sort_config.getOrderByList().size());

    const bool ordering = sort_config.getOrdering()[0];
    const Scalar &order_by = sort_config.getOrderByList()[0];
    const Type &type = order_by.getType();
    if (ordering == kSortAscending) {
      comparator_.reset(
          ComparisonFactory::GetComparison(ComparisonID::kGreater)
              .makeUncheckedComparatorForTypes(type.getNonNullableVersion(),
                                               type.getNonNullableVersion()));
    } else {
      comparator_.reset(
          ComparisonFactory::GetComparison(ComparisonID::kLess)
              .makeUncheckedComparatorForTypes(type.getNonNullableVersion(),
                                               type.getNonNullableVersion()));
    }
  }

  /**
   * @brief Get the column comparator.
   **/
  inline const UncheckedComparator &getComparator() const {
    return *comparator_;
  }

 private:
  std::unique_ptr<UncheckedComparator> comparator_;
};

/**
 * @brief Single column comparator to be used in max-heap for merging runs.
 *
 * @warning This comparator assumes that the values compared are never NULL. The
 *          merger is expected to handle NULLs outside of the heap.
 **/
class SingleColumnHeapComparator {
 public:
  /**
   * @brief Constructor.
   *
   * @param internal Internal generic comparator instance initialized with sort
   *        configuration.
   **/
  explicit SingleColumnHeapComparator(
      const SingleColumnHeapComparatorInternal &internal)
      : comparator_(internal.getComparator()) {}

  /**
   * @brief Comparison operator().
   *
   * @param left Left heap node operand in the comparison.
   * @param right Right heap node operand in the comparison.
   **/
  inline bool operator()(const SingleColumnHeapNode &left,
                         const SingleColumnHeapNode &right) {
    // This needs to be greater than comparator to implement min-heap using
    // std::make_heap, std::push_heap, std::pop_heap.
    return comparator_.compareDataPtrs(left.value, right.value);
  }

 private:
  const UncheckedComparator &comparator_;
};

/**
 * @}
 */

}  // namespace merge_run_operator

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_SORT_MERGE_RUN_OPERATOR_HELPERS_HPP_
