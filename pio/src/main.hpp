#pragma once

#include "globals.hpp"
#include "option.hpp"
#include "timeCache.hpp"
#include <functional>
#include <string>
#include <vector>

using namespace std;

Option<string> applyKeys(const JsonObject &o,
                         const vector<string>::iterator begin,
                         const vector<string>::iterator end) {
  LOG("apply keys");
  Option<string> emptyOption;
  auto it = begin;
  if (next(it) == end) {
    const char *r = o[(*it).c_str()].as<const char *>();
    if (r == nullptr) {
      return emptyOption;
    } else
      return string(r);
  } else {
    return applyKeys(o[(*it).c_str()], next(begin), end);
  }
}

Option<string> getSiteData(uint16_t port, string host, string path,
                           vector<string> keys) {
  HTTPClient http;
  string server;
  port == httpsPort ? server.append("https://") : server.append("http://");
  server.append(host).append(path);
  http.begin(server.c_str());
  int httpCode = http.GET();
  LOGN(httpCode);
  LOG(HTTP_CODE_OK);
  string data = http.getString().c_str();
  http.end();
  LOGN("data ");
  LOG(data.c_str());
  LOG("search for the left brace");
  int i = data.find('{');
  if (i == data.npos) {
    i = 0;
  }
  LOG("take the substring of the string");
  data = data.substr(i);
  LOG(data.c_str());
  Option<string> o(data);
  DynamicJsonBuffer jsonBuffer(2000);
  function<Option<string>(std::string)> f([&](string str) -> Option<string> {
    JsonObject &o = jsonBuffer.parseObject(str.c_str());
    if (not o.success())
      return Option<string>();
    return applyKeys(o, keys.begin(), keys.end());
  });
  LOG("json shit");
  o.flatMap(f);
  return o;
}

TimeCache<Option<string>> bitcoin(
    []() {
      auto port = httpPort;
      auto host = "api.coindesk.com";
      auto path = "/v1/bpi/currentprice.json";
      vector<string> keys = {"bpi", "USD", "rate_float"};
      return getSiteData(port, host, path, keys);
    },
    60 * 60 * 1000);
TimeCache<Option<string>> etherium(
    []() {
      auto port = httpsPort;
      auto host = "coinmarketcap-nexuist.rhcloud.com";
      auto path = "/api/eth";
      vector<string> keys = {"price", "usd"};
      return getSiteData(port, host, path, keys);
    },
    60 * 60 * 1000);
TimeCache<Option<string>> openWeatherMapHumidity(
    []() {
      auto port = httpPort;
      auto host = "api.openweathermap.org";
      auto path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=" +
                  openWeatherMapApiKey;
      vector<string> keys = {"main", "humidity"};
      return getSiteData(port, host, path, keys);
    },
    60 * 60 * 1000);
TimeCache<Option<string>> openWeatherMapTemp(
    []() {
      auto port = httpPort;
      auto host = "api.openweathermap.org";
      auto path = "/data/2.5/weather?q=Tampa,us&units=imperial&APPID=" +
                  openWeatherMapApiKey;
      vector<string> keys = {"main", "temp"};
      return getSiteData(port, host, path, keys);
    },
    60 * 60 * 1000);

string whatToDisplay(long time) {
  long frameTime = time % 10;
  if (frameTime < 2) {
    string bitcoinPrice("bitcoin price = ");
    bitcoinPrice.append(bitcoin.get().getOrElse("no data"));
    return bitcoinPrice;
  } else if (frameTime < 4) {
    return "hello world";
  } else {
    return NTP.getTimeStr(time).c_str();
  }
}

int getWhichFrame(int numFrames, long timePerFrame, long currentTime) {
  int period = numFrames * timePerFrame;
  if (currentTime < 0)
    currentTime += period * (currentTime / period + 1);
  int currentFrame = (currentTime % (numFrames * timePerFrame)) / timePerFrame;
  return currentFrame;
}