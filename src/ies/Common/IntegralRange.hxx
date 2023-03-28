#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

//'' disable Warning C4127 conditional expression is constant.
//'' Because this is designed to be C++11 compatible, cannot use C++17 if constexpr.
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#endif

#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>

#include "ies/Common/Extremum.hpp"
#include "ies/Common/RangeSide.hpp"
#include "ies/Common/SmartEnum.hxx"

//! @brief IntegralRange: a range [beginValue, endValue) of integral types.
//! Type satisfy std::is_integral, e.g. int, unsigned long long.
//! Default is iterating forwardly, can use RangeDirection::Reverse to iterating reverserly.
//! @note RangeDirection::Reverse only affect iterating direction,
//! range is still [beginValue, endValue), not (beginValue, endValue], see example.
//! @note consider use boost::irange for simple usage.
//! Use IntRange if you need IntRange function interface or reduce boost compile effort.
//!
//! @note 6 using are provided for convienence:
//! ies::IntRange = ies::IntegralRange<int>
//! ies::ReverseIntRange = ies::IntegralRange<int, IntegralRangeDirections::Reverse>
//! ies::UIntRange = ies::IntegralRange<unsigned int>
//! ies::ReverseUIntRange = ies::IntegralRange<unsigned int, IntegralRangeDirections::Reverse>
//! ies::IndexRange = ies::IntegralRange<std::size_t>
//! ies::ReverseIndexRange = ies::IntegralRange<std::size_t, IntegralRangeDirections::Reverse>
//!
//! @example
//! for (auto i : IntRange(0, 5)) { std::cout << i; }
//! // result: 01234
//! for (auto i : ReverseIntRange(0, 5)) { std::cout << i; }
//! // result: 43210 NOT 54321!! since range is [0, 5), not (0, 5]

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Detail {

template <typename IntType>
class IntegralRangeIterator
{
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = IntType;
    using difference_type = std::ptrdiff_t;
    using pointer = IntType*;
    using reference = IntType&;

        IntegralRangeIterator(IntType current)
        :   mCurrent(current)
        {}

        bool
        operator==(const IntegralRangeIterator &rhs)
        const
        {
            return mCurrent==rhs.mCurrent;
        }

        bool
        operator!=(const IntegralRangeIterator &rhs)
        const
        {
            return !(*this==rhs);
        }

        IntType &
        operator*()
        {
            return mCurrent;
        }

        const IntType &
        operator*()
        const
        {
            return mCurrent;
        }

protected:
    IntType mCurrent;
};

template <typename IntType>
class IntegralRangeForwardIterator : public IntegralRangeIterator<IntType>
{
public:
        IntegralRangeForwardIterator(IntType current)
        :   IntegralRangeIterator<IntType>(current)
        {}

        IntegralRangeForwardIterator<IntType> &
        operator++()
        {
            ++(this->mCurrent);
            return *this;
        }
};

template <typename IntType>
class IntegralRangeReverseIterator : public IntegralRangeIterator<IntType>
{
public:
        IntegralRangeReverseIterator(IntType current)
        :   IntegralRangeIterator<IntType>(current)
        {}

        IntegralRangeReverseIterator<IntType> &
        operator++()
        {
            --(this->mCurrent);
            return *this;
        }
};

}}

namespace ies
{

IES_SMART_ENUM(EmptyPolicy,
    NotAllow,
    Allow
);

IES_SMART_ENUM(RangeDirection,
    Forward,
    Reverse
);

//! @brief Integral range in [beginValue, endValue) of type IntType.
//! @note IntegralRangeTypes specifies if or not IntRange allows empty range: i.e. [n, n)
//! EmptyPolicy::NotAllow to represent non-empty IntRange which throw exception when [n, n).
//! EmptyPolicy::Allow when using possible empty range in range-based for.
//! Both have valid use cases, default is AllowEmpty::No.
//! EmptyRange is mostly only have range-based for usage, accessing range value is invalid.
//! @note ReverseRange is valid values of ForwardRange iterating in reverse way, not means the range is declared reversed.
//! e.g. IntRange(2, 5) = {2, 3, 4}, ReverseIntRange(2, 5) = {4, 3, 2} (not {5, 4, 3}!)
template <typename IntType, RangeDirection Direction=RangeDirection::Forward>
class IntegralRange
{
public:
    using IteratorType = typename std::conditional<Direction==RangeDirection::Forward,
                                                   Detail::IntegralRangeForwardIterator<IntType>,
                                                   Detail::IntegralRangeReverseIterator<IntType>>::type;

