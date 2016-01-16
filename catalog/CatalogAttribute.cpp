/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "catalog/CatalogAttribute.hpp"

#include <cstring>
#include <string>

#include "catalog/Catalog.pb.h"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeFactory.hpp"

#include "glog/logging.h"

using std::strcmp;
using std::string;

namespace quickstep {

bool CatalogAttribute::ProtoIsValid(const serialization::CatalogAttribute &proto) {
  // Check that proto is fully initialized and the type is a valid type.
  return proto.IsInitialized() && TypeFactory::ProtoIsValid(proto.type());
}

CatalogAttribute::CatalogAttribute(const serialization::CatalogAttribute &proto)
    : parent_(nullptr),
      id_(-1),
      name_(proto.name()),
      type_(&(TypeFactory::ReconstructFromProto(proto.type()))) {
  DCHECK(ProtoIsValid(proto))
      << "Attempted to create CatalogAttribute from an invalid proto description:\n"
      << proto.DebugString();

  if (proto.has_display_name()) {
    display_name_ = proto.display_name();
  }
}

serialization::CatalogAttribute CatalogAttribute::getProto() const {
  serialization::CatalogAttribute proto;

  proto.set_name(name_);
  proto.mutable_type()->CopyFrom(type_->getProto());

  if (!display_name_.empty()) {
    proto.set_display_name(display_name_);
  }

  return proto;
}

}  // namespace quickstep
