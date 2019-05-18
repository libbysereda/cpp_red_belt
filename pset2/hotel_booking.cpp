#include <iostream>
#include <cstdint>
#include <vector>
#include <queue>
#include <map>

#include "test_runner.h"
#include "profile.h"

using namespace std;

struct Event {
  uint64_t time;
  string hotel_name;
  unsigned int client_id;
  int room_count;
};

class BookingManager {
private:
  queue<Event> events;
  map<string, uint64_t> clients;
  map<string, uint64_t> rooms;

  void Adjust(uint64_t current_time) {
    while (!events.empty()
           && events.front().time < current_time - 86400)
    {
      --clients[events.front().hotel_name];
      rooms[events.front().hotel_name] -= events.front().room_count;
      events.pop();
    }
  }

public:
  BookingManager() {}

  void Book(uint64_t time, const string& hotel_name,
            unsigned int client_id, int room_count)
  {
    events.push({time, hotel_name, client_id, room_count});
    ++clients[hotel_name];
    rooms[hotel_name] += room_count;

    cout << "New booking: " << time << " " << hotel_name << " " << client_id << " " << room_count << endl;
    cout << "Booking before adjusting: " << endl;
    while (!events.empty()){
      cout << events.front().time << " " << events.front().hotel_name << " " << events.front().client_id << " "
           << events.front().room_count << endl;
    }
    Adjust(time);

    cout << "Booking after adjusting: " << endl;
    while (!events.empty()){
      cout << events.front().time << " " << events.front().hotel_name << " " << events.front().client_id << " "
           << events.front().room_count << endl;
    }
  }

  uint64_t GetClients(const string& hotel_name) {
    return clients[hotel_name];
  }

  uint64_t GetRooms(const string& hotel_name) {
    return rooms[hotel_name];
  }

};

struct TestQuery {
  string query;
  uint64_t time;
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

void stressTest() {
  vector<TestQuery> queries;
  for (int i = 0, j = 0; i < 100000; ++i) {
    if (j % 100 == 0) {
      ++j;
    }
    queries.push_back({"BOOK", static_cast<uint64_t>(i), "Marriott", static_cast<unsigned int>(i), j});
    queries.push_back({"BOOK", static_cast<uint64_t>(i), "FourSeasons", static_cast<unsigned int>(i), j});
  }

  BookingManager manager;
  {
    LOG_DURATION("Book");
    for (const auto& q : queries) {
      manager.Book(q.time, q.hotel_name, q.client_id, q.room_count);
    }
  }

  vector<TestQuery> queries1;
  for (int i = 0; i < 50000; ++i) {
    queries1.push_back({"CLIENTS", 0, "Marriott", 0, 0});
    queries1.push_back({"CLIENTS", 0, "FourSeasons", 0, 0});
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
  TestRunner tr;
  RUN_TEST(tr, testBookingManager);
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
