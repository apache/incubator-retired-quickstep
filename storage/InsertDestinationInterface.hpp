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

#ifndef QUICKSTEP_STORAGE_INSERT_DESTINATION_INTERFACE_HPP_
#define QUICKSTEP_STORAGE_INSERT_DESTINATION_INTERFACE_HPP_

#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "types/containers/Tuple.hpp"

namespace quickstep {

class CatalogRelationSchema;
class ValueAccessor;

/** \addtogroup Storage
 *  @{
 */

/**
 * @brief A pure interface class that InsertDestination and its subclasses
 *        implement. This class is used to separate interface from
 *        implementation and resolve what would otherwise be a circular
 *        dependency between StorageBlock and InsertDestination.
 *
 * @note This interface only contains methods to logically insert tuples, plus
 *       a few basic informational methods. Methods involving explicit
 *       management of blocks are in the InsertDestination class.
 **/
class InsertDestinationInterface {
 public:
  virtual ~InsertDestinationInterface() {
  }

  /**
   * @brief Get the relation which tuples are inserted into.
   *
   * @return The relation which tuples are inserted into.
   **/
  virtual const CatalogRelationSchema& getRelation() const = 0;

  /**
   * @brief Get the attribute ID used for partitioning, if any.
   * @note This is intended only for use by StorageBlock::update(), to
   *       determine whether it is safe to do in-place updates or whether all
   *       updated Tuples must be relocated to land in the correct partition.
   *
   * @return The ID of the attribute used for partitioning, or -1 if there is
   *         no partitioning.
   **/
  virtual attribute_id getPartitioningAttribute() const = 0;

  /**
   * @brief Insert a single tuple into a block managed by this
   *        InsertDestination.
   *
   * @param tuple The tuple to insert.
   * @exception TupleTooLargeForBlock Even though a block was initially empty,
   *            the tuple was too large to insert. Only thrown if a block is
   *            initially empty, otherwise failure to insert simply causes
   *            another block to be used.
   **/
  virtual void insertTuple(const Tuple &tuple) = 0;

  /**
   * @brief Insert a single tuple into a block managed by this
   *        InsertDestination using the batch-insert path (i.e. do not rebuild
   *        blocks until they are full or all insertions are complete).
   *
   * @param tuple The tuple to insert.
   * @exception TupleTooLargeForBlock Even though a block was initially empty,
   *            the tuple was too large to insert. Only thrown if a block is
   *            initially empty, otherwise failure to insert simply causes
   *            another block to be used.
   **/
  virtual void insertTupleInBatch(const Tuple &tuple) = 0;

  /**
   * @brief Bulk-insert tuples from a ValueAccessor into blocks managed by this
   *        InsertDestination.
   *
   * @param accessor A ValueAccessor whose tuples will by inserted into blocks
   *        from this InsertDestination.
   * @param always_mark_full If \c true, always mark the blocks full after
   *        insertion from ValueAccessor even when partially full.
   **/
  virtual void bulkInsertTuples(ValueAccessor *accessor,
                                bool always_mark_full = false) = 0;

  /**
   * @brief Bulk-insert tuples from a ValueAccessor with differently-ordered
   *        attributes into blocks managed by this InsertDestination.
   *
   * @param attribute_map A vector which maps the attributes of this
   *        InsertDestination's relation (in order with no gaps) to the
   *        corresponding attributes which should be read from accessor.
   * @param accessor A ValueAccessor whose tuples will by inserted into blocks
   *        from this InsertDestination.
   * @param always_mark_full If \c true, always mark the blocks full after
   *        insertion from ValueAccessor even when partially full.
   **/
  virtual void bulkInsertTuplesWithRemappedAttributes(
      const std::vector<attribute_id> &attribute_map,
      ValueAccessor *accessor,
      bool always_mark_full = false) = 0;

  /**
   * @brief Bulk-insert tuples from one or more ValueAccessors
   *        into blocks managed by this InsertDestination.
   *
   * @warning It is implicitly assumed that all the input ValueAccessors have
   *          the same number of tuples in them.
   *
   * @param accessor_attribute_map A vector of pairs of ValueAccessor and
   *        corresponding attribute map
   *        The i-th attribute ID in the attr map for a value accessor is "n" 
   *        if the attribute_id "i" in the output relation
   *        is the attribute_id "n" in corresponding input value accessor.
   *        Set the i-th element to kInvalidCatalogId if it doesn't come from
   *        the corresponding value accessor.
   * @param always_mark_full If \c true, always mark the blocks full after
   *        insertion from ValueAccessor even when partially full.
   **/
  virtual void bulkInsertTuplesFromValueAccessors(
      std::vector<std::pair<ValueAccessor *, std::vector<attribute_id>>> accessor_attribute_map,
      bool always_mark_full = false) = 0;

  /**
   * @brief Insert tuples from a range of Tuples in a vector.
   * @warning Unlike bulkInsertTuples(), this is not well-optimized and not
   *          intended for general use. It should only be used by
   *          StorageBlock::update().
   *
   * @param begin The first element in the range of Tuples to insert.
   * @param end One-past-the-end of the range of Tuples to insert.
   **/
  virtual void insertTuplesFromVector(std::vector<Tuple>::const_iterator begin,
                                      std::vector<Tuple>::const_iterator end) = 0;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_STORAGE_INSERT_DESTINATION_INTERFACE_HPP_
