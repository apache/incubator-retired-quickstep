/**
 * This file copyright (c) 2011-2015, Quickstep Technologies LLC.
 * All rights reserved.
 * See file CREDITS.txt for details.
 **/

#include "utility/TreeStringSerializable.hpp"

#include <memory>
#include <string>
#include <vector>

#include "glog/logging.h"
#include "gtest/gtest.h"

namespace quickstep {

class TreeStringSerializableTest : public ::testing::Test {
 protected:
  class TestTree;
  typedef std::shared_ptr<const TestTree> TestTreePtr;

  TreeStringSerializableTest() {
    GenerateInlineFields(5, "root", &root_inline_field_names_, &root_inline_field_values_);
    root_inline_field_names_.push_back("short_name_0");
    root_inline_field_values_.push_back("short_value_0");
    root_inline_field_names_.push_back("short_name_1");
    root_inline_field_values_.push_back("short_value_1");

    GenerateInlineFields(3, "child", &child_inline_field_names_, &child_inline_field_values_);
  }

  class TestTree : public TreeStringSerializable<std::shared_ptr<const TestTree>> {
   public:
    std::string getName() const override {
      return name_;
    }

    void getFieldStringItems(std::vector<std::string> *inline_field_names,
                             std::vector<std::string> *inline_field_values,
                             std::vector<std::string> *non_container_child_field_names,
                             std::vector<TestTreePtr> *non_container_child_fields,
                             std::vector<std::string> *container_child_field_names,
                             std::vector<std::vector<TestTreePtr>> *container_child_fields) const override {
      *inline_field_names = inline_field_names_;
      *inline_field_values = inline_field_strings_;
      *non_container_child_field_names = non_container_child_field_names_;
      *non_container_child_fields = non_container_child_fields_;
      *container_child_field_names = container_child_field_names_;
      *container_child_fields = container_child_fields_;
    }

    static std::shared_ptr<const TestTree> Create(
        const std::string &name, const std::vector<std::string> &inline_field_names,
        const std::vector<std::string> &inline_field_strings,
        const std::vector<std::string> &non_container_child_field_names,
        const std::vector<TestTreePtr> &non_container_child_fields,
        const std::vector<std::string> &container_child_field_names,
        const std::vector<std::vector<TestTreePtr>> &container_child_fields) {
      CHECK_EQ(non_container_child_field_names.size(), non_container_child_fields.size());
      CHECK_EQ(container_child_field_names.size(), container_child_fields.size());

      return std::shared_ptr<const TestTree>(
          new TestTree(name, inline_field_names, inline_field_strings, non_container_child_field_names,
                       non_container_child_fields, container_child_field_names, container_child_fields));
    }

   private:
    TestTree(const std::string &name, const std::vector<std::string> &inline_field_names,
             const std::vector<std::string> &inline_field_strings,
             const std::vector<std::string> &non_container_child_field_names,
             const std::vector<TestTreePtr> &non_container_child_fields,
             const std::vector<std::string> &container_child_field_names,
             const std::vector<std::vector<TestTreePtr>> &container_child_fields)
        : name_(name),
          inline_field_names_(inline_field_names),
          inline_field_strings_(inline_field_strings),
          non_container_child_field_names_(non_container_child_field_names),
          non_container_child_fields_(non_container_child_fields),
          container_child_field_names_(container_child_field_names),
          container_child_fields_(container_child_fields) {
      CHECK_EQ(inline_field_names.size(), inline_field_strings.size());
    }

    const std::string name_;
    const std::vector<std::string> inline_field_names_;
    const std::vector<std::string> inline_field_strings_;
    const std::vector<std::string> non_container_child_field_names_;
    const std::vector<TestTreePtr> non_container_child_fields_;
    const std::vector<std::string> container_child_field_names_;
    const std::vector<std::vector<TestTreePtr>> container_child_fields_;
  };

  // Helper method to generate inline fields.
  static void GenerateInlineFields(int num_fields,
                                   const std::string &prefix,
                                   std::vector<std::string> *field_names,
                                   std::vector<std::string> *field_values) {
    field_names->clear();
    field_values->clear();
    for (int i = 0; i < num_fields; ++i) {
      field_names->push_back(std::string(prefix).append("_child_field_").append(std::to_string(i)));
      field_values->push_back(std::string(prefix).append("_child_field_value_").append(std::to_string(i)));
    }
  }

