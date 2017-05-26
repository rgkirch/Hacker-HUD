#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include "display.h"

using ::testing::Return;
using ::testing::Matcher;
using ::testing::A;

class MockSerial : public Print {
public:
    MockSerial(int p) : Print(p) {}
    MOCK_METHOD0(getPort, int());
    MOCK_METHOD1(print, size_t(std::string));
};

class MyMockSerial : public Print {
public:
//    ~MyMockSerial() {};
    MyMockSerial(int port) : Print(port) {}
    size_t print(std::string str) {
        return 5;
    }
};

std::string getTemp() {
    return "45C";
}

TEST(sanity, one) {
    int x = 1;
    ASSERT_EQ(x, x);
}

TEST(sanity, two) {
    int x = 1;
    ASSERT_NE(x, x + 1);
}

TEST(why, one) {
    MockSerial *serial = new MockSerial(1);
    HardwareDisplay *display = new HardwareDisplay(20, 2, serial);
//    ON_CALL(*serial, print(A<std::string>()))
//            .WillByDefault(Return(4));
//    EXPECT_CALL(*serial, print(A<std::string>()))
//            .WillOnce(Return("\x0b\x01"))
//    EXPECT_CALL(*serial, print(A<std::string>()))
//            .WillOnce(Return("temp             45C"));
    EXPECT_CALL(*serial, print("\x0b\x01"));
    EXPECT_CALL(*serial, print("temp             45C"));
    display->setUpperLine("temp", "45C");
}

TEST(my, one) {
    Print *print = new MyMockSerial(1);
    HardwareDisplay *display = new HardwareDisplay(20, 2, print);
    std::string temp = "temp";
    std::string currentTemp = getTemp();
    display->setUpperLine(temp, currentTemp);
}
