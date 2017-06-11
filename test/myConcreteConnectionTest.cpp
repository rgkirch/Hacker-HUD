#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <myClient.hpp>
#include <myConcreteConnection.hpp>

using ::testing::A;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

class MockMyClient: public MyClient {
public:
//    MockAbstractSerial(int rx, int tx) {};
    MOCK_METHOD2(connect, int(const char *host, uint16_t port));
    MOCK_METHOD1(print, size_t(const char[]));
    MOCK_METHOD0(connected, uint8_t());
    MOCK_METHOD0(read, int());
    MOCK_METHOD0(stop, void());
};

TEST(downloadData, basic) {
    int port = 80;
    const char *host = "api.coindesk.com";
    const char *path = "v1/bpi/currentprice.json";
    std::string data = MyConcreteConnection(port, host, path).read();
}

