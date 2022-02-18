#include "ies/Common/AdjacentArrayRange.hxx"

#include <list>
#include <map>

#include "gtest/gtest.h"

namespace ies
{

TEST(AdjacentArrayRange, N1)
{
    std::list<int> set{1, 2, 3, 4, 5};

    std::list<std::array<int, 1>> actualN1;
    for (auto itArray : MakeAdjacentArrayRange<1>(set))
    {
        auto array = ToValueArray(set, itArray);
        actualN1.emplace_back(array);
    }

    std::list<std::array<int, 1>> expectN1{{1}, {2}, {3}, {4}, {5}};

    ASSERT_EQ(expectN1, actualN1);
}

TEST(AdjacentArrayRange, N2)
{
    std::list<int> set{1, 2, 3, 4, 5};

    std::list<std::array<int, 2>> actualN2;
    for (auto itArray : MakeAdjacentArrayRange<2>(set))
    {
        std::array<int, 2> array{-1, -1};
        for (auto &i : UIntRange(0, 2))
        {
            array[i] = *itArray[i];
        }
        actualN2.emplace_back(array);
    }

    std::list<std::array<int, 2>> expectN2{{1, 2}, {2, 3}, {3, 4}, {4, 5}};

    ASSERT_EQ(expectN2, actualN2);
}

TEST(AdjacentArrayRange, N3)
{
    std::list<int> set{1, 2, 3, 4, 5};

    std::list<std::array<int, 3>> actualN3;
    for (auto itArray : MakeAdjacentArrayRange<3>(set))
    {
        auto array = ToValueArray(set, itArray);
        actualN3.emplace_back(array);
    }

    std::list<std::array<int, 3>> expectN3{{1, 2, 3}, {2, 3, 4}, {3, 4, 5}};

    ASSERT_EQ(expectN3, actualN3);
}

TEST(AdjacentArrayRange, EmptyN3)
{
    std::list<int> set{1, 2};

    std::list<std::array<int, 3>> actualN3;
    for (auto itArray : MakeAdjacentArrayRange<3>(set))
    {
        auto array = ToValueArray(set, itArray);
        actualN3.emplace_back(array);
    }

    std::list<std::array<int, 3>> expectN3{};

    ASSERT_EQ(expectN3, actualN3);
}

TEST(AdjacentArrayRange, StringN2)
{
    std::list<std::string> set{"a", "b", "c", "d"};

    std::list<std::array<std::string, 2>> actualN2;
    for (auto itArray : MakeAdjacentArrayRange<2>(set))
    {
        auto array = ToValueArray(set, itArray);
        actualN2.emplace_back(array);
    }

    std::list<std::array<std::string, 2>> expectN2{{"a", "b"}, {"b", "c"}, {"c", "d"}};

    ASSERT_EQ(expectN2, actualN2);
}

TEST(AdjacentArrayRange, Map)
{
    std::map<std::string, int> map{{"a", 1}, {"b", 2}, {"c", 3}, {"d", 4}};

    std::list<std::array<int, 2>> actual;
    for (auto itArray : MakeAdjacentArrayRange<2>(map))
    {
        auto array = ToValueArray(map, itArray);
        actual.emplace_back(array);
    }

    std::list<std::array<int, 2>> expect{{1, 2}, {2, 3}, {3, 4}};

    ASSERT_EQ(expect, actual);
}

}
