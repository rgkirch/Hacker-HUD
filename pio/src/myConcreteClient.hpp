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
            port(port),
            client(
                    static_cast<Client*>(port == httpsPort ? new WiFiClientSecure() : new WiFiClient())
            ) {};
    ~MyConcreteClient() {
        if(client != nullptr) {
            LOG("mycc deleting client");
            delete client;
        }
    };
    int connect(const char *host) {
        if(client != nullptr) {
            LOG("mycc connect");
            return client->connect(host, port);
        }
    };
    uint8_t connected() {
        if(client != nullptr) {
            LOG("mycc check that client is connected");
            return client->connected();
        }
    };
    size_t print(const char *cs) {
        if(client != nullptr) {
            LOG("mycc print to client");
            return client->print(cs);
        }
    };
    int read() {
        if(client != nullptr) {
            return client->read();
        }
    };
    void stop() {
        if(client != nullptr) {
            LOG("mycc stop client");
            client->stop();
        }
    };
private:
    uint16_t port;
    Client *client;
};

#endif //MYCONCRETECLIENT_HPP
