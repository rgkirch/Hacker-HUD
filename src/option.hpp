#ifndef OPTION_HPP
#define OPTION_HPP

template <typename T>
class Option {
public:
    Option<T>* map(std::function<T(T)> f)
    {
        if (not this->isEmpty())
        {
            this->data = f(this->data);
        }
        return this;
    }
    Option() : empty(true) {};
    Option(T d) : data(d), empty(false) {}; // check not null
    Option(Option &o) : data(o.data), empty(o.empty) {};
    bool isEmpty() { return this->empty; };
    T getOrElse(T e) { if(not this->isEmpty()) return data; else return e; };
private:
    T data;
    bool empty;
};

#endif
