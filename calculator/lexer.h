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

      bool operator == (const token & t2) const { return type == t2.type && s == t2.s && pos == t2.pos; };
};

const std::vector<token> tokenize(const std::string &);

#endif


