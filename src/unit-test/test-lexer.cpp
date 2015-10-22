// test-lexer.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "lexer.h"

TEST(Tokenize, Empty)
{
    std::vector<token> empty{ 
        { tok_t::end, "", 0 } 
    };
    EXPECT_EQ(tokenize(""), empty);
}

TEST(Tokenize, Number)
{
    std::vector<token> number{ 
        { tok_t::num, "1.125e-1", 3 },
        { tok_t::end, "", 14 }
    };
    EXPECT_EQ(tokenize("   1.125e-1   "), number);
}

TEST(Tokenize, Id)
{
    std::vector<token> id{ 
        { tok_t::id, "x", 0 },
        { tok_t::end, "", 1 } 
    };
    EXPECT_EQ(tokenize("x"), id);
}

TEST(Tokenize, Addition)
{
    std::vector<token> addition{ 
        { tok_t::num,   "1", 1 },
        { tok_t::punct, "+", 3 },
        { tok_t::id,    "x", 5 },
        { tok_t::end,   "", 9 }
    };
    EXPECT_EQ(tokenize(" 1 + x   "), addition);
}