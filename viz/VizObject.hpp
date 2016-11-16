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

#ifndef QUICKSTEP_VIZ_VIZ_OBJECT_HPP_
#define QUICKSTEP_VIZ_VIZ_OBJECT_HPP_

#include <string>
#include <vector>

#include "catalog/CatalogTypedefs.hpp"
#include "utility/Macros.hpp"
#include "utility/TreeStringSerializable.hpp"

namespace quickstep {
namespace viz {

/** \addtogroup Viz
 *  @{
 */

class VizObject : public TreeStringSerializable<const VizObject *> {
 public:
  virtual ~VizObject() {}

 protected:
  VizObject() {}

  void getFieldStringItems(
      std::vector<std::string> *inline_field_names,
      std::vector<std::string> *inline_field_values,
      std::vector<std::string> *non_container_child_field_names,
      std::vector<const VizObject *> *non_container_child_fields,
      std::vector<std::string> *container_child_field_names,
      std::vector<std::vector<const VizObject *>> *container_child_fields) const override;

 private:
  DISALLOW_COPY_AND_ASSIGN(VizObject);
};

// ----------------------------------------------------------------------------
// Light-weight VizObject subclasses.

class AttributeIdVector : public VizObject {
 public:
  AttributeIdVector(const std::vector<attribute_id> &attribute_ids)
      : attribute_ids_(attribute_ids) {}

  std::string getName() const override {
    return "AttributeIdVector";
  }

  inline const std::vector<attribute_id>& getAttributeIds() const {
    return attribute_ids_;
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
  const std::vector<attribute_id> attribute_ids_;

  DISALLOW_COPY_AND_ASSIGN(AttributeIdVector);
};

class StringValue : public VizObject {
 public:
  StringValue(const std::string &value)
      : value_(value) {}

  std::string getName() const override {
    return "StringValue";
  }

  inline const std::string& value() const {
    return value_;
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
  const std::string value_;

  DISALLOW_COPY_AND_ASSIGN(StringValue);
};

/** @} */

}  // namespace viz
}  // namespace quickstep

#endif  // QUICKSTEP_VIZ_VIZ_OBJECT_HPP_
