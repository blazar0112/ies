#include "ies/StdUtil/EraseIf.hxx"

#include "gtest/gtest.h"

#include <set>
#include <map>

namespace ies
{

TEST(EraseIf, Set)
{
    std::set<int> set{1, 2, 3};
    EraseIf(set, [](const int &key){ return key%2==1; });

    EXPECT_EQ(1u, set.size());
    ASSERT_EQ(std::set<int>{2}, set);
}

TEST(EraseIf, Map)
{
    std::map<std::string, int> map{{"a", 1}, {"b", 2}, {"ab", 3}, {"abc", 4}};
    EraseIf(map, [](const decltype(map)::value_type &pair){ return pair.first[0]=='a' && pair.second%3==1; });

    std::map<std::string, int> expectMap{{"b", 2}, {"ab", 3}};

    EXPECT_EQ(2u, map.size());
    ASSERT_EQ(expectMap, map);
}

}
