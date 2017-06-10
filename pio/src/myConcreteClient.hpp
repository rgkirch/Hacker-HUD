#ifndef PROJECT_WIFICLIENTADAPTER_HPP
#define PROJECT_WIFICLIENTADAPTER_HPP

#include <cstddef>
#include <cstdint>
#include "Client.h"

class MyClient {
public:
    MyClient(Client *client) : client(client) {};
    int connect(const char *host, uint16_t port) { client->connect(host, port); };
    size_t print(const char *cs) { client->print(cs); };
    int read() { client->read(); };
    void stop() { client->stop(); };
private:
    Client *client;
};

#endif //PROJECT_WIFICLIENTADAPTER_HPP
