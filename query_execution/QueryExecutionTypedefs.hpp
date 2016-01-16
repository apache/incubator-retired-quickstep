/**
 * This file copyright (c) 2011-2015, the Quickstep authors.
 * Copyright (c) 2015, Pivotal Software, Inc.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#ifndef QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_TYPEDEFS_HPP_
#define QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_TYPEDEFS_HPP_

#include "threading/ThreadIDBasedMap.hpp"

#include "tmb/address.h"
#include "tmb/id_typedefs.h"
#include "tmb/message_style.h"
#include "tmb/pure_memory_message_bus.h"
#include "tmb/tagged_message.h"

namespace quickstep {

/** \addtogroup QueryExecution
 *  @{
 */

typedef tmb::Address Address;
typedef tmb::AnnotatedMessage AnnotatedMessage;
typedef tmb::MessageBus MessageBus;
typedef tmb::MessageStyle MessageStyle;
typedef tmb::Priority Priority;
typedef tmb::PureMemoryMessageBus<false> MessageBusImpl;
typedef tmb::TaggedMessage TaggedMessage;
typedef tmb::client_id client_id;
typedef tmb::message_type_id message_type_id;

using ClientIDMap = ThreadIDBasedMap<client_id,
                                     'C',
                                     'l',
                                     'i',
                                     'e',
                                     'n',
                                     't',
                                     'I',
                                     'D',
                                     'M',
                                     'a',
                                     'p'>;

enum QueryExecutionMessageType : message_type_id {
  kWorkOrderMessage,
  kRebuildWorkOrderMessage,
  kWorkOrderCompleteMessage,
  kRebuildWorkOrderCompleteMessage,
  kDataPipelineMessage,
  kWorkOrdersAvailableMessage,
  kPoisonMessage,
  kWorkOrderFeedbackMessage,
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_QUERY_EXECUTION_QUERY_EXECUTION_TYPEDEFS_HPP_
