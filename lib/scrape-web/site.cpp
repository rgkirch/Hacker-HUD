#include "site.hpp"
#include "scrapeWeb.hpp"


int Site::connect()
{
    return client->connect(host.data(), httpPort);
}

int Site::print(std::string str)
{
    return client->print(str.data());
}

int Site::available()
{
    return client->available();
}

int Site::read()
{
    return client->read();
}

void Site::stop()
{
    client-> stop();
}

const std::string &Site::getHost() const {
    return host;
}

const std::string &Site::getPath() const {
    return path;
}

Site* Site::Builder::build()
{
    WiFiClient* client;
    if(secure)
    {
        client = new WiFiClientSecure();
        this->httpPort = 443;
    } else {
        client = new WiFiClient();
        this->httpPort = 80;
    }
    return new Site(this->host, this->path, this->httpPort, client);
}

Site::Builder& Site::Builder::setHost(std::string host) {
    this->host = host;
    return *this;
}

Site::Builder& Site::Builder::setPath(std::string path) {
    this->path = path;
    return *this;
}

Site::Builder& Site::Builder::setSecure(bool secure) {
    this->secure = secure;
    return *this;
}
