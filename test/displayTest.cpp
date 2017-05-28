#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
//#include <SoftwareSerial.h>
//#include "display.h"

using ::testing::Return;
using ::testing::Matcher;
using ::testing::A;
using ::testing::AtLeast;

//class MockSerial : public MySerial {
//public:
//    MockSerial(int rx, int tx);
//    MOCK_METHOD1(write, size_t(uint8_t));
//    MOCK_METHOD0(read, int());
//    MOCK_METHOD0(available, int());
//    MOCK_METHOD0(flush, void());
//};

class SoftwareSerial
{
public:
    // public methods
    SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false) {};
//    ~SoftwareSerial();
    void begin(long speed) {};
    bool listen() {};
    void end() {};
    bool isListening() {};
    bool stopListening() {};
    bool overflow() {};
    int peek() {};

    virtual size_t write(uint8_t byte) {};
    virtual int read() {};
    virtual int available() {};
    virtual void flush() {};
    operator bool() { return true; }
};

class MockSoftwareSerial : public SoftwareSerial {
public:
    MockSoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic = false) : SoftwareSerial(receivePin, transmitPin, inverse_logic) {};
//    ~MockSoftwareSerial() {};
    MOCK_METHOD1(write, size_t(uint8_t byte));
    MOCK_METHOD0(read, int());
    MOCK_METHOD0(available, int());
    MOCK_METHOD0(flush, void());

};

TEST(mockSoftwareSerial, read) {
    uint8_t rx = 5;
    uint8_t tx = 6;
    MockSoftwareSerial serial(rx, tx);
    EXPECT_CALL(serial, read())
            .Times(AtLeast(1))
            .WillOnce(Return(1));
    ON_CALL(serial, read())
            .WillByDefault(Return(1));
    serial.read();
}

//TEST(vfd, new) {
//    MockSerial *serial = new MockSerial(5, 6);
//    VFD *vfd = new VFD(20, 2, serial);
//    ON_CALL(*vfd, write(A<char>())).WillByDefault(Return(1));
//    EXPECT_CALL(*serial, print("\x1B\x51\x41"));
//    EXPECT_CALL(*serial, print("hello               "));
//    EXPECT_CALL(*serial, print("\x0D"));
//    vfd->setUpperLine("hello");
//}

TEST(sanity, one) {
    int x = 1;
    ASSERT_EQ(x, x);
}

TEST(sanity, two) {
    int x = 1;
    ASSERT_NE(x, x + 1);
}

//TEST(why, one) {
//    MockSerial *serial = new MockSerial(1);
//    HardwareDisplay *display = new HardwareDisplay(20, 2, serial);
////    ON_CALL(*serial, print(A<std::string>()))
////            .WillByDefault(Return(4));
////    EXPECT_CALL(*serial, print(A<std::string>()))
////            .WillOnce(Return("\x0b\x01"))
////    EXPECT_CALL(*serial, print(A<std::string>()))
////            .WillOnce(Return("temp             45C"));
//    EXPECT_CALL(*serial, print("\x0b\x01"));
//    EXPECT_CALL(*serial, print("temp             45C"));
//    display->setUpperLine("temp", "45C");
//}
//
//TEST(my, one) {
//    Print *print = new MyMockSerial(1);
//    HardwareDisplay *display = new HardwareDisplay(20, 2, print);
//    std::string temp = "temp";
//    std::string currentTemp = getTemp();
//    display->setUpperLine(temp, currentTemp);
//}
