#pragma once

template <typename T> struct TimeCache {
  TimeCache(function<T(void)> f, int cacheTime) : f(f), cacheTime(cacheTime) {}
  virtual ~TimeCache() = default;

  T get() {
    if (millis() > lastUpdated + cacheTime) {
      lastUpdated = millis();
      data = f();
    }
    return data;
  }

  T data;
  function<T(void)> f;
  int cacheTime;
  unsigned long lastUpdated;
};
