#include "simple_vector.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void TestConstruction() {
  SimpleVector<int> empty;
  ASSERT_EQUAL(empty.Size(), 0u);
  ASSERT_EQUAL(empty.Capacity(), 0u);
  ASSERT(empty.begin() == empty.end());

  SimpleVector<string> five_strings(5);
  ASSERT_EQUAL(five_strings.Size(), 5u);
  ASSERT(five_strings.Size() <= five_strings.Capacity());
  for (auto& item : five_strings) {
    ASSERT(item.empty());
  }
  five_strings[2] = "Hello";
  ASSERT_EQUAL(five_strings[2], "Hello");
}


void TestPushBack() {
  {
    SimpleVector<int> v;
    vector<int> expected;
    for (auto& i : v) {
      expected.push_back(i);
    }

    ASSERT_EQUAL(v.Size(), expected.size());
    ASSERT(equal(begin(v), end(v), begin(expected)));
  }
  {
    SimpleVector<int> v;
    for (int i = 0; i < 1; ++i) {
      v.PushBack(i);
    }

    vector<int> expected;
    for (auto& i : v) {
      expected.push_back(i);
    }

    ASSERT_EQUAL(v.Size(), expected.size());
    ASSERT(equal(begin(v), end(v), begin(expected)));
    ASSERT_EQUAL(v[0], expected[0]);
  }
  {
    SimpleVector<int> v;
    for (int i = 0; i < 5; ++i) {
      v.PushBack(i);
    }

    vector<int> expected_for;
    for (auto& i : v) {
      expected_for.push_back(i);
    }

    vector<int> expected = {0, 1, 2, 3, 4};

    ASSERT_EQUAL(v.Size(), 5u);
    ASSERT_EQUAL(expected, expected_for);
  }
  /*
  {
    SimpleVector<int> v(5);
    for (int i = 0; i < v.Size(); ++i) {
      v.PushBack(i);
    }

    vector<int> expected_for;
    for (auto& i : v) {
      expected_for.push_back(i);
    }

    vector<int> expected = {0, 1, 2, 3, 4};

    ASSERT_EQUAL(v.Size(), 5u);
    ASSERT_EQUAL(expected, expected_for);
  } */
  { /*
    SimpleVector<int> v;
    for (int i = 10; i >= 1; --i) {
      v.PushBack(i);
      ASSERT(v.Size() <= v.Capacity());
    }
    sort(begin(v), end(v));

    const vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    ASSERT_EQUAL(v.Size(), expected.size());
    ASSERT(equal(begin(v), end(v), begin(expected)));
    */
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestConstruction);
  RUN_TEST(tr, TestPushBack);
  return 0;
}
