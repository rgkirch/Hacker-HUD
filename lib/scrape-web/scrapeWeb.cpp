#include "scrapeWeb.hpp"

// TODO - work on this
//websites and data to parse -----
//Site coinDesk = (Site) {.url = "/v1/bpi/currentprice.json", .host = "api.coindesk.com", .secure = false};
//jsonThing coinDeskJson {"rate_float", 12, 18};
//jsonThing timeUtc{"updated",10,30};
//
//Site etheriumHashes = (Site) {.url = "/v1/eth/avghashratelimited/0x884e51352e7c68BfC9bA230f487be963a11de11B/1", .host = "api.nanopool.org", .secure = true};
//jsonThing ethereumHashesJson = {"data",6,12};
//
//Site etheriumPrice = (Site) {.url = "/v1/eth/prices", .host = "api.nanopool.org", .secure = true};
//jsonThing etheriumJson {"price_usd", 11, 16};
//
////Function to connect and retrieve web data -----
//String scrapeWeb(Site site) {
//    String priceString;
//    // Connect to API
//    Serial.print("connecting to ");
//    Serial.println(site.host);
//
//    // Use WiFiClient class to create TCP connections
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
//
////fnc to parse data from web page (json) -----
//String parseData (String answer, jsonThing whatToParse){
//
//    // Convert to JSON
//    String jsonAnswer;
//    int jsonIndex;
//
//    for (int i = 0; i < answer.length(); i++) {
//        if (answer[i] == '{') {
//            jsonIndex = i;
//            break;
//        }
//    }
//
//    // Get JSON data
//    jsonAnswer = answer.substring(jsonIndex);
//    Serial.println();
//    Serial.println("JSON answer: ");
//    Serial.println(jsonAnswer);
//    jsonAnswer.trim();
//
//    // Get rate as float
//    String priceString = jsonAnswer.substring(jsonAnswer.indexOf(whatToParse.keyword) + whatToParse.begin);
//    int indexOutput = 0;
//
//    if (priceString.indexOf(",") == -1){
//        indexOutput = priceString.indexOf("}");
//    } else if (priceString.indexOf("}") == -1){
//        indexOutput = priceString.indexOf(",");
//    } else {
//        int less;
//        if (priceString.indexOf("}") < priceString.indexOf(",")) {
//            less = priceString.indexOf("}");
//        } else {
//            less = priceString.indexOf(",");
//        }
//        indexOutput = less;
//    }
//    priceString = priceString.substring(0,indexOutput);
//    int decimal = priceString.indexOf(".");
//    if (decimal > -1) priceString = priceString.substring(0,decimal + 3);
//    priceString.trim();
//    float price = priceString.toFloat();
//
//    Serial.println("end function");
//    return priceString;
//}

class JsonDownloader
{
    std::vector<char> getValue()
    {
        WiFiClient client;
        if (client.connect(host, 80))
        {
            client.print(request);
            while(client.connected())
            {
                if(client.available())
                {
                    try
                    {
                        std::vector<char> buffer.push_back(client->read());
                    } catch exception(e) {
                        Serial.println(e);
                    }
                }
            }
            client.stop();
            Serial.println("\n[Disconnected]");
        }
        else
        {
            Serial.println("connection failed!]");
            client.stop();
        }
    }
    std::vector<char> request;
    std::vector<std::vector<char>> jsonKeys;
    std::vector<std::vector<char>> jsonValues;
}
class JsonDownloaderBuilder
{
    JsonDownloader buildJsonDownloader()
    {
        request.reserve(one.size() + two.size() + three.size() + host.size() + path.size());
        request.insert(std::begin(request), std::begin(one), std::end(one));
        request.insert(std::begin(request), std::begin(host), std::end(host));
        request.insert(std::begin(request), std::begin(two), std::end(two));
        request.insert(std::begin(request), std::begin(path), std::end(path));
        request.insert(std::begin(request), std::begin(three), std::end(three));
    }
    void setHost()
    void setPath();
    std::vector<char> host;
    std::vector<char> path;
    std::vector<char> request;
    std::vector<char> one("GET /");
    std::vector<char> two(" HTTP/1.1\r\nHost: ");
    std::vector<char> three("\r\n" + "Connection: close\r\n" + "\r\n" );
}

char* getJsonValues(std::vector<char> host, std::vector<char> path)
{
    if(host == nullptr) return nullptr;
    if(path == nullptr) return nullptr;
    WiFiClient client;
    if (client.connect(host, 80))
    {
        client.print(String("GET /") + String(path) + " HTTP/1.1\r\n" + "Host: " + String(host) + "\r\n" + "Connection: close\r\n" + "\r\n" );
        while(client.connected())
        {
            if(client.available())
            {
                String line = client.readStringUntil('\n');
                Serial.println(line);
            }
        }
        client.stop();
        Serial.println("\n[Disconnected]");
    }
    else
    {
        Serial.println("connection failed!]");
        client.stop();
    }
    return nullptr;
}
