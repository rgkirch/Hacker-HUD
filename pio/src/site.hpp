#ifndef SITE_HPP
#define SITE_HPP
#include <string>
//#include <functional>
#include <vector>
#include "site.hpp"
#include "option.hpp"

struct Site {
    unsigned int updateInterval;
    unsigned int lastUpdated;
    uint16_t port;
    const char* host;
    const char* path;
    Option<std::string> lastResult;
    std::vector<std::string> keys;
//    typedef <typename T>
//    float(*f)(JsonObject);
};

#endif
