#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
//#include <SoftwareSerial.h>
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

TEST(vfd, setUpperLine) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(20, 2, &serial);
//    ON_CALL(serial, print(A<const char*>())).WillByDefault(Invoke([](const char* cs) { return strlen(cs); }));
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("hello               ")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine("hello");
}

TEST(vfd, setUpperLineCalledWithLongArgument) {
    MockAbstractSerial serial(4, 5);
    VFD vfd(18, 2, &serial);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("one two three four")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine("one two three four five six");
}

TEST(vfd, setUpperLineEmptyString) {
    MockAbstractSerial serial(3, 4);
    VFD vfd(7, 3, &serial);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("       ")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine("");
}

TEST(vfd, setLowerLine) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(20, 2, &serial);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x42")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("hello               ")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setLowerLine("hello");
}

TEST(vfd, setLowerLineCalledWithLongArgument) {
    MockAbstractSerial serial(4, 5);
    VFD vfd(18, 2, &serial);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x42")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("one two three four")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setLowerLine("one two three four five six");
}

TEST(vfd, setLowerLineEmptyString) {
    MockAbstractSerial serial(3, 4);
    VFD vfd(7, 3, &serial);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x42")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("       ")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setLowerLine("");
}

TEST(vfd, setUpperLineTwoStrings) {
    MockAbstractSerial serial(5, 6);
    VFD vfd(20, 2, &serial);
    std::string temp = "temp", deg = "45C";
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x1B\x51\x41")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("temp             45C")))).Times(1);
    EXPECT_CALL(serial, print(Matcher<const char*>(StrEq("\x0D")))).Times(1);
    vfd.setUpperLine(temp, deg);
}

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
