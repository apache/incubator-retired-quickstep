/**
 *   Copyright 2011-2015 Quickstep Technologies LLC.
 *   Copyright 2015-2016 Pivotal Software, Inc.
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

#include <algorithm>
#include <cstddef>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "catalog/Catalog.hpp"
#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogDatabase.hpp"
#include "catalog/CatalogRelation.hpp"
#include "catalog/CatalogTypedefs.hpp"
#include "storage/StorageBlockInfo.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "storage/SubBlockTypeRegistryMacros.hpp"
#include "types/Type.hpp"
#include "types/TypeFactory.hpp"
#include "types/TypeID.hpp"
#include "utility/Macros.hpp"

#include "gtest/gtest.h"

namespace quickstep {

QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(BasicColumnStoreTupleStorageSubBlock);
QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(CSBTreeIndexSubBlock);
QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(CompressedColumnStoreTupleStorageSubBlock);
QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(CompressedPackedRowStoreTupleStorageSubBlock);
QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(PackedRowStoreTupleStorageSubBlock);
QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(SMAIndexSubBlock);
QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(SplitRowStoreTupleStorageSubBlock);

class CatalogTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    db_ = cat_.getDatabaseByIdMutable(cat_.addDatabase(new CatalogDatabase(nullptr, "db")));
  }

  CatalogRelation* createCatalogRelation(const std::string &name, bool temporary = false) {
    return db_->getRelationByIdMutable(db_->addRelation(new CatalogRelation(nullptr, name, -1, temporary)));
  }

  static void CompareCatalogAttribute(const CatalogAttribute &expected, const CatalogAttribute &checked) {
    EXPECT_EQ(expected.getID(), checked.getID());
    EXPECT_EQ(expected.getName(), checked.getName());
    EXPECT_TRUE(expected.getType().equals(checked.getType()));
    EXPECT_EQ(expected.getDisplayName(), checked.getDisplayName());
  }

  static void CompareTupleStorageSubBlockDescription(
      const TupleStorageSubBlockDescription &expected,
      const TupleStorageSubBlockDescription &checked) {
    EXPECT_EQ(expected.sub_block_type(), checked.sub_block_type());

    switch (expected.sub_block_type()) {
      case TupleStorageSubBlockDescription::PACKED_ROW_STORE:
        break;
      case TupleStorageSubBlockDescription::BASIC_COLUMN_STORE:
        EXPECT_TRUE(expected.HasExtension(BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
        EXPECT_TRUE(checked.HasExtension(BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
        EXPECT_EQ(
            expected.GetExtension(BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id),
            checked.GetExtension(BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
        break;
      case TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE:
        ASSERT_EQ(
            expected.ExtensionSize(CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id),
            checked.ExtensionSize(CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id));

        for (int i = 0;
             i < expected.ExtensionSize(
                     CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id);
             ++i) {
          EXPECT_EQ(
              expected.GetExtension(
                  CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, i),
              checked.GetExtension(
                  CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, i));
        }
        break;
      case TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE:
        EXPECT_TRUE(expected.HasExtension(CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
        EXPECT_TRUE(checked.HasExtension(CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));
        EXPECT_EQ(
            expected.GetExtension(CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id),
            checked.GetExtension(CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id));

        ASSERT_EQ(
            expected.ExtensionSize(CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id),
            checked.ExtensionSize(CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id));

        for (int i = 0;
             i < expected.ExtensionSize(CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id);
             ++i) {
          EXPECT_EQ(
              expected.GetExtension(CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id, i),
              checked.GetExtension(CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id, i));
        }
        break;
      case TupleStorageSubBlockDescription::SPLIT_ROW_STORE:
        break;
      default:
        FATAL_ERROR("Unknown TupleStorageSubBlockType encountered in CompareTupleStorageSubBlockDescription");
    }
  }

  static void CompareIndexSubBlockDescription(
      const IndexSubBlockDescription &expected,
      const IndexSubBlockDescription &checked) {
    EXPECT_EQ(expected.sub_block_type(), checked.sub_block_type());

    ASSERT_EQ(expected.indexed_attribute_ids_size(), checked.indexed_attribute_ids_size());
    for (int i = 0; i < expected.indexed_attribute_ids_size(); ++i) {
      EXPECT_EQ(expected.indexed_attribute_ids(i), checked.indexed_attribute_ids(i));
    }
  }

  static void CompareStorageBlockLayoutDescription(
      const StorageBlockLayoutDescription &expected,
      const StorageBlockLayoutDescription &checked) {
    EXPECT_EQ(expected.num_slots(), checked.num_slots());
    CompareTupleStorageSubBlockDescription(expected.tuple_store_description(), checked.tuple_store_description());

    ASSERT_EQ(expected.index_description_size(), checked.index_description_size());
    for (int i = 0; i < expected.index_description_size(); ++i) {
      CompareIndexSubBlockDescription(expected.index_description(i), checked.index_description(i));
    }
  }

  static void CompareCatalogRelation(const CatalogRelation &expected, const CatalogRelation &checked) {
    EXPECT_EQ(expected.getID(), checked.getID());
    EXPECT_EQ(expected.getName(), checked.getName());
    EXPECT_EQ(expected.isTemporary(), checked.isTemporary());

    CompareStorageBlockLayoutDescription(
        expected.getDefaultStorageBlockLayout().getDescription(),
        checked.getDefaultStorageBlockLayout().getDescription());

    EXPECT_EQ(expected.blocks_, checked.blocks_);

    ASSERT_EQ(expected.size(), checked.size());
    for (CatalogRelation::const_iterator it1 = expected.begin(), it2 = checked.begin();
         it1 != expected.end();
         ++it1, ++it2) {
      CompareCatalogAttribute(*it1, *it2);
    }

    ASSERT_EQ((expected.index_scheme_ != nullptr), (checked.index_scheme_ != nullptr));

    if (expected.index_scheme_ != nullptr && checked.index_scheme_ != nullptr) {
      ASSERT_EQ(expected.index_scheme_->getNumIndices(), checked.index_scheme_->getNumIndices());

      std::string expected_index_scheme_serialized_proto, checked_index_scheme_serialized_proto;
      expected.index_scheme_->getProto().SerializeToString(&expected_index_scheme_serialized_proto);
      checked.index_scheme_->getProto().SerializeToString(&checked_index_scheme_serialized_proto);
      std::sort(expected_index_scheme_serialized_proto.begin(), expected_index_scheme_serialized_proto.end());
      std::sort(checked_index_scheme_serialized_proto.begin(), checked_index_scheme_serialized_proto.end());
      EXPECT_EQ(expected_index_scheme_serialized_proto, checked_index_scheme_serialized_proto);
    }
  }

  static void CompareCatalogDatabase(const CatalogDatabase &expected, const CatalogDatabase &checked) {
    EXPECT_EQ(expected.getID(), checked.getID());
    EXPECT_EQ(expected.getName(), checked.getName());
    ASSERT_EQ(expected.size(), checked.size());
    for (CatalogDatabase::const_iterator it1 = expected.begin(), it2 = checked.begin();
         it1 != expected.end();
         ++it1, ++it2) {
      CompareCatalogRelation(*it1, *it2);
    }
  }

  void checkCatalogSerialization() {
    Catalog cat_from_proto(cat_.getProto());
    ASSERT_EQ(cat_.size(), cat_from_proto.size());
    for (Catalog::const_iterator it = cat_.begin(), it_from_proto = cat_from_proto.begin();
         it != cat_.end();
         ++it, ++it_from_proto) {
      CompareCatalogDatabase(*it, *it_from_proto);
    }
  }

  Catalog cat_;
  // db_ is owned by cat_.
  CatalogDatabase *db_;
};

TEST_F(CatalogTest, CatalogAttributeSerializationTest) {
  CatalogRelation* const rel_numeric = createCatalogRelation("rel_numeric");

  rel_numeric->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 14));
  rel_numeric->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 7));

  rel_numeric->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt), -1 /* id */, "int"));
  rel_numeric->addAttribute(
      new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong), -1 /* id */, "long"));
  rel_numeric->addAttribute(
      new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat), -1 /* id */, "float"));
  rel_numeric->addAttribute(
      new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble), -1 /* id */, "double"));

  CatalogRelation* const rel_string = createCatalogRelation("rel_string");

  rel_string->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 19));
  rel_string->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 4));

  StorageBlockLayoutDescription layout_description_string(
      rel_string->getDefaultStorageBlockLayout().getDescription());

  std::size_t str_type_length = 0;
  rel_string->addAttribute(
      new CatalogAttribute(nullptr,
                           "char_length_0",
                           TypeFactory::GetType(kChar, str_type_length),
                           -1 /* id */,
                           "char_0"));
  rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_0", TypeFactory::GetType(kVarChar, str_type_length)));

  str_type_length = 8;
  rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_8", TypeFactory::GetType(kChar, str_type_length)));
  rel_string->addAttribute(
      new CatalogAttribute(nullptr,
                           "var_char_length_8",
                           TypeFactory::GetType(kVarChar, str_type_length),
                           -1 /* id */,
                           "vchar_8"));

  str_type_length = 1024;
  rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_1024", TypeFactory::GetType(kChar, str_type_length)));
  rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_1024", TypeFactory::GetType(kVarChar, str_type_length)));

  str_type_length = 65536;
  rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_65536", TypeFactory::GetType(kChar, str_type_length)));
  rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_65536", TypeFactory::GetType(kVarChar, str_type_length)));

  // COMPRESSED_PACKED_ROW_STORE supports variable-length attributes.
  TupleStorageSubBlockDescription* tuple_store_description_string =
      layout_description_string.mutable_tuple_store_description();
  tuple_store_description_string->set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
  for (int i = 0; i <= rel_string->getMaxAttributeId(); ++i) {
    tuple_store_description_string->AddExtension(
        CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, i);
  }
  rel_string->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel_string, layout_description_string));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, NullableCatalogAttributeSerializationTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");

  StorageBlockLayoutDescription layout_description(
      rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_int_nullable", TypeFactory::GetType(kInt, true)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_long_nullable", TypeFactory::GetType(kLong, true)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_float_nullable", TypeFactory::GetType(kFloat, true)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_double_nullable", TypeFactory::GetType(kDouble, true)));

  std::size_t str_type_length = 0;
  rel->addAttribute(
      new CatalogAttribute(nullptr, "char_length_0_nullable", TypeFactory::GetType(kChar, str_type_length, true)));
  rel->addAttribute(
      new CatalogAttribute(nullptr,
                           "var_char_length_0_nullable",
                           TypeFactory::GetType(kVarChar, str_type_length, true)));

  str_type_length = 8;
  rel->addAttribute(
      new CatalogAttribute(nullptr, "char_length_8_nullable", TypeFactory::GetType(kChar, str_type_length, true)));
  rel->addAttribute(
      new CatalogAttribute(nullptr,
                           "var_char_length_8_nullable",
                           TypeFactory::GetType(kVarChar, str_type_length, true)));

  str_type_length = 1024;
  rel->addAttribute(
      new CatalogAttribute(nullptr, "char_length_1024_nullable", TypeFactory::GetType(kChar, str_type_length, true)));
  rel->addAttribute(
      new CatalogAttribute(nullptr,
                           "var_char_length_1024_nullable",
                           TypeFactory::GetType(kVarChar, str_type_length, true)));

  str_type_length = 65536;
  rel->addAttribute(
      new CatalogAttribute(nullptr, "char_length_65536_nullable", TypeFactory::GetType(kChar, str_type_length, true)));
  rel->addAttribute(
      new CatalogAttribute(nullptr,
                           "var_char_length_65536_nullable",
                           TypeFactory::GetType(kVarChar, str_type_length, true)));

  TupleStorageSubBlockDescription* tuple_store_description =
      layout_description.mutable_tuple_store_description();
  tuple_store_description->set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
  for (int i = 0; i <= rel->getMaxAttributeId(); ++i) {
    tuple_store_description->AddExtension(
        CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, i);
  }
  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, TemporaryCatalogRelationSerializationTest) {
  CatalogRelation* const tmp_rel_numeric = createCatalogRelation("tmp_rel_numeric", true);

  tmp_rel_numeric->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 14));
  tmp_rel_numeric->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 7));

  tmp_rel_numeric->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));
  tmp_rel_numeric->addAttribute(new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong)));
  tmp_rel_numeric->addAttribute(new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat)));
  tmp_rel_numeric->addAttribute(new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble)));

  CatalogRelation* const tmp_rel_string = createCatalogRelation("tmp_rel_string", true);

  tmp_rel_string->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 19));
  tmp_rel_string->addBlock(BlockIdUtil::GetBlockId(0 /* domain */, 4));

  StorageBlockLayoutDescription layout_description_string(
      tmp_rel_string->getDefaultStorageBlockLayout().getDescription());

  std::size_t str_type_length = 0;
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_0", TypeFactory::GetType(kChar, str_type_length)));
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_0", TypeFactory::GetType(kVarChar, str_type_length)));

  str_type_length = 8;
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_8", TypeFactory::GetType(kChar, str_type_length)));
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_8", TypeFactory::GetType(kVarChar, str_type_length)));

  str_type_length = 1024;
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_1024", TypeFactory::GetType(kChar, str_type_length)));
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_1024", TypeFactory::GetType(kVarChar, str_type_length)));

  str_type_length = 65536;
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "char_length_65536", TypeFactory::GetType(kChar, str_type_length)));
  tmp_rel_string->addAttribute(
      new CatalogAttribute(nullptr, "var_char_length_65536", TypeFactory::GetType(kVarChar, str_type_length)));

  // COMPRESSED_PACKED_ROW_STORE supports variable-length attributes.
  TupleStorageSubBlockDescription* tuple_store_description_string =
      layout_description_string.mutable_tuple_store_description();
  tuple_store_description_string->set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
  for (int i = 0; i <= tmp_rel_string->getMaxAttributeId(); ++i) {
    tuple_store_description_string->AddExtension(
        CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, i);
  }
  tmp_rel_string->setDefaultStorageBlockLayout(new StorageBlockLayout(*tmp_rel_string, layout_description_string));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, DroppedCatalogRelationSerializationTest) {
  for (int i = 0; i < 20; ++i) {
    std::ostringstream oss;
    oss << "rel" << i;
    createCatalogRelation(oss.str());
  }

  db_->dropRelationByName("rel10");

  checkCatalogSerialization();
}

