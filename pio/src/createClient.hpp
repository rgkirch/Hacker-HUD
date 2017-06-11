#ifndef CREATECLIENT_HPP
#define CREATECLIENT_HPP

//#include "myClient.hpp"
//#include "site.hpp"
//#include "scrapeWeb.hpp"
//#include "myConcreteClient.hpp"
//#include "makeGetRequest.hpp"

//MyClient* helper(int port) {
//    MyClient *client;
//    if(port == httpsPort) {
//        client = new MyConcreteClient(new WiFiClientSecure());
//    } else if (port == httpPort){
//        client = new MyConcreteClient(new WiFiClient());
//    } else return nullptr; //"port incorrect"
//    return client;
//}

//MyClient* createClient(int port, const char *host, const char *path) {
//    MyClient *client = helper(port);
//    if (not client->connect(host, port)) {
//        return nullptr; //"client connect failed"
//    }
//    if (not client->connected()) {
//        return nullptr; //"client not connected?!?!"
//    }
//    client->print(makeGetRequest(host, path).c_str());
//    return client;
//}

#endif //CREATECLIENT_HPP
