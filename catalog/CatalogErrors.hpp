/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_CATALOG_CATALOG_ERRORS_HPP_
#define QUICKSTEP_CATALOG_CATALOG_ERRORS_HPP_

#include <exception>
#include <string>

namespace quickstep {

/** \addtogroup Catalog
 *  @{
 */

class CatalogIDOverflow : public std::exception {
 public:
  explicit CatalogIDOverflow(const std::string &item_type)
      : message_("CatalogIDOverflow: Exceeded the maximum number of ") {
    message_.append(item_type);
    message_.append("s in catalog.");
  }

  ~CatalogIDOverflow() throw() {}

  virtual const char* what() const throw() {
    return message_.c_str();
  }

 private:
  std::string message_;
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_CATALOG_CATALOG_ERRORS_HPP_
