#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

enum class tok_t {
    end, num, id, punct
};

struct token {
      tok_t type;
      std::string s;
      size_t pos;
};

std::vector<token> tokenize(const std::string &);

bool operator == (const token & t1, const token & t2);

#endif


