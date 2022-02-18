#include "ies/Common/IntegralRangeList.hxx"

#include "gtest/gtest.h"

namespace ies
{

TEST(IntegralRangeList, HasSpace)
{
    IntegralRangeList<int> list;

    list.AddRange(IntRange(0, 5));

    EXPECT_TRUE(list.HasSpace(IntRange(5, 7)));
    ASSERT_FALSE(list.HasSpace(IntRange(1, 2)));
}

TEST(IntegralRangeList, HasRange)
{
    IntegralRangeList<int> list;

    list.AddRange(IntRange(7, 9));

    EXPECT_FALSE(list.HasRange(5));
    EXPECT_FALSE(list.HasRange(9));
    ASSERT_TRUE(list.HasRange(8));
}

TEST(IntegralRangeList, GetRange)
{
    IntegralRangeList<int> list;

    list.AddRange(IntRange(0, 5));
    list.AddRange(IntRange(7, 9));

    EXPECT_EQ(IntRange(7, 9), list.GetRange(7));
    ASSERT_EQ(IntRange(7, 9), list.GetRange(8));
}

TEST(IntegralRangeList, GetRangeUnsigned)
{
    IntegralRangeList<unsigned int> list;

    list.AddRange(UIntRange(0, 3));

    ASSERT_TRUE(list.HasRange(0));
}

TEST(IntegralRangeList, Bug)
{
    IntegralRangeList<unsigned long long> list;

    auto address1 = IntegralRange<unsigned long long>(0x40010100, 0x40010100+0xFF);
    EXPECT_TRUE(list.HasSpace(address1));
    list.AddRange(address1);

    auto address2 = IntegralRange<unsigned long long>(0x40010200, 0x40010200+0x3FFF);
    EXPECT_TRUE(list.HasSpace(address2));
    list.AddRange(address2);

    auto address3 = IntegralRange<unsigned long long>(0x45300000, 0x45300000+0x3FFF);
    EXPECT_TRUE(list.HasSpace(address3));
    list.AddRange(address3);

    auto address4 = IntegralRange<unsigned long long>(0x40300000, 0x40300000+0xFFFFF);
    EXPECT_TRUE(list.HasSpace(address4));
    list.AddRange(address4);

    auto address5 = IntegralRange<unsigned long long>(0x50000000, 0x50000000+0x2FFFFFFF);

    ASSERT_TRUE(list.HasSpace(address5));
}

TEST(IntegralRangeList, GetRanges)
{
    IntegralRangeList<int> list;

    list.AddRange(IntRange(0, 3));
    list.AddRange(IntRange(6, 7));

    std::list<IntRange> expectedList{{0, 3}, {6, 7}};

    ASSERT_EQ(expectedList, list.GetRanges());
}

TEST(IntegralRangeList, FindOverlappingRanges)
{
    IntegralRangeList<int> list;

    list.AddRange(IntRange(0, 3));
    list.AddRange(IntRange(4, 5));
    list.AddRange(IntRange(7, 9));
    list.AddRange(IntRange(10, 12));

    std::list<IntRange> expectedList{{4, 5}, {7, 9}, {10, 12}};

    auto overlappingList = list.FindOverlappingRanges({4, 11});

    ASSERT_EQ(expectedList, overlappingList.GetRanges());
}

TEST(IntegralRangeList, FindAdjacentRanges)
{
    IntegralRangeList<int> list;

    EXPECT_ANY_THROW(
        list.FindAdjacentRanges(3);
    );

    list.AddRange(IntRange(3, 6));

    EXPECT_EQ(0u, list.FindAdjacentRanges(3).size());

    list.AddRange(IntRange(2, 3));

    EXPECT_EQ(1u, list.FindAdjacentRanges(3).size());
    EXPECT_EQ(IntRange(2, 3), list.FindAdjacentRanges(3).at(RangeSide::Begin));

    list.AddRange(IntRange(6, 7));
    EXPECT_EQ(2u, list.FindAdjacentRanges(3).size());
    EXPECT_EQ(IntRange(6, 7), list.FindAdjacentRanges(3).at(RangeSide::End));

    list.AddRange(IntRange(9, 13));
    list.AddRange(IntRange(13, 14));

    EXPECT_EQ(1u, list.FindAdjacentRanges(13).size());
    ASSERT_EQ(IntRange(9, 13), list.FindAdjacentRanges(13).at(RangeSide::Begin));
}

}
