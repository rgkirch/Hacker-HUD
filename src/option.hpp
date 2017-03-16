template <typename T>
class Option {
public:
    Option(T d) : data(d), empty(false) {}; // check not null
    Option() : empty(true) {};
//    Option(Option &o) {
//        if (o.isEmpty()) {
//            empty = true;
//        } else {
//            data = o.data;
//            empty = false;
//        }
//    };
    bool isEmpty() { return this->empty; };
    T getOrElse(T e) { if(not this->isEmpty()) return data; else return e; };
private:
//    Option() : empty(true) {};
    T data;
    bool empty;
};
