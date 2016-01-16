// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_INTERNAL_MEMORY_MIRROR_CANCELLATION_SET_H_
#define TMB_INTERNAL_MEMORY_MIRROR_CANCELLATION_SET_H_

namespace tmb {
namespace internal {

/**
 * @brief A shared object which tracks the information needed to efficiently
 *        cancel a message in the PersistentBusStateInterface underlying a
 *        MemoryMirrorMessageBus.
 **/
class MemoryMirrorCancellationSet {
 public:
  MemoryMirrorCancellationSet() {
  }

  virtual ~MemoryMirrorCancellationSet() {
  }

 private:
  // Disallow copy and assign:
  MemoryMirrorCancellationSet(
      const MemoryMirrorCancellationSet &orig) = delete;
  MemoryMirrorCancellationSet& operator=(
      const MemoryMirrorCancellationSet &rhs) = delete;
};

}  // namespace internal
}  // namespace tmb

#endif  // TMB_INTERNAL_MEMORY_MIRROR_CANCELLATION_SET_H_
