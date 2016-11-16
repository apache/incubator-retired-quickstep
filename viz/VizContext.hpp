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

#ifndef QUICKSTEP_VIZ_VIZ_CONTEXT_HPP_
#define QUICKSTEP_VIZ_VIZ_CONTEXT_HPP_

#include <cstddef>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "utility/Macros.hpp"
#include "viz/VizObject.hpp"

#include "glog/logging.h"

namespace quickstep {
namespace viz {

/** \addtogroup Viz
 *  @{
 */

class VizContext;
typedef std::shared_ptr<const VizContext> VizContextPtr;

class VizContext : public VizObject {
 public:
  VizContext()
      : parent_(nullptr),
        depth_(0) {}

  explicit VizContext(const VizContextPtr &parent)
      : parent_(parent),
        depth_(parent->depth_+1) {}

  std::string getName() const override {
    return "VizContext";
  }

  inline bool hasLocal(const std::string &key) const {
    return objects_.find(key) != objects_.end();
  }

  bool has(const std::string &key) const {
    return hasLocal(key) || (parent_ != nullptr && parent_->has(key));
  }

  inline void set(const std::string &key,
                  VizObject *object) {
    objects_[key].reset(object);
  }

  template <typename VizObjectT>
  inline VizObjectT *getLocal(const std::string &key) {
    CHECK(hasLocal(key));
    VizObjectT *object = dynamic_cast<VizObjectT *>(objects_.at(key).get());

    CHECK(object != nullptr);
    return object;
  }

  template <typename VizObjectT>
  inline const VizObjectT *getLocal(const std::string &key) const {
    CHECK(hasLocal(key));
    const VizObjectT *object =
        dynamic_cast<const VizObjectT *>(objects_.at(key).get());

    CHECK(object != nullptr);
    return object;
  }

  template <typename VizObjectT>
  const VizObjectT *get(const std::string &key) const {
    if (hasLocal(key)) {
      return getLocal<VizObjectT>(key);
    } else {
      CHECK(parent_ != nullptr);
      return parent_->get<VizObjectT>(key);
    }
  }

  template <typename VizObjectT, typename FunctorT>
  void forEach(const std::string &key, FunctorT &functor) const {
    if (parent_ != nullptr) {
      parent_->forEach<VizObjectT, FunctorT>(key, functor);
    }
    if (hasLocal(key)) {
      functor(getLocal<VizObjectT>(key));
    }
  }

 protected:
  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const VizObject *> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const VizObject *>> *container_child_fields) const override;

 private:
  const VizContextPtr parent_;
  const std::size_t depth_;

  std::map<std::string, std::unique_ptr<VizObject>> objects_;

  DISALLOW_COPY_AND_ASSIGN(VizContext);
};

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_VIZ_CONTEXT_HPP_
