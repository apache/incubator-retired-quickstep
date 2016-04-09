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

#ifndef QUICKSTEP_TRANSACTION_LOCK_HPP_
#define QUICKSTEP_TRANSACTION_LOCK_HPP_

#include "transaction/AccessMode.hpp"
#include "transaction/ResourceId.hpp"

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Class for representing resource locks.
 **/
class Lock {
 public:
  /**
   * @brief Constructor.
   *
   * @param rid Resource id of the resource on which the lock is taken.
   * @param access_mode Mode of the lock access.
   **/
  Lock(const ResourceId &rid, const AccessMode &access_mode)
      : rid_(rid),
        access_mode_(access_mode) {
  }

  /**
   * @brief Equality operator for lock.
   *
   * @param other Reference to the other Lock object that will be
   *        compared against to this.
   * @return True if both locks have equal resource ids
   *         and access modes, false otherwise.
   **/
  inline bool operator==(const Lock &other) const {
    return rid_ == other.rid_ && access_mode_ == other.access_mode_;
  }

  /**
   * @brief Inequality operator for lock.
   *
   * @param other Reference to the other Lock object that will be
   *        compared against to this.
   * @return False if one of them has different resource id
   *         or access mode, true otherwise.
   **/
  inline bool operator!=(const Lock &other) const {
    return !(*this == other);
  }

  /**
   * @brief Getter for resource id.
   *
   * @return Resource id of the lock.
   **/
  inline const ResourceId& getResourceId() const {
    return rid_;
  }

  /**
   * @brief Getter for access mode.
   *
   * @return Access mode of the lock.
   **/
  inline const AccessMode& getAccessMode() const {
    return access_mode_;
  }

 private:
  // Id of the resource that is locked.
  const ResourceId rid_;

  // Lock's access type.
  AccessMode access_mode_;
};

/** @} */

}  // namespace transaction

}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_LOCK_HPP_
