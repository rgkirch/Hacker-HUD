#include "scrapeWeb.hpp"
#include "../option/option.hpp"

Option<std::string> scrapeSite(Site site) {
    WiFiClient* client;
    if(site.port == httpsPort)
    {
        client = new WiFiClientSecure;
    } else if (site.port == httpPort){
        client = new WiFiClient;
    } else return Option::error("site port incorrect");
    if (client == NULL) return Option::error("couldn't make client");

    if(!client->connect(site.host, site.port)) {
        return Option::error("client connect failed");
    }

    client->print(makeGetRequest(site.host, site.path));
    unsigned long timeout = millis();
    while (!client->available()) {
        if (millis() - timeout > 5000) {
            client->stop();
            return Option::error("client timed out");
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client->available())
    {
        Serial.println(client->readStringUntil('\r'));
    }

    client->stop();
    delete client;
    return Option<std::string>("");
}

// todo - does client.available() have a max size so that i might have to buffer the incomming message in parts OR can I always read the data into one buffer
// todo - maybe make this a class and use the builder pattern on it as well
// todo don't use a new client that you have to free... prefer stack objects
std::string getJsonValue(const Site& client, std::string key)
{
    std::string value = stringNotFound;
    if(client.connect())
    {
        // todo - move this into site?
        client.print(makeGetRequest(client.getHost(), client.getPath()));
        std::string text;
        while(value == stringNotFound)
        {
            std::string buffer;
            buffer.reserve(text.length() + client.available());
            buffer.append(text);
            for(int i = 0; i < client.available(); ++i)
            {
                buffer.push_back(client.read());
            }
            value = parseJson(buffer, key);
            int keyIndex;
            if(keyIndex = buffer.find(key) != std::string::npos)
            {
                text = buffer.substr(keyIndex);
            } else text = buffer;
        }
    }
    client.stop();
    return value;
}

std::string get(std::string host, std::string path, bool secure)
{
    if(secure)
    {
        return secureGet(host, path);
    } else {
        return get(host, path);
    }
}
std::string secureGet(std::string host, std::string path)
{
    std::string text;
    WiFiClientSecure client;
    int httpPort = 443;
    Serial.print("connecting to  -> ");
    serialPrintln(host);
    Serial.print("on port  -> ");
    Serial.println(httpPort);
    client.connect(host.data(), httpPort);
//    for(int timeout = millis() + 10000; !client.connected() && millis() < timeout;)
    while(!client.connected())
    {
        yield();
    }
//    if(!client.connected())
//    {
//        Serial.println("failed to connect");
//        return std::string();
//    }
    delay(1000);
    std::string request = makeGetRequest(host, path);
    client.print(request.data());
    Serial.print("request -> ");
    serialPrintln(request);
//    for(int timeout = millis() + 5000; !client.available() && millis() < timeout;)
    while(!client.available())
    {
        yield();
    }
    if(!client.available())
    {
        Serial.println("nothing available");
        return std::string();
    }
    while(client.connected())
    {
        int available = client.available();
        Serial.println(available);
        text.reserve(text.length() + available);
        for(int i = 0; i < available; ++i)
        {
            text.push_back(client.read());
        }
    }
    client.stop();
    return text;
}
std::string get(std::string host, std::string path)
{
    std::string text;
    WiFiClient client;
    int httpPort = 80;
    serialPrintln(host);
    Serial.println(httpPort);
    if(client.connect(host.data(), httpPort))
    {
        std::string request = makeGetRequest(host, path);
        client.print(request.data());
        serialPrintln(request);
        while(client.connected())
        {
            int available = client.available();
            Serial.println(available);
            text.reserve(text.length() + available);
            for(int i = 0; i < available; ++i)
            {
                text.push_back(client.read());
            }
        }
        client.stop();
    }
    return text;
}
