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

#ifndef QUICKSTEP_TRANSACTION_PREDICATE_HPP_
#define QUICKSTEP_TRANSACTION_PREDICATE_HPP_

#include <memory>
#include <unordered_set>
#include <vector>

namespace quickstep {
namespace transaction {

class Predicate
{
public:
  enum PredicateType
  {
    Equality,
    Range
  };
  PredicateType type;
  Predicate(){};
  virtual ~Predicate(){};
  virtual bool intersect(const Predicate& predicate) const = 0;
};

}
}

#endif
