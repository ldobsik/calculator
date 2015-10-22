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

        if (c >= '0' && c <= '9' || c == '.') {
            // number
            size_t numlen = 0;
            std::stod(s.substr(i), &numlen);
            result.push_back({ tok_t::num, s.substr(i,numlen), i});
            i += numlen;
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
