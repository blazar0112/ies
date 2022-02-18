#include "ies/String/SplitString.hpp"

#include "gtest/gtest.h"

#include <iostream>

namespace ies
{

TEST(SplitString, EmptyString)
{
    auto args = SplitString("", "");
    ASSERT_EQ(static_cast<std::size_t>(0), args.size());
}

TEST(SplitString, SplitString)
{
    auto args = SplitString("() ,", "(x,y), ,(a b)");

    EXPECT_EQ(4u, args.size());
    EXPECT_EQ("x", args[0]);
    EXPECT_EQ("y", args[1]);
    EXPECT_EQ("a", args[2]);
    EXPECT_EQ("b", args[3]);

    std::string input = "a,b,c,,,e";
    auto tokens = SplitString(",", input);

    ASSERT_EQ(4u, tokens.size());
}

TEST(SplitString, PreserveEmpty)
{
    std::string input = ",,b,,,e,,";
    auto tokens = SplitStringPreserve(",", input, 8);

    EXPECT_EQ("", tokens[0]);
    EXPECT_EQ("", tokens[1]);
    EXPECT_EQ("b", tokens[2]);
    EXPECT_EQ("", tokens[3]);
    EXPECT_EQ("", tokens[4]);
    EXPECT_EQ("e", tokens[5]);
    EXPECT_EQ("", tokens[6]);
    EXPECT_EQ("", tokens[7]);

    ASSERT_EQ(8u, tokens.size());
}

}
