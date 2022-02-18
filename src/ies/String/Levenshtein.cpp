#include "ies/String/Levenshtein.hpp"

#include <algorithm>
#include <vector>

#include "ies/Common/IntegralRange.hxx"

namespace ies
{

std::pair<std::size_t, double>
DamerauLevenshtein(const std::string &str1, const std::string &str2)
{
    auto dim1Size = str1.size()+1;
    auto dim2Size = str2.size()+1;

    std::vector<std::vector<std::size_t>> distanceTable
    (
        dim1Size,
        std::vector<std::size_t>(dim2Size, 0)
    );

    for (auto i : IndexRange(0, dim1Size))
    {
        distanceTable[i][0] = i;
    }

    for (auto j : IndexRange(0, dim2Size))
    {
        distanceTable[0][j] = j;
    }

    for (auto i : IndexRange(1, dim1Size, EmptyPolicy::Allow))
    {
        for (auto j : IndexRange(1, dim2Size, EmptyPolicy::Allow))
        {
            std::size_t cost = 0;
            if (str1[i-1]!=str2[j-1])
            {
                cost = 1;
            }

            distanceTable[i][j] = std::min({distanceTable[i-1][j]+1,
                                            distanceTable[i][j-1]+1,
                                            distanceTable[i-1][j-1]+cost});

            if (i>1 && j>1 && str1[i-1]==str2[j-2] && str1[i-2]==str2[j-1])
            {
                distanceTable[i][j] = std::min(distanceTable[i][j], distanceTable[i-2][j-2]+1);
            }
        }
    }

    auto maxSize = std::max(str1.size(), str2.size());
    auto distance = distanceTable[dim1Size-1][dim2Size-1];
    if (maxSize==0)
    {
        return {distance, 1.0};
    }

    auto normalize = static_cast<double>(maxSize-distance)/maxSize;

    return {distance, normalize};
}

}
