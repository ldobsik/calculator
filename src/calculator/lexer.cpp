#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>

#include "lexer.h"

using std::vector;
using std::string;

const vector<token> tokenize(const string &s)
{
    vector<token> result;
    size_t i = 0;

    for (; i < s.size();) {
        char c = s[i];
        if (isspace(c)) {
            i++; 
            continue;
        }

        if (isdigit(c) || c == '.') {
            // number
            const size_t numstart = i++;

            // fractional
            for (; i < s.size() && isdigit(s[i]); i++) {}
            if (c == '.') {
                if (i == numstart + 1) { // this was just a dot
                    result.push_back({ tok_t::punct, { c }, i-1 });
                    continue;
                }
            }
            else if(i < s.size() && s[i] == '.')  for (i++; i < s.size() && isdigit(s[i]); i++) {}

            // exponent
            const size_t expstart = i;
            if (i < s.size() && tolower(s[i]) == 'e') {
                i++;
                if (i < s.size() && (s[i] == '+' | s[i] == '-')) { i++; }
                if (i == s.size() || !isdigit(s[i])) {
                    i = expstart; // invalid exponent, recover to fractional only
                }
                for (; i < s.size() && isdigit(s[i]); i++) {};
            }

            result.push_back({ tok_t::num, s.substr(numstart,i-numstart), numstart });
        }
        else if (isalpha(c)) {
            // identifier
            const size_t idstart = i++;
            for (; i < s.size() && isalnum(s[i]); i++) {};
            result.push_back({ tok_t::id, s.substr(idstart,i-idstart), idstart});
        }
        else {
            // 1-character punctuator
            result.push_back({ tok_t::punct,{c}, i++ });
        }
    }

    result.push_back({ tok_t::end, "", s.size() });
    return result;
}
