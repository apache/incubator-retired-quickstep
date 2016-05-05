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

#ifndef QUICKSTEP_TRANSACTION_ACCESS_MODE_HPP_
#define QUICKSTEP_TRANSACTION_ACCESS_MODE_HPP_

#include <cstdint>
#include <type_traits>

namespace quickstep {
namespace transaction {

/** \addtogroup Transaction
 *  @{
 */

/**
 * @brief Represents mode type. Possible options are NL, IS, IX, S, SIX, X.
 **/
enum class AccessModeType : std::uint8_t {
  kNoLockMode = 0,
  kIsLockMode,
  kIxLockMode,
  kSLockMode,
  kSixLockMode,
  kXLockMode,
  kNumAccessModeTypes,
};

/**
 * @brief Class for representing resource lock's access mode
 **/
class AccessMode {
 public:
  /**
   * @brief Only constructor for access mode.
   *
   * @param access_mode Mode type of the object.
   **/
  explicit AccessMode(const AccessModeType access_mode)
      : access_mode_(access_mode) {}

  /**
   * @brief Factory method for NoLockMode.
   *
   * @return NoLockMode instance.
   **/
  static AccessMode NoLockMode() {
    return AccessMode(AccessModeType::kNoLockMode);
  }

  /**
   * @brief Factory method for IsLockMode.
   *
   * @return IsLockMode instance.
   **/
  static AccessMode IsLockMode() {
    return AccessMode(AccessModeType::kIsLockMode);
  }

  /**
   * @brief Factory method for IxLockMode.
   *
   * @return IxLockMode instance.
   **/
  static AccessMode IxLockMode() {
    return AccessMode(AccessModeType::kIxLockMode);
  }

  /**
   * @brief Factory method for SixLockMode.
   *
   * @return SixLockMode instance.
   **/
  static AccessMode SixLockMode() {
    return AccessMode(AccessModeType::kSixLockMode);
  }

  /**
   * @brief Factory method for SLockMode.
   *
   * @return SLockMode instance.
   **/
  static AccessMode SLockMode() {
    return AccessMode(AccessModeType::kSLockMode);
  }

  /**
   * @brief Factory method for XLockMode.
   *
   * @return XLockMode instance.
   **/
  static AccessMode XLockMode() {
    return AccessMode(AccessModeType::kXLockMode);
  }

  /**
   * @brief Checks whether this access mode is compatible with the other.
   *
   * @param other Other access mode that will be checked against to this one.
   * @return True if they are compatible, otherwise false.
   **/
  inline bool isCompatible(const AccessMode &other) const {
    const access_mode_underlying_type this_mode =
        static_cast<access_mode_underlying_type>(access_mode_);
    const access_mode_underlying_type other_mode =
        static_cast<access_mode_underlying_type>(other.access_mode_);
    return AccessMode::kLockCompatibilityMatrix[this_mode][other_mode];
  }

  /**
   * @brief Checks whether this access mode is IS mode.
   *
   * @return True if it is IS mode, false otherwise.
   **/
  inline bool isIntentionShareLock() const {
    return access_mode_ == AccessModeType::kIsLockMode;
  }

  /**
   * @brief Checks whether this access mode is IX mode.
   *
   * @return True if it is IX mode, false otherwise.
   **/
  inline bool isIntentionExclusiveLock() const {
    return access_mode_ == AccessModeType::kIxLockMode;
  }

  /**
   * @brief Checks whether this access mdoe is SIX mode.
   *
   * @return True if it is SIX mode, false otherwise.
   **/
  inline bool isShareAndIntentionExclusiveLock() const {
    return access_mode_ == AccessModeType::kSixLockMode;
  }

  /**
   * @brief Checks whether this access mode is S mode.
   *
   * @return True if it is S mode, false otherwise.
   **/
  inline bool isShareLock() const {
    return access_mode_ == AccessModeType::kSLockMode;
  }

  /**
   * @brief Checks whether this access mode is X mode.
   *
   * @return True if it is X mode, false otherwise.
   **/
  inline bool isExclusiveLock() const {
    return access_mode_ == AccessModeType::kXLockMode;
  }

  /**
   * @brief Checks whether this access mode is in
   *        the same level with other mode.
   *
   * @return True if both modes have the same level.
   **/
  inline bool operator==(const AccessMode &other) const {
    return access_mode_ == other.access_mode_;
  }

  /**
   * @brief Checks whether this access mode is in
   *        the different level with other mode.
   *
   * @return True if the modes have different levels.
   **/
  inline bool operator!=(const AccessMode &other) const {
    return access_mode_ != other.access_mode_;
  }

 private:
  typedef std::underlying_type<AccessModeType>::type
      access_mode_underlying_type;

  // The compatibility matrix should be N by N. kNumberLocks == N.
  static constexpr std::uint64_t kNumberLocks =
      static_cast<access_mode_underlying_type>(
          AccessModeType::kNumAccessModeTypes);

  // Compatibility matrix for checking access modes.
  // True means they are compatible.
  static const bool kLockCompatibilityMatrix[kNumberLocks][kNumberLocks];
  // Type of access, the possible values are
  // NoLock, IsLock, IxLock, SLock, SixLock, XLock
  AccessModeType access_mode_;
};

/** @} */

}  // namespace transaction
}  // namespace quickstep

#endif  // QUICKSTEP_TRANSACTION_ACCESS_MODE_HPP_
