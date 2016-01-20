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

#ifndef QUICKSTEP_STORAGE_TUPLE_ID_SEQUENCE_HPP_
#define QUICKSTEP_STORAGE_TUPLE_ID_SEQUENCE_HPP_

#include <cstddef>
#include <limits>
#include <vector>

#include "storage/StorageBlockInfo.hpp"
#include "utility/BitVector.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A sequence of Tuple IDs (implemented as a thin wrapper around
 *        BitVector), used to communicate information about multiple tuples
 *        between SubBlocks.
 **/
class TupleIdSequence {
 public:
  /**
   * @brief Const-iterator over tuple IDs set in a TupleIdSequence.
   **/
  class ConstIterator {
   public:
    ConstIterator()
        : bitvector_(nullptr) {
    }

    inline tuple_id operator*() const {
      DEBUG_ASSERT(bitvector_ != nullptr);
      DEBUG_ASSERT(current_position_ < bitvector_->size());
      return current_position_;
    }

    inline ConstIterator& operator++() {
      DEBUG_ASSERT(bitvector_ != nullptr);
      const std::size_t search_pos = current_position_ + 1;
      current_position_ = (search_pos < bitvector_->size()) ? bitvector_->firstOne(search_pos)
                                                            : bitvector_->size();
      return *this;
    }

    inline ConstIterator& operator--() {
      DEBUG_ASSERT(bitvector_ != nullptr);
      current_position_ = bitvector_->lastOne(current_position_);
      if (current_position_ == bitvector_->size()) {
        current_position_ = std::numeric_limits<std::size_t>::max();
      }
      return *this;
    }

    ConstIterator operator++(int) {
      ConstIterator result(*this);
      ++(*this);
      return result;
    }

    ConstIterator operator--(int) {
      ConstIterator result(*this);
      --(*this);
      return result;
    }

    inline bool operator==(const ConstIterator& other) const {
      return ((bitvector_ == other.bitvector_)
              && (current_position_ == other.current_position_));
    }

    inline bool operator!=(const ConstIterator& other) const {
      return !(*this == other);
    }

   private:
    friend class TupleIdSequence;

    ConstIterator(const BitVector<false> *bitvector,
                  const std::size_t initial_position)
        : bitvector_(bitvector),
          current_position_(initial_position) {
    }

    const BitVector<false> *bitvector_;
    std::size_t current_position_;
  };

  typedef ConstIterator const_iterator;
  typedef std::size_t size_type;

  /**
   * @brief Constructor.
   *
   * @param length The number of tuple ID slots in this TupleIdSequence.
   *        Typically TupleStorageSubBlock::getMaxTupleID() + 1.
   **/
  explicit TupleIdSequence(const tuple_id length)
      : internal_bitvector_(length) {
  }

  /**
   * @brief Constructor which initializes a TupleIdSequence from bits set in a
   *        (possibly longer) BitVector.
   *
   * @param length The number of tuple ID slots in this TupleIdSequence.
   *        Typically TupleStorageSubBlock::getMaxTupleID() + 1.
   * @param bit_vector A BitVector of at least length bits to initially assign
   *        from.
   **/
  TupleIdSequence(const tuple_id length,
                  const BitVector<false> &bit_vector)
      : internal_bitvector_(length) {
    internal_bitvector_.assignFromLonger(bit_vector);
  }

  /**
   * @brief Destructor.
   **/
  ~TupleIdSequence() {
  }

  /**
   * @brief Set a particular tuple ID as being on or off in this sequence.
   *
   * @param tuple The ID to set.
   * @param on If true, tuple should be part of this sequence. If false,
   *        remove tuple from this sequence (if it was present).
   **/
  inline void set(const tuple_id tuple, const bool on) {
    DEBUG_ASSERT(tuple >= 0);
    DEBUG_ASSERT(static_cast<std::size_t>(tuple) < internal_bitvector_.size());
    internal_bitvector_.setBit(tuple, on);
  }

