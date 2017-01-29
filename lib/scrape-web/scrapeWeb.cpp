#include "scrapeWeb.hpp"
#include "site.hpp"

extern std::string stringNotFound;
extern void serialPrintln(std::string str);
extern void serialPrint(std::string str);


// TODO - work on this
//websites and data to parse -----
//Site coinDesk = (Site) {.url = "/v1/bpi/currentprice.json", .host = "api.coindesk.com", .secure = false};
//jsonThing coinDeskJson {"rate_float", 12, 18};
//jsonThing timeUtc{"updated",10,30};

//Site etheriumHashes = (Site) {.url = "/v1/eth/avghashratelimited/0x884e51352e7c68BfC9bA230f487be963a11de11B/1", .host = "api.nanopool.org", .secure = true};
//jsonThing ethereumHashesJson = {"data",6,12};

//Site etheriumPrice = (Site) {.url = "/v1/eth/prices", .host = "api.nanopool.org", .secure = true};
//jsonThing etheriumJson {"price_usd", 11, 16};

//std::string scrapeWeb(Site &site)
//{
//    WiFiClient* client;
//    int httpPort;
//    if(site.secure)
//    {
//        client = new WiFiClientSecure;
//        httpPort = 443;
//    } else {
//        client = new WiFiClient;
//        httpPort = 80;
//    }
//}
//
//////Function to connect and retrieve web data -----
//String scrapeSite(Site site) {
//    WiFiClient* client;
//    int httpPort;
//    if(site.secure)
//    {
//        client = new WiFiClientSecure;
//        httpPort = 443;
//    } else {
//        client = new WiFiClient;
//        httpPort = 80;
//    }
//
//    if(!client->connect(site.host, httpPort)) {
//        Serial.println("connection failed");
//        return String();
//    }
//
//    // We now create a URI for the request "/v1/bpi/currentprice.json"
//    Serial.print("Requesting URL: ");
//    Serial.println(site.url);
//    Serial.print("host: ");
//    Serial.println(site.host);
//
//    // This will send the request to the server
//    client->print(String("GET ") + String(site.url) + " HTTP/1.1\r\n" +
//                  "Host: " + String(site.host) + "\r\n" +
//                  "Connection: close\r\n\r\n");
//    unsigned long timeout = millis();
//    while (!client->available()) {
//        if (millis() - timeout > 5000) {
//            Serial.println(">>> Client Timeout !");
//            client->stop();
//        }
//    }
//
//    // Read all the lines of the reply from server and print them to Serial
//    String answer;
//    while(client->available())
//    {
//        String line = client->readStringUntil('\r');
//        answer += line;
//    }
//
//    client->stop();
//    Serial.println();
//    Serial.println("closing connection");
//
//    // Process answer
//    Serial.println();
//    Serial.println("Answer: ");
//    Serial.println(answer);
//
//    delete client;
//    return answer;
//}

// maybe just search for one at a time
//fnc to parse data from web page (json) -----
std::string parseJson(std::string text, std::string key){
    int index;
    int begin;
    int end;
    if(text.length() < key.length()) return stringNotFound;
    if(text.length() < 3) return stringNotFound;
    if((index = text.find(key.data())) == std::string::npos) return stringNotFound;// key is there but value may be cut off
    if((begin = text.find(':', index + key.length()) + 1) == std::string::npos) return stringNotFound; // need more data
    if(text.length() <= begin) return stringNotFound;
    if(text.at(begin) == '"') // it's a string - look for end quote
    {
        begin++;
        if(text.length() <= begin) return stringNotFound;
        end = begin;
        while(!(text.at(end) == '"' && text.at(end - 1) != '\\')) // find first unescaped quote
        {
            end++;
            if(text.length() <= end) return stringNotFound;
        }
    } else { // it's a number
        if(text.find(',', begin) == std::string::npos && text.find('}', begin) == std::string::npos) {
            return stringNotFound;
        } else if(text.find(',', begin) == std::string::npos && text.find('}', begin) > 0) {
            end = text.find('}', begin);
        } else if(text.find(',', begin) > 0 && text.find('}', begin) == std::string::npos) {
            end = text.find(',', begin);
        } else {
            end = min(text.find(',', begin), text.find('}', begin));
        }
    }
    return text.substr(begin, end - begin);
}

std::string makeGetRequest(std::string host, std::string path)
{
    std::string request;
//    request.reserve(one.size() + two.size() + three.size() + host.size() + path.size());
    std::string one = {"GET /"};
    std::string two = {" HTTP/1.1\r\nHost: "};
    std::string three = {"\r\nConnection: close\r\n\r\n"};
    request.append(one).append(path).append(two).append(host).append(three);
    return request;
}

// todo - does client.available() have a max size so that i might have to buffer the incomming message in parts OR can I always read the data into one buffer
// todo - maybe make this a class and use the builder pattern on it as well
// todo don't use a new client that you have to free... prefer stack objects
std::string getJsonValue(Site *client, std::string key)
{
    std::string value = stringNotFound;
    if(client->connect())
    {
        client->print(makeGetRequest(client->getHost(), client->getPath()));
        std::string text;
        while(value == stringNotFound)
        {
            std::string buffer;
            buffer.reserve(text.length() + client->available());
            buffer.append(text);
            for(int i = 0; i < client->available(); ++i)
            {
                buffer.push_back(client->read());
            }
            value = parseJson(buffer, key);
            int keyIndex;
            if(keyIndex = buffer.find(key.data()) != std::string::npos)
            {
                text = buffer.substr(keyIndex);
            } else text = buffer;
        }
    }
    client->stop();
    return value;
}
