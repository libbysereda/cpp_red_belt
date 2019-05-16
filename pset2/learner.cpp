#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <set>

#include "test_runner.h"
#include "profile.h"

using namespace std;

class Learner {
private:
  set<string> dict;

public:

  int Learn(const vector<string>& words) {
    int counter = dict.size();
    for (const string & w : words) {
      dict.insert(w);
    }
    return dict.size() - counter;
  }

  vector<string> KnownWords() {
    return {dict.begin(), dict.end()};
  }
};

void getWords(vector<string>& words) {
  ifstream infile("words_to_test.txt");
  string line;
  while (getline(infile, line))
  {
    istringstream iss(line);
    string word;
    while (iss >> word) {
      words.push_back(word);
    }
  }
}

void testLearn() {
  Learner learner;

  vector<string> words_to_learn = {
    "lake", "new", "world hello", "hello"
  };

  vector<int> words_counter;
  string line;

  for (const auto& l : words_to_learn) {
    vector<string> learned_words;
    stringstream ss(l);
    string word;
    while (ss >> word) {
      learned_words.push_back(word);
    }
    words_counter.push_back(learner.Learn(learned_words));
  }

  vector<int> expected = {1, 1, 2, 0};
  ASSERT_EQUAL(words_counter, expected);
}

void testKnownWords() {
  Learner learner;

  vector<string> words_to_learn = {
    "lake", "new", "world hello", "hello"
  };

  vector<int> words_counter;
  string line;

  for (const auto& l : words_to_learn) {
    vector<string> learned_words;
    stringstream ss(l);
    string word;
    while (ss >> word) {
      learned_words.push_back(word);
    }
    words_counter.push_back(learner.Learn(learned_words));
  }

  vector<string> expected = {"hello", "lake", "new", "world"};
  ASSERT_EQUAL(learner.KnownWords(), expected);
}

void stressTest() {
  vector<string> words_to_learn;
  getWords(words_to_learn);

  Learner learner;
  vector<int> words_counter;
  string line;

  {
    LOG_DURATION("Learn");
    for (const auto& l : words_to_learn) {
      vector<string> learned_words;
      stringstream ss(l);
      string word;
      while (ss >> word) {
        learned_words.push_back(word);
      }
      words_counter.push_back(learner.Learn(learned_words));
    }
  }
  {
    LOG_DURATION("KnownWords");
    vector<string> v = learner.KnownWords();
  }
}

int main() {
  TestRunner tr;
  stressTest();
  RUN_TEST(tr, testLearn);
  RUN_TEST(tr, testKnownWords);
}
