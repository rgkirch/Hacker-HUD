#ifndef SITE_HPP
#define SITE_HPP
#include <string>
#include <vector>
#include "site.hpp"
#include "option.hpp"

using std::string;

struct Site {
public:
    Site(unsigned int updateInterval, unsigned int lastUpdated, uint16_t port, const char *host, const char *path, const Option<string> &lastResult, const vector<string> &keys) :
            updateInterval(updateInterval),
            lastUpdated(lastUpdated),
            port(port),
            host(host),
            path(path),
            lastResult(lastResult),
            keys(keys) {}
    unsigned int updateInterval;
    unsigned int lastUpdated;
    uint16_t port;
    const char* host;
    const char* path;
    Option<string> lastResult;
    std::vector<string> keys;
};

#endif
