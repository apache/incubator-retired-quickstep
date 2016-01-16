/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include <string>

#include "storage/StorageBlockInfo.hpp"
#include "utility/StringUtil.hpp"

using std::string;

namespace quickstep {

string BlockIdUtil::ToString(const block_id block) {
  string block_str("(");
  block_str.append(ToZeroPaddedString(Domain(block), kBlockIdDomainLengthInDigits));
  block_str.append(", ");
  block_str.append(ToZeroPaddedString(Counter(block), kBlockIdCounterLengthInDigits));
  block_str.append(")");

  return block_str;
}

const char *kTupleStorageSubBlockTypeNames[] = {
  "PackedRowStore",
  "BasicColumnStore",
  "CompressedPackedRowStore",
  "CompressedColumnStore",
  "SplitRowStore"
};

const char *kIndexSubBlockTypeNames[] = {
  "CSBTree",
};

}  // namespace quickstep

