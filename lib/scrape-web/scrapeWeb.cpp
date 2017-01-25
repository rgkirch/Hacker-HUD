#include "scrapeWeb.hpp"

// TODO - work on this
//websites and data to parse -----
Site coinDesk = (Site) {.url = "/v1/bpi/currentprice.json", .host = "api.coindesk.com", .secure = false};
jsonThing coinDeskJson {"rate_float", 12, 18};
jsonThing timeUtc{"updated",10,30};

Site etheriumHashes = (Site) {.url = "/v1/eth/avghashratelimited/0x884e51352e7c68BfC9bA230f487be963a11de11B/1", .host = "api.nanopool.org", .secure = true};
jsonThing ethereumHashesJson = {"data",6,12};

Site etheriumPrice = (Site) {.url = "/v1/eth/prices", .host = "api.nanopool.org", .secure = true};
jsonThing etheriumJson {"price_usd", 11, 16};

////Function to connect and retrieve web data -----
String scrapeSite(Site site) {
    WiFiClient* client;
    int httpPort;
    if(site.secure)
    {
        client = new WiFiClientSecure;
        httpPort = 443;
    } else {
        client = new WiFiClient;
        httpPort = 80;
    }

    if(!client->connect(site.host, httpPort)) {
        Serial.println("connection failed");
        return String();
    }

    // We now create a URI for the request "/v1/bpi/currentprice.json"
    Serial.print("Requesting URL: ");
    Serial.println(site.url);
    Serial.print("host: ");
    Serial.println(site.host);

    // This will send the request to the server
    client->print(String("GET ") + String(site.url) + " HTTP/1.1\r\n" +
                  "Host: " + String(site.host) + "\r\n" +
                  "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (!client->available()) {
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client->stop();
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    String answer;
    while(client->available())
    {
        String line = client->readStringUntil('\r');
        answer += line;
    }

    client->stop();
    Serial.println();
    Serial.println("closing connection");

    // Process answer
    Serial.println();
    Serial.println("Answer: ");
    Serial.println(answer);

    delete client;
    return answer;
}

//fnc to parse data from web page (json) -----
String parseData (String answer, jsonThing whatToParse){

    // Convert to JSON
    String jsonAnswer;
    int jsonIndex;

    for (int i = 0; i < answer.length(); i++) {
        if (answer[i] == '{') {
            jsonIndex = i;
            break;
        }
    }

    // Get JSON data
    jsonAnswer = answer.substring(jsonIndex);
    Serial.println();
    Serial.println("JSON answer: ");
    Serial.println(jsonAnswer);
    jsonAnswer.trim();

    // Get rate as float
    String priceString = jsonAnswer.substring(jsonAnswer.indexOf(whatToParse.keyword) + whatToParse.begin);
    int indexOutput = 0;

    if (priceString.indexOf(",") == -1){
        indexOutput = priceString.indexOf("}");
    } else if (priceString.indexOf("}") == -1){
        indexOutput = priceString.indexOf(",");
    } else {
        int less;
        if (priceString.indexOf("}") < priceString.indexOf(",")) {
            less = priceString.indexOf("}");
        } else {
            less = priceString.indexOf(",");
        }
        indexOutput = less;
    }
    priceString = priceString.substring(0,indexOutput);
    int decimal = priceString.indexOf(".");
    if (decimal > -1) priceString = priceString.substring(0,decimal + 3);
    priceString.trim();
    float price = priceString.toFloat();

    Serial.println("end function");
    return priceString;
}

std::vector<char> makeGetRequest(std::vector<char> host, std::vector<char> path)
{
    std::vector<char> request;
    std::vector<char> one = {'G', 'E', 'T', ' ', '/'};
    std::vector<char> two = {' ', 'H', 'T', 'T', 'P', '/', '1', '.', '1', '\r', '\n', 'H', 'o', 's', 't', ':', ' '};
    std::vector<char> three = {'\r', '\n', 'C', 'o', 'n', 'n', 'e', 'c', 't', 'i', 'o', 'n', ':', ' ', 'c', 'l', 'o', 's', 'e', '\r', '\n', '\r', '\n'};
    request.reserve(one.size() + two.size() + three.size() + host.size() + path.size());
    request.insert(std::begin(request), std::begin(one), std::end(one));
    request.insert(std::begin(request), std::begin(host), std::end(host));
    request.insert(std::begin(request), std::begin(two), std::end(two));
    request.insert(std::begin(request), std::begin(path), std::end(path));
    request.insert(std::begin(request), std::begin(three), std::end(three));
    return request;
}

// todo - does client.available() have a max size so that i might have to buffer the incomming message in parts OR can I always read the data into one buffer
// todo - maybe make this a class and use the builder pattern on it as well
// todo - don't build with strings, use strcat
int getJsonValue(const bool secureClient, std::vector<char> host, std::vector<char> path, const char* key)
{
    if(key == nullptr) return 0;
    WiFiClient* client;
    int httpPort;
    if(secureClient)
    {
        client = new WiFiClientSecure;
        httpPort = 443;
    } else {
        client = new WiFiClient;
        httpPort = 80;
    }
    if (client->connect(host.data(), httpPort))
    {
        auto get = makeGetRequest(host, path);
        client->print(get.data());
        std::vector<char> buffer;
        for(int bufferIndex = 0; bufferIndex < 2000 && client->connected();) {
            for(int i = min(2000 - bufferIndex, client->available()); i > 0; ++i) {
                buffer[bufferIndex] = (char)client->read();
                ++bufferIndex;
            }
        }
        client->stop();
    } else {
        client->stop();
    }
    return 0;
}
