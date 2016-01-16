/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "relational_operators/SortMergeRunOperatorHelpers.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

#include "storage/StorageBlock.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageManager.hpp"
#include "storage/ValueAccessor.hpp"
#include "storage/ValueAccessorUtil.hpp"
#include "threading/SpinMutex.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/Macros.hpp"
#include "utility/PtrVector.hpp"
#include "utility/SortConfiguration.hpp"

namespace quickstep {

namespace merge_run_operator {

constexpr std::size_t MergeTree::kFinalLevelUninitialized;

void MergeTree::initializeTree(const std::size_t initial_runs) {
  // Compute expected runs per level.
  runs_expected_.clear();
  runs_expected_.push_back(initial_runs);  // Intialize first level.
  while (runs_expected_.back() > merge_factor_) {
    // Compute number of runs in current level based on number of runs in
    // previous level.
    runs_expected_.push_back((runs_expected_.back() + merge_factor_ - 1) /
                             merge_factor_);
  }

  // Initialize scheduled runs per level.
  runs_scheduled_.resize(runs_expected_.size(), 0);

  // Pending jobs per level.
  {
    SpinMutexLock lock(pending_mutex_);
    pending_.resize(runs_expected_.size() + 1);
  }

  num_levels_ = runs_expected_.size();
  cur_level_ = 0;
  final_level_ = num_levels_ - 1;
}

void MergeTree::initializeForPipeline() {
  runs_scheduled_.push_back(0);

  // Temporary number of expected initial runs till pipelining is done.
  runs_expected_.push_back(MergeTree::kFinalLevelUninitialized);
  num_levels_ = 1;
  cur_level_ = 0;
  final_level_ = kFinalLevelUninitialized;
  pending_.emplace_back();
  pending_.emplace_back();  // For storing output of first level merges.
}

// TODO(shoban): If catalog has support to reassign blocks from one relation
// to another (schema permitting), we can avoid this copy.
void MergeTree::checkAndFixFinalMerge() {
  // Find if the final output merge was already scheduled to write to
  // run_destination_.
  SpinMutexLock lock(pending_mutex_);
  if ((runs_expected_[0] == merge_factor_) &&
      (runs_scheduled_[0] == merge_factor_)) {
    // Need to generate a work-order to copy the run that was already scheduled
    // to be generated in the run_relation_ and run_destination_ into the
    // output_destination_. Following simply achieves that by creating a
    // MergeWorkOrder merging one run from pending_[1] into pending[2].
    pending_.emplace_back();
    runs_expected_.push_back(1);
    runs_scheduled_.push_back(0);
    ++final_level_;
    ++num_levels_;
  }
}

bool MergeTree::getMergeJobs(std::vector<MergeJob> *jobs) {
  // Check each merge level, if there are jobs there to be scheduled.
  for (std::size_t level = cur_level_; level < num_levels_; ++level) {
    if (runs_expected_[level] > runs_scheduled_[level]) {
      SpinMutexLock lock(pending_mutex_);
      std::size_t available_jobs = pending_[level].size();
      while ((available_jobs >= merge_factor_) ||
             (available_jobs && (available_jobs + runs_scheduled_[level] ==
                                 runs_expected_[level]))) {
        // Can generate a merge job.
        std::size_t merge_size = std::min(available_jobs, merge_factor_);
        std::vector<Run> runs;
        getRuns(level, merge_size, &runs);

        jobs->emplace_back(level, level == final_level_, std::move(runs));
        runs_scheduled_[level] += merge_size;

        available_jobs = pending_[level].size();
      }
    } else {
      ++cur_level_;
    }
  }

  // Done generating merge jobs, if final level is not uninitialized and final
  // merge job is scheduled.
  return (final_level_ != kFinalLevelUninitialized) &&
         (runs_scheduled_[final_level_] == runs_expected_[final_level_]);
}

void RunMerger::doMerge() {
  block_id first_valid_block = kInvalidBlockId;
  for (const Run &run : input_runs_) {
    if (!run.empty()) {
      first_valid_block = run.front();
      break;
    }
  }
  DEBUG_ASSERT(first_valid_block != kInvalidBlockId);

  BlockReference block(
      storage_manager_->getBlock(first_valid_block, run_relation_));
  // Create a value-accessor just to figure the type.
  std::unique_ptr<ValueAccessor> first_accessor(
      block->getTupleStorageSubBlock().createValueAccessor());

  if (input_runs_.size() == 1) {
    // Only one input run; use fast copy implementation.
    if (top_k_ > 0) {
      copyToOutput<true>(input_runs_[0], first_accessor.get());
    } else {
      copyToOutput<false>(input_runs_[0], first_accessor.get());
    }
  } else if (sort_config_.getOrderByList().size() == 1) {
    // Only one ORDER BY column; use fast implementation for this case.
    if (sort_config_.getNullOrdering()[0] == kSortNullLast) {
      if (top_k_ > 0) {
        mergeSingleColumnNullLast<true>(first_accessor.get());
      } else {
        mergeSingleColumnNullLast<false>(first_accessor.get());
      }
    } else {
      if (top_k_ > 0) {
        mergeSingleColumnNullFirst<true>(first_accessor.get());
      } else {
        mergeSingleColumnNullFirst<false>(first_accessor.get());
      }
    }
  } else {
    // Fallback to generic implementation for any number of input runs and ORDER
    // BY columns.
    if (top_k_ > 0) {
      mergeGeneric<true>(first_accessor.get());
    } else {
      mergeGeneric<false>(first_accessor.get());
    }
  }

  // Flush the final block actively, since Foreman will not destruct the
  // WorkOrder before handling blocks generated from WorkOrders.
  output_run_creator_.flushBlock();
}

template <bool check_top_k>
void RunMerger::mergeGeneric(ValueAccessor *first_accessor) {
  GenericHeapComparatorInternal comp_internal(sort_config_);

  InvokeOnValueAccessorNotAdapter(
      first_accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    typedef typename std::remove_reference<decltype(*accessor)>::type ValueAccessorT;

    GenericHeapComparator<ValueAccessorT> comp(comp_internal);
    PtrVector<RunIterator<ValueAccessorT>> iterators;

    // Initialize heap with top tuple in each run.
    std::vector<GenericHeapNode<ValueAccessorT>> heap;
    for (std::vector<Run>::size_type run_id = 0;
         run_id < input_runs_.size();
         ++run_id) {
      iterators.push_back(new RunIterator<ValueAccessorT>(
          input_runs_[run_id], storage_manager_, run_relation_));
      if (iterators.back().next()) {
        heap.push_back({run_id, iterators.back().getValueAccessor()});
      }
    }
    std::make_heap(heap.begin(), heap.end(), comp);

    std::unique_ptr<Tuple> tuple;
    std::size_t num_tuples = 0;
    while (!heap.empty()) {
      // Pop top tuple in the heap.
      std::pop_heap(heap.begin(), heap.end(), comp);
      GenericHeapNode<ValueAccessorT> current = heap.back();
      heap.pop_back();

      tuple.reset(iterators[current.run_id].getValueAccessor()->getTuple());
      output_run_creator_.appendTuple(*tuple);

      // Check if top-k tuples are inserted already.
      if (check_top_k && (++num_tuples == top_k_)) {
        return;
      }

      if (iterators[current.run_id].next()) {
        // Insert next tuple from the run into heap.
        heap.push_back(
            {current.run_id, iterators[current.run_id].getValueAccessor()});
        std::push_heap(heap.begin(), heap.end(), comp);
      }
    }
  });
}

template <bool check_top_k>
void RunMerger::mergeSingleColumnNullFirst(ValueAccessor *first_accessor) {
  const attribute_id attr_id =
      sort_config_.getOrderByList()[0].getAttributeIdForValueAccessor();
  DEBUG_ASSERT(attr_id != -1);

  SingleColumnHeapComparatorInternal comp_internal(sort_config_);
  SingleColumnHeapComparator comp(comp_internal);

  InvokeOnValueAccessorNotAdapter(
      first_accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    typedef typename std::remove_reference<decltype(*accessor)>::type ValueAccessorT;

    PtrVector<RunIterator<ValueAccessorT>, true> iterators;
    std::vector<SingleColumnHeapNode> heap;
    std::unique_ptr<Tuple> tuple;
    std::size_t num_tuples = 0;

    // Initialize heap with the first non-NULL sort key tuple.
    for (std::vector<Run>::size_type run_id = 0;
         run_id < input_runs_.size();
         ++run_id) {
      std::unique_ptr<RunIterator<ValueAccessorT>> run_it(
          new RunIterator<ValueAccessorT>(
              input_runs_[run_id], storage_manager_, run_relation_));
      bool heap_inserted = false;
      while (run_it->next()) {
        const void *value = run_it->getValueAccessor()->getUntypedValue(attr_id);
        if (value == nullptr) {
          // NULL value; insert tuple into output run.
          tuple.reset(run_it->getValueAccessor()->getTuple());
          output_run_creator_.appendTuple(*tuple);

          if (check_top_k && (++num_tuples == top_k_)) {
            return;
          }
        } else {
          // Finished exhausting NULL values.
          heap.push_back({run_id, value});
          heap_inserted = true;
          break;
        }
      }
      if (heap_inserted) {
        iterators.push_back(run_it.release());
      } else {
        // Exhausted the run.
        iterators.push_back(nullptr);
      }
    }
    std::make_heap(heap.begin(), heap.end(), comp);

    while (!heap.empty()) {
      // Pop the top tuple from heap.
      std::pop_heap(heap.begin(), heap.end(), comp);
      SingleColumnHeapNode current = heap.back();
      heap.pop_back();

      tuple.reset(iterators[current.run_id].getValueAccessor()->getTuple());
      output_run_creator_.appendTuple(*tuple);

      // Check if top-K tuples are already inserted.
      if (check_top_k && (++num_tuples == top_k_)) {
        return;
      }

      if (iterators[current.run_id].next()) {
        // Insert next tuple from the run into heap.
        heap.push_back({current.run_id,
                        iterators[current.run_id]
                            .getValueAccessor()
                            ->template getUntypedValue<false>(attr_id)});
        std::push_heap(heap.begin(), heap.end(), comp);
      }
    }
  });
}

