#include <vector>
#include <string>
#include <iostream>

#include "lexer.h"
#include "parser.h"
#include "affine.h"

using numtype  = double;
using atomtype = affine<numtype>;
using restype  = parser<atomtype>::result;


void printAnswer(restype &z)
{
    std::cout << "Answer: ";

    if (!z.equal_to_zero) 
    {
        // expression
        std::cout << z.atom << std::endl;
    }
    else 
    {
        //equation
        auto &d = z.atom.d; // rhs
        auto &x = z.atom.x;

        // collect free variables
        std::vector<std::string> free_vars;
        for (auto i = begin(x); i != end(x);) {
            if (i->second == numtype(0)) {
                free_vars.push_back(i->first);
                i = x.erase(i);
            }
            else ++i;
        }

        // no fixed variables
        if (x.size() == 0) {
            if (d == 0) std::cout << "True.";
            else        std::cout << "Not true.";
        }
        // 1 fixed variable
        else if (x.size() == 1) {
            std::cout << begin(x)->first << " = " << -d / begin(x)->second;
        }
        // more than 1 fixed variable
        else {
            std::cout << z.atom << " = 0";
        }

        if (free_vars.size() != 0) {
            std::cout << "    This holds for any ";
            std::copy(begin(free_vars), end(free_vars), std::ostream_iterator<std::string>(std::cout, ","));
            std::cout << "\b.";
        }
        std::cout << std::endl;
    }
}

int main()
{
    for (;;) {
        std::string input;

        std::getline(std::cin, input);
        if (input.empty()) break;       // empty input line exits the application 

        auto t = tokenize(input);
        try {
            auto result = parser<atomtype>::parse(t);
            for (auto &z : result) {
                // iterate over all comma-separated equations/expressions
                printAnswer(z);
            }

        }
        catch (parser<atomtype>::error &e) {
            auto pos = e.t.pos;
            std::cerr << std::endl << input << std::endl;
            std::cerr << std::string(pos, ' ') << "^~~~~ " << e.msg << std::endl;
        }
    }

    return 0;
}
