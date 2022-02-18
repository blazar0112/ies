#include "ies/StdUtil/IsIn.hxx"

#include "gtest/gtest.h"

#include <set>
#include <map>
#include <unordered_map>

namespace ies
{

TEST(IsIn, String)
{
    std::string s = "abcde";

    EXPECT_FALSE(IsIn(s, "1"));
    ASSERT_TRUE(IsIn(s, "a"));
}

TEST(IsIn, NonMap)
{
    std::set<int> s{0, 1, 2, 3, 4, 5};

    EXPECT_TRUE(IsIn(s, 1));
    EXPECT_FALSE(IsIn(s, 6));
}

TEST(IsIn, Map)
{
    std::map<std::string, int> m1{ {"a", 1}, {"b", 2} };
    std::unordered_map<std::string, int> m2{ {"a", 1}, {"b", 2} };

    EXPECT_TRUE(IsIn(m1, "a"));
    EXPECT_FALSE(IsIn(m1, "c"));
    EXPECT_TRUE(IsIn(m2, "a"));
    ASSERT_FALSE(IsIn(m2, "c"));
}

}
