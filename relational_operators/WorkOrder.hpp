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

#ifndef QUICKSTEP_RELATIONAL_OPERATORS_WORK_UNIT_HPP_
#define QUICKSTEP_RELATIONAL_OPERATORS_WORK_UNIT_HPP_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>
#include <vector>

#include "query_execution/QueryExecutionTypedefs.hpp"
#include "utility/Macros.hpp"

#include "glog/logging.h"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_bus.h"
#include "tmb/message_style.h"
#include "tmb/tagged_message.h"

namespace quickstep {

/** \addtogroup RelationalOperators
 *  @{
 */


/**
 * @brief A single unit of work in a query plan, produced by a
 *        RelationalOperator. Where possible, WorkOrders should be of
 *        single-block granularity to maximize the opportunity for parallelism.
 **/
class WorkOrder {
 public:
  /**
   * @brief Type of feedback message sent to relational operator.
   *
   * @note This is a per-operator type that is set and understood only by the
   *       relational operator.
   */
  typedef std::uint16_t FeedbackMessageType;

  /**
   * @brief Header struct used in feedback message.
   *
   * @note This is a per-operator type that is set and understood only by the
   *       relational operator.
   */
  struct FeedbackMessageHeader {
    std::size_t rel_op_index;
    std::size_t payload_size;
    FeedbackMessageType payload_type;

    /**
     * @brief Header constructor.
     * @param relational_op_index Index of the relation operator.
     * @param payload_size Size of the payload of the message.
     * @param payload_type Type of payload.
     */
    FeedbackMessageHeader(const std::size_t relational_op_index,
                          const std::size_t payload_size,
                          const FeedbackMessageType payload_type)
        : rel_op_index(relational_op_index),
          payload_size(payload_size),
          payload_type(payload_type) {}
  };

  /**
   * @brief A generic tagged message that can be sent from work order to
   *        relational operator.
   **/
  class FeedbackMessage {
   public:
    /**
     * @brief Feedback message constructor.
     *
     * @param type Type of the message.
     * @param rel_op_index Relational operator index.
     * @param payload Blob of payload.
     * @param payload_size Size of the payload blob.
     * @param ownership Whether to take ownership of the payload blob.
     */
    FeedbackMessage(const FeedbackMessageType type,
                    const std::size_t rel_op_index,
                    void *payload,
                    const std::size_t payload_size,
                    const bool ownership = true)
        : header_(rel_op_index, payload_size, type),
          payload_(payload),
          ownership_(ownership) {}

    /**
     * @brief Deserializing feedback message constructor.
     *
     * @param serialized_bytes Serialized byte stream of feedback message.
     * @param num_bytes Number of bytes in stream.
     * @param copy_payload Copy payload and take ownership.
     */
    FeedbackMessage(void *serialized_bytes,
                    const std::size_t num_bytes,
                    const bool copy_payload = false)
        : header_(*static_cast<const FeedbackMessageHeader *>(serialized_bytes)),
          ownership_(copy_payload) {
      CHECK_EQ(num_bytes, sizeof(header_) + header_.payload_size);
      if (copy_payload) {
        payload_ = static_cast<char *>(std::malloc(header_.payload_size));
        std::memcpy(payload_,
                    static_cast<const char *>(serialized_bytes) + sizeof(header_),
                    header_.payload_size);
      } else {
        payload_ = static_cast<char *>(serialized_bytes) + sizeof(header_);
      }
    }

    /**
     * @brief Move constructor.
     *
     * @param orig Message to be moved.
     */
    FeedbackMessage(FeedbackMessage &&orig)
        : header_(orig.header_),
          payload_(orig.payload_),
          ownership_(orig.ownership_) {
      orig.header_.payload_size = 0;
      orig.payload_ = nullptr;
    }

    /**
     * @brief Move assignment.
     *
     * @param orig Message to be moved.
     */
    FeedbackMessage& operator=(FeedbackMessage &&orig) {
      if (this != &orig) {
        if (ownership_) {
          std::free(payload_);
        }

        header_ = orig.header_;
        payload_ = orig.payload_;
        ownership_ = orig.ownership_;
        orig.payload_ = nullptr;
        orig.header_.payload_size = 0;
      }
      return *this;
    }

