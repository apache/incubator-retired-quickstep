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

#include "utility/TemplateUtil.hpp"

#include <memory>
#include <string>
#include <sstream>
#include <tuple>
#include <utility>

#include "utility/Macros.hpp"

#include "gtest/gtest.h"

namespace quickstep {

class SomeArgType {
 public:
  explicit SomeArgType(const std::string &value)
      : value_(value) {
  }

  SomeArgType(SomeArgType &&arg)
      : value_(std::move(arg.value_)) {
  }

  std::string toString() const {
    return value_;
  }

 private:
  const std::string value_;

  DISALLOW_COPY_AND_ASSIGN(SomeArgType);
};

class BaseClass {
 public:
  virtual std::string toString() const = 0;
};

template <bool c1, bool c2, bool c3, bool c4, bool c5, bool c6>
class SomeClass : public BaseClass {
 public:
  SomeClass(const int a1, SomeArgType &&a2)
      : a1_(a1), a2_(std::forward<SomeArgType>(a2)) {
  }

  std::string toString() const override {
    std::ostringstream oss;
    oss << "{ ";
    if (c1) {
      oss << "c1 ";
    }
    if (c2) {
      oss << "c2 ";
    }
    if (c3) {
      oss << "c3 ";
    }
    if (c4) {
      oss << "c4 ";
    }
    if (c5) {
      oss << "c5 ";
    }
    if (c6) {
      oss << "c6 ";
    }
    oss << "} " << a1_ << " " << a2_.toString();
    return oss.str();
  }

 private:
  const int a1_;
  const SomeArgType a2_;
};

void RunTest(const bool c1, const bool c2, const bool c3,
             const bool c4, const bool c5, const bool c6,
             const std::string &expected) {
  // arg should be perfectly forwarded.
  SomeArgType arg("xyz");

  std::unique_ptr<BaseClass> base(
      CreateBoolInstantiatedInstance<SomeClass, BaseClass>(std::forward_as_tuple(10, std::move(arg)),
                                                           c1, c2, c3, c4, c5, c6));
  EXPECT_STREQ(expected.c_str(), base->toString().c_str());
}

TEST(TemplateUtilTest, TemplateUtilTest) {
  RunTest(true, false, true, false, true, false, "{ c1 c3 c5 } 10 xyz");
  RunTest(true, true, true, true, true, true, "{ c1 c2 c3 c4 c5 c6 } 10 xyz");
  RunTest(false, false, true, true, false, false, "{ c3 c4 } 10 xyz");
  RunTest(false, false, false, false, false, false, "{ } 10 xyz");
}

}  // namespace quickstep