    //! @brief Construct a run-time IntegralRange [beginValue, endValue). Values are immutable afer creation.
    //! @param emptyPolicy: can create an empty range if begin==end and EmptyPolicy==Allow.
    //! @param errorHint: to add (name) in error message when construct invalid range.
    //!     Range is consider invalid if 1. begin>end 2. begin==end and EmptyPolicy==NotAllow.
        IntegralRange(IntType beginValue,
                      IntType endValue,
                      EmptyPolicy emptyPolicy=EmptyPolicy::NotAllow,
                      const std::string &errorHint="")
        :   mBeginValue(beginValue), mEndValue(endValue)
        {
            static_assert(std::is_integral<IntType>::value, "IntegralRange only applies to integral type.");

            if (mBeginValue>mEndValue
                || (emptyPolicy==EmptyPolicy::NotAllow && mBeginValue==mEndValue))
            {
                auto name = errorHint;
                if (!errorHint.empty())
                {
                    name = " ["+errorHint+"]";
                }
                auto description = "["+std::to_string(mBeginValue)+", "+std::to_string(mEndValue)+")";
                throw std::runtime_error("invalid IntegralRange"+name+": "+description);
            }
        }

        IntegralRange(const IntegralRange &rhs)
        :   mBeginValue(rhs.mBeginValue), mEndValue(rhs.mEndValue)
        {}

        IntegralRange &
        operator=(const IntegralRange &rhs)
        {
            if (this==&rhs) { return *this; }
            mBeginValue = rhs.mBeginValue;
            mEndValue = rhs.mEndValue;
            return *this;
        }

    //! @brief STL compatible range access: begin().
    //! @note This is begin iterator, to get begin value use GetBegin().
        IteratorType
        begin()
        const
        {
            if (Direction==RangeDirection::Reverse)
            {
                return IteratorType(mEndValue-1);
            }
            return IteratorType(mBeginValue);
        }

    //! @brief STL compatible range access: end().
    //! @note This is end iterator, to get end value use GetEnd().
        IteratorType
        end()
        const
        {
            if (Direction==RangeDirection::Reverse)
            {
                return IteratorType(mBeginValue-1);
            }
            return IteratorType(mEndValue);
        }

    //! @brief STL compatible range access: size().
        std::size_t size() const { return static_cast<std::size_t>(mEndValue-mBeginValue); }

    //! @brief STL compatible range access: empty().
        bool empty() const { return size()==0; }

    //! @brief Return BeginValue of range.
        IntType GetBegin() const { return mBeginValue; }

    //! @brief Return EndValue of range.
        IntType GetEnd() const { return mEndValue; }

    //! @brief Return min value of range. (= BeginValue)
        IntType GetMin() const { return mBeginValue; }

    //! @brief Return max valid value of range. (= EndValue-1)
    //! @note It's invalid to access max value for EmptyRange.
        IntType
        GetMax()
        const
        {
            if (empty())
            {
                throw std::runtime_error("cannot get max value of empty IntegralRange.");
            }
            return mEndValue-1;
        }

    //! @brief Get range's BeginValue or EndValue by side.
        IntType
        Get(RangeSide side)
        const
        {
            if (side==RangeSide::Begin) { return GetBegin(); }
            return GetEnd();
        }

