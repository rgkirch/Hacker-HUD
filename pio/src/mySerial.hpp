#ifndef MYSERIAL_HPP
#define MYSERIAL_HPP

class MyPrint {
public:
    virtual size_t print(const char) =0;
    virtual size_t print(const char*) =0;
};


#endif //MYSERIAL_HPP
