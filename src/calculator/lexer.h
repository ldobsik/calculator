#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// token types
enum class tok_t {
    end, num, id, punct
};


struct token {
      tok_t type;
      std::string s; // extracted token string
      size_t pos;    // position in the original string
};

std::vector<token> tokenize(const std::string &);

bool operator == (const token & t1, const token & t2);

#endif


