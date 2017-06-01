#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
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

#define TESTUPPERLINEONE(test_case_name, test_case, display_width, input_string, match_string) \
TEST(test_case_name, test_case) { \
    MockAbstractSerial serial(1, 2); \
    VFD vfd(display_width, 2, &serial); \
    std::string str = input_string; \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq(match_string)))).Times(1); \
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1); \
    vfd.setUpperLine(str); \
}

TESTUPPERLINEONE(vfd, setUpperLine, 20, "hello", "hello               ")
TESTUPPERLINEONE(vfd, setUpperLineCalledWithLongArgument, 18, "one two three four five six", "one two three four")
TESTUPPERLINEONE(vfd, setUpperLineEmptyString, 7, "", "       ")
TESTUPPERLINEONE(vfd, setLowerLine, 20, "hello", "hello               ")
TESTUPPERLINEONE(vfd, setLowerLineCalledWithLongArgument, 18, "one two three four five six", "one two three four")
TESTUPPERLINEONE(vfd, setLowerLineEmptyString, 7, "", "       ")

//TESTUPPERLINETWO(vfd, setUpperLineTwoStrings, 20, "temp", "45C", "temp             45C")

TEST(vfd, setUpperLineTwoStringsTooSmallForRightMostAndSomeOfLeft) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(8, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("temperat")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setUpperLineTwoStringsTooSmallForRightString) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(11, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("temperature")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setUpperLineTwoStringsRightStringGetsChopped) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(12, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("temperature4")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setLowerLineTwoStringsLeftChopOneChar) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(10, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x42")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("temperatur")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setLowerLine(temp, deg);
}
