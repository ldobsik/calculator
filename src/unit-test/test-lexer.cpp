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

TEST(Tokenize, Number1)
{
    std::vector<token> number{ 
        { tok_t::num, "1.125e-1", 3 },
        { tok_t::end, "", 14 }
    };
    EXPECT_EQ(tokenize("   1.125e-1   "), number);
}

TEST(Tokenize, Number2)
{
    std::vector<token> number{
        { tok_t::num, ".1e+1", 0 },
        { tok_t::end, "", 5 }
    };
    EXPECT_EQ(tokenize(".1e+1"), number);
}

TEST(Tokenize, Number3)
{
    std::vector<token> number{
        { tok_t::num, "1.1e-1", 0 },
        { tok_t::end, "", 6 }
    };
    EXPECT_EQ(tokenize("1.1e-1"), number);
}

TEST(Tokenize, Number4)
{
    std::vector<token> number{
        { tok_t::num, "1.e1", 0 },
        { tok_t::end, "", 4 }
    };
    EXPECT_EQ(tokenize("1.e1"), number);
}

TEST(Tokenize, VerySmallNumber)
{
    std::vector<token> number{
        { tok_t::num, "1e-1000", 3 },
        { tok_t::end, "", 13 }
    };
    EXPECT_EQ(tokenize("   1e-1000   "), number);
}

TEST(Tokenize, VeryBigNumber)
{
    std::vector<token> number{
        { tok_t::num, "1e+1000", 3 },
        { tok_t::end, "", 13 }
    };
    EXPECT_EQ(tokenize("   1e+1000   "), number);
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
