#ifndef SITE_HPP
#define SITE_HPP

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <string>

class Site {
public:
    class Builder;
private:
    Site(const std::string &host, const std::string &path, const WiFiClient *client) : host(host), path(path), client(client) {}
    const std::string host;
    const std::string path;
    const WiFiClient *client;
};

class Site::Builder {
private:
    std::string host = std::string("");
    std::string path = std::string("");
    bool secure = false;
public:
    Site build();
    Builder() = default;
    Site::Builder& setHost(const std::string &host);
    Site::Builder& setPath(const std::string &path);
    Site::Builder& setSecure(const bool secure);
};

#endif
