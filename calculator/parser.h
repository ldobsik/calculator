#ifndef PARSER_H
#define PARSER_H

#include <sstream>
#include <iterator>
#include <functional>
#include <cassert>
#include <vector>
#include <map>
#include <type_traits>
#include <cmath>
#include <exception>
#include <utility>
#include <iostream>

#include "lexer.h"

//-------------------------------------------------------

template<typename T>
class parser {
public:
    struct result {
        T    atom;
        bool equal_to_zero;
    };

    struct parerror {
        const std::string msg;
        const token  t;
        parerror(const token *it, const std::string &im) :msg(im), t(*it) {};
    };

    std::vector<result> parse(const token*);
private:

    enum class expr_rule {
        additive,
        multiplicative,
        unary,
        power,
        primary,
        parentheses,
        function,
    };

    T parse_expr(const token*&, const expr_rule);
    result parse_eq(const token*&);
};


template<typename T>
T parser<T>::parse_expr(const token*& pt, const expr_rule cr)
{
    const token* ot = pt;
    T result(0);
    try {
        switch (cr) {
        case expr_rule::additive:
            result = parse_expr(pt, expr_rule::multiplicative);
            for (;;) {
                if (pt->s == "+") {
                    ot = pt;
                    pt++;
                    result = result + parse_expr(pt, expr_rule::multiplicative);
                }
                else if (pt->s == "-") {
                    ot = pt;
                    pt++;
                    result = result - parse_expr(pt, expr_rule::multiplicative);
                }
                else break;
            }
            break;
        case expr_rule::multiplicative:
            result = parse_expr(pt, expr_rule::unary);
            for (;;) {
                if (pt->s == "*") {
                    ot = pt;
                    pt++;
                    result = result * parse_expr(pt, expr_rule::unary);
                }
                else if (pt->s == "/") {
                    ot = pt;
                    pt++;
                    result = result / parse_expr(pt, expr_rule::unary);
                }
                else break;
            }
            break;
        case expr_rule::unary:
            if (pt->s == "-") {
                ot = pt;
                pt++;
                result = -parse_expr(pt, expr_rule::unary);
            }
            else if (pt->s == "+") {
                ot = pt;
                pt++;
                result = parse_expr(pt, expr_rule::unary);
            }
            else {
                result = parse_expr(pt, expr_rule::power);
            }
            break;
        case expr_rule::power:
            result = parse_expr(pt, expr_rule::primary);
            if (pt->s == "^") {
                ot = pt;
                pt++;
                result = pow(result, parse_expr(pt, expr_rule::unary));
            }
            break;
        case expr_rule::primary:
            if (pt->type == tok_t::num) {
                std::istringstream ss(pt->s);
                ss >> result;
                pt++;
            }
            else if (pt->type == tok_t::id) {
                if (pt->s == "log") {
                    // function
                    ot = pt;
                    pt++;
                    T arg = parse_expr(pt, expr_rule::parentheses);
                    result = log(arg);
                }
                else {
                    //variable
                    std::istringstream ss(pt->s);
                    try {
                        ss >> result;
                        if (ss.fail()) throw 0;
                    }
                    catch (...) {
                        throw parerror(pt, "backend can't handle variables");
                    }
                    pt++;
                }
            }
            else if (pt->s == "(") {
                result = parse_expr(pt, expr_rule::parentheses);
            }
            else throw parerror(pt, "missing operand");
            break;
        case expr_rule::parentheses:
            if (pt->s != "(") throw parerror(pt, "missing left parenthesis");
            pt++;
            result = parse_expr(pt, expr_rule::additive);
            if (pt->s != ")") throw parerror(pt, "missing right parenthesis");
            pt++;
            break;
        case expr_rule::function:
            break;
        }
    }
    catch (std::exception &e) {
        throw parerror(ot, e.what());
    }

    return result;
}

template<typename T>
typename parser<T>::result parser<T>::parse_eq(const token*& pt)
{
    T lhs = parse_expr(pt, expr_rule::additive);

    if (pt->s != "=") return result{ lhs, false };
    
    const token* ot = pt;
    pt++;

    try {
        lhs = lhs - parse_expr(pt, expr_rule::additive);
    }
    catch (std::exception &e) {
        throw parerror(ot, e.what());
    }

    return result{ lhs, true };
}

template<typename T>
std::vector<typename parser<T>::result> parser<T>::parse(const token* pt)
{
    std::vector<result> r;

    if (pt->type == tok_t::end) return r;

    r.push_back(parse_eq(pt));

    for (;;) {
        if (pt->type == tok_t::end) return r;
        if (pt->s != ",") throw parerror(pt, "unexpected input");
        pt++;
        r.push_back(parse_eq(pt));
    }
}

#endif
