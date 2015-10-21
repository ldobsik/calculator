#ifndef LINEAR_H
#define LINEAR_H

#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <exception>
#include <map>

template<typename T>
struct linear {
    struct operror : public std::exception {
        const std::string msg;
        operror(const std::string & im) :msg(im) {};

        virtual const char* what() const throw()
        {
            return msg.c_str();
        }
    };

    linear(const T& id = 0) : d(id) {};
    linear(const std::string& ix) : d(0) { x[ix] = 1; };
    template<typename U>
    friend std::ostream& operator << (std::ostream& os, const linear<U>& b);

    std::map<std::string, T> x;
    T d;

private:
    std::string prettyPrintTerm(const std::pair<std::string, T> & t) const;
};

template<typename T>
std::istream& operator>> (std::istream& is, linear<T>& b)
{
    if (isalpha(is.peek())) {
        // variable
        std::string name;
        is >> name;
        b.x.clear();
        b.d = 0;
        b.x[name] = 1;
    }
    else {
        // value
        b.x.clear();
        is >> b.d;
    }
    return is;
}

template<typename T>
std::string linear<T>::prettyPrintTerm(const std::pair<std::string, T> & t) const
{
    std::ostringstream ss;

    if (t.first == "") ss << t.second;
    else {
        if (t.second == -1) ss << '-';
        else if (t.second == 1);
        else ss << t.second << "*";

        ss << t.first;
    }

    return ss.str();
}

template<typename T>
std::ostream& operator<< (std::ostream& os, const linear<T>& b)
{
    bool pvars = false;
    for (auto i = begin(b.x);i != end(b.x); i++) {
        T x = i->second;
        if (x == 0) continue;
        if (pvars) {
            if(x > 0) os << " + ";
            else os << " - ";
        }
        os << b.prettyPrintTerm(*i).substr(pvars && x < 0);
        pvars = true;
    }

    if (b.d != 0 || !pvars) {
        if (pvars) {
            if (b.d > 0) os << " + ";
            else os << " - ";
        }
        os << b.prettyPrintTerm(std::make_pair("", b.d)).substr(pvars && b.d < 0);
    }


    return os;
}

template<typename T>
linear<T> operator+(const linear<T> &b1, const linear<T> &b2)
{
    linear<T> res(b1.d + b2.d);

    res.x = b1.x;
    for (auto ix = begin(b2.x); ix != end(b2.x); ix++) {
        res.x[ix->first] = res.x[ix->first] + ix->second;
    }
    
    return res;
}

template<typename T>
linear<T> operator-(const linear<T> &b1, const linear<T> &b2)
{
    linear<T> res(b1.d - b2.d);

    res.x = b1.x;
    for (auto ix = begin(b2.x); ix != end(b2.x); ix++) {
        res.x[ix->first] = res.x[ix->first] - ix->second;
    }

    return res;
}

template<typename T>
linear<T> operator*(const linear<T> &b1, const linear<T> &b2)
{
    if(!b1.x.empty() && !b2.x.empty()) throw typename linear<T>::operror("polynomial of order > 1 not allowed");
    
    linear<T> res(b1.d * b2.d);

    auto &x = b1.x.empty() ? b2.x : b1.x;
    auto &d = b1.x.empty() ? b1.d : b2.d;

    if (d != 0) {
        for (auto ix = begin(x); ix != end(x); ix++) {
            res.x[ix->first] = ix->second * d;
        }
    }

    return res;
}


template<typename T>
linear<T> operator/(const linear<T> &b1, const linear<T> &b2)
{
    if (b2.d == 0 && b2.x.empty()) throw typename linear<T>::operror("division by zero");

    if (b2.d != 0 && b2.x.empty()) {
        linear<T> q(1 / b2.d);
        return  q * b1;
    }

    throw typename linear<T>::operror("polynomial fraction not allowed");
}

template<typename T>
linear<T> operator-(const linear<T> &b)
{
    linear<T> res(0);
    
    return res - b;
}

template<typename T>
linear<T> log(const linear<T> &b)
{
    if (!b.x.empty()) throw typename linear<T>::operror("log of polynomial not allowed");

    linear<T> res(log(b.d));

    return res;
}

template<typename T>
linear<T> pow(const linear<T> &b1, const linear<T> &b2)
{
    if (!b2.x.empty()) throw typename linear<T>::operror("polynomial exponent not allowed");
    if (!b1.x.empty()) throw typename linear<T>::operror("power of polynomial not allowed");

    linear<T> res(pow(b1.d, b2.d));

    return res;
}
#endif
