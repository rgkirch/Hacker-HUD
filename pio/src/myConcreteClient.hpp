#ifndef MYCONCRETECLIENT_HPP
#define MYCONCRETECLIENT_HPP

#include <cstddef>
#include <cstdint>
#include "myClient.hpp"

class MyConcreteClient : public MyClient {
public:
//    MyConcreteClient(Client *client) : client(client) {};
    MyConcreteClient(MyConcreteClient& c) : client(c.client) {};
    MyConcreteClient(uint16_t port) :
            client(
                    static_cast<Client*>(port == httpsPort ? new WiFiClientSecure() : new WiFiClient())
            ) {};
    int connect(const char *host, uint16_t port) { return client->connect(host, port); };
    uint8_t connected() { return client->connected(); };
    size_t print(const char *cs) { return client->print(cs); };
    int read() { return client->read(); };
    void stop() { client->stop(); };
private:
    Client *client;
};

#endif //MYCONCRETECLIENT_HPP
