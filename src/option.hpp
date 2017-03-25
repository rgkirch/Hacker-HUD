#ifndef OPTION_HPP
#define OPTION_HPP

template <typename T>
class Option {
public:
    Option<T> &map(const std::function<T(T)> &f)
    {
        if (not this->isEmpty())
        {
            this->data = f(this->data);
        }
        return *this;
    }
//    Option<T> &operator=(const T &t) {
//        this->data = t;
//        this->empty = false;
//        return *this;
//    }; // check not null
//    Option<T> &operator=(const Option<T> &o) {
//        if (this != &o) {
//            this->data = o.data;
//            this->empty = o.empty;
//        }
//        return *this;
//    }; // check not null
//    Option<T> &operator=(const Option<T> &&o) {
//        if (this != &o) {
//            this->data = std::move(o.data);
//            this->empty = std::move(o.empty);
//        }
//        return *this;
//    }; // check not null
    Option() : empty(true) {};
    Option(T d) : data(d), empty(false) {}; // check not null
//    Option(Option<T> &o) : data(o.data), empty(o.empty) {};
    bool isEmpty() { return this->empty; };
    T getOrElse(T e) { if(not this->isEmpty()) return data; else return e; };
private:
    T data;
    bool empty;
};

#endif
