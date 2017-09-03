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

#include "relational_operators/TableExportOperator.hpp"

#include <cstdio>
#include <exception>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "catalog/CatalogAttribute.hpp"
#include "query_execution/QueryContext.hpp"
#include "query_execution/WorkOrderProtosContainer.hpp"
#include "query_execution/WorkOrdersContainer.hpp"
#include "relational_operators/WorkOrder.pb.h"
#include "storage/StorageBlockInfo.hpp"
#include "storage/ValueAccessor.hpp"
#include "threading/SpinMutex.hpp"
#include "types/TypedValue.hpp"
#include "types/containers/Tuple.hpp"
#include "utility/BulkIoConfiguration.hpp"
#include "utility/StringUtil.hpp"

#include "glog/logging.h"

#include "tmb/id_typedefs.h"

namespace quickstep {

TableExportOperator::~TableExportOperator() {
  if (file_ != nullptr && file_ != stdout && file_ != stderr) {
    std::fclose(file_);
  }
  file_ = nullptr;
}

bool TableExportOperator::getAllWorkOrders(
    WorkOrdersContainer *container,
    QueryContext *query_context,
    StorageManager *storage_manager,
    const tmb::client_id scheduler_client_id,
    tmb::MessageBus *bus) {
  const auto add_work_order =
      [&](const block_id input_block_id,  // NOLINT(build/c++11)
          const bool is_first_work_order) -> void {
    std::unique_ptr<std::string> output_buffer = std::make_unique<std::string>();
    container->addNormalWorkOrder(
        new TableExportToStringWorkOrder(query_id_,
                                         input_relation_,
                                         input_block_id,
                                         options_->getFormat(),
                                         is_first_work_order && options_->hasHeader(),
                                         options_->getDelimiter(),
                                         options_->escapeStrings(),
                                         options_->getQuoteCharacter(),
                                         options_->getNullString(),
                                         output_buffer.get(),
                                         op_index_,
                                         scheduler_client_id,
                                         storage_manager,
                                         bus),
        op_index_);

    SpinMutexLock lock(output_buffers_mutex_);
    output_buffers_.emplace(input_block_id, BlockBuffer(output_buffer.release()));
  };

  if (input_relation_is_stored_) {
    if (!started_) {
      for (std::size_t i = 0; i < input_relation_block_ids_.size(); ++i) {
        add_work_order(input_relation_block_ids_[i], i == 0);
      }
      num_workorders_generated_ = input_relation_block_ids_.size();
      started_ = true;
    }
    return true;
  } else {
    while (num_workorders_generated_ < input_relation_block_ids_.size()) {
      add_work_order(input_relation_block_ids_[num_workorders_generated_],
                     num_workorders_generated_ == 0);
      ++num_workorders_generated_;
    }
    return done_feeding_input_relation_;
  }
}

bool TableExportOperator::getAllWorkOrderProtos(
    WorkOrderProtosContainer *container) {
  // TODO(quickstep-team): Implement TextExportOperator for the distributed case.
  LOG(FATAL) << "TableExportOperator::getAllWorkOrderProtos() is not supported";
}

void TableExportOperator::receiveFeedbackMessage(
    const WorkOrder::FeedbackMessage &msg) {
  DCHECK(TableExportOperator::kBlockOutputMessage == msg.type());
  DCHECK(msg.payload_size() == sizeof(block_id));

  if (file_ == nullptr) {
    const std::string lo_file_name = ToLower(file_name_);
    if (lo_file_name == "$stdout") {
      file_ = stdout;
    } else if (lo_file_name == "$stderr") {
      file_ = stderr;
    } else {
      DCHECK(!file_name_.empty());
      DCHECK_EQ('@', file_name_.front());
      file_ = std::fopen(file_name_.substr(1).c_str(), "wb");
      // TODO(quickstep-team): Decent handling of exceptions at query runtime.
      if (file_ == nullptr) {
        throw std::runtime_error("Can not open file " + file_name_ + " for writing");
      }
    }
  }

  // Mark block done.
  const block_id done_block_id = *static_cast<const block_id*>(msg.payload());
  {
    SpinMutexLock lock(output_buffers_mutex_);
    DCHECK(output_buffers_.find(done_block_id) != output_buffers_.end());
    output_buffers_.at(done_block_id).done = true;
  }

  // FIXME(jianqiao): Use work orders to perform the "write to file" operation
  // instead of doing it here inside this thread -- as it may stall the scheduler.
  while (num_blocks_written_ < num_workorders_generated_) {
    // Write block exported strings to file in the same order as the blocks are
    // in \p input_relation_block_ids_.
    block_id next_block_id;
    {
      SpinMutexLock lock(block_ids_mutex_);
      next_block_id = input_relation_block_ids_[num_blocks_written_];
    }
    std::unique_ptr<std::string> output_buffer;
    {
      SpinMutexLock lock(output_buffers_mutex_);
      auto it = output_buffers_.find(next_block_id);
      if (it == output_buffers_.end() || !it->second.done) {
        break;
      }
      output_buffer = std::move(it->second.buffer);
      output_buffers_.erase(it);
    }
    std::fwrite(output_buffer->c_str(), 1, output_buffer->length(), file_);
    ++num_blocks_written_;
  }
}

void TableExportToStringWorkOrder::execute() {
  BlockReference block(
      storage_manager_->getBlock(input_block_id_, input_relation_));
  std::unique_ptr<ValueAccessor> accessor(
      block->getTupleStorageSubBlock().createValueAccessor());

  switch (format_) {
    case BulkIoFormat::kCsv:
      writeToString<&TableExportToStringWorkOrder::quoteCSVField>(
          accessor.get(), output_buffer_);
      break;
    case BulkIoFormat::kText:
      writeToString<&TableExportToStringWorkOrder::escapeTextField>(
          accessor.get(), output_buffer_);
      break;
    default:
      LOG(FATAL) << "Unsupported export format in TableExportWorkOrder::execute()";
  }

  // Send completion message to operator.
  FeedbackMessage msg(TableExportOperator::kBlockOutputMessage,
                      getQueryID(),
                      operator_index_,
                      new block_id(input_block_id_),
                      sizeof(input_block_id_));
  SendFeedbackMessage(
      bus_, ClientIDMap::Instance()->getValue(), scheduler_client_id_, msg);
}

inline std::string TableExportToStringWorkOrder::quoteCSVField(
    std::string &&field) const {  // NOLINT(whitespace/operators)x
  bool need_quote = false;
  for (const char c : field) {
    if (c == field_terminator_ || c == quote_character_ || c == '\n') {
      need_quote = true;
      break;
    }
  }
  if (!need_quote) {
    return std::move(field);
  }

  std::string quoted;
  quoted.push_back(quote_character_);
  for (const char c : field) {
    if (c == quote_character_) {
      quoted.push_back(c);
    }
    quoted.push_back(c);
  }
  quoted.push_back(quote_character_);
  return quoted;
}


inline std::string TableExportToStringWorkOrder::escapeTextField(
    std::string &&field) const {  // NOLINT(whitespace/operators)
  if (escape_strings_ == false || field == "\\N") {
    return std::move(field);
  }
  bool need_escape = false;
  for (const unsigned char c : field) {
    if (c < ' ' || c == '\\' || c == field_terminator_) {
      need_escape = true;
      break;
    }
  }
  if (!need_escape) {
    return std::move(field);
  }

  std::string escaped;
  for (const unsigned char c : field) {
    if (c < 32) {
      switch (c) {
        case '\b':
          // Backspace.
          escaped.append("\\b");
          break;
        case '\f':
          // Form-feed.
          escaped.append("\\f");
          break;
        case '\n':
          // Newline.
          escaped.append("\\n");
          break;
        case '\r':
          // Carriage return.
          escaped.append("\\r");
          break;
        case '\t':
          // Tab.
          escaped.append("\\t");
          break;
        case '\v':
          // Vertical tab
          escaped.append("\\v");
          break;
        default: {
          // Use hexidecimal representation.
          static const std::string digits = "0123456789ABCDEF";
          escaped.append("\\x");
          escaped.push_back(digits.at(c >> 4));
          escaped.push_back(digits.at(c & 0xF));
          break;
        }
      }
    } else {
      if (c == '\\' || c == field_terminator_) {
        escaped.push_back('\\');
      }
      escaped.push_back(c);
    }
  }
  return escaped;
}

template <std::string (TableExportToStringWorkOrder::*transform)(std::string&&) const,  // NOLINT
          typename Container, typename Functor>
inline void TableExportToStringWorkOrder::writeEachToString(const Container &container,
                                                            std::string *output,
                                                            const Functor &functor) const {
  auto it = container.begin();
  if (it != container.end()) {
    std::size_t idx = 0;
    output->append((this->*transform)(functor(*it, idx++)));
    while ((++it) != container.end()) {
      output->push_back(field_terminator_);
      output->append((this->*transform)(functor(*it, idx++)));
    }
  }
}

template <std::string (TableExportToStringWorkOrder::*transform)(std::string&&) const>  // NOLINT
void TableExportToStringWorkOrder::writeToString(ValueAccessor *accessor,
                                                 std::string *output) const {
  std::vector<const Type*> value_types;
  value_types.reserve(input_relation_.size());
  for (const CatalogAttribute &attribute : input_relation_) {
    value_types.emplace_back(&attribute.getType());
  }

  // Write table header to the output buffer.
  if (print_header_) {
    writeEachToString<transform>(
        input_relation_, output,
        [&](const CatalogAttribute &attr,  // NOLINT(build/c++11)
            const std::size_t idx) -> std::string {
      return attr.getDisplayName();
    });
    output->push_back('\n');
  }

  // Write table rows to the output buffer.
  accessor->beginIterationVirtual();
  while (accessor->nextVirtual()) {
    std::unique_ptr<Tuple> tuple(accessor->getTupleVirtual());
    writeEachToString<transform>(
        *tuple, output,
        [&](const TypedValue &value,  // NOLINT(build/c++11)
            const std::size_t idx) -> std::string {
      if (value.isNull()) {
        return null_string_;
      } else {
        return value_types[idx]->printValueToString(value);
      }
    });
    output->push_back('\n');
  }
}

}  // namespace quickstep
