#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <list>
#include <map>
#include <string>

#include "ies/Common/IntegralRange.hxx"
#include "ies/Common/RangeSide.hpp"
#include "ies/StdUtil/IsIn.hxx"
#include "ies/StdUtil/ReverseEqualRange.hxx"

namespace ies
{

//! @brief A list of non-overlapping IntegralRanges that can check if adding range overlap existing and space remain.
//! e.g. [2, 5)--[7, 10), HasSpace([5, 7)), AddRange([2, 3)) fails.
template <typename IntType>
class IntegralRangeList
{
public:
    //! @brief Add range to list if HasSpace, otherwise ignore the range.
        void
        AddRange(const IntegralRange<IntType> &range);

    //! @brief Check if has space in list for range.
        bool
        HasSpace(const IntegralRange<IntType> &range)
        const;

    //! @brief Check if list has any range containing value or not.
        bool
        HasRange(IntType value)
        const;

    //! @brief If HasRange(value), get corresponding Range containing value, otherwise throw exception.
        IntegralRange<IntType>
        GetRange(IntType value)
        const;

    //! @brief Get all ranges currently in list (dynamic generated.).
        std::list<IntegralRange<IntType>>
        GetRanges()
        const;

    //! @brief Find overlapping ranges in list and return as IntegralRangeList. (May be empty.)
        IntegralRangeList<IntType>
        FindOverlappingRanges(const IntegralRange<IntType> &range)
        const;

    //! @brief Find ranges adjacent to range begin with value rangeBegin.
    //! First: GetRange(rangeBegin).GetBegin()==rangeBegin, so if no range begins with rangeBegin throws exception.
    //! Second: check previous and next range (if available) in list is adjacent or not.
    //! If it's adjacent (a range's end equal to next range's begin), return by {RangeSide, Range} as map.
    //! @example IntegralRangeList is [0, 3)-[5, 7)-[7, 10)
    //! FindAdjacentRanges(5):
    //! 1. GetRange(5) = [5, 7)
    //! 2. Previous(Begin side) and next(End side) ranges of [5, 7) are [0, 3) and [7, 10)
    //!     Begin side: [0, 3) is not adjacent to [5, 7).
    //!     End side: [7, 10) is adjacanet to [5, 7).
    //! 3. Return {{RangeSide::End, [7, 10)}}
        std::map<RangeSide, IntegralRange<IntType>>
        FindAdjacentRanges(IntType rangeBegin)
        const;

private:
    std::map<IntType, IntType> mBeginToEndMap;
};

template <typename IntType>
void
IntegralRangeList<IntType>::
AddRange(const IntegralRange<IntType> &range)
{
    if (HasSpace(range))
    {
        mBeginToEndMap[range.GetBegin()] = range.GetEnd();
    }
}

template <typename IntType>
bool
IntegralRangeList<IntType>::
HasSpace(const IntegralRange<IntType> &range)
const
{
    return FindOverlappingRanges(range).GetRanges().empty();
}

template <typename IntType>
bool
IntegralRangeList<IntType>::
HasRange(IntType value)
const
{
    auto possibleBeginIt = mBeginToEndMap.upper_bound(value);
    if (possibleBeginIt==mBeginToEndMap.begin())
    {
        return false;
    }
    --possibleBeginIt;
    return IntegralRange<IntType>(possibleBeginIt->first, possibleBeginIt->second).IsInRange(value);
}

template <typename IntType>
IntegralRange<IntType>
IntegralRangeList<IntType>::
GetRange(IntType value)
const
{
    if (!HasRange(value))
    {
        throw std::runtime_error("GetRange(): cannot get range of value.");
    }
    auto it = ReverseLowerBound(mBeginToEndMap, value);
    if (it==mBeginToEndMap.rend())
    {
        throw std::runtime_error("GetRange(): logic error.");
    }
    return IntegralRange<IntType>(it->first, it->second);
}

template <typename IntType>
std::list<IntegralRange<IntType>>
IntegralRangeList<IntType>::
GetRanges()
const
{
    std::list<IntegralRange<IntType>> ranges;
    for (auto beginEnd : mBeginToEndMap)
    {
        ranges.emplace_back(beginEnd.first, beginEnd.second);
    }
    return ranges;
}

template <typename IntType>
IntegralRangeList<IntType>
IntegralRangeList<IntType>::
FindOverlappingRanges(const IntegralRange<IntType> &range)
const
{
    IntegralRangeList<IntType> overlappingRanges;

    auto possibleBeginIt = mBeginToEndMap.upper_bound(range.GetBegin());
    if (possibleBeginIt!=mBeginToEndMap.begin())
    {
        --possibleBeginIt;
    }

    auto possibleEndIt = mBeginToEndMap.lower_bound(range.GetEnd());

    for (; possibleBeginIt!=possibleEndIt&&possibleBeginIt!=mBeginToEndMap.end(); ++possibleBeginIt)
    {
        const IntegralRange<IntType> comparingRange{possibleBeginIt->first, possibleBeginIt->second};
        if (range.Overlaps(comparingRange))
        {
            overlappingRanges.AddRange(comparingRange);
        }
    }

    return overlappingRanges;
}

template <typename IntType>
std::map<RangeSide, IntegralRange<IntType>>
IntegralRangeList<IntType>::
FindAdjacentRanges(IntType rangeBegin)
const
{
    std::map<RangeSide, IntegralRange<IntType>> adjacentRanges;

    if (!IsIn(mBeginToEndMap, rangeBegin))
    {
        throw std::runtime_error("no range begins with ["+std::to_string(rangeBegin)+"].");
    }

    const IntegralRange<IntType> currentRange{rangeBegin, mBeginToEndMap.at(rangeBegin)};
    auto currentIt = mBeginToEndMap.find(rangeBegin);
    if (currentIt!=mBeginToEndMap.begin())
    {
        auto prev = std::prev(currentIt);
        if (prev->second==currentRange.GetBegin())
        {
            adjacentRanges.emplace(RangeSide::Begin, IntegralRange<IntType>{prev->first, prev->second});
        }
    }
    auto next = std::next(currentIt);
    if (next!=mBeginToEndMap.end())
    {
        if (next->first==currentRange.GetEnd())
        {
            adjacentRanges.emplace(RangeSide::End, IntegralRange<IntType>{next->first, next->second});
        }
    }
    return adjacentRanges;
}

}
