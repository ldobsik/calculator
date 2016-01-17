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

    struct error {
        const std::string msg;
        const token  t;
        error(const token *it, const std::string &im) :msg(im), t(*it) {};
    };

    static std::vector<result> parse(const std::vector<token>&);
private:

    enum class expr_rule {
        additive,
        multiplicative,
        unary,
        power,
        primary,
        parentheses,
    };

    const token* pt;

    parser(const token* it) : pt(it) {};
    T parse_expr(const expr_rule);
    result parse_eq();
    std::vector<result> parse_list();
};


template<typename T>
T parser<T>::parse_expr(const expr_rule cr)
{
    const token* ot = pt;   // operator token (for diagnostics)
    T result{ 0 };
    try {
        switch (cr) {
        case expr_rule::additive:
            result = parse_expr(expr_rule::multiplicative);
            for (;;) {
                if (pt->s == "+") {
                    ot = pt;
                    pt++;
                    result = result + parse_expr(expr_rule::multiplicative);
                }
                else if (pt->s == "-") {
                    ot = pt;
                    pt++;
                    result = result - parse_expr(expr_rule::multiplicative);
                }
                else break;
            }
            break;
        case expr_rule::multiplicative:
            result = parse_expr(expr_rule::unary);
            for (;;) {
                if (pt->s == "*") {
                    ot = pt;
                    pt++;
                    result = result * parse_expr(expr_rule::unary);
                }
                else if (pt->s == "/") {
                    ot = pt;
                    pt++;
                    result = result / parse_expr(expr_rule::unary);
                }
                else break;
            }
            break;
        case expr_rule::unary:
            if (pt->s == "-") {
                ot = pt;
                pt++;
                result = -parse_expr(expr_rule::unary);
            }
            else if (pt->s == "+") {
                ot = pt;
                pt++;
                result = parse_expr(expr_rule::unary);
            }
            else {
                result = parse_expr(expr_rule::power);
            }
            break;
        case expr_rule::power:
            result = parse_expr(expr_rule::primary);
            if (pt->s == "^") {
                ot = pt;
                pt++;
                result = pow(result, parse_expr(expr_rule::unary));
            }
            break;
        case expr_rule::primary:
            if (pt->type == tok_t::num) {
                std::istringstream ss(pt->s);
                ss >> result;
                if(ss.fail()) throw error(pt, "unable to parse the input as a floating point number");
                pt++;
            }
            else if (pt->type == tok_t::id) {
                if (pt->s == "log") {
                    // function
                    ot = pt;
                    pt++;
                    T arg = parse_expr(expr_rule::parentheses);
                    result = log(arg);
                }
                else {
                    //variable
                    std::istringstream ss(pt->s);
                    ot = pt;
                    ss >> result;
                    if (ss.fail()) throw  error(pt, "backend can't handle variables");
                    pt++;
                }
            }
            else if (pt->s == "(") {
                result = parse_expr(expr_rule::parentheses);
            }
            else throw error(pt, "missing operand");
            break;
        case expr_rule::parentheses:
            if (pt->s != "(") throw error(pt, "missing left parenthesis");
            pt++;
            result = parse_expr(expr_rule::additive);
            if (pt->s != ")") throw error(pt, "missing right parenthesis");
            pt++;
            break;
        }
    }
    catch (std::exception &e) {
        throw error(ot, e.what());
    }

    return result;
}

template<typename T>
typename parser<T>::result parser<T>::parse_eq()
{
    T lhs = parse_expr(expr_rule::additive);

    if (pt->s != "=") return result{ lhs, false };
    
    const token* ot = pt;
    pt++;

    try {
        lhs = lhs - parse_expr(expr_rule::additive);
    }
    catch (std::exception &e) {
        throw error(ot, e.what());
    }

    return result{ lhs, true };
}

template<typename T>
std::vector<typename parser<T>::result> parser<T>::parse_list()
{
    std::vector<result> r;

    if (pt->type == tok_t::end) return r;

    r.push_back(parse_eq());

    for (;;) {
        if (pt->type == tok_t::end) return r;
        if (pt->s != ",") throw error(pt, "unexpected input");
        pt++;
        r.push_back(parse_eq());
    }
}

template<typename T>
std::vector<typename parser<T>::result> parser<T>::parse(const std::vector<token>& vt)
{
    assert(vt.back().type == tok_t::end);

    parser<T> p(&vt[0]);

    return p.parse_list();
}

#endif
