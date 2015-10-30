#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>

#include "lexer.h"

using std::vector;
using std::string;

//-------------------------------------------------------------------
static inline void skipDigits_(const string &s, size_t &i)
{
  for (; i < s.size() && isdigit(s[i]); i++) {}
}

vector<token> tokenize(const string &s)
{
    vector<token> result;
    
    for (size_t i = 0; i < s.size();) {
        char c = s[i];
        if (isspace(c)) {
            i++; 
            continue;
        }

        if (isdigit(c) || c == '.') {
            // number
            const size_t numstart = i;

            // fractional 
            skipDigits_(s, i);
            if (s[i] == '.') { 
                i++;
                skipDigits_(s, i);

                if (i == numstart + 1) {
                    // this was just a dot
                    result.push_back({ tok_t::punct,{ c }, numstart });
                    continue;
                }
            }

            // exponent
            const size_t expstart = i;
            if (i < s.size() && tolower(s[i]) == 'e') {
                i++;
                if (i < s.size() && (s[i] == '+' || s[i] == '-')) { i++; }
                if (i == s.size() || !isdigit(s[i])) {
                    i = expstart; // invalid exponent, recover to fractional only
                }
                else {
                    skipDigits_(s, i);
                }
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

//-------------------------------------------------------------------

bool operator == (const token & t1, const token & t2)
{
    return (t1.type == t2.type) && (t1.s == t2.s) && (t1.pos == t2.pos);
};
