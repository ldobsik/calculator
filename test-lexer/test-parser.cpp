#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "lexer.h"
//#include "parser.h"



TEST(ParserDouble, Empty)
{
    std::vector<token> empty{
        { tok_t::end, "", 0 }
    };

//    std::vector<std::pair<double, bool>> v;

  //  EXPECT_EQ(parser<double>().parse(empty), v);
}

