#include "ies/Type/IsMap.hxx"

#include "gtest/gtest.h"

#include <array>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

TEST(IsMap, Map)
{
    std::map<std::string, int> map{{"a", 2}, {"b", 5}};
    ASSERT_TRUE(IsMap<decltype(map)>::value);
}

TEST(IsMap, Set)
{
    std::set<int> set{2, 5};
    ASSERT_FALSE(IsMap<decltype(set)>::value);
}

TEST(IsMap, String)
{
    ASSERT_FALSE(IsMap<std::string>::value);
}

TEST(IsMap, Vector)
{
    std::vector<int> vector{2, 5};
    ASSERT_FALSE(IsMap<decltype(vector)>::value);
}

}}
