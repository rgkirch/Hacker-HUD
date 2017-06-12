#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <myClient.hpp>
#include <myConcreteConnection.hpp>

using ::testing::A;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::InvokeWithoutArgs;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

class MockMyClient: public MyClient {
public:
//    MockMyClient(uint16_t port) {};
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
    MockMyClient client;
    MyConcreteConnection connection(&client, host, path);
    std::string str = makeGetRequest(host, path);
    std::string json = "one two three {four five six}";
    EXPECT_CALL(client, print(Matcher<const char*>(StrEq(str.c_str())))).Times(1);
    ON_CALL(client, read()).WillByDefault(Return(InvokeWithoutArgs([&json]()->int {
        static auto it = json.begin();
        return (int)(*(it++));
    })));
    std::string data = connection.read();
}

