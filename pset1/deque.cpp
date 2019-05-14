/*
** Write class Deque using two vectors;
*/

#include <iostream>
#include <vector>
#include "test_runner.h"
#include <string>

using namespace std;

template <typename T>
class Deque {
public:
  Deque() {}

  bool Empty() const {
    return front_.empty() && back_.empty();
  }

  size_t Size() const {
    return front_.size() + back_.size();
  }

  T& At(const size_t& index) {
    if (index < front_.size()) {
      return front_[front_.size() - 1 - index];
    }

    if (index - front_.size() < back_.size()) {
      return back_[index - front_.size()];
    }

    throw out_of_range("Index is out of range");
  }

  const T& At(size_t index) const {
    return const_cast<T&>(At(index));
  }

  T& operator[](size_t index) {
    if (index < front_.size()) {
      return front_[front_.size() - 1 - index];
    }
    return back_[index - front_.size()];
  }

  const T& operator[](size_t index) const {
    return const_cast<T&>(At(index));
  }

  T& Front() {
    return front_.empty()
      ? back_.front()
      : front_.back();
  }

  const T& Front() const {
    return const_cast<T&>(Front());
  }

  T& Back() {
    return back_.empty()
      ? front_.front()
      : back_.back();
  }

  const T& Back() const {
    return const_cast<T&>(Back());
  }

  void PushFront(const T& value) {
    front_.push_back(value);
  }

  void PushBack(const T& value) {
    back_.push_back(value);
  }

private:
  vector<T> front_;
  vector<T> back_;
};


// Tests
void testConstructor() {
  Deque<string> d;
  ASSERT_EQUAL(d.Size(), 0);
  d.PushFront("A");
  ASSERT_EQUAL(d.Front(), "A");
}

void testSize() {
  Deque<int> d;
  // test Empty()
  ASSERT(d.Empty());

  // test Size()
  d.PushFront(1);
  d.PushBack(2);
  ASSERT_EQUAL(d.Size(), 2);

  ASSERT(!d.Empty());
}

void testPushBack() {
  // test push_back
  Deque<int> d;
  d.PushBack(1);
  d.PushBack(2);
  d.PushBack(3);
  ASSERT_EQUAL(d.Back(), 3);

  d.PushFront(4);
  ASSERT_EQUAL(d.Back(), 3);
}

void testPushFrontBack() {
  Deque<int> d;
  d.PushFront(1);
  d.PushBack(2);
  d.PushFront(3);
  d.PushBack(4);
  d.PushFront(5);
  d.PushBack(6);

  ASSERT_EQUAL(d.Front(), 5);
  ASSERT_EQUAL(d.Back(), 6);
  ASSERT_EQUAL(d[0], 5);
  ASSERT_EQUAL(d[1], 3);
  ASSERT_EQUAL(d[2], 1);
  ASSERT_EQUAL(d[3], 2);
  ASSERT_EQUAL(d[4], 4);
  ASSERT_EQUAL(d[5], 6);

  Deque<int> d1;
  d1.PushFront(1);
  d1.PushFront(2);
  d1.PushBack(3);
  d1.PushBack(4);
  d1.PushFront(5);
  d1.PushBack(6);
  d1.PushBack(7);
  d1.PushBack(8);
  d1.PushFront(9);
  d1.PushFront(10);
  d1.PushBack(11);

  ASSERT_EQUAL(d1[0], 10);
  ASSERT_EQUAL(d1[1], 9);
  ASSERT_EQUAL(d1[2], 5);
  ASSERT_EQUAL(d1[3], 2);
  ASSERT_EQUAL(d1[4], 1);
  ASSERT_EQUAL(d1[5], 3);
  ASSERT_EQUAL(d1[6], 4);
  ASSERT_EQUAL(d1[7], 6);
  ASSERT_EQUAL(d1[8], 7);
  ASSERT_EQUAL(d1[9], 8);
  ASSERT_EQUAL(d1[10], 11);
}

void testPushBackFront() {
  Deque<int> d;
  d.PushBack(1);
  d.PushFront(2);
  d.PushBack(3);
  d.PushFront(4);
  d.PushBack(5);
  d.PushFront(6);

  ASSERT_EQUAL(d.Front(), 6);
  ASSERT_EQUAL(d.Back(), 5);
  ASSERT_EQUAL(d[0], 6);
  ASSERT_EQUAL(d[1], 4);
  ASSERT_EQUAL(d[2], 2);
  ASSERT_EQUAL(d[3], 1);
  ASSERT_EQUAL(d[4], 3);
  ASSERT_EQUAL(d[5], 5);

}

void testPushFront() {
  // test push_front
  Deque<int> d;
  d.PushFront(1);
  d.PushFront(2);
  d.PushFront(4);
  ASSERT_EQUAL(d.Front(), 4);

  d.PushBack(5);
  ASSERT_EQUAL(d.Front(), 4);
}

void testFront() {
  // test front()
  Deque<int> d;
  d.PushFront(1);
  ASSERT_EQUAL(d.Front(), 1);
  ASSERT_EQUAL(d.Back(), 1);

  d.PushFront(2);
  ASSERT_EQUAL(d.Front(), 2);
  ASSERT_EQUAL(d.Back(), 1);

  d.PushFront(3);
  int& f = d.Front();
  ++f;
  ASSERT_EQUAL(d.Front(), 4);
}

void testBack() {
  // test back
  Deque<int> d;
  d.PushBack(1);
  ASSERT_EQUAL(d.Front(), 1);
  ASSERT_EQUAL(d.Back(), 1);

  d.PushBack(2);
  ASSERT_EQUAL(d.Front(), 1);
  ASSERT_EQUAL(d.Back(), 2);

  d.PushBack(3);
  int& b = d.Back();
  ++b;
  ASSERT_EQUAL(d.Back(), 4);
}

void testEmptyDeque() {
  Deque<int> d;
  cout << d.Front() << endl;
  cout << d.Back() << endl;
}

void testOperator() {
  // test []
  Deque<int> d;
  d.PushBack(1);
  d.PushBack(2);
  ASSERT_EQUAL(d[0], 1);
  ASSERT_EQUAL(d[1], 2);

  // test [] - change value;
  int& i = d[0];
  ++i;
  ASSERT_EQUAL(d[0], 2);

  d.PushFront(3);
  ASSERT_EQUAL(d[0], 3);

  d.PushFront(4);
  ASSERT_EQUAL(d[0], 4);

  ASSERT_EQUAL(d[3], 2);
}

void testAt() {
  // test At()
  Deque<int> d;
  d.PushBack(3);
  d.PushBack(4);

  ASSERT_EQUAL(d.At(0), 3);
  ASSERT_EQUAL(d.At(1), 4);

  // test At() - change value
  int& i = d.At(0);
  ++i;
  ASSERT_EQUAL(d.At(0), 4);

  // check out of range exception
  try {
    d.At(5);
  } catch (const out_of_range& e) {
    ASSERT_EQUAL(e.what(), string("Index is out of range"));
  }
}

void testDeque() {
  testConstructor();
  testSize();
  testPushBack();
  testPushFront();
  testFront();
  testBack();
  testOperator();
  testAt();
  testPushFrontBack();
  testPushBackFront();
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, testDeque);
  return 0;
}
