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

class MockSoftwareSerial : public SoftwareSerial {
public:
    MockSoftwareSerial(int rx, int tx) : SoftwareSerial(rx, tx) {};
    MOCK_METHOD1(print, size_t(const char *));
};

TEST(vfd, setUpperLine) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(20, 2, &serial);
//    ON_CALL(serial, print(A<const char*>())).WillByDefault(Invoke([](const char* cs) { return strlen(cs); }));
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("hello               "))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine("hello");
}

TEST(vfd, setUpperLineCalledWithLongArgument) {
    MockSoftwareSerial serial(4, 5);
    VFD vfd(18, 2, &serial);
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("one two three four"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine("one two three four five six");
}

TEST(vfd, setUpperLineEmptyString) {
    MockSoftwareSerial serial(3, 4);
    VFD vfd(7, 3, &serial);
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("       "))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine("");
}

TEST(vfd, setLowerLine) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(20, 2, &serial);
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x42"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("hello               "))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setLowerLine("hello");
}

TEST(vfd, setLowerLineCalledWithLongArgument) {
    MockSoftwareSerial serial(4, 5);
    VFD vfd(18, 2, &serial);
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x42"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("one two three four"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setLowerLine("one two three four five six");
}

TEST(vfd, setLowerLineEmptyString) {
    MockSoftwareSerial serial(3, 4);
    VFD vfd(7, 3, &serial);
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x42"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("       "))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setLowerLine("");
}

TEST(vfd, setUpperLineTwoStrings) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(20, 2, &serial);
    std::string temp = "temp", deg = "45C";
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("temp             45C"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setUpperLineTwoStringsTooSmallForRightMostAndSomeOfLeft) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(8, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("temperat"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setUpperLineTwoStringsTooSmallForRightString) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(11, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("temperature"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setUpperLineTwoStringsRightStringGetsChopped) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(12, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x41"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("temperature4"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setUpperLine(temp, deg);
}

TEST(vfd, setLowerLineTwoStringsLeftChopOneChar) {
    MockSoftwareSerial serial(5, 6);
    VFD vfd(10, 2, &serial);
    std::string temp = "temperature", deg = "45C";
    EXPECT_CALL(serial, print(StrEq("\x1B\x51\x42"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("temperatur"))).Times(1);
    EXPECT_CALL(serial, print(StrEq("\x0D"))).Times(1);
    vfd.setLowerLine(temp, deg);
}
