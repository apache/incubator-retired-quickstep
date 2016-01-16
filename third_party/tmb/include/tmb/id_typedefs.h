// Copyright (c) 2014-2015, Quickstep Technologies LLC. All Rights Reserved.

#ifndef TMB_ID_TYPEDEFS_H_
#define TMB_ID_TYPEDEFS_H_

#include <cstdint>
#include <limits>

namespace tmb {

/** \addtogroup TMB
 *  @{
 */

/**
 * @brief An ID, unique among all the clients of a particular MessageBus, of a
 *        particular client (an entity capable of sending and/or receiving
 *        messages).
 **/
typedef std::uint32_t client_id;
const client_id kClientIdNone
    = std::numeric_limits<client_id>::max();

/**
 * @brief An ID which uniquely identifies a particular application-specific
 *        message type. Clients can use this to determine how to safely
 *        interpret or cast received messages.
 **/
typedef std::uint32_t message_type_id;
const message_type_id kMessageTypeIdNone
    = std::numeric_limits<message_type_id>::max();

/** @} */

}  // namespace tmb

#endif  // TMB_ID_TYPEDEFS_H_
