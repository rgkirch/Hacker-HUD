#ifndef OPTION_HPP
#define OPTION_HPP

template <typename T>
class Option {
public:
    Option<T> &map(std::function<Option<T>(T)> &f)
    {
        if (not this->isEmpty())
        {
            Option<T> o = f(this->data);
            if (not o.isEmpty()) {
                this->data = o.data;
            } else {
                this->empty = true;
            }
        }
        return *this;
    }
    Option() : empty(true) {};
    Option(T d) : data(d), empty(false) {}; // check not null
    bool isEmpty() { return this->empty; };
    T orElse(T e) { if(not this->isEmpty()) return data; else return e; };
private:
    T data;
    bool empty;
};

#endif
