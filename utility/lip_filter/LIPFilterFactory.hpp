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

#ifndef QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_FACTORY_HPP_
#define QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_FACTORY_HPP_

#include "utility/Macros.hpp"

namespace quickstep {

namespace serialization { class LIPFilter; }

class LIPFilter;

/** \addtogroup Utility
 *  @{
 */

/**
 * @brief All-static factory object that provides access to various implementations
 *        of LIPFilters.
 */
class LIPFilterFactory {
 public:
  /**
   * @brief Reconstruct a LIPFilter from its serialized Protocol Buffer form.
   *
   * @param proto The Protocol Buffer representation of a LIPFilter object,
   * @return A new LIPFilter reconstructed from the supplied Protocol Buffer.
   *         Caller should take ownership of the returned object.
   */
  static LIPFilter* ReconstructFromProto(const serialization::LIPFilter &proto);

  /**
   * @brief Check whether a serialization::LIPFilter is fully-formed and
   *        all parts are valid.
   *
   * @param proto A serialized Protocol Buffer representation of a LIPFilter.
   * @return Whether proto is fully-formed and valid.
   **/
  static bool ProtoIsValid(const serialization::LIPFilter &proto);

 private:
  LIPFilterFactory() {}

  DISALLOW_COPY_AND_ASSIGN(LIPFilterFactory);
};

/** @} */

}  // namespace quickstep

#endif  // QUICKSTEP_UTILITY_LIP_FILTER_LIP_FILTER_FACTORY_HPP_
