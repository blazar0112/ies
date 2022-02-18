#include "ies/Common/IntegralRange.hxx"

#include <limits>
#include <list>

#include "gtest/gtest.h"

namespace ies
{

TEST(IntegralRange, EmptyRange)
{
    IntRange range1{-2, -2, EmptyPolicy::Allow};
    ReverseIntRange range2{std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max(), EmptyPolicy::Allow};
    UIntRange range3{0, 0, EmptyPolicy::Allow};

    EXPECT_THROW(
        {
            IntRange nonEmptyRange(0, 0);
        },
        std::runtime_error
    );

    std::list<int> actual1;
    std::list<int> expected1;
    for (auto i : range1)
    {
        actual1.emplace_back(i);
    }

    EXPECT_EQ(expected1, actual1);

    std::list<int> actual2;
    std::list<int> expected2;
    for (auto i : range2)
    {
        actual2.emplace_back(i);
    }

    EXPECT_EQ(expected2, actual2);

    std::list<int> actual3;
    std::list<int> expected3;
    for (auto i : range3)
    {
        actual3.emplace_back(i);
    }

    EXPECT_EQ(expected3, actual3);

    EXPECT_EQ(-2, range1.GetBegin());
    EXPECT_EQ(-2, range1.GetEnd());
    EXPECT_EQ(-2, range1.GetMin());
    EXPECT_THROW(
        {
            range1.GetMax();
        },
        std::runtime_error
    );

    EXPECT_TRUE(range1.empty());
    EXPECT_TRUE(range2.empty());
    ASSERT_TRUE(range3.empty());
}

TEST(IntegralRange, Forward)
{
    IntRange range{0, 5};

    std::list<int> actual;
    std::list<int> expected{0, 1, 2, 3, 4};

    for (auto &element: range)
    {
        actual.emplace_back(element);
    }

    EXPECT_EQ(5u, range.size());
    EXPECT_FALSE(range.empty());
    ASSERT_EQ(expected, actual);
}

TEST(IntegralRange, Reverse)
{
    ReverseIntRange revRange{0, 5};

    std::list<int> actual;
    std::list<int> expected{4, 3, 2, 1, 0};

    for (auto &element: revRange)
    {
        actual.emplace_back(element);
    }

    ASSERT_EQ(expected, actual);
}

TEST(IntegralRange, UIntReverse)
{
    //'' test that reverse does not go <0.
    ReverseUIntRange range{0, 3};

    std::list<int> actual;
    std::list<int> expected{2, 1, 0};

    for (auto &element: range)
    {
        actual.emplace_back(element);
    }

    ASSERT_EQ(expected, actual);
}

TEST(IntegralRange, Contains)
{
    IntRange range{-1, 7};
    ReverseIntRange otherRange{-1, 5};
    ReverseIntRange otherRange2{3, 7};
    ReverseIntRange otherRange3{3, 9};
    ReverseIntRange otherRange4{9, 11};
    ReverseIntRange otherRange5{-3, -1};
    EXPECT_TRUE(range.Contains(otherRange2));
    EXPECT_FALSE(range.Contains(otherRange3));
    EXPECT_FALSE(range.Contains(otherRange4));
    EXPECT_FALSE(range.Contains(otherRange5));

    ASSERT_TRUE(range.Contains(otherRange));
}

TEST(IntegralRange, Overlaps)
{
    IntRange range{-1, 3};
    ReverseIntRange otherRange{2, 5};

    ASSERT_TRUE(range.Overlaps(otherRange));
}

TEST(IntegralRange, NotLimitTo32BitInt)
{
    IntegralRange<int64_t> range{static_cast<int64_t>(std::numeric_limits<int32_t>::max())+1,
                                 static_cast<int64_t>(std::numeric_limits<int32_t>::max())+100};

    ASSERT_EQ(99u, range.size());
}

TEST(IntegralRange, ClosestAndDistance)
{
    IntRange range{0, 5};

    EXPECT_EQ(static_cast<int>(0), range.FindClosest(-2));
    EXPECT_EQ(static_cast<int>(0), range.FindClosest(0));
    EXPECT_EQ(3, range.FindClosest(3));
    EXPECT_EQ(5, range.FindClosest(5));
    EXPECT_EQ(5, range.FindClosest(8));

    EXPECT_EQ(2, range.FindDistance(-2));
    EXPECT_EQ(static_cast<int>(0), range.FindDistance(0));
    EXPECT_EQ(static_cast<int>(0), range.FindDistance(3));
    EXPECT_EQ(static_cast<int>(0), range.FindDistance(5));
    EXPECT_EQ(3, range.FindDistance(8));

    ASSERT_EQ(static_cast<int>(0), range.FindDistance(3));
}

TEST(IntegralRange, Get)
{
    IntRange range{-1, 3};
    ReverseIntRange revRange{2, 5};

    EXPECT_EQ(-1, range.Get(RangeSide::Begin));
    EXPECT_EQ(3, range.Get(RangeSide::End));
    EXPECT_EQ(-1, range.Get(Extremum::Min));
    EXPECT_EQ(2, range.Get(Extremum::Max));
    EXPECT_EQ(2, revRange.Get(RangeSide::Begin));
    ASSERT_EQ(5, revRange.Get(RangeSide::End));
}

TEST(IntegralRange, Shift)
{
    IntRange range{-1, 3};
    auto shifted = Shift(range, -2);
    IntRange expect{-3, 1};

    ASSERT_EQ(expect, shifted);
}

TEST(IntegralRange, ToString)
{
    IntRange range{-1, 3};
    auto s = ToString(range);

    ASSERT_EQ("[-1, 3)", s);
}

}
