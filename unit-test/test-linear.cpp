#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "linear.h"

TEST(Linear, ConstructN)
{
    linear<double> a{ 5 };

    EXPECT_EQ(a.d, 5);
    EXPECT_EQ(a.x.size(), 0);
}

TEST(Linear, ConstructX)
{
    linear<double> a{ "x" };

    EXPECT_EQ(a.d, 0);
    EXPECT_EQ(a.x["x"], 1);
}

TEST(Linear, Add)
{
    auto a = linear<double>(3,"x");
    auto b = linear<double>(4);
    auto r = a + b;

    EXPECT_EQ(r.x["x"], 3);
    EXPECT_EQ(r.d, 4);
}

TEST(Linear, Sub)
{
    auto a = linear<double>(8,"z");
    auto b = linear<double>(1);
    auto r = a - b;

    EXPECT_EQ(r.x["z"], 8);
    EXPECT_EQ(r.d, -1);
}

TEST(Linear, Mul)
{
    auto a = linear<double>(8,"y");
    auto b = linear<double>(7);
    auto r = a * b;

    EXPECT_EQ(r.x["y"], 56);
    EXPECT_EQ(r.d, 0);
}

TEST(Linear, Div)
{
    auto a = linear<double>(8,"y");
    auto b = linear<double>(4);
    auto r = a / b;

    EXPECT_EQ(r.x["y"], 2);
    EXPECT_EQ(r.d, 0);
}
