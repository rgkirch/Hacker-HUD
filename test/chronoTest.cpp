#include "gtest/gtest.h"
#include "gmock/gmock.h"

TEST(dates, a) {
    std::time_t rawtime;
    std::tm* timeinfo;
    char buffer[80];
    std::time(&rawtime);
    timeinfo = std::localtime(&rawtime);
    std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);
    std::puts("\n");
    std::puts(buffer);
}

TEST(dates, b) {
    const int len = 80;
    char s[len];
    time_t t = time(0);
    strftime(s, len, "%Y-%m-%d", localtime(&t));
    std::cout << std::endl << s << '\n';
}

TEST(dates, c) {
    const int len = 20;
    char s[len];
    time_t t = 1499516750;
    t -= 100 * 24 * 60 * 60;
    strftime(s, len, "%Y-%m-%d", localtime(&t));
    std::cout << std::endl << s << '\n';
}

TEST(dates, d) {
    std::cout << std::endl << time(0) << '\n';
}
