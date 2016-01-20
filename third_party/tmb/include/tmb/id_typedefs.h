//   Copyright 2014-2015 Quickstep Technologies LLC.
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

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
