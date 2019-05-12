#include "test_runner.h"

#include <ostream>
using namespace std;

// rewrite macro PRINT_VALUES to pass all tests;

// original:
// #define PRINT_VALUES(out, x, y) out << (x) << endl; out << (y) << endl

// rewritten:
#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl

void testMacro() {
  {
    ostringstream output;
    PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "5\nred belt\n");
  }
  {
    ostringstream output;
    if (false)
        PRINT_VALUES(output, 5, "red belt");
    ASSERT_EQUAL(output.str(), "");
  }
  {
    ostringstream output;
    if (false)
        PRINT_VALUES(output, 5, "red belt");
    else
        PRINT_VALUES(output, 5, "yellow belt");

    ASSERT_EQUAL(output.str(), "5\nyellow belt\n");
  }
  {
    ostringstream output;
    for (int i = 0; i < 5; ++i)
        PRINT_VALUES(output, 5, "brown belt");

    ASSERT_EQUAL(output.str(),
                 "5\nbrown belt\n5\nbrown belt\n5\nbrown belt\n5\nbrown belt\n5\nbrown belt\n"
                );
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, testMacro);
}