TEST_F(CatalogTest, CatalogPackedRowStoreSerializationTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");
  StorageBlockLayoutDescription layout_description(rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble)));

  const std::size_t str_type_length = 20;
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_char", TypeFactory::GetType(kChar, str_type_length)));
  // NOTE(zuyu): PackedRowStoreTupleStorageSubBlock does NOT support variable-length attributes.

  layout_description.mutable_tuple_store_description()->set_sub_block_type(
      TupleStorageSubBlockDescription::PACKED_ROW_STORE);

  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, CatalogBasicColumnStoreSerializationTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");
  StorageBlockLayoutDescription layout_description(rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble)));

  const std::size_t str_type_length = 120;
  const attribute_id attr_id =
      rel->addAttribute(new CatalogAttribute(nullptr, "attr_char", TypeFactory::GetType(kChar, str_type_length)));
  // NOTE(zuyu): BasicColumnStoreTupleStorageSubBlock does NOT support variable-length attributes.

  TupleStorageSubBlockDescription* tuple_store_description =
      layout_description.mutable_tuple_store_description();
  tuple_store_description->set_sub_block_type(TupleStorageSubBlockDescription::BASIC_COLUMN_STORE);
  tuple_store_description->SetExtension(
      BasicColumnStoreTupleStorageSubBlockDescription::sort_attribute_id, attr_id);

  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, CatalogCompressedPackedRowStoreSerializationTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");
  StorageBlockLayoutDescription layout_description(rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble)));

  const std::size_t str_type_length = 1024;
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_char", TypeFactory::GetType(kChar, str_type_length)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_var_char", TypeFactory::GetType(kVarChar, str_type_length)));

  TupleStorageSubBlockDescription* tuple_store_description =
      layout_description.mutable_tuple_store_description();
  tuple_store_description->set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_PACKED_ROW_STORE);
  for (int i = 0; i <= rel->getMaxAttributeId(); ++i) {
    tuple_store_description->AddExtension(
        CompressedPackedRowStoreTupleStorageSubBlockDescription::compressed_attribute_id, i);
  }

  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, CatalogCompressedColumnStoreSerializationTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");
  StorageBlockLayoutDescription layout_description(rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble)));

  const std::size_t str_type_length = 65536;
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_char", TypeFactory::GetType(kChar, str_type_length)));
  const attribute_id attr_id =
      rel->addAttribute(
          new CatalogAttribute(nullptr, "attr_var_char", TypeFactory::GetType(kVarChar, str_type_length)));

  TupleStorageSubBlockDescription* tuple_store_description =
      layout_description.mutable_tuple_store_description();
  tuple_store_description->set_sub_block_type(
      TupleStorageSubBlockDescription::COMPRESSED_COLUMN_STORE);
  tuple_store_description->SetExtension(
      CompressedColumnStoreTupleStorageSubBlockDescription::sort_attribute_id, attr_id);
  for (int i = 0; i <= rel->getMaxAttributeId(); ++i) {
    tuple_store_description->AddExtension(
        CompressedColumnStoreTupleStorageSubBlockDescription::compressed_attribute_id, i);
  }

  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, CatalogSplitRowStoreSerializationTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");
  StorageBlockLayoutDescription layout_description(rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_int", TypeFactory::GetType(kInt)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_long", TypeFactory::GetType(kLong)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_float", TypeFactory::GetType(kFloat)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_double", TypeFactory::GetType(kDouble)));

  const std::size_t str_type_length = std::numeric_limits<std::size_t>::max();
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_char", TypeFactory::GetType(kChar, str_type_length)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_var_char", TypeFactory::GetType(kVarChar, str_type_length)));

  layout_description.mutable_tuple_store_description()->set_sub_block_type(
      TupleStorageSubBlockDescription::SPLIT_ROW_STORE);

  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  checkCatalogSerialization();
}

