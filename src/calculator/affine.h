#ifndef AFFINE_H
#define AFFINE_H

#include <algorithm>
#include <sstream>
#include <iostream>
#include <string>
#include <cmath>
#include <exception>
#include <map>

template<typename T>
struct affine {
    struct error : public std::exception {
        const std::string msg;
        error(const std::string & im) :msg(im) {};

        virtual const char* what() const throw()
        {
            return msg.c_str();
        }
    };

    affine(const T& id = 0) : d(id) {};
    affine(const T& ix, const std::string& iname) : d(0) { x[iname] = ix; };
    template<typename U>
    friend std::ostream& operator << (std::ostream& os, const affine<U>& b);
    
    bool isConstant() const { return std::all_of(x.begin(), x.end(), [](auto &x) {return x.second == 0;}); };

    std::map<std::string, T> x;
    T d;

private:
    std::string prettyPrintTerm(const std::pair<std::string, T> & t) const;
};

template<typename T>
std::istream& operator>> (std::istream& is, affine<T>& b)
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
std::string affine<T>::prettyPrintTerm(const std::pair<std::string, T> & t) const
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
std::ostream& operator<< (std::ostream& os, const affine<T>& b)
{
    bool is_first = true;
    for (auto &x: b.x) {
        if (x.second == 0) continue;
        if (!is_first) {
            if(x.second > 0) os << " + ";
            else os << " - ";
        }
        os << b.prettyPrintTerm(x).substr(!is_first && x.second < 0);
        is_first = false;
    }

    if (b.d != 0 || is_first) {
        if (!is_first) {
            if (b.d > 0) os << " + ";
            else os << " - ";
        }
        os << b.prettyPrintTerm(std::make_pair("", b.d)).substr(!is_first && b.d < 0);
    }


    return os;
}

template<typename T>
bool operator==(const affine<T> &b1, const affine<T> &b2)
{
    auto diff = b1 - b2;

    return (diff.d == 0) && diff.isConstant();
}

template<typename T>
affine<T> operator+(const affine<T> &b1, const affine<T> &b2)
{
    affine<T> res(b1.d + b2.d);

    res.x = b1.x;
    for (auto &x: b2.x) {
        res.x[x.first] = res.x[x.first] + x.second;
    }
    
    return res;
}

template<typename T>
affine<T> operator-(const affine<T> &b1, const affine<T> &b2)
{
    affine<T> res(b1.d - b2.d);

    res.x = b1.x;
    for (auto &x : b2.x) {
        res.x[x.first] = res.x[x.first] - x.second;
    }

    return res;
}

template<typename T>
affine<T> operator*(const affine<T> &b1, const affine<T> &b2)
{
    affine<T> res(b1.d * b2.d);

    if (b1.isConstant()) {
        for (auto &x:b2.x)  res.x[x.first] = x.second * b1.d;
    }
    else if (b2.isConstant()) {
        for (auto &x : b1.x)  res.x[x.first] = x.second * b2.d;
    }
    else throw typename affine<T>::error("polynomial of order > 1 not allowed");

    return res;
}


template<typename T>
affine<T> operator/(const affine<T> &b1, const affine<T> &b2)
{
    if (!b2.isConstant()) throw typename affine<T>::error("polynomial fraction not allowed"); 
    else if(b2.d == 0)    throw typename affine<T>::error("division by zero");
    
    affine<T> q(1 / b2.d);
    return  q * b1;
}

template<typename T>
affine<T> operator-(const affine<T> &b)
{
    affine<T> zero(0);
    
    return zero - b;
}

template<typename T>
affine<T> log(const affine<T> &b)
{
    if (!b.isConstant()) throw typename affine<T>::error("log of polynomial not allowed");

    return log(b.d);
}

template<typename T>
affine<T> pow(const affine<T> &b1, const affine<T> &b2)
{
    if (!b2.isConstant()) throw typename affine<T>::error("polynomial exponent not allowed");
    if (!b1.isConstant()) throw typename affine<T>::error("power of polynomial not allowed");

    return pow(b1.d, b2.d);
}
#endif
