#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <array>
#include <iterator>
#include <utility>

#include "ies/Common/IntegralRange.hxx"
#include "ies/Type/IsMap.hxx"

//'' courtesy : http://stackoverflow.com/questions/14826893/how-do-i-loop-over-consecutive-pairs-in-an-stl-container-using-range-based-loop

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Detail {

template <typename FwdIt, std::size_t N>
class AdjacentArrayIterator final
{
public:
    using ElementType = std::array<FwdIt, N>;

        //'' element array: container {begin, ..., elementArray{first, arrayend-1}, next, ..., end-1}, end
        //'' first: first in element array (for first array, first=begin of container)
        //'' next: first element after element array
        //'' last: end of container
        //'' adjacent_array_iterator(f, l) is valid if all iterator in element array is valid, else
        //'' invalid and/or represent end is (l, l).
        AdjacentArrayIterator(FwdIt first, FwdIt last)
        :   mFirst(first), mNext(first), mLast(last)
        {
            for (auto &i : IndexRange{0, N})
            {
                mElementItArray[i] = mLast;
            }

            if (mFirst!=mLast)
            {
                for (auto &i : IndexRange{0, N})
                {
                    mElementItArray[i] = mNext;
                    ++mNext;

                    if (mNext==mLast && i!=N-1)
                    {
                        mFirst=mLast;
                        break;
                    }
                }
            }
        }

        bool
        operator==(const AdjacentArrayIterator &rhs)
        const
        {
            return mFirst==rhs.mFirst && mNext==rhs.mNext;
        }

        bool
        operator!=(const AdjacentArrayIterator &rhs)
        const
        {
            return !(*this==rhs);
        }

        AdjacentArrayIterator &
        operator++()
        {
            if (mFirst==mLast)
            {
                throw std::runtime_error("try to increment end iterator");
            }
            ++mFirst;
            if (mNext==mLast)
            {
                for (auto &i : IndexRange{0, N})
                {
                    mElementItArray[i] = mLast;
                }
                mFirst = mLast;
            }
            else
            {
                for (auto &i : IndexRange{0, N})
                {
                    mElementItArray[i] = std::next(mElementItArray[i]);
                }
                ++mNext;
            }
            return *this;
        }

        ElementType
        operator*()
        {
            return mElementItArray;
        }

        ElementType
        operator*()
        const
        {
            return mElementItArray;
        }

private:
    ElementType mElementItArray;
    FwdIt mFirst;
    FwdIt mNext;
    FwdIt mLast;
};

template <typename FwdIt, std::size_t N>
class AdjacentArrayRange final
{
public:
    using IteratorType = AdjacentArrayIterator<FwdIt, N>;

public:
        AdjacentArrayRange(FwdIt first, FwdIt last)
        :   mFirst(first), mLast(last)
        {}

        IteratorType
        begin()
        const
        {
            return IteratorType(mFirst, mLast);
        }

        IteratorType
        end()
        const
        {
            return IteratorType(mLast, mLast);
        }

private:
    FwdIt mFirst;
    FwdIt mLast;
};

}}

namespace ies
{

//! @brief Create a range of adjacent elements, with compile-time size, stride in origin container,
//! each element of range is current window std::array<N>{const_iterator}.
//! e.g. Container = {0, 1, 2, 3}, first element of AdjacentArrayRange<3>, is {it0, it1, it2}, where *it0 = 0, etc.
//!                   ^  ^  ^      first element  {it0, it1, it2}
//!                      ^  ^  ^   second element {it1, it2, it3}
//! @note Do not use reference of element (Due to impl limit).
//! e.g. do
//!     for (auto itArray : MakeAdjacentArrayRange<N>(container))
//!     {
//!         std::cout << "{
//!         for (auto it : itArray)
//!         {
//!             std::cout << " " << *it;
//!         }
//!         std::cout << " }" << std::endl;
//!     }
//! do not write
//!     for (auto &itArray : MakeAdjacentArrayRange<N>(container)).
template <std::size_t N, typename Container>
Detail::AdjacentArrayRange<typename Container::const_iterator, N>
MakeAdjacentArrayRange(const Container &container)
{
    return Detail::AdjacentArrayRange<typename Container::const_iterator, N>(std::begin(container), std::end(container));
}

//! @brief [Non-map] Convenient method to convert AdjacentArrayRange's itArray element to *copied* value array.
//! @note It assume Container has value_type.
//! If Container is std::map, then use mapped_type. (due to map's value_type cannot be copied).
//! To avoid copying value, directly use itArray and dereference.
template <typename Container,
          typename FwdIt,
          std::size_t N,
          typename std::enable_if<!Type::IsMap<Container>::value, int>::type = 0>
std::array<typename Container::value_type, N>
ToValueArray(const Container &container,
             const std::array<FwdIt, N> &iteratorArray)
{
    (void)container;
    std::array<typename Container::value_type, N> array{};
    for (auto &i : IndexRange{0, N})
    {
        array[i] = *iteratorArray[i];
    }
    return array;
}

//! @brief [Map version] Convenient method to convert AdjacentArrayRange's itArray element to *copied* value array.
//! If Container is std::map, then use mapped_type. (due to map's value_type cannot be copied).
//! @note To access with key, use itArray instead.
//!     for (auto itArray : MakeAdjacentArrayRange)
//!     {
//!         for (auto it : itArray)
//!         {
//!             it->first ...
//!         }
//!     }
template <typename Container,
          typename FwdIt,
          std::size_t N,
          typename std::enable_if<Type::IsMap<Container>::value, int>::type = 0>
std::array<typename Container::mapped_type, N>
ToValueArray(const Container &container,
             const std::array<FwdIt, N> &iteratorArray)
{
    (void)container;
    std::array<typename Container::mapped_type, N> array{};
    for (auto &i : IndexRange{0, N})
    {
        array[i] = iteratorArray[i]->second;
    }
    return array;
}

}