    //! @brief Get range's MinValue or MaxValue by extremum.
    //! @note It's invalid to access max value for EmptyRange.
        IntType
        Get(Extremum extremum)
        const
        {
            if (extremum==Extremum::Min) { return GetMin(); }
            return GetMax();
        }

    //! @brief Check if n is in range [beginValue, endValue).
        bool IsInRange(IntType n) const { return mBeginValue<=n && n<mEndValue; }
    //! @brief Check if n is in range [beginValue, endValue], including the endValue.
        bool IsInRangeOrEnd(IntType n) const { return mBeginValue<=n && n<=mEndValue; }

    //! @brief Find closest value to n in range [beginValue, endValue], including the endValue.
    //! @note If n is in the range, then closest value is n itself,
    //! if n is outside of range, return closest side value.
        IntType
        FindClosest(IntType n)
        const
        {
            if (n<mBeginValue) { return mBeginValue; }
            if (n>mEndValue) { return mEndValue; }
            return n;
        }

    //! @brief Find distance from n to range [beginValue, endValue], including the endValue.
    //! Distance is in absolute value of IntType.
    //! @note FindDistance = Distance(n, FindClosest(n)).
    //! @note If n is in the range, distance is 0 by definition.
    //! if n is outside of range, is non-zero distance to closest side value.
        IntType
        FindDistance(IntType n)
        const
        {
            return std::abs(FindClosest(n)-n);
        }

    //! @brief Check if this range entirely contains other range. (need has same IntType, can be different RangeDirection)
        template <RangeDirection OtherDirection=Direction>
        bool
        Contains(const IntegralRange<IntType, OtherDirection> &other)
        const
        {
            return IsInRange(other.GetBegin()) && IsInRangeOrEnd(other.GetEnd());
        }

    //! @brief Check if this range overlaps with other range. (need has same IntType, can be different RangeDirection)
        template <RangeDirection OtherDirection=Direction>
        bool
        Overlaps(const IntegralRange<IntType, OtherDirection> &other)
        const
        {
            return !(GetEnd()<=other.GetBegin() || other.GetEnd()<=GetBegin());
        }

private:
    IntType mBeginValue;
    IntType mEndValue;
};

template <typename IntType>
bool
operator==(const IntegralRange<IntType> &lhs, const IntegralRange<IntType> &rhs)
{
    return lhs.GetBegin()==rhs.GetBegin() && lhs.GetEnd()==rhs.GetEnd();
}

template <typename IntType>
bool
operator!=(const IntegralRange<IntType> &lhs, const IntegralRange<IntType> &rhs)
{
    return !(lhs==rhs);
}

//! @brief Create new range by shifting n from origin IntegralRange. Origin range cannot be empty.
template <typename IntType>
IntegralRange<IntType>
Shift(const IntegralRange<IntType> &range, IntType n)
{
    if (range.empty())
    {
        throw std::runtime_error("cannot shift empty IntegralRange.");
    }
    return {range.GetBegin()+n, range.GetEnd()+n};
}

//! @brief Convert IntegralRange to string formatted in '[beginValue, endValue)'.
template <typename IntType>
std::string
ToString(const IntegralRange<IntType> &range)
{
    return "["+std::to_string(range.GetBegin())+", "+std::to_string(range.GetEnd())+")";
}

//! @brief Convenient using Types still under ies namespace:
//! IntRange: 32bit int (usually)
//! IndexRange: when access size_t interface, usually as index for array, vector, string.
using IntRange = ies::IntegralRange<int>;
using ReverseIntRange = ies::IntegralRange<int, ies::RangeDirection::Reverse>;
using UIntRange = ies::IntegralRange<unsigned int>;
using ReverseUIntRange = ies::IntegralRange<unsigned int, ies::RangeDirection::Reverse>;
using IndexRange = ies::IntegralRange<std::size_t>;
using ReverseIndexRange = ies::IntegralRange<std::size_t, ies::RangeDirection::Reverse>;

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
