#include "ies/String/SplitStringView.hpp"

#include "gtest/gtest.h"

#include <iostream>

namespace ies
{

TEST(SplitStringView, SplitStringView)
{
    auto args = SplitStringView("() ,", "(x,y), ,(a b)");

    EXPECT_EQ(4u, args.size());
    EXPECT_EQ("x", args[0]);
    EXPECT_EQ("y", args[1]);
    EXPECT_EQ("a", args[2]);
    EXPECT_EQ("b", args[3]);

    std::string input = "a,b,c,,,e";
    auto tokens = SplitStringView(",", input);

    std::string_view inputView = input;
    auto emptyView = inputView.substr(2, 0);
    EXPECT_EQ("", emptyView);

    ASSERT_EQ(4u, tokens.size());
}

TEST(SplitStringView, PreserveEmpty)
{
    std::string input = ",,b,,,e,,";
    auto tokens = SplitStringViewPreserve(",", input, 8);

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
