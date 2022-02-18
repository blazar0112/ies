#include "ies/String/RecursiveReplace.hpp"

#include "gtest/gtest.h"

#include <iostream>

namespace ies
{

TEST(RecursiveReplace, Empty)
{
    std::string s;
    RecursiveReplace(s, "//", "/");
    ASSERT_EQ("", s);
}

TEST(RecursiveReplace, EmptyFrom)
{
    std::string s = "a///////b//c/d//e////";
    RecursiveReplace(s, "", "/");
    ASSERT_EQ("a///////b//c/d//e////", s);
}

TEST(RecursiveReplace, EmptyTo)
{
    std::string s = "a///////b//c/d//e////";
    RecursiveReplace(s, "//", "");
    ASSERT_EQ("a/bc/de", s);
}

TEST(RecursiveReplace, RecursiveReplace)
{
    std::string s = "a///////b//c/d//e////";
    RecursiveReplace(s, "//", "/");
    ASSERT_EQ("a/b/c/d/e/", s);
}

}
