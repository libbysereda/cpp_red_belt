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
private:
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGES_ = 1000;

  vector<int> users_to_pages;
  vector<int> pages_to_users;
  vector<double> pages_rating;
  int total_users = 0;

  void updateRating(int page_number) {
    int user_count = 0;
    for (size_t i = 1; i < page_number; ++i) {
      user_count += pages_to_users[i];
    }
    if (total_users > 2) {
        pages_rating[page_number] = user_count * 1.0 / (total_users - 1);
    }
  }

public:
  ReadingManager()
  : users_to_pages(MAX_USER_COUNT_ + 1, 0)
  , pages_to_users(MAX_PAGES_ + 1, 0)
  , pages_rating(MAX_PAGES_ + 1, 0.0)
  {}

  void Read(int user_id, int page_number) {
    if (users_to_pages[user_id] == 0) {
      ++total_users;
    } else {
      --pages_to_users[users_to_pages[user_id]];
    }

    users_to_pages[user_id] = page_number;
    ++pages_to_users[page_number];
    updateRating(page_number);
  }

  double Cheer(int user_id) {
    if (users_to_pages[user_id] == 0) {
      return 0;
    }
    if (total_users == 1) {
      return 1;
    }
    return pages_rating[users_to_pages[user_id]];
  }
};

void testEbookManager() {
  vector<string> queries = {"CHEER", "READ", "CHEER", "READ", "READ",
                            "CHEER", "CHEER", "READ", "CHEER", "READ",
                            "CHEER", "CHEER"};
  vector<vector<int>> values = {{5}, {1, 10}, {1}, {2, 5}, {3, 7}, {2},
                                {3}, {3, 10}, {3}, {3, 11}, {3}, {1}};

  ReadingManager manager;
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
    values.push_back({i, j});
  }

  {
    LOG_DURATION("Read");
    for (int i = 0; i < queries.size(); ++i) {
      string query_type = queries[i];
      int user_id = values[i][0];
      int page_count = values[i][1];
      manager.Read(user_id, page_count);
    }
  }
}

void stressTestCheer() {
  vector<string> queries;
  vector<vector<int>> values;
  ReadingManager manager;

  for (int i = 1; i <= 100000; ++i) {
    queries.push_back("CHEER");
    values.push_back({i});
  }

  {
    LOG_DURATION("Cheer");
    for (int i = 0; i < queries.size(); ++i) {
      string query_type = queries[i];
      int user_id = values[i][0];
      manager.Cheer(user_id);
    }
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, testEbookManager);
  stressTestRead();
  stressTestCheer();

  return 0;
}