TEST_F(CatalogTest, CatalogIndexTest) {
  CatalogRelation* const rel = createCatalogRelation("rel");
  StorageBlockLayoutDescription layout_description(rel->getDefaultStorageBlockLayout().getDescription());

  rel->addAttribute(new CatalogAttribute(nullptr, "attr_idx1", TypeFactory::GetType(kInt)));
  rel->addAttribute(new CatalogAttribute(nullptr, "attr_idx2", TypeFactory::GetType(kInt)));

  layout_description.mutable_tuple_store_description()->set_sub_block_type(
      TupleStorageSubBlockDescription::PACKED_ROW_STORE);

  rel->setDefaultStorageBlockLayout(new StorageBlockLayout(*rel, layout_description));

  std::vector<IndexSubBlockDescription> index_descriptions;
  IndexSubBlockDescription index_description;
  index_description.set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
  index_description.add_indexed_attribute_ids(rel->getAttributeByName("attr_idx1")->getID());
  index_descriptions.emplace_back(index_description);

  EXPECT_TRUE(rel->addIndex("idx1", index_descriptions));
  EXPECT_TRUE(rel->hasIndexWithName("idx1"));
  // Adding an index with duplicate name should return false.
  EXPECT_FALSE(rel->addIndex("idx1", index_descriptions));
  // Adding an index of same type with different name on the same attribute should return false.
  EXPECT_FALSE(rel->addIndex("idx2", index_descriptions));

  index_descriptions.clear();
  index_description.set_sub_block_type(IndexSubBlockDescription::CSB_TREE);
  index_description.add_indexed_attribute_ids(rel->getAttributeByName("attr_idx2")->getID());
  index_descriptions.emplace_back(index_description);

  EXPECT_TRUE(rel->addIndex("idx2", index_descriptions));
  EXPECT_TRUE(rel->hasIndexWithName("idx2"));

  checkCatalogSerialization();
}

}  // namespace quickstep
