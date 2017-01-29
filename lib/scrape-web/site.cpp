#include "site.hpp"

Site Site::Builder::build()
{
    WiFiClient* client;
    if(secure)
    {
        client = new WiFiClientSecure();
    } else {
        client = new WiFiClient();
    }
    return Site(this->host, this->path, client);
}

Site::Builder& Site::Builder::setHost(const std::string &host) {
    this->host = host;
    return *this;
}

Site::Builder& Site::Builder::setPath(const std::string &path) {
    this->path = path;
}

Site::Builder& Site::Builder::setSecure(bool secure) {
    this->secure = secure;
}
