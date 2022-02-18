#include "ies/Common/AdjacentVectorRange.hxx"

#include "gtest/gtest.h"

#include "ies/StdUtil/FormatString.hxx"

namespace ies
{

TEST(AdjacentVectorRange, DynamicSize)
{
    std::vector<int> v{1, 2, 3, 4, 5, 6};
    std::vector<int> expectCount{5, 4, 3, 2, 1};
    std::vector<int> actualCount{0, 0, 0, 0, 0};
    for (auto windowSize : IndexRange(2, 7))
    {
        for (auto itVector : MakeAdjacentVectorRange(v, windowSize))
        {
            ++actualCount[windowSize-2];
        }
    }

    ASSERT_EQ(expectCount, actualCount);
}

}
