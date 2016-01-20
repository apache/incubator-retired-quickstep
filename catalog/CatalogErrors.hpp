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
