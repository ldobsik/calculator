#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "lexer.h"
#include "parser.h"
#include "linear.h"


TEST(ParserDouble, Empty)
{
    std::vector<token> empty{
        { tok_t::end, "", 0 }
    };

    EXPECT_TRUE(parser<double>::parse(empty).empty());
}

TEST(ParserDouble, Expr1)
{
    std::vector<token> expr{  /*  (3+(4-1))*5 */
        { tok_t::punct, "(", 1 },
        { tok_t::num,   "3", 0 },
        { tok_t::punct, "+", 1 },
        { tok_t::punct, "(", 1 },
        { tok_t::num,   "4", 2 },
        { tok_t::punct, "-", 1 },
        { tok_t::num,   "1", 2 },
        { tok_t::punct, ")", 1 },
        { tok_t::punct, ")", 1 },
        { tok_t::punct, "*", 1 },
        { tok_t::num,   "5", 2 },
        { tok_t::end,   "",  3 }
    };

    EXPECT_EQ(parser<double>::parse(expr)[0].atom, 30.0);
}

TEST(ParserDouble, Equation1)
{
    std::vector<token> equation{  /* 2*x+0.5=1 */
        { tok_t::num,   "2", 0 },
        { tok_t::punct, "*", 1 },
        { tok_t::id,    "x", 2 },
        { tok_t::punct, "+", 3 },
        { tok_t::num,   "0.5", 4 },
        { tok_t::punct, "=", 7 },
        { tok_t::num,   "1", 8 },
        { tok_t::end,   "",  9 }
    };

    using atom = linear<double>;

    EXPECT_EQ(parser<linear<double>>::parse(equation)[0].atom, atom(2,"x")-atom(0.5));
    EXPECT_TRUE(parser<linear<double>>::parse(equation)[0].equal_to_zero);
}

