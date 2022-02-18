#include "ies/StdUtil/ReverseEqualRange.hxx"

#include "gtest/gtest.h"

#include <list>
#include <map>
#include <unordered_map>

namespace ies
{

TEST(ReverseEqualRange, ReverseLowerBound)
{
    std::vector<std::string> v1{ {"b"}, {"b"}, {"c"}, {"e"} };

    auto itA = ReverseLowerBound(v1, "a");
    EXPECT_EQ(v1.crend(), itA);

    auto itB = ReverseLowerBound(v1, "b");
    EXPECT_NE(v1.crend(), itB);
    auto indexB = v1.size()-static_cast<std::size_t>(itB-v1.crbegin())-1;
    EXPECT_EQ(1u, indexB);

    auto itC = ReverseLowerBound(v1, "c");
    auto indexC = v1.size()-static_cast<std::size_t>(itC-v1.crbegin())-1;
    EXPECT_EQ(2u, indexC);

    auto itD = ReverseLowerBound(v1, "d");
    auto indexD = v1.size()-static_cast<std::size_t>(itD-v1.crbegin())-1;
    EXPECT_EQ(indexC, indexD);

    auto itF = ReverseLowerBound(v1, "f");
    ASSERT_EQ(v1.crbegin(), itF);
}

TEST(ReverseEqualRange, ReverseLowerBoundMap)
{
    std::map<std::string, int> m1{ {"b", 1}, {"c", 2} };

    EXPECT_EQ(m1.crend(), ReverseLowerBound(m1, "a"));
    EXPECT_EQ(m1.crbegin(), ReverseLowerBound(m1, "c"));
    ASSERT_NE(m1.crend(), ReverseLowerBound(m1, "b"));
}

TEST(ReverseEqualRange, ReverseUpperBound)
{
    std::vector<std::string> v1{ {"b"}, {"b"}, {"c"}, {"e"} };

    auto itA = ReverseUpperBound(v1, "a");
    EXPECT_EQ(v1.crend(), itA);

    auto itB = ReverseUpperBound(v1, "b");
    EXPECT_EQ(v1.crend(), itB);

    auto itC = ReverseUpperBound(v1, "c");
    auto indexC = v1.size()-static_cast<std::size_t>(itC-v1.crbegin())-1;
    EXPECT_EQ(1u, indexC);

    auto itD = ReverseUpperBound(v1, "d");
    auto indexD = v1.size()-static_cast<std::size_t>(itD-v1.crbegin())-1;
    EXPECT_NE(indexC, indexD);
    EXPECT_EQ(2u, indexD);

    auto itF = ReverseUpperBound(v1, "f");
    ASSERT_EQ(v1.crbegin(), itF);
}

TEST(ReverseEqualRange, ReverseUpperBoundMap)
{
    std::map<std::string, int> m1{ {"b", 1}, {"c", 2} };

    EXPECT_EQ(m1.crend(), ReverseUpperBound(m1, "a"));
    EXPECT_EQ(--m1.crend(), ReverseUpperBound(m1, "c"));
    ASSERT_EQ(m1.crend(), ReverseUpperBound(m1, "b"));
}

TEST(ReverseEqualRange, ReverseEqualRange)
{
    std::vector<int> v{0, 1, 2, 2, 2, 3, 4, 4, 4, 5, 7};
    //'' index         0  1  2  3  4  5  6  7  8  9 10
    //'' equal range                  -> [        )
    //'' reverse equal range          (        ] <-

    auto equalRange = std::equal_range(v.begin(), v.end(), 4);
    auto indexLB = static_cast<std::size_t>(equalRange.first-v.cbegin());
    auto indexUB = static_cast<std::size_t>(equalRange.second-v.cbegin());

    EXPECT_EQ(6u, indexLB);
    EXPECT_EQ(9u, indexUB);

    auto reverseEqualRange = ReverseEqualRange(v, 4);
    auto indexRLB = v.size()-static_cast<std::size_t>(reverseEqualRange.first-v.crbegin())-1;
    auto indexRUB = v.size()-static_cast<std::size_t>(reverseEqualRange.second-v.crbegin())-1;

    EXPECT_EQ(8u, indexRLB);
    EXPECT_EQ(5u, indexRUB);

    std::vector<int> vEq{equalRange.first, equalRange.second};
    std::vector<int> vReq{reverseEqualRange.first, reverseEqualRange.second};

    ASSERT_EQ(vEq, vReq);
}

}
