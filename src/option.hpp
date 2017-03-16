template <typename T>
class Option {
public:
    Option(T d) : data(d), empty(false) {}; // check not null
    Option() : empty(true) {};
    bool isEmpty() { return this->empty; };
    T getOrElse(T e) { if(not this->isEmpty()) return data; else return e; };
private:
    T data;
    bool empty;
};
