
//
//  log.cpp
//  red_belt
//
//  Created by Liubov Sereda on 30/01/2019.
//  Copyright © 2019 Liubov Sereda. All rights reserved.
//

#include "test_runner.h"
#include <sstream>
#include <string>
using namespace std;

class Logger {
public:
    explicit Logger(ostream& output_stream) : os(output_stream) {
    }

    void SetLogLine(bool value) { log_line = value; }
    void SetLogFile(bool value) { log_file= value; }

    bool GetLogLine() {
        return log_line;
    }

    bool GetLogFile() {
        return log_file;
    }

    void Log(const string& message) {
        os << message;
    }

private:
    ostream& os;
    bool log_line = false;
    bool log_file = false;
};

#define LOG(logger, message) {                               \
  ostringstream os;                                          \
  if (logger.GetLogFile()) {                                 \
    os << __FILE__;                                          \
    if (logger.GetLogLine()) os << ":" << __LINE__;          \
  } else if (logger.GetLogLine()) os << "Line " << __LINE__; \
  if (!os.str().empty()) os << " ";                          \
  os << message << "\n";                                     \
  logger.Log(os.str());                                      \
}

/*
void TestLog() {

#line 1 "logger.cpp"

    ostringstream logs;
    Logger l(logs);
    LOG(l, "hello");

    l.SetLogFile(true);
    LOG(l, "hello");

    l.SetLogLine(true);
    LOG(l, "hello");

    l.SetLogFile(false);
    LOG(l, "hello");

    string expected = "hello\n";
    expected += "logger.cpp hello\n";
    expected += "logger.cpp:10 hello\n";
    expected += "Line 13 hello\n";
    ASSERT_EQUAL(logs.str(), expected);
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestLog);

}
*/