  std::vector<std::string> root_inline_field_names_;
  std::vector<std::string> root_inline_field_values_;
  std::vector<std::string> child_inline_field_names_;
  std::vector<std::string> child_inline_field_values_;
};

TEST_F(TreeStringSerializableTest, GetShortStringTest) {
  const TestTreePtr test_tree_root = TestTree::Create("TestTree" /* name */,
                                                      root_inline_field_names_ /* inline_field_names */,
                                                      root_inline_field_values_ /* inline_field_strings */,
                                                      {} /* non_container_child_field_names */,
                                                      {} /* non_container_child_fields */,
                                                      {} /* container_child_field_names */,
                                                      {} /* container_child_fields */);

  const std::string expected_string =
      "TestTree[root_child_field_0=root_child_field_value_0,root_child_field_1=root_child_field_value_1,"
      "root_child_field_2=root_child_field_value_2,root_child_field_3=root_child_field_value_3,"
      "root_child_field_4=root_child_field_value_4,short_name_0=short_value_0,short_name_1=short_value_1]";
  EXPECT_EQ(expected_string, test_tree_root->getShortString());
}

TEST_F(TreeStringSerializableTest, ToStringWithoutChildrenTest) {
  const TestTreePtr test_tree_root = TestTree::Create("TestTree" /* name */,
                                                      root_inline_field_names_ /* inline_field_names */,
                                                      root_inline_field_values_ /* inline_field_strings */,
                                                      {} /* non_container_child_field_names */,
                                                      {} /* non_container_child_fields */,
                                                      {} /* container_child_field_names */,
                                                      {} /* container_child_fields */);

  const std::string expected_string =
R"(TestTree[root_child_field_0=root_child_field_value_0,
  root_child_field_1=root_child_field_value_1,
  root_child_field_2=root_child_field_value_2,
  root_child_field_3=root_child_field_value_3,
  root_child_field_4=root_child_field_value_4,short_name_0=short_value_0,
  short_name_1=short_value_1]
)";
  EXPECT_EQ(expected_string, test_tree_root->toString());
}

TEST_F(TreeStringSerializableTest, ToStringWithChildrenTest) {
  const TestTreePtr test_tree_leaf_0 = TestTree::Create("TestTreeLeaf_0" /* name */,
                                                        {} /* inline_field_names */,
                                                        {} /* inline_field_strings */,
                                                        {} /* non_container_child_field_names */,
                                                        {} /* non_container_child_fields */,
                                                        {} /* container_child_field_names */,
                                                        {} /* container_child_fields */);
  const TestTreePtr test_tree_leaf_1 = TestTree::Create("TestTreeLeaf_1" /* name */,
                                                        child_inline_field_names_ /* inline_field_names */,
                                                        child_inline_field_values_ /* inline_field_strings */,
                                                        {} /* non_container_child_field_names */,
                                                        {} /* non_container_child_fields */,
                                                        {} /* container_child_field_names */,
                                                        {} /* container_child_fields */);

  const TestTreePtr test_tree_child_0 = TestTree::Create(
      "TestTreeChild_child0" /* name */,
      child_inline_field_names_ /* inline_field_names */,
      child_inline_field_values_ /* inline_field_strings */,
      {"child_0", "child_1", "child_2"} /* non_container_child_field_names */,
      {test_tree_leaf_0, test_tree_leaf_0, test_tree_leaf_1} /* non_container_child_fields */,
      {} /* container_child_field_names */,
      {} /* container_child_fields */);
  const TestTreePtr test_tree_child_1 = TestTree::Create(
      "TestTreeChild_child1" /* name */,
      {} /* inline_field_names */,
      {} /* inline_field_strings */,
      {"child_0", "child_1", "child_2" }, /* non_container_child_field_names */
      {test_tree_child_0,
       test_tree_leaf_0,
       test_tree_child_0} /* non_container_child_fields */,
      {} /* container_child_field_names */, {} /* container_child_fields */);

  const TestTreePtr test_tree_root =
      TestTree::Create(
          "TestTree" /* name */,
          root_inline_field_names_ /* inline_field_names */,
          root_inline_field_values_ /* inline_field_strings */,
          { "child_0", "child_1", "child_2", "child_3", "child_4" } /* non_container_child_field_names */,
          {test_tree_child_0,
           test_tree_leaf_0,
           test_tree_leaf_1,
           test_tree_child_1,
           test_tree_child_1} /* non_container_child_fields */,
          {"list_0", "list_1"} /* container_child_field_names */,
          {{test_tree_child_0,
            test_tree_leaf_0,
            test_tree_leaf_1,
            test_tree_child_1},
           {test_tree_leaf_0,
            test_tree_leaf_1}} /* container_child_fields */);

  const std::string expected_string =
R"(TestTree[root_child_field_0=root_child_field_value_0,
  root_child_field_1=root_child_field_value_1,
  root_child_field_2=root_child_field_value_2,
  root_child_field_3=root_child_field_value_3,
  root_child_field_4=root_child_field_value_4,short_name_0=short_value_0,
  short_name_1=short_value_1]
