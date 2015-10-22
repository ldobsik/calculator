#include <iostream>
#include <vector>

#include <gtest/gtest.h>

#include "affine.h"

TEST(Affine, ConstructN)
{
    affine<double> a(5);

    EXPECT_EQ(a.d, 5);
    EXPECT_EQ(a.x.size(), 0);
}

TEST(Affine, ConstructX)
{
    affine<double> a(7, "x");

    EXPECT_EQ(a.d, 0);
    EXPECT_EQ(a.x["x"], 7);
}

TEST(Affine, Add)
{
    auto a = affine<double>(3,"x");
    auto b = affine<double>(4);
    auto r = a + b;

    EXPECT_EQ(r.x["x"], 3);
    EXPECT_EQ(r.d, 4);
}

TEST(Affine, Sub)
{
    auto a = affine<double>(8,"z");
    auto b = affine<double>(1);
    auto r = a - b;

    EXPECT_EQ(r.x["z"], 8);
    EXPECT_EQ(r.d, -1);
}

TEST(Affine, Mul)
{
    auto a = affine<double>(8,"y");
    auto b = affine<double>(7);
    auto r = a * b;

    EXPECT_EQ(r.x["y"], 56);
    EXPECT_EQ(r.d, 0);
}

TEST(Affine, Div)
{
    auto a = affine<double>(8,"y");
    auto b = affine<double>(4);
    auto r = a / b;

    EXPECT_EQ(r.x["y"], 2);
    EXPECT_EQ(r.d, 0);
}
