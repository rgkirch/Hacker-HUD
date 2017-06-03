#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "scrapeWeb.hpp"

using ::testing::A;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::Matcher;
using ::testing::Return;
using ::testing::StrEq;
using ::testing::_;

TEST(makeGetRequest, basic) {
    std::string str = makeGetRequest("api.coindesk.com", "v1/bpi/currentprice.json");
    EXPECT_STREQ(str.c_str(), "GET /v1/bpi/currentprice.json HTTP/1.1\r\nHost: api.coindesk.com\r\nConnection: close\r\n\r\n");
}

TEST(downloadSiteData, basic) {
    int port = 80;
    const char *host = "api.coindesk.com";
    const char *path = "v1/bpi/currentprice.json";
    downloadSiteData(port, host, path);
}

TEST(sanity, one) {
    ASSERT_EQ(1, 1);
}


