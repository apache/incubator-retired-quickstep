#include <cstddef>
#include <cstdint>
#include <memory>

#include "catalog/CatalogAttribute.hpp"
#include "catalog/CatalogRelation.hpp"
#include "storage/SplitRowStoreTupleStorageSubBlock.hpp"
#include "storage/StorageBlockLayout.hpp"
#include "storage/StorageBlockLayout.pb.h"
#include "types/TypeFactory.hpp"
#include "utility/ScopedBuffer.hpp"

#include "gtest/gtest.h"

namespace quickstep {

class SplitRowStoreRegression : public ::testing::TestWithParam<std::size_t> {
 protected:
  static const size_t kSubBlockSize = 0x1000000;  // 16 MB.

  virtual void SetUp() {
    relation_.reset(new CatalogRelation(nullptr, "RegressionRelation"));

    std::size_t num_of_attributes = GetParam();
    for (std::size_t i = 0; i < num_of_attributes; ++i) {
      std::string name_of_attribute = "double_attr" + std::to_string(i);
      CatalogAttribute *double_attr = new CatalogAttribute(relation_.get(),
                                                           name_of_attribute,
                                                           TypeFactory::GetType(kDouble, false));
      ASSERT_EQ(i, relation_->addAttribute(double_attr));
    }

    tuple_store_description_.reset(new TupleStorageSubBlockDescription());
    tuple_store_description_->set_sub_block_type(TupleStorageSubBlockDescription::SPLIT_ROW_STORE);


    tuple_store_memory_.reset(kSubBlockSize);
    std::memset(tuple_store_memory_.get(), 0x0, kSubBlockSize);

    tuple_store_.reset(new SplitRowStoreTupleStorageSubBlock(*relation_,
                                                             *tuple_store_description_,
                                                             true,
                                                             tuple_store_memory_.get(),
                                                             kSubBlockSize));

  }

  std::unique_ptr<CatalogRelation> relation_;
  ScopedBuffer tuple_store_memory_;
  std::unique_ptr<TupleStorageSubBlockDescription> tuple_store_description_;
  std::unique_ptr<SplitRowStoreTupleStorageSubBlock> tuple_store_;
};

TEST_P(SplitRowStoreRegression, StrideAccess) {
  std::size_t bytes_for_value = sizeof(double);
  std::size_t step = GetParam();
  std::size_t bytes_for_step = sizeof(double) * step;
  ASSERT_NE(tuple_store_->numTuples(), 0);
  double sum = 0;

  for (std::size_t i = 0; i < 1; i += bytes_for_step) {
    sum += i + bytes_for_value;
  }
}

INSTANTIATE_TEST_CASE_P(SplitRowStoreRegressionStrideTest,
                        SplitRowStoreRegression,
                        ::testing::Values(1, 2, 4, 8, 16, 32, 64, 128, 256),);

}
