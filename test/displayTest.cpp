#include <gtest/gtest.h>
#include <gmock/gmock.h>
//#include <string>
#include "display.h"

using ::testing::A;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

class MockAbstractSerial : public AbstractSerial {
public:
    MockAbstractSerial(int rx, int tx) {};
    MOCK_METHOD1(print, size_t(const char));
    MOCK_METHOD1(print, size_t(const char *));
};

#define TESTUPPERLINE(test_case_name, test_case, display_width, expected, ...) \
TEST(test_case_name, test_case) { \
    MockAbstractSerial serial(1, 2); \
    VFD vfd(display_width, 2, &serial); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq(expected)))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1); \
    vfd.setUpperLine(__VA_ARGS__); \
}

TESTUPPERLINE(vfd, setUpperLine, 20, "hello               ", "hello")
TESTUPPERLINE(vfd, setUpperLineCalledWithLongArgument, 18, "one two three four", "one two three four five six")
TESTUPPERLINE(vfd, setUpperLineEmptyString, 7, "       ", "")
TESTUPPERLINE(vfd, setLowerLine, 20, "hello               ", "hello")
TESTUPPERLINE(vfd, setLowerLineCalledWithLongArgument, 18, "one two three four", "one two three four five six")
TESTUPPERLINE(vfd, setLowerLineEmptyString, 7, "       ", "")
TESTUPPERLINE(vfd, setUpperLineTwoStrings, 20, "temp             45C", "temp", "45C")
TESTUPPERLINE(vfd, setUpperLineTwoStringsTooSmallForRightMostAndSomeOfLeft, 8, "temperat", "temperature", "45C")
TESTUPPERLINE(vfd, setUpperLineTwoStringsTooSmallForRightString, 11, "temperature", "temperature", "45C")
TESTUPPERLINE(vfd, setUpperLineTwoStringsRightStringGetsChopped, 12, "temperature4", "temperature", "45C")
TESTUPPERLINE(vfd, setLowerLineTwoStringsLeftChopOneChar, 10, "temperatur", "temperature", "45C")
