#ifndef wificlientsecure_h
#define wificlientsecure_h
#include "WiFiClient.h"
//#include "include/ssl.h"


class SSLContext;

class WiFiClientSecure : public WiFiClient {
public:
  WiFiClientSecure() {};
//  ~WiFiClientSecure() override {};
  WiFiClientSecure(const WiFiClientSecure&) {};
  WiFiClientSecure& operator=(const WiFiClientSecure&) {};

//  int connect(IPAddress ip, uint16_t port) override;
  int connect(const char* name, uint16_t port) override { return 0; };

  bool verify(const char* fingerprint, const char* domain_name) { return false; };
  bool verifyCertChain(const char* domain_name) { return false; };

  uint8_t connected() override { return 0; };
  size_t write(const uint8_t *buf, size_t size) override { return 0; };
  int read(uint8_t *buf, size_t size) override { return 0; };
  int available() override { return 0; };
  int read() override { return 0; };
  int peek() override { return 0; };
  size_t peekBytes(uint8_t *buffer, size_t length) override { return 0; };
  void stop() override {};

  bool setCACert(const uint8_t* pk, size_t size) { return false; };
  bool setCertificate(const uint8_t* pk, size_t size) { return false; };
  bool setPrivateKey(const uint8_t* pk, size_t size) { return false; };

  bool loadCACert(Stream& stream, size_t size) { return false; };
  bool loadCertificate(Stream& stream, size_t size) { return false; };
  bool loadPrivateKey(Stream& stream, size_t size) { return false; };

  template<typename TFile>
  bool loadCertificate(TFile& file) { return false; };

  template<typename TFile>
  bool loadPrivateKey(TFile& file) { return false; };

protected:
    int _connectSSL(const char* hostName) { return 0; };
    bool _verifyDN(const char* name) { return false; };

    SSLContext* _ssl = nullptr;
};

#endif //wificlientsecure_h