    /**
     * @brief Destructor.
     */
    virtual ~FeedbackMessage() {
      if (ownership_) {
        std::free(payload_);
      }
    }

    /**
     * @brief Serialize the feedback message into a byte stream.
     *
     * @return A pair containing the message stream and the message size.
     *
     * @note The caller is responsible for freeing the byte stream.
     */
    std::pair<void*, std::size_t> serializeMessage() const {
      char *msg = static_cast<char *>(
          std::malloc(sizeof(header_) + header_.payload_size));
      CHECK(nullptr != msg) << "Unable to allocate byte stream.";
      std::memcpy(msg, &header_, sizeof(header_));
      std::memcpy(msg + sizeof(header_), payload_, header_.payload_size);
      return std::make_pair(msg, sizeof(header_) + header_.payload_size);
    }

    /**
     * @brief Message type accessor.
     */
    FeedbackMessageType type() const { return header_.payload_type; }

    /**
     * @brief Header accessor.
     */
    const FeedbackMessageHeader& header() const { return header_; }

    /**
     * @brief Payload accessor.
     */
    const void* payload() const { return payload_; }

    /**
     * @brief Payload size accessor.
     */
    std::size_t payload_size() const { return header_.payload_size; }

    /**
     * @brief Ownership.
     */
    bool ownership() const { return ownership_; }

    /**
     * @brief Drop ownership.
     *
     * @note The entity calling this should take responsibility of freeing the
     * payload.
     */
    void dropOwnership() {
      ownership_ = false;
    }

   private:
    FeedbackMessageHeader header_;
    void* payload_;
    bool ownership_;

    DISALLOW_COPY_AND_ASSIGN(FeedbackMessage);
  };

  /**
   * @brief Virtual destructor.
   **/
  virtual ~WorkOrder() {}

  /**
   * @brief Run this WorkOrder in the calling thread.
   **/
  virtual void execute() = 0;

  /**
   * @brief Get the preferred NUMA node(s) where this WorkOrder should be
   *        executed.
   *
   * @return A vector of preferred NUMA nodes. An empty vector indicates that
   *         the WorkOrder can be executed on any NUMA node.
   **/
  const std::vector<int>& getPreferredNUMANodes() const {
    return preferred_numa_nodes_;
  }

  /**
   * @brief Send message to relational operator.
   *
   * @param bus A pointer to the TMB.
   * @param sender_id The client ID of the sender.
   * @param receiver_id The client ID of the receiver.
   * @param feedback_msg Feedback message to be sent to relational operator.
   **/
  static void SendFeedbackMessage(tmb::MessageBus *bus,
                                  tmb::client_id sender_id,
                                  tmb::client_id receiver_id,
                                  const FeedbackMessage &feedback_msg) {
    std::pair<void *, std::size_t> stream = feedback_msg.serializeMessage();

    tmb::TaggedMessage msg;
    msg.acquire_message(stream.first, stream.second, kWorkOrderFeedbackMessage);

    tmb::Address receiver_address;
    receiver_address.AddRecipient(receiver_id);

    tmb::MessageStyle single_receiver_style;

    DCHECK(bus != nullptr);
    const tmb::MessageBus::SendStatus send_status =
        bus->Send(sender_id,
                  receiver_address,
                  single_receiver_style,
                  std::move(msg));
    CHECK(send_status == tmb::MessageBus::SendStatus::kOK) << "Message could"
        " not be sent from thread with TMB client ID " << sender_id << " to"
        " receiver thread with TMB client ID " << receiver_id;
  }

 protected:
  WorkOrder() {}

  // A vector of preferred NUMA node IDs where this workorder should be executed.
  // These node IDs typically indicate the NUMA node IDs of the input(s) of the
  // workorder. Derived classes should ensure that there are no duplicate entries
  // in this vector.
  std::vector<int> preferred_numa_nodes_;

 private:
  DISALLOW_COPY_AND_ASSIGN(WorkOrder);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_RELATIONAL_OPERATORS_WORK_UNIT_HPP_
