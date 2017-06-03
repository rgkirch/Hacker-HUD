#ifndef wificlient_h
#define wificlient_h
#include <memory>
#include "Arduino.h"
#include "Print.h"
#include "Client.h"
#include "IPAddress.h"
#include "include/slist.h"

#define WIFICLIENT_MAX_PACKET_SIZE 1460

class ClientContext;
class WiFiServer;

class WiFiClient : public Client, public SList<WiFiClient> {
protected:
  WiFiClient(ClientContext* client) {};

public:
  WiFiClient() {};
//  virtual ~WiFiClient() {};
  WiFiClient(const WiFiClient&) {};
  WiFiClient& operator=(const WiFiClient&) {};

  uint8_t status();
//  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port) { return 0; };
  virtual size_t write(uint8_t) { return 0; };
  virtual size_t write(const uint8_t *buf, size_t size) { return 0; };
  size_t write_P(PGM_P buf, size_t size) { return 0; };
  size_t write(Stream& stream) { return 0; };
  virtual int available() { return 0; };
  virtual int read() { return 0; };
  virtual int read(uint8_t *buf, size_t size) { return 0; };
  virtual int peek() { return 0; };
  virtual size_t peekBytes(uint8_t *buffer, size_t length) { return 0; };
  size_t peekBytes(char *buffer, size_t length) { return 0; }
  virtual void flush() {};
  virtual void stop() {};
  virtual uint8_t connected() { return 0; };
  virtual operator bool() { return false; };

//  IPAddress remoteIP();
  uint16_t  remotePort() { return 0; };
//  IPAddress localIP();
  uint16_t  localPort() { return 0; };
  bool getNoDelay() { return false; };
  void setNoDelay(bool nodelay) {};
  static void setLocalPortStart(uint16_t port) {};
  size_t availableForWrite() { return 0; };

  friend class WiFiServer;

  using Print::write;

  static void stopAll() {};
  static void stopAllExcept(WiFiClient * c) {};

protected:

  static int8_t _s_connected(void* arg, void* tpcb, int8_t err) { return 0; };
  static void _s_err(void* arg, int8_t err) {};

  int8_t _connected(void* tpcb, int8_t err) { return 0; };
//  void _err(int8_t err);

  ClientContext* _client;
  static uint16_t _localPort;
};

#endif
