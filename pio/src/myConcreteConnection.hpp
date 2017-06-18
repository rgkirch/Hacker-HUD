#ifndef MYCONNECTION_HPP
#define MYCONNECTION_HPP

#include <string>
#include "myConnection.hpp"
#include "myClient.hpp"
#include "makeGetRequest.hpp"
#include "globals.hpp"

//std::string makeGetRequest(std::string host, std::string path)
//{
//    std::string request;
//    std::string get = {"GET /"};
//    std::string http = {" HTTP/1.1\r\nHost: "};
//    std::string close = {"\r\nConnection: close\r\n\r\n"};
//    request.append(get);
//    request.append(path);
//    request.append(http);
//    request.append(host);
//    request.append(close);
//    return request;
//}

//MyConnection* makeConnection(uint16_t port, const char *host, const char *path) {
//    MyClient *client = new MyConcreteClient(port);
//    return static_cast<MyConnection *>(new MyConcreteConnection(client, host, path));
//}

class MyConcreteConnection : MyConnection {
public:
    MyConcreteConnection(MyConcreteConnection& connection) : client(connection.client), port(connection.port), host(connection.host), path(connection.path) {};
//    MyConcreteConnection(MyConcreteConnection&& client) : client(std::move(client.client)), port(std::move(client.port)), host(std::move(client.host)), path(std::move(client.path)) {};
//    MyConcreteConnection& operator=(const MyConcreteConnection& c) {
//        client = std::unique_ptr<MyClient>(new MyConcreteClient(*c.client));
//        port = c.port;
//        host = c.host;
//        path = c.path;
//        return *this;
//    };
//    MyConcreteConnection& operator=(const MyConcreteConnection&& c) {
////        client = std::unique_ptr<MyClient>(c.client.release());
//        client = std::move(c.client);
//        port = std::move(c.port);
//        host = std::move(c.host);
//        path = std::move(c.path);
//        return *this;
//    };
    MyConcreteConnection(MyClient* c, const char *h, const char *p) : client(c), host(h), path(p) {
        LOG("connection constructor");
        int i = client->connect(h);
        LOGN("client -> connect returned ");
        LOG(i);
        LOG("print the get request");
        client->print(makeGetRequest(host.c_str(), path.c_str()).c_str());
    };
    ~MyConcreteConnection() {
        LOG("stop the client in the destructor");
        client->stop();
//        delete client;
    };
//    int write(const char *cs) {
//        if (not client->connected()) {
//            client->connect(host.c_str(), port);
//        }
//        return client->print(cs);
//    }
//    size_t connect() {
//        return client->print(makeGetRequest(host.c_str(), path.c_str()).c_str());
//    };
//    uint8_t connected() { return client->connected(); };
    size_t print(const char *cs) override { return client->print(cs); };
    std::string read() override {
        LOG("read from the client a bunch of times");
        std::string data;
        for(int read = 0; (read = client->read()) > -1; data.push_back(static_cast<char>(read)));
//    for(char c; client->readBytes(&c, 1) > -1; data.push_back(c));
//    for(uint8_t c; client->read(&c, 1) > -1; data.push_back(c));
//        LOG(data.c_str());
        LOG("search for the left brace");
        auto i = data.find('{');
        if (i == data.npos) {
            i = 0;
        }
        LOG("take the substring of the string");
        return data.substr(i);
    };
private:
//    std::unique_ptr<WiFiClient> client;
    MyClient *client;
    uint16_t port;
    std::string host;
    std::string path;
};

////static auto makePartialClient(uint16_t port, const char *host) {
//static std::function<MyConcreteConnection&&(const char *path)> makePartialClient(uint16_t port, const char *host) {
////    return [](const char *path)->auto { MyConcreteConnection(port, host, path); };
//    return std::bind([](uint16_t port, const char *host, const char *path)->MyConcreteConnection&&{ return std::move(MyConcreteConnection(port, host, path)); }, port, host, std::placeholders::_1);
//}


#endif //MYCONNECTION_HPP
