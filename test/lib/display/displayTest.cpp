#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
//#include <SoftwareSerial.h>
#include "display.h"
#include "../../../../../../.platformio/packages/framework-arduinoespressif8266/cores/esp8266/Print.h"
#include "../../../../../../.platformio/packages/framework-arduinoespressif8266/libraries/SoftwareSerial/SoftwareSerial.h"

using ::testing::Return;
using ::testing::Matcher;
using ::testing::A;

class MockSerial : public MySerial {
public:
    MockSerial(int rx, int tx);
    MOCK_METHOD1(write, size_t(uint8_t));
    MOCK_METHOD0(read, int());
    MOCK_METHOD0(available, int());
    MOCK_METHOD0(flush, void());
};

TEST(vfd, new) {
    MySerial *serial = new MockSerial(5, 6);
    VFD *vfd = new VFD(20, 2, serial);
    ON_CALL(*vfd, write(A<char>())).WillByDefault(Return(1));
    EXPECT_CALL(*serial, print("\x1B\x51\x41"));
    EXPECT_CALL(*serial, print("hello               "));
    EXPECT_CALL(*serial, print("\x0D"));
    vfd->setUpperLine("hello");
}

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
