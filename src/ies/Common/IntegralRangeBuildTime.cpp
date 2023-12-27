#include "ies/Common/IntegralRangeUsing.hpp"

namespace IntegralRangeBuildTime
{

int Test(int &sum)
{
    for (auto i : IntRange(0, 10))
    {
        sum += i;
    }
    return sum;
}

} // namespace IntegralRangeBuildTime