  /**
   * @brief Set a range of tuple IDs all at once.
   *
   * @param first_tuple The first ID to set.
   * @param num_tuples The number of consecutive tuple IDs to set.
   * @param on If true, tuple IDs in range should be part of this sequence. If
   *        false, remove tuple IDs in range from this sequence.
   **/
  inline void setRange(const tuple_id first_tuple,
                       const tuple_id num_tuples,
                       const bool on) {
    DEBUG_ASSERT(first_tuple >= 0);
    if (num_tuples > 0) {
      internal_bitvector_.setBitRange(first_tuple, num_tuples, on);
    }
  }

  /**
   * @brief Set the value of a word of bits simultaneously.
   *
   * @param word_id The position of the word whose value should be set.
   * @param word_value The word value to set for the bits in the word.
   **/
  inline void setWord(const std::size_t word_id,
                      const std::size_t word_value) {
    DEBUG_ASSERT(word_id * (sizeof(std::size_t) << 3)
        < internal_bitvector_.size());
    internal_bitvector_.setBitWord(word_id, word_value);
  }

  /**
   * @brief Determine if a particular tuple ID is part of this sequence.
   *
   * @param tuple The tuple ID to check.
   * @return true if tuple is part of this sequence, false if not.
   **/
  inline bool get(const tuple_id tuple) const {
    DEBUG_ASSERT(tuple >= 0);
    DEBUG_ASSERT(static_cast<std::size_t>(tuple) < internal_bitvector_.size());
    return internal_bitvector_.getBit(tuple);
  }

  /**
   * @brief Get the value of a word of bits.
   *
   * @param word_id The position of the word.
   * @return The word value at word_id.
   **/
  inline std::size_t getWord(const std::size_t word_id) const {
    DEBUG_ASSERT(word_id * (sizeof(std::size_t) << 3)
        < internal_bitvector_.size());
    return internal_bitvector_.getBitWord(word_id);
  }

  /**
   * @brief Determine if ANY tuple ID is part of this sequence.
   *
   * @return true if any tuple ID is part of this sequence, false otherwise.
   **/
  inline bool empty() const {
    return !internal_bitvector_.any();
  }

  /**
   * @brief Get the number of tuple ID slots in this sequence.
   *
   * @note This gives the number of slots. To get the actual number of tuple
   *       IDs that are "on" in this sequence, use numTuples().
   *
   * @return The number of tuple ID slots in this sequence.
   **/
  inline size_type length() const {
    return internal_bitvector_.size();
  }

  /**
   * @brief Get the number of tuple IDs that are actually part of this
   *        sequence (i.e. set to "on").
   *
   * @return The number of tuple IDs in this sequence.
   **/
  inline size_type numTuples() const {
    return internal_bitvector_.onesCount();
  }

  /**
   * @brief Get the number of tuple IDs that are actually part of this
   *        sequence (i.e. set to "on").
   *
   * @return The number of tuple IDs in this sequence.
   **/
  inline size_type size() const {
    return internal_bitvector_.onesCount();
  }

  /**
   * @brief Get an iterator to the first tuple ID that is actually part of this
   *        sequence (i.e. set to "on").
   *
   * @return An iterator pointing to the first tuple ID in this sequence.
   **/
  inline const_iterator begin() const {
    return const_iterator(&internal_bitvector_, internal_bitvector_.firstOne());
  }

  /**
   * @brief Get an iterator one-before-the-beginning of tuple IDs actually in
   *        this sequence.
   * @warning The iterator returned must be incremented to become valid.
   *
   * @return An iterator one-before-the-beginning of tuple IDs in this
   *         sequence.
   **/
  inline const_iterator before_begin() const {
    return const_iterator(&internal_bitvector_, std::numeric_limits<std::size_t>::max());
  }

