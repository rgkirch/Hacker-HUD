#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "myClient.hpp"
#include "myConcreteConnection.hpp"

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
    MOCK_METHOD2(connect, int(uint16_t port, const char *host));
    MOCK_METHOD1(print, size_t(const char[]));
    MOCK_METHOD0(connected, uint8_t());
    MOCK_METHOD0(read, int());
    MOCK_METHOD0(stop, void());
};

TEST(downloadData, basic) {
    int port = 80;
    const char *host = "api.coindesk.com";
    const char *path = "v1/bpi/currentprice.json";
    MockMyClient client; // connection happens through client
    MyConcreteConnection connection(&client, host, path); // connection prints text to client and reads from client
    std::string str = makeGetRequest(host, path); // the text that connection will print to client
//    std::string str = "hi";
    std::string json = "one two three {four five six}";
    EXPECT_CALL(client, print(Matcher<const char*>(StrEq(str.c_str())))).Times(1); // the client's print should be called with str
    ON_CALL(client, read()).WillByDefault(Return(InvokeWithoutArgs([&json]()->int { // when the connection calls the clients read, it should
        static auto it = json.begin();t 
        return (int)(*(it++)); // return the text in json
    })));
//    ON_CALL(client, read()).WillByDefault(Return(8));
    std::string data = connection.read();
}

TEST(sanity, one) {
    ASSERT_EQ(1,1);
}