template <bool check_top_k>
void RunMerger::mergeSingleColumnNullLast(ValueAccessor *first_accessor) {
  const attribute_id attr_id =
      sort_config_.getOrderByList()[0].getAttributeIdForValueAccessor();
  DEBUG_ASSERT(attr_id != -1);

  SingleColumnHeapComparatorInternal comp_internal(sort_config_);
  SingleColumnHeapComparator comp(comp_internal);

  InvokeOnValueAccessorNotAdapter(
      first_accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    typedef typename std::remove_reference<decltype(*accessor)>::type ValueAccessorT;

    PtrVector<RunIterator<ValueAccessorT>> iterators;
    std::vector<SingleColumnHeapNode> heap;
    std::vector<RunIterator<ValueAccessorT> *> null_iterators;
    std::unique_ptr<Tuple> tuple;

    // Initialize the heap with head tuple in each run.
    for (std::vector<Run>::size_type run_id = 0;
         run_id < input_runs_.size();
         ++run_id) {
      iterators.push_back(new RunIterator<ValueAccessorT>(
          input_runs_[run_id], storage_manager_, run_relation_));
      if (iterators.back().next()) {
        const void *value =
            iterators.back().getValueAccessor()->getUntypedValue(attr_id);
        if (value != nullptr) {
          // Non-NULL value.
          heap.push_back({run_id, value});
        } else {
          // Only NULL values from now. Add to null_iterators to exhaust them in
          // the end.
          null_iterators.push_back(&iterators[run_id]);
        }
      }
    }
    std::make_heap(heap.begin(), heap.end(), comp);

    std::size_t num_tuples = 0;
    while (!heap.empty()) {
      // Pop top tuple from heap.
      std::pop_heap(heap.begin(), heap.end(), comp);
      SingleColumnHeapNode current = heap.back();
      heap.pop_back();

      tuple.reset(iterators[current.run_id].getValueAccessor()->getTuple());
      output_run_creator_.appendTuple(*tuple);

      // Check if top-K tuples are inserted already.
      if (check_top_k && (++num_tuples == top_k_)) {
        return;
      }

      if (iterators[current.run_id].next()) {
        // Insert next tuple from the run in heap (if the sort key is not NULL.)
        const void *value =
            iterators[current.run_id].getValueAccessor()->getUntypedValue(
                attr_id);
        if (value != nullptr) {
          heap.push_back({current.run_id, value});
          std::push_heap(heap.begin(), heap.end(), comp);
        } else {
          null_iterators.push_back(&iterators[current.run_id]);
        }
      }
    }

    // Insert all NULLs now.
    for (RunIterator<ValueAccessorT> *run_it : null_iterators) {
      // First tuple in all iterators in null_iterators is already
      // next()-checked.
      do {
        tuple.reset(run_it->getValueAccessor()->getTuple());
        output_run_creator_.appendTuple(*tuple);

        if (check_top_k && (++num_tuples == top_k_)) {
          return;
        }
      } while (run_it->next());
    }
  });
}

template <bool check_top_k>
void RunMerger::copyToOutput(const Run &run,
                             ValueAccessor *first_accessor) {
  DEBUG_ASSERT(input_runs_.size() == 1);

  InvokeOnValueAccessorNotAdapter(
      first_accessor,
      [&](auto *accessor) -> void {  // NOLINT(build/c++11)
    RunIterator<typename std::remove_reference<decltype(*accessor)>::type> run_it(
        input_runs_[0], storage_manager_, run_relation_);

    std::unique_ptr<Tuple> tuple;
    std::size_t num_tuples = 0;
    while (run_it.next()) {
      tuple.reset(run_it.getValueAccessor()->getTuple());
      output_run_creator_.appendTuple(*tuple);
      if (check_top_k && (++num_tuples == top_k_)) {
        return;
      }
    }
  });
}

}  // namespace merge_run_operator

}  // namespace quickstep
