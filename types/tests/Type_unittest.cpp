/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <cstddef>

#include "types/CharType.hpp"
#include "types/Type.hpp"
#include "types/Type.pb.h"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "types/VarCharType.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest.h"

using std::size_t;

namespace quickstep {

void CheckTypeSerialization(const Type &type) {
  serialization::Type proto = type.getProto();
  switch (type.getTypeID()) {
    case kInt:
      EXPECT_EQ(serialization::Type::INT, proto.type_id());
      break;
    case kLong:
      EXPECT_EQ(serialization::Type::LONG, proto.type_id());
      break;
    case kFloat:
      EXPECT_EQ(serialization::Type::FLOAT, proto.type_id());
      break;
    case kDouble:
      EXPECT_EQ(serialization::Type::DOUBLE, proto.type_id());
      break;
    case kDatetime:
      EXPECT_EQ(serialization::Type::DATETIME, proto.type_id());
      break;
    case kDatetimeInterval:
      EXPECT_EQ(serialization::Type::DATETIME_INTERVAL, proto.type_id());
      break;
    case kYearMonthInterval:
      EXPECT_EQ(serialization::Type::YEAR_MONTH_INTERVAL, proto.type_id());
      break;
    case kChar:
      EXPECT_EQ(serialization::Type::CHAR, proto.type_id());
      EXPECT_TRUE(proto.HasExtension(serialization::CharType::length));
      EXPECT_EQ((static_cast<const CharType&>(type)).getStringLength(),
                proto.GetExtension(serialization::CharType::length));
      break;
    case kVarChar:
      EXPECT_EQ(serialization::Type::VAR_CHAR, proto.type_id());
      EXPECT_TRUE(proto.HasExtension(serialization::VarCharType::length));
      EXPECT_EQ((static_cast<const VarCharType&>(type)).getStringLength(),
                proto.GetExtension(serialization::VarCharType::length));
      break;
    default:
      FATAL_ERROR("type is not a valid Type in CheckTypeSerialization");
  }

  EXPECT_EQ(type.isNullable(), proto.nullable());
  EXPECT_TRUE(type.equals(TypeFactory::ReconstructFromProto(proto)));
}

TEST(TypeTest, TypeSerializationTest) {
  CheckTypeSerialization(TypeFactory::GetType(kInt));
  CheckTypeSerialization(TypeFactory::GetType(kInt, true));

  CheckTypeSerialization(TypeFactory::GetType(kLong));
  CheckTypeSerialization(TypeFactory::GetType(kLong, true));

  CheckTypeSerialization(TypeFactory::GetType(kFloat));
  CheckTypeSerialization(TypeFactory::GetType(kFloat, true));

  CheckTypeSerialization(TypeFactory::GetType(kDouble));
  CheckTypeSerialization(TypeFactory::GetType(kDouble, true));

  CheckTypeSerialization(TypeFactory::GetType(kDatetime));
  CheckTypeSerialization(TypeFactory::GetType(kDatetime, true));

  CheckTypeSerialization(TypeFactory::GetType(kDatetimeInterval));
  CheckTypeSerialization(TypeFactory::GetType(kDatetimeInterval, true));

  CheckTypeSerialization(TypeFactory::GetType(kYearMonthInterval));
  CheckTypeSerialization(TypeFactory::GetType(kYearMonthInterval, true));

  size_t length = 0;
  CheckTypeSerialization(TypeFactory::GetType(kChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kChar, length, true));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length, true));

  length = 8;
  CheckTypeSerialization(TypeFactory::GetType(kChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kChar, length, true));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length, true));

  length = 1024;
  CheckTypeSerialization(TypeFactory::GetType(kChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kChar, length, true));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length, true));

  length = 65536;
  CheckTypeSerialization(TypeFactory::GetType(kChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kChar, length, true));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length));
  CheckTypeSerialization(TypeFactory::GetType(kVarChar, length, true));
}

}  // namespace quickstep