+-child_0=TestTreeChild_child0[child_child_field_0=child_child_field_value_0,
| child_child_field_1=child_child_field_value_1,
| child_child_field_2=child_child_field_value_2]
| +-child_0=TestTreeLeaf_0
| +-child_1=TestTreeLeaf_0
| +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
|   child_child_field_1=child_child_field_value_1,
|   child_child_field_2=child_child_field_value_2]
+-child_1=TestTreeLeaf_0
+-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| child_child_field_1=child_child_field_value_1,
| child_child_field_2=child_child_field_value_2]
+-child_3=TestTreeChild_child1
| +-child_0=TestTreeChild_child0[child_child_field_0=child_child_field_value_0,
| | child_child_field_1=child_child_field_value_1,
| | child_child_field_2=child_child_field_value_2]
| | +-child_0=TestTreeLeaf_0
| | +-child_1=TestTreeLeaf_0
| | +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| |   child_child_field_1=child_child_field_value_1,
| |   child_child_field_2=child_child_field_value_2]
| +-child_1=TestTreeLeaf_0
| +-child_2=TestTreeChild_child0[child_child_field_0=child_child_field_value_0,
|   child_child_field_1=child_child_field_value_1,
|   child_child_field_2=child_child_field_value_2]
|   +-child_0=TestTreeLeaf_0
|   +-child_1=TestTreeLeaf_0
|   +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
|     child_child_field_1=child_child_field_value_1,
|     child_child_field_2=child_child_field_value_2]
+-child_4=TestTreeChild_child1
| +-child_0=TestTreeChild_child0[child_child_field_0=child_child_field_value_0,
| | child_child_field_1=child_child_field_value_1,
| | child_child_field_2=child_child_field_value_2]
| | +-child_0=TestTreeLeaf_0
| | +-child_1=TestTreeLeaf_0
| | +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| |   child_child_field_1=child_child_field_value_1,
| |   child_child_field_2=child_child_field_value_2]
| +-child_1=TestTreeLeaf_0
| +-child_2=TestTreeChild_child0[child_child_field_0=child_child_field_value_0,
|   child_child_field_1=child_child_field_value_1,
|   child_child_field_2=child_child_field_value_2]
|   +-child_0=TestTreeLeaf_0
|   +-child_1=TestTreeLeaf_0
|   +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
|     child_child_field_1=child_child_field_value_1,
|     child_child_field_2=child_child_field_value_2]
+-list_0=
| +-TestTreeChild_child0[child_child_field_0=child_child_field_value_0,
| | child_child_field_1=child_child_field_value_1,
| | child_child_field_2=child_child_field_value_2]
| | +-child_0=TestTreeLeaf_0
| | +-child_1=TestTreeLeaf_0
| | +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| |   child_child_field_1=child_child_field_value_1,
| |   child_child_field_2=child_child_field_value_2]
| +-TestTreeLeaf_0
| +-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| | child_child_field_1=child_child_field_value_1,
| | child_child_field_2=child_child_field_value_2]
| +-TestTreeChild_child1
|   +-child_0=TestTreeChild_child0[
|   | child_child_field_0=child_child_field_value_0,
|   | child_child_field_1=child_child_field_value_1,
|   | child_child_field_2=child_child_field_value_2]
|   | +-child_0=TestTreeLeaf_0
|   | +-child_1=TestTreeLeaf_0
|   | +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
|   |   child_child_field_1=child_child_field_value_1,
|   |   child_child_field_2=child_child_field_value_2]
|   +-child_1=TestTreeLeaf_0
|   +-child_2=TestTreeChild_child0[
|     child_child_field_0=child_child_field_value_0,
|     child_child_field_1=child_child_field_value_1,
|     child_child_field_2=child_child_field_value_2]
|     +-child_0=TestTreeLeaf_0
|     +-child_1=TestTreeLeaf_0
|     +-child_2=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
|       child_child_field_1=child_child_field_value_1,
|       child_child_field_2=child_child_field_value_2]
+-list_1=
  +-TestTreeLeaf_0
  +-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
    child_child_field_1=child_child_field_value_1,
    child_child_field_2=child_child_field_value_2]
)";
  EXPECT_EQ(expected_string, test_tree_root->toString());
}

