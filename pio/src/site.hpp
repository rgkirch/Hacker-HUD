#ifndef SITE_HPP
#define SITE_HPP
#include <string>
#include <vector>
#include "site.hpp"
#include "option.hpp"

using std::string;

struct Site {
    unsigned int updateInterval;
    unsigned int lastUpdated;
    uint16_t port;
    const char* host;
    const char* path;
    Option<string> lastResult;
    std::vector<string> keys;
};

#endif
