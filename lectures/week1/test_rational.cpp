#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include "test_runner.h"

using namespace std;

//correct implementation
class Rational {
public:
  // Вы можете вставлять сюда различные реализации,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный

    Rational() {
        numerator = 0;
        denominator = 1;
    }
    Rational(int new_numerator, int new_denominator) {
        if (new_denominator == 0) {
            throw invalid_argument("Invalid argument");
        }

        const int divisor = gcd(new_numerator, new_denominator);
        numerator = new_numerator / divisor;
        denominator = new_denominator / divisor;

        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
    }

    int Numerator() const {
        return numerator;
    }

    int Denominator() const {
        return numerator;
    }
private:
    int numerator;
    int denominator;

    int gcd(int a, int b) {
        if (b == 0) return a;
        a %= b;
        return gcd(b, a);
    }
};

bool operator ==(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() == rhs.Numerator() && lhs.Denominator() == rhs.Denominator();
}

bool operator !=(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() != rhs.Numerator() || lhs.Denominator() != rhs.Denominator();
}

ostream& operator <<(ostream& os, const Rational& r) {
    os << r.Numerator() << "/" << r.Denominator();
    return os;
}

void TestDefaultConstructor() {
    //test default constructor 0/1
    Rational r;
    Assert(r.Numerator() == 0 && r.Denominator() == 1, "Test default constructor");
}

void TestReductionToLowestTerms() {
    Rational r(6,8);
    Rational expected(3, 4);
    AssertEqual(r, expected, "Test1 reduction to lowest terms");
}

void TestReductionWithNegativeNumerator() {
    Rational r(-24, 48);
    Rational expected(-1, 2);
    Assert(r.Numerator() == -1 && r.Denominator() == 2, "Test3 reduction to lowest terms with negative numerator");
}

void TestReductionWithNegativeDenominator() {
    Rational r(24, -48);
    Assert(r.Numerator() == -1 && r.Denominator() == 2, "Test4 reduction to lowest terms with negative denominator");
}

void TestEqualFractions() {
    Rational r1(2,3);
    Rational r2(8,12);
    Assert(r1.Numerator() == 2 && r2.Denominator() == 3 &&
            r2.Numerator() == 2 && r2.Denominator() == 3, "Test equal fractions");
}

void TestInequalFractions() {
    Rational r1(2,3);
    Rational r2(8,14);
    Assert(r1.Numerator() == 2 && r1.Denominator() == 3 &&
           r2.Numerator() == 4 && r2.Denominator() == 7, "Test inequal fractions");
}

void TestEqualFractionsNegative() {
    Rational r1(-1, 2);
    Rational r2(1, -2);
    Assert(r1.Numerator() == -1 && r1.Denominator() == 2 &&
           r2.Numerator() == -1 && r2.Denominator() == 2,"Test equal fraction with negative sign");
}

void TestInequalFractionsNegPos() {
    Rational r1(-1, 2);
    Rational r2(-1, -2);
    Assert(r1.Numerator() == -1 && r1.Denominator() == 2 &&
           r2.Numerator() == 1 && r2.Denominator() == 2, "Test inequal fractions: one negative one positive");
}

void TestNumeratorZero() {
    Rational r(0, 1);
    Assert(r.Numerator() == 0 && r.Denominator() == 1, "Test numerator == 0");
}

void TestNegativeFractionPosDenominator() {
    Rational r(-1, 2);
    Assert(r.Numerator() == -1 && r.Denominator() == 2, "Test negative numerator, positive denominator");
}

void TestNegativeFractionPosNumerator() {
    Rational r(1, -2);
    Assert(r.Numerator() == -1 && r.Denominator() == 2, "Test positive numerator, negative denominator");
}

void TestPositiveFraction() {
    Rational r(-2, -3);
    Assert(r.Numerator() == 2 && r.Denominator() == 3, "Test negative numerator, negative denominator");
}

void TestZeroNegativeOne() {
    Rational r(0, -1);
    Assert(r.Numerator() == 0 && r.Denominator() == 1, "Test 0/-1");
}

int main() {
  TestRunner runner;
  runner.RunTest(TestDefaultConstructor, "Default constructor");
  runner.RunTest(TestReductionToLowestTerms, "Reduction to lowest terms");
  return 0;
}

/*
  runner.RunTest(TestReductionWithNegativeNumerator, "Reduction negative numerator");
  runner.RunTest(TestReductionWithNegativeDenominator, "Reduction negative denominator");
  runner.RunTest(TestEqualFractions, "Equal fractions");
  runner.RunTest(TestInequalFractions, "Inequal fractions");
  runner.RunTest(TestEqualFractionsNegative, "Equal fractions negative");
  runner.RunTest(TestInequalFractionsNegPos, "Inequal fractions negative and positive");
  runner.RunTest(TestNumeratorZero, "Numerator zero");
  runner.RunTest(TestNegativeFractionPosDenominator, "Negative fraction with positive denominator");
  runner.RunTest(TestNegativeFractionPosNumerator, "Negative fraction with positive numerator");
  runner.RunTest(TestPositiveFraction, "Test positive fractions");
  runner.RunTest(TestZeroNegativeOne, "Test 0/-1");
  */