TEST_F(TreeStringSerializableTest, ToStringChildNodesWithEmptyNamesTest) {
  const TestTreePtr test_tree_leaf_0 = TestTree::Create("TestTreeLeaf_0" /* name */,
                                                        {} /* inline_field_names */,
                                                        {} /* inline_field_strings */,
                                                        {} /* non_container_child_field_names */,
                                                        {} /* non_container_child_fields */,
                                                        {} /* container_child_field_names */,
                                                        {} /* container_child_fields */);
  const TestTreePtr test_tree_leaf_1 = TestTree::Create("TestTreeLeaf_1" /* name */,
                                                        child_inline_field_names_ /* inline_field_names */,
                                                        child_inline_field_values_ /* inline_field_strings */,
                                                        {} /* non_container_child_field_names */,
                                                        {} /* non_container_child_fields */,
                                                        {} /* container_child_field_names */,
                                                        {} /* container_child_fields */);
  const TestTreePtr test_tree_root = TestTree::Create(
      "TestTree" /* name */,
      root_inline_field_names_ /* inline_field_names */,
      root_inline_field_values_ /* inline_field_strings */,
      {"", "", "", "child_3", "child_4"} /* non_container_child_field_names */,
      {test_tree_leaf_0, test_tree_leaf_0, test_tree_leaf_1, test_tree_leaf_0,
       test_tree_leaf_1} /* non_container_child_fields */,
      {"list_0", "", "", "list_3"} /* container_child_field_names */,
      {{test_tree_leaf_0, test_tree_leaf_0}, {test_tree_leaf_0, test_tree_leaf_1},
       {test_tree_leaf_1, test_tree_leaf_0}, {test_tree_leaf_1, test_tree_leaf_1} } /* container_child_fields */);
  const std::string expected_string =
R"(TestTree[root_child_field_0=root_child_field_value_0,
  root_child_field_1=root_child_field_value_1,
  root_child_field_2=root_child_field_value_2,
  root_child_field_3=root_child_field_value_3,
  root_child_field_4=root_child_field_value_4,short_name_0=short_value_0,
  short_name_1=short_value_1]
+-TestTreeLeaf_0
+-TestTreeLeaf_0
+-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| child_child_field_1=child_child_field_value_1,
| child_child_field_2=child_child_field_value_2]
+-child_3=TestTreeLeaf_0
+-child_4=TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| child_child_field_1=child_child_field_value_1,
| child_child_field_2=child_child_field_value_2]
+-list_0=
| +-TestTreeLeaf_0
| +-TestTreeLeaf_0
+-TestTreeLeaf_0
+-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| child_child_field_1=child_child_field_value_1,
| child_child_field_2=child_child_field_value_2]
+-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
| child_child_field_1=child_child_field_value_1,
| child_child_field_2=child_child_field_value_2]
+-TestTreeLeaf_0
+-list_3=
  +-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
  | child_child_field_1=child_child_field_value_1,
  | child_child_field_2=child_child_field_value_2]
  +-TestTreeLeaf_1[child_child_field_0=child_child_field_value_0,
    child_child_field_1=child_child_field_value_1,
    child_child_field_2=child_child_field_value_2]
)";
  EXPECT_EQ(expected_string, test_tree_root->toString());
}

}  // namespace quickstep
