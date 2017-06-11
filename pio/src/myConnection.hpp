#ifndef MYABSTRACTCONNECTION_HPP
#define MYABSTRACTCONNECTION_HPP

#include <cstdint>
#include <cstddef>
#include <string>

class MyConnection {
public:
//   virtual int connect(uint16_t port, const char *host) =0;
   virtual size_t print(const char *cs) =0;
   virtual std::string read() =0;
};

#endif //MYABSTRACTCONNECTION_HPP
