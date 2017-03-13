template <typename T>
class Option {
public:
    Option(T d) : data(d), err(false) {};
//    Option(Option &o) {
//        if (o.isError()) {
//            err = true;
//        } else {
//            data = o.data;
//            err = false;
//        }
//    };
    bool isError() { return this->err; };
    template <typename E>
    T getOrElse(E e) { if(not this->isError()) return data; else return e; };
    static Option error() { return new Option(); };
    static Option error(std::string str) { return new Option(); }; // placeholder
//    static Option error(std::string str) { return new Option(str); }; // make new option, set string, return option
    std::string errorString;
private:
    Option() : err(true) {};
//    Option(std::string str) : err(true), errorString(str) {}; // delete this constructor if conflict with template constructor
    T data;
    bool err;
};
