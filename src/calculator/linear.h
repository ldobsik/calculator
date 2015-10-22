#ifndef LINEAR_H
#define LINEAR_H

#include <algorithm>
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <exception>
#include <map>

template<typename T>
struct linear {
    struct error : public std::exception {
        const std::string msg;
        error(const std::string & im) :msg(im) {};

        virtual const char* what() const throw()
        {
            return msg.c_str();
        }
    };

    linear(const T& id = 0) : d(id) {};
    linear(const T& ix, const std::string& iname) : d(0) { x[iname] = ix; };
    template<typename U>
    friend std::ostream& operator << (std::ostream& os, const linear<U>& b);
    
    bool isConstant() const { return std::all_of(x.begin(), x.end(), [](auto &x) {return x.second == 0;}); };

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
    for (auto &x: b.x) {
        if (x.second == 0) continue;
        if (pvars) {
            if(x.second > 0) os << " + ";
            else os << " - ";
        }
        os << b.prettyPrintTerm(x).substr(pvars && x.second < 0);
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
bool operator==(const linear<T> &b1, const linear<T> &b2)
{
    auto diff = b1 - b2;

    return (diff.d == 0) && diff.isConstant();
}

template<typename T>
linear<T> operator+(const linear<T> &b1, const linear<T> &b2)
{
    linear<T> res(b1.d + b2.d);

    res.x = b1.x;
    for (auto &x: b2.x) {
        res.x[x.first] = res.x[x.first] + x.second;
    }
    
    return res;
}

template<typename T>
linear<T> operator-(const linear<T> &b1, const linear<T> &b2)
{
    linear<T> res(b1.d - b2.d);

    res.x = b1.x;
    for (auto &x : b2.x) {
        res.x[x.first] = res.x[x.first] - x.second;
    }

    return res;
}

template<typename T>
linear<T> operator*(const linear<T> &b1, const linear<T> &b2)
{
    linear<T> res(b1.d * b2.d);

    if (b1.isConstant()) {
        for (auto &x:b2.x)  res.x[x.first] = x.second * b1.d;
    }
    else if (b2.isConstant()) {
        for (auto &x : b1.x)  res.x[x.first] = x.second * b2.d;
    }
    else throw typename linear<T>::error("polynomial of order > 1 not allowed");

    return res;
}


template<typename T>
linear<T> operator/(const linear<T> &b1, const linear<T> &b2)
{
    if (!b2.isConstant()) throw typename linear<T>::error("polynomial fraction not allowed"); 
    else if(b2.d == 0)    throw typename linear<T>::error("division by zero");
    
    linear<T> q(1 / b2.d);
    return  q * b1;
}

template<typename T>
linear<T> operator-(const linear<T> &b)
{
    linear<T> zero(0);
    
    return zero - b;
}

template<typename T>
linear<T> log(const linear<T> &b)
{
    if (!b.isConstant()) throw typename linear<T>::error("log of polynomial not allowed");

    return log(b.d);
}

template<typename T>
linear<T> pow(const linear<T> &b1, const linear<T> &b2)
{
    if (!b2.isConstant()) throw typename linear<T>::error("polynomial exponent not allowed");
    if (!b1.isConstant()) throw typename linear<T>::error("power of polynomial not allowed");

    return pow(b1.d, b2.d);
}
#endif
