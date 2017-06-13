#ifndef MYCONCRETECLIENT_HPP
#define MYCONCRETECLIENT_HPP

#include <cstddef>
#include <cstdint>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include "myClient.hpp"
#include "Client.h"
#include "globals.hpp"

class MyConcreteClient : public MyClient {
public:
//    MyConcreteClient(Client *client) : client(client) {};
    MyConcreteClient(MyConcreteClient& c) : client(c.client) {};
    MyConcreteClient(uint16_t port) :
            client(
                    static_cast<Client*>(port == httpsPort ? new WiFiClientSecure() : new WiFiClient())
            ) {};
    ~MyConcreteClient() { delete client; };
    int connect(uint16_t port, const char *host) { return client->connect(host, port); };
    uint8_t connected() { return client->connected(); };
    size_t print(const char *cs) { return client->print(cs); };
    int read() { return client->read(); };
    void stop() { client->stop(); };
private:
    Client *client;
};

#endif //MYCONCRETECLIENT_HPP
