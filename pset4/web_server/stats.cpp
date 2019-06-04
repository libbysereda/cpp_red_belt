#include "stats.h"

using namespace std;

Stats::Stats() {
  methods = {
    {"GET", 0},
    {"PUT", 0},
    {"POST", 0},
    {"DELETE", 0},
    {"UNKNOWN", 0}
  };

  uris = {
    {"/", 0},
    {"/order", 0},
    {"/product", 0},
    {"/basket", 0},
    {"/help", 0},
    {"unknown", 0}
  };
}

void Stats::AddMethod(string_view method) {
  if (methods.find(method) != methods.end()) {
    methods[method]++;
  } else {
    methods[string_view("UNKNOWN")]++;
  }
}

void Stats::AddUri(string_view uri) {
  if (uris.find(uri) != uris.end()) {
    uris[uri]++;
  } else {
    uris[string_view("unknown")]++;
  }
}

const map<string_view, int>& Stats::GetMethodStats() const {
  return methods;
}

const map<string_view, int>& Stats::GetUriStats() const {
  return uris;
}

HttpRequest ParseRequest(string_view line) {
  HttpRequest request;

  size_t pos = 0;
  size_t delimiter = line.find(' ', pos);

  request.method = line.substr(pos, delimiter);

  pos = delimiter + 1;
  delimiter = line.find(' ', pos);

  request.uri = line.substr(pos, delimiter);
  request.protocol = line.substr(++delimiter);
}
