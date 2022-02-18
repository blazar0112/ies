#include "ies/Common/IntRangeUtil.hpp"

#include "gtest/gtest.h"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace IntRangeUtil {

TEST(IntRangeUtil, FindOverlappingRange)
{
    IntRange range1{2, 5};
    IntRange range2{3, 7};
    IntRange range3{0, 2};
    IntRange range4{9, 12};
    IntRange range5{5, 6};
    IntRange expectEmptyRange13{2, 2, EmptyPolicy::Allow};
    IntRange expectEmptyRange14{5, 5, EmptyPolicy::Allow};
    EXPECT_EQ(std::size_t{0}, FindOverlappingRange(range1, range3).size());
    EXPECT_EQ(std::size_t{0}, FindOverlappingRange(range1, range4).size());
    EXPECT_EQ(expectEmptyRange13, FindOverlappingRange(range1, range3));
    EXPECT_EQ(expectEmptyRange14, FindOverlappingRange(range1, range4));

    IntRange expectEmptyRange{2, 2, EmptyPolicy::Allow};
    EXPECT_EQ(expectEmptyRange, FindOverlappingRange(expectEmptyRange13, expectEmptyRange14));

    IntRange expectOverlappingRange25{5, 6};
    EXPECT_EQ(expectOverlappingRange25, FindOverlappingRange(range2, range5));

    IntRange expectOverlappingRange{3, 5};
    ASSERT_EQ(expectOverlappingRange, FindOverlappingRange(range1, range2));
}

TEST(IntRangeUtil, FindOverlappingPortion)
{
    EXPECT_DOUBLE_EQ(0.0, FindOverlappingPortion(IntRange(2, 4), IntRange(4, 5)));
    EXPECT_DOUBLE_EQ(0.5, FindOverlappingPortion(IntRange(2, 4), IntRange(3, 4)));
    EXPECT_DOUBLE_EQ(1.0, FindOverlappingPortion(IntRange(3, 4), IntRange(2, 4)));
    EXPECT_DOUBLE_EQ(1.0, FindOverlappingPortion(IntRange(2, 4), IntRange(1, 5)));

    ASSERT_DOUBLE_EQ(0.4, FindOverlappingPortion(IntRange(3, 8), IntRange(5, 7)));
}

}}
