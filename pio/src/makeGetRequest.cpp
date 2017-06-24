#include "makeGetRequest.hpp"

using std::string;

string makeGetRequest(const char *host, const char *path)
{
    string request;
    string get = {"GET /"};
    string http = {" HTTP/1.1\r\nHost: "};
    string close = {"\r\nConnection: close\r\n\r\n"};
    request.append(get);
    if (path != nullptr) request.append(path);
    request.append(http);
    if (host != nullptr) request.append(host);
    request.append(close);
    return request;
}

