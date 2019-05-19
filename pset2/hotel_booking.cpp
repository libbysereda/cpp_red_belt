#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <map>

#include "test_runner.h"
#include "profile.h"

using namespace std;

struct Event {
  int64_t time;
  string hotel_name;
  unsigned int client_id;
  int room_count;
};

class BookingManager {
private:
  queue<Event> events;
  map<string, map<uint64_t, uint64_t>> clients;
  map<string, uint64_t> rooms;

  void Adjust(int64_t current_time) {
    while (!events.empty()
           && events.front().time + 86400 <= current_time)
    {
      auto it = events.front();
      clients[it.hotel_name][it.client_id] -= it.room_count;
      if (clients[it.hotel_name][it.client_id] == 0) {
        clients[it.hotel_name].erase(it.client_id);
      }

      rooms[it.hotel_name] -= it.room_count;

      events.pop();
    }
  }

public:
  BookingManager() {}

  void Book(int64_t time, const string& hotel_name,
            unsigned int client_id, int room_count)
  {
    events.push({time, hotel_name, client_id, room_count});
    clients[hotel_name][client_id] += room_count;
    rooms[hotel_name] += room_count;

    Adjust(time);
  }

  uint64_t GetClients(const string& hotel_name) {
    return clients[hotel_name].size();
  }

  uint64_t GetRooms(const string& hotel_name) {
    return rooms[hotel_name];
  }

};

struct TestQuery {
  string query;
  int64_t time;
  string hotel_name;
  unsigned int client_id;
  int room_count;
};

void testBookingManager() {
  vector<TestQuery> queries =
  {
    {"CLIENTS", 0, "Marriott", 0, 0},
    {"ROOMS", 0, "Marriott", 0, 0},
    {"BOOK", 10, "FourSeasons", 1, 2},
    {"BOOK", 10, "Marriott", 1, 1},
    {"BOOK", 86409, "FourSeasons", 2, 1},
    {"CLIENTS", 0, "FourSeasons", 0, 0},
    {"ROOMS", 0, "FourSeasons", 0, 0},
    {"CLIENTS", 0, "Marriott", 0, 0},
    {"BOOK", 86410, "Marriott", 2, 10},
    {"ROOMS", 0, "FourSeasons", 0, 0},
    {"ROOMS", 0, "Marriott", 0, 0}
  };

  BookingManager manager;
  vector<uint64_t> result;
  for (const auto& q : queries) {
    if (q.query == "BOOK") {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    } else if (q.query == "CLIENTS") {
      result.push_back(manager.GetClients(q.hotel_name));
    } else {
      result.push_back(manager.GetRooms(q.hotel_name));
    }
  }

  vector<uint64_t> expected = {0, 0, 2, 3, 1, 1, 10};
  ASSERT_EQUAL(result, expected);
}

void testRooms() {
  {
    vector<Event> queries;
    BookingManager manager;

    for (int i = 0; i < 10; ++i) {
      queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 2});
      queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), 2});
    }

    for (const auto& q : queries) {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.GetRooms("Marriott"), 20);
    ASSERT_EQUAL(manager.GetRooms("FourSeasons"), 20);
  }
  {
    vector<Event> queries;
    BookingManager manager;

    for (int i = 0; i < 86500; ++i) {
      queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 1});
      queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), 1});
    }

    for (const auto& q : queries) {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.GetRooms("Marriott"), 86400);
    ASSERT_EQUAL(manager.GetRooms("FourSeasons"), 86400);
  }
}

void testClients() {
  {
    vector<Event> queries;
    BookingManager manager;

    for (int i = 0; i < 10; ++i) {
      queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 2});
      queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), 2});
    }

    for (const auto& q : queries) {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.GetClients("Marriott"), 1);
    ASSERT_EQUAL(manager.GetClients("FourSeasons"), 10);
  }
  {
    vector<Event> queries;
    BookingManager manager;

    for (int i = 0; i < 86500; ++i) {
       queries.push_back({static_cast<int64_t>(i), "Marriott", 1, 1});
       queries.push_back({static_cast<int64_t>(i), "FourSeasons", 1, 1});
    }

    for (const auto& q : queries) {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    }

    ASSERT_EQUAL(manager.GetClients("Marriott"), 1);
    ASSERT_EQUAL(manager.GetClients("FourSeasons"), 1);
  }

}

void stressTest() {
  vector<Event> queries;
  for (int i = 0, j = 0; i < 100000; ++i) {
    if (j % 100 == 0) {
      ++j;
    }
    queries.push_back({static_cast<int64_t>(i), "Marriott", static_cast<unsigned int>(i), j});
    queries.push_back({static_cast<int64_t>(i), "FourSeasons", static_cast<unsigned int>(i), j});
  }

  BookingManager manager;
  {
    LOG_DURATION("Book");
    for (const auto& q : queries) {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    }
  }

  vector<Event> queries1;
  for (int i = 0; i < 50000; ++i) {
    queries1.push_back({0, "Marriott", 0, 0});
    queries1.push_back({0, "FourSeasons", 0, 0});
  }

  {
    LOG_DURATION("GetClients");
    for (const auto& q : queries1) {
      manager.GetClients(q.hotel_name);
    }
  }

  vector<TestQuery> queries2;
  for (int i = 0; i < 50000; ++i) {
    queries2.push_back({"ROOMS", 0, "Marriott", 0, 0});
    queries2.push_back({"ROOMS", 0, "FourSeasons", 0, 0});
  }

  {
    LOG_DURATION("GetRooms");
    for (const auto& q : queries2) {
      manager.GetRooms(q.hotel_name);
    }
  }
}

int main() {
  //TestRunner tr;
  //RUN_TEST(tr, testBookingManager);
  //RUN_TEST(tr, testRooms);
  //RUN_TEST(tr, testClients);
  //stressTest();

  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  BookingManager manager;
  int query_count;
  cin >> query_count;

  for (size_t i = 0; i < query_count; ++i) {
    string query_type;
    cin >> query_type;

    string hotel_name;
    if (query_type == "BOOK") {
      uint64_t time;
      unsigned int client_id;
      int room_count;

      cin >> time >> hotel_name
          >> client_id >> room_count;

      manager.Book(time, hotel_name, client_id, room_count);

    } else {
      cin >> hotel_name;
      if (query_type == "CLIENTS") {
        cout << manager.GetClients(hotel_name) << '\n';
      } else if (query_type == "ROOMS") {
        cout << manager.GetRooms(hotel_name) << '\n';
      }
    }
  }

  return 0;
}