  /**
   * @brief Get an iterator one-past-the-end of tuple IDs actually in this
   *        sequence.
   *
   * @return An iterator one-past-the-end of tuple IDs in this sequence.
   **/
  inline const_iterator end() const {
    return const_iterator(&internal_bitvector_, internal_bitvector_.size());
  }

  /**
   * @brief Get the smallest tuple ID in this sequence.
   *
   * @warning This is only safe to call if empty() is false.
   *
   * @return The smallest tuple ID in this sequence.
   **/
  inline tuple_id front() const {
    DEBUG_ASSERT(!empty());
    return internal_bitvector_.firstOne();
  }

  /**
   * @brief Get the largest tuple ID in this sequence.
   *
   * @warning This is only safe to call if empty() is false.
   *
   * @return The largest tuple ID in this sequence.
   **/
  inline tuple_id back() const {
    DEBUG_ASSERT(!empty());
    return internal_bitvector_.lastOne();
  }

  /**
   * @brief Invert this tuple ID sequence, switching all matches to non-matches
   *        and vice-versa.
   * @warning If this sequence represents tuples in a TupleStorageSubBlock
   *          which is not packed (i.e. which has holes in its sequence of
   *          tuple IDs), then this method can cause non-existent Tuple IDs
   *          to appear as matches. After inverting, the intersectWith() should
   *          be called with an existence bitmap to filter out such IDs.
   **/
  inline void invert() {
    internal_bitvector_.flipBits();
  }

  /**
   * @brief Assign this TupleIdSequence to be the same as another
   *        TupleIdSequence of the same length.
   *
   * @param other Another TupleIdSequence to copy from.
   **/
  inline void assignFrom(const TupleIdSequence &other) {
    internal_bitvector_.assignFrom(other.internal_bitvector_);
  }

  /**
   * @brief Take the intersection of this TupleIdSequence with another,
   *        modifying this TupleIdSequence in-place.
   * @warning This TupleIdSequence must be the same length as the other, and
   *          the intersection only has semantic meaning if both
   *          TupleIdSequences refer to tuples in the same block.
   *
   * @param other Another TupleIdSequence to intersect with.
   **/
  inline void intersectWith(const TupleIdSequence &other) {
    internal_bitvector_.intersectWith(other.internal_bitvector_);
  }

  /**
   * @brief Take the intersection of this TupleIdSequence with another's
   *        complement (i.e. set difference), modifying this TupleIdSequence
   *        in-place.
   * @warning This TupleIdSequence must be the same length as the other, and
   *          the set-difference only has semantic meaning if both
   *          TupleIdSequences refer to tuples in the same block.
   *
   * @param other Another TupleIdSequence to intersect with the complement of.
   **/
  inline void intersectWithComplement(const TupleIdSequence &other) {
    DEBUG_ASSERT(length() == other.length());
    internal_bitvector_.unsetFrom(other.internal_bitvector_);
  }

  /**
   * @brief Take the union of this TupleIdSequence with another,
   *        modifying this TupleIdSequence in-place.
   * @warning This TupleIdSequence must be the same length as the other, and
   *          the union only has semantic meaning if both
   *          TupleIdSequences refer to tuples in the same block.
   *
   * @param other Another TupleIdSequence to union with.
   **/
  inline void unionWith(const TupleIdSequence &other) {
    internal_bitvector_.unionWith(other.internal_bitvector_);
  }

  /**
   * @brief Get a const reference to this TupleIdSequence's underlying
   *        BitVector representation.
   **/
  const BitVector<false>& getInternalBitVector() const {
    return internal_bitvector_;
  }

 private:
  BitVector<false> internal_bitvector_;

  DISALLOW_COPY_AND_ASSIGN(TupleIdSequence);
};

/**
 * @brief An ordered sequence of Tuple IDs used to communicate information about
 *        multiple tuples between SubBlocks.
 */
typedef std::vector<tuple_id> OrderedTupleIdSequence;

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_TUPLE_ID_SEQUENCE_HPP_
