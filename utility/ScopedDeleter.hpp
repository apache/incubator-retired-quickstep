/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_UTILITY_SCOPED_DELETER_HPP_
#define QUICKSTEP_UTILITY_SCOPED_DELETER_HPP_

#include <utility>
#include <vector>

#include "utility/Macros.hpp"

namespace quickstep {

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief RAII helper object that holds pointers to new-allocated objects of
 *        any class and deletes them when it goes out of scope.
 **/
class ScopedDeleter {
 public:
  /**
   * @brief Constructor.
   **/
  ScopedDeleter() {
  }

  /**
   * @brief Destructor. Deletes any objects held by this ScopedDeleter.
   **/
  ~ScopedDeleter() {
    deleteAll();
  }

  /**
   * @brief Add an object to the pool of objects to be deleted by this
   *        ScopedDeleter.
   *
   * @param object A pointer to an object that will be deleted when this
   *        ScopedDeleter goes out of scope. Must be allocated with new and
   *        should not be deleted by any other code.
   **/
  template <typename T>
  inline void addObject(T *object) {
    objects_.emplace_back(&DeleteObject<T>, object);
  }

  /**
   * @brief Delete all objects held by this ScopedDeleter without destroying
   *        it.
   **/
  void deleteAll() {
    for (std::pair<DeleterFunction, void*> &item : objects_) {
      (*item.first)(item.second);
    }

    objects_.clear();
  }

 private:
  typedef void (*DeleterFunction)(void*);

  template <typename T>
  static void DeleteObject(void *object) {
    delete static_cast<T*>(object);
  }

  std::vector<std::pair<DeleterFunction, void*>> objects_;

  DISALLOW_COPY_AND_ASSIGN(ScopedDeleter);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_SCOPED_DELETER_HPP_
