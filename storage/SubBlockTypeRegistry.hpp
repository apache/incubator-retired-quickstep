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

#ifndef QUICKSTEP_STORAGE_SUB_BLOCK_TYPE_REGISTRY_HPP_
#define QUICKSTEP_STORAGE_SUB_BLOCK_TYPE_REGISTRY_HPP_

#include <cstddef>
#include <type_traits>
#include <unordered_map>

#include "storage/StorageBlockLayout.pb.h"
#include "utility/Macros.hpp"

namespace quickstep {

class CatalogRelationSchema;

/** \addtogroup Expressions
 *  @{
 */

/**
 * @brief Singleton registry object that allows different implementations of
 *        TupleStorageSubBlock and IndexSubBlock to register their
 *        layout-verification and size-estimation functions for use by
 *        StorageBlockLayout.
 *
 * Some macros are provided to make using the SubBlockTypeRegistry easier. To
 * register an implementation of TupleStorageSubBlock, do the following:
 *     1. Include "storage/SubBlockTypeRegistry.hpp" in the implementation
 *        (.cpp) file, and put an invocation of the
 *        QUICKSTEP_REGISTER_TUPLE_STORE() macro in the quickstep namespace.
 *        For example:
 *        QUICKSTEP_REGISTER_TUPLE_STORE(PackedRowStoreTupleStorageSubBlock, PACKED_ROW_STORE);
 *        The first argument to the macro is the name of the class, the second
 *        is the name of the class' corresponding case in the
 *        TupleStorageSubBlockDescription::TupleStorageSubBlockType enum.
 *     2. Include "storage/SubBlockTypeRegistryMacros.hpp" in the header (.hpp)
 *        file, and put an invocation of the
 *        QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED() macro in the quickstep
 *        namespace like so:
 *        QUICKSTEP_DECLARE_SUB_BLOCK_TYPE_REGISTERED(PackedRowStoreTupleStorageSubBlock);
 * 
 * Registration of IndexSubBlock implementations works the same way, except the
 * first macro used should be QUICKSTEP_REGISTER_INDEX() instead of
 * QUICKSTEP_REGISTER_TUPLE_STORE().
 *
 * SubBlockTypeRegistryMacros.hpp additionally provides a macro
 * QUICKSTEP_FORCE_SUB_BLOCK_REGISTRATION(block_type) that can be used by
 * executable targets to force linking and registration of particular SubBlock
 * types in executables that never actually instantiate the block types in
 * question. This is mainly useful for tests that need to verify the
 * correctness of a StorageBlockLayout, but never actually create any
 * StorageBlocks. See Catalog_unittest for an example.
 **/
class SubBlockTypeRegistry {
 private:
  typedef typename std::underlying_type<TupleStorageSubBlockDescription::TupleStorageSubBlockType>::type
      TupleStoreTypeIntegral;

  typedef typename std::underlying_type<IndexSubBlockDescription::IndexSubBlockType>::type
      IndexTypeIntegral;

 public:
  typedef bool (*TupleStoreDescriptionIsValidFunction)(
      const CatalogRelationSchema&,
      const TupleStorageSubBlockDescription&);

  typedef std::size_t (*TupleStoreEstimateBytesPerTupleFunction)(
      const CatalogRelationSchema&,
      const TupleStorageSubBlockDescription&);

  typedef bool (*IndexDescriptionIsValidFunction)(
      const CatalogRelationSchema&,
      const IndexSubBlockDescription&);

  typedef std::size_t (*IndexEstimateBytesPerTupleFunction)(
      const CatalogRelationSchema&,
      const IndexSubBlockDescription&);

  static bool RegisterTupleStoreDescriptionIsValidFunction(
      const TupleStorageSubBlockDescription::TupleStorageSubBlockType sub_block_type,
      TupleStoreDescriptionIsValidFunction function) {
    Instance()->tuple_store_description_is_valid_functions_[
        static_cast<TupleStoreTypeIntegral>(sub_block_type)] = function;
    return true;
  }

  static bool RegisterTupleStoreEstimateBytesPerTupleFunction(
      const TupleStorageSubBlockDescription::TupleStorageSubBlockType sub_block_type,
      TupleStoreEstimateBytesPerTupleFunction function) {
    Instance()->tuple_store_estimate_bytes_per_tuple_functions_[
        static_cast<TupleStoreTypeIntegral>(sub_block_type)] = function;
    return true;
  }

