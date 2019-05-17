#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <map>

#include "test_runner.h"
#include "profile.h"

using namespace std;

class ReadingManager {
public:
  ReadingManager()
  : users_to_pages(MAX_USER_COUNT_ + 1, 0)
  {};

  void Read(int user_id, int page_count) {
    users_to_pages[user_id] = page_count;
    ++total_users;
    pages_to_users[page_count] += 1;
  }

  double Cheer(int user_id) const {
    const int page = users_to_pages[user_id];
    if (page == 0) {
      return 0;
    }
    if (total_users == 1) {
      return 1;
    }

    auto pos = pages_to_users.lower_bound(page);
    int counter = 0;
    for (auto it = pages_to_users.begin(); it != pos; ++it) {
      counter += it->second;
    }
    return counter * 1.0 / total_users - 1;
  }

private:
  static const int MAX_USER_COUNT_ = 100'000;
  map<int, int> pages_to_users;
  vector<int> users_to_pages;
  int total_users = 0;
};
/*
class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0),
        sorted_users_(),
        user_positions_(MAX_USER_COUNT_ + 1, -1) {}

  void Read(int user_id, int page_count) {
    if (user_page_counts_[user_id] == 0) {
      AddUser(user_id);
    }
    user_page_counts_[user_id] = page_count;
    int& position = user_positions_[user_id];
    while (position > 0 && page_count > user_page_counts_[sorted_users_[position - 1]]) {
      SwapUsers(position, position - 1);
    }
  }

  double Cheer(int user_id) const {
    if (user_page_counts_[user_id] == 0) {
      return 0;
    }
    const int user_count = GetUserCount();
    if (user_count == 1) {
      return 1;
    }
    const int page_count = user_page_counts_[user_id];
    int position = user_positions_[user_id];
    while (position < user_count &&
      user_page_counts_[sorted_users_[position]] == page_count) {
      ++position;
    }
    if (position == user_count) {
        return 0;
    }
    // По умолчанию деление целочисленное, поэтому
    // нужно привести числитель к типу double.
    // Простой способ сделать это — умножить его на 1.0.

  }

private:
  // Статическое поле не принадлежит какому-то конкретному
  // объекту класса. По сути это глобальная переменная,
  // в данном случае константная.
  // Будь она публичной, к ней можно было бы обратиться снаружи
  // следующим образом: ReadingManager::MAX_USER_COUNT.
  //static const int MAX_USER_COUNT_ = 100'000;
  //for stress test
  static const int MAX_USER_COUNT_ = 100'000'000;

  vector<int> user_page_counts_;
  vector<int> sorted_users_;   // отсортированы по убыванию количества страниц
  vector<int> user_positions_; // позиции в векторе sorted_users_

  int GetUserCount() const {
    return sorted_users_.size();
  }
  void AddUser(int user_id) {
    sorted_users_.push_back(user_id);
    user_positions_[user_id] = sorted_users_.size() - 1;
  }
  void SwapUsers(int lhs_position, int rhs_position) {
    const int lhs_id = sorted_users_[lhs_position];
    const int rhs_id = sorted_users_[rhs_position];
    swap(sorted_users_[lhs_position], sorted_users_[rhs_position]);
    swap(user_positions_[lhs_id], user_positions_[rhs_id]);
  }
};
*/
vector<double> getResults(ReadingManager& manager,
                          vector<string>& queries,
                          vector<vector<int>> values)
  {
  vector<double> result;
  for (int i = 0; i < queries.size(); ++i) {
    string query_type = queries[i];
    int user_id = values[i][0];

    if (query_type == "READ") {
      int page_count = values[i][1];
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      result.push_back(manager.Cheer(user_id));
    }
  }

  return result;
}

void testEbookManager() {
  vector<string> queries = {"CHEER", "READ", "CHEER", "READ", "READ",
                            "CHEER", "CHEER", "READ", "CHEER", "READ",
                            "CHEER", "CHEER"};
  vector<vector<int>> values = {{5}, {1, 10}, {1}, {2, 5}, {3, 7}, {2},
                                {3}, {3, 10}, {3}, {3, 11}, {3}, {1}};

  ReadingManager manager;
  vector<double> result = getResults(manager, queries, values);

  vector<double> expected = {0, 1, 0, 0.5, 0.5, 1, 0.5};
  ASSERT_EQUAL(result, expected);
}

void stressTestRead() {
  vector<string> queries;
  vector<vector<int>> values;
  ReadingManager manager;

  for (int i = 1, j = 1; i <= 100000; ++i) {
    if (j % 100 == 0) {
      ++j;
    }
    queries.push_back("READ");
    values.push_back({i});
  }

  {
    LOG_DURATION("Read");
    vector<double> result = getResults(manager, queries, values);
  }
}

void stressTestCheer() {
  vector<string> queries;
  vector<vector<int>> values;
  ReadingManager manager;

  for (int i = 1, j = 1; i <= 1000000; ++i) {
    if (j % 100 == 0) {
      ++j;
    }
    queries.push_back("CHEER");
    values.push_back({i, j});
  }

  {
    LOG_DURATION("Cheer");
    vector<double> result = getResults(manager, queries, values);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, testEbookManager);
  stressTestRead();
  stressTestCheer();

  return 0;
}
