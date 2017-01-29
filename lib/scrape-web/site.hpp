#ifndef SITE_HPP
#define SITE_HPP

#include <WiFiClient.h>
#include <WiFiClientSecure.h>

#include <string>

class Site {
public:
    class Builder;
    int connect() const;
    int print(std::string) const;
    int available() const;
    int read() const;
    void stop() const;
    const std::string &getHost() const;
    const std::string &getPath() const;

private:
    Site(const std::string &host, const std::string &path, int httpPort, WiFiClient *client) : host(host), path(path), httpPort(httpPort), client(client) {}
    const std::string host;
    const std::string path;
    const int httpPort;
    WiFiClient *client;
};

class Site::Builder {
private:
    std::string host = std::string("");
    std::string path = std::string("");
    int httpPort;
    bool secure = false;
public:
    std::unique_ptr<Site> build();
    Builder() = default;
    Site::Builder& setHost(std::string host);
    Site::Builder& setPath(std::string path);
    Site::Builder& setSecure(bool secure);
};

#endif