  static bool RegisterIndexDescriptionIsValidFunction(
      const IndexSubBlockDescription::IndexSubBlockType sub_block_type,
      IndexDescriptionIsValidFunction function) {
    Instance()->index_description_is_valid_functions_[
        static_cast<IndexTypeIntegral>(sub_block_type)] = function;
    return true;
  }

  static bool RegisterIndexEstimateBytesPerTupleFunction(
      const IndexSubBlockDescription::IndexSubBlockType sub_block_type,
      IndexEstimateBytesPerTupleFunction function) {
    Instance()->index_estimate_bytes_per_tuple_functions_[
        static_cast<IndexTypeIntegral>(sub_block_type)] = function;
    return true;
  }

  static bool LayoutDescriptionIsValid(
      const CatalogRelationSchema &relation,
      const StorageBlockLayoutDescription &description);

  static std::size_t EstimateBytesPerTupleForTupleStore(
      const CatalogRelationSchema &relation,
      const TupleStorageSubBlockDescription &description);

  static std::size_t EstimateBytesPerTupleForIndex(
      const CatalogRelationSchema &relation,
      const IndexSubBlockDescription &description);

 private:
  std::unordered_map<TupleStoreTypeIntegral,
                     TupleStoreDescriptionIsValidFunction>
      tuple_store_description_is_valid_functions_;

  std::unordered_map<TupleStoreTypeIntegral,
                     TupleStoreEstimateBytesPerTupleFunction>
      tuple_store_estimate_bytes_per_tuple_functions_;

  std::unordered_map<IndexTypeIntegral,
                     IndexDescriptionIsValidFunction>
      index_description_is_valid_functions_;

  std::unordered_map<IndexTypeIntegral,
                     IndexEstimateBytesPerTupleFunction>
      index_estimate_bytes_per_tuple_functions_;

  SubBlockTypeRegistry() {
  }

  static SubBlockTypeRegistry* Instance() {
    static SubBlockTypeRegistry instance;
    return &instance;
  }

  DISALLOW_COPY_AND_ASSIGN(SubBlockTypeRegistry);
};

/** @} */

}  // namespace quickstep

#define QUICKSTEP_REGISTER_TUPLE_STORE(classname, proto_enum_case)                         \
namespace registry {                                                                       \
  static const bool classname##_desc_registered                                            \
      = quickstep::SubBlockTypeRegistry::RegisterTupleStoreDescriptionIsValidFunction(     \
          TupleStorageSubBlockDescription::proto_enum_case,                                \
          &classname::DescriptionIsValid);                                                 \
  static const bool classname##_size_registered                                            \
      = quickstep::SubBlockTypeRegistry::RegisterTupleStoreEstimateBytesPerTupleFunction(  \
          TupleStorageSubBlockDescription::proto_enum_case,                                \
          &classname::EstimateBytesPerTuple);                                              \
  const bool classname##_registered                                                        \
      = classname##_desc_registered && classname##_size_registered;                        \
  bool classname##_check_registered() { return classname##_registered; }                   \
}  /* namespace registry */                                                                \
using registry::classname##_registered


#define QUICKSTEP_REGISTER_INDEX(classname, proto_enum_case)                          \
namespace registry {                                                                  \
  static const bool classname##_desc_registered                                       \
      = quickstep::SubBlockTypeRegistry::RegisterIndexDescriptionIsValidFunction(     \
          IndexSubBlockDescription::proto_enum_case,                                  \
          &classname::DescriptionIsValid);                                            \
  static const bool classname##_size_registered                                       \
      = quickstep::SubBlockTypeRegistry::RegisterIndexEstimateBytesPerTupleFunction(  \
          IndexSubBlockDescription::proto_enum_case,                                  \
          &classname::EstimateBytesPerTuple);                                         \
  const bool classname##_registered                                                   \
      = classname##_desc_registered && classname##_size_registered;                   \
  bool classname##_check_registered() { return classname##_registered; }              \
}  /* namespace registry */                                                           \
using registry::classname##_registered

#endif  // QUICKSTEP_STORAGE_SUB_BLOCK_TYPE_REGISTRY_HPP_
