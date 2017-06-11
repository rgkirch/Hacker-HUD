#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "makeGetRequest.hpp"

using ::testing::A;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

TEST(a, b) {
    std::string str = makeGetRequest("api.coindesk.com", "v1/bpi/currentprice.json");
    EXPECT_STREQ(str.c_str(), "GET /v1/bpi/currentprice.json HTTP/1.1\r\nHost: api.coindesk.com\r\nConnection: close\r\n\r\n");
}
