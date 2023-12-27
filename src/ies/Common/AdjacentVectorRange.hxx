#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <iterator>
#include <vector>
#include <utility>

#include "ies/Common/IntegralRange.hxx"
#include "ies/Type/IsMap.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Detail {

template <typename FwdIt>
class AdjacentVectorIterator final
{
public:
    using ElementType = std::vector<FwdIt>;

        //'' element array: container {begin, ..., elementArray{first, arrayend-1}, next, ..., end-1}, end
        //'' first: first in element array (for first array, first=begin of container)
        //'' next: first element after element array
        //'' last: end of container
        //'' adjacent_array_iterator(f, l) is valid if all iterator in element array is valid, else
        //'' invalid and/or represent end is (l, l).
        AdjacentVectorIterator(FwdIt first, FwdIt last, std::size_t size)
        :   mFirst(first), mNext(first), mLast(last), mSize(size), mRange(0, size)
        {
            mElementItVector.resize(mSize, mLast);

            if (mFirst!=mLast)
            {
                for (auto &i : mRange)
                {
                    mElementItVector[i] = mNext;
                    ++mNext;

                    if (mNext==mLast && i!=mSize-1)
                    {
                        mFirst=mLast;
                        break;
                    }
                }
            }
        }

        bool
        operator==(const AdjacentVectorIterator &rhs)
        const
        {
            return mFirst==rhs.mFirst && mNext==rhs.mNext && mSize==rhs.mSize;
        }

        bool
        operator!=(const AdjacentVectorIterator &rhs)
        const
        {
            return !(*this==rhs);
        }

        AdjacentVectorIterator &
        operator++()
        {
            if (mFirst==mLast)
            {
                throw std::runtime_error("try to increment end iterator");
            }
            ++mFirst;
            if (mNext==mLast)
            {
                for (auto &i : mRange)
                {
                    mElementItVector[i] = mLast;
                }
                mFirst = mLast;
            }
            else
            {
                for (auto &i : mRange)
                {
                    mElementItVector[i] = std::next(mElementItVector[i]);
                }
                ++mNext;
            }
            return *this;
        }

        ElementType
        operator*()
        {
            return mElementItVector;
        }

        ElementType
        operator*()
        const
        {
            return mElementItVector;
        }

private:
    ElementType mElementItVector;
    FwdIt mFirst;
    FwdIt mNext;
    FwdIt mLast;
    std::size_t mSize;
    IndexRange mRange;
};

template <typename FwdIt>
class AdjacentVectorRange final
{
public:
    using IteratorType = AdjacentVectorIterator<FwdIt>;

public:
        AdjacentVectorRange(FwdIt first, FwdIt last, std::size_t size)
        :   mFirst(first), mLast(last), mSize(size)
        {
            if (mSize<=1)
            {
                throw std::runtime_error("size cannot <=1.");
            }
        }

        IteratorType
        begin()
        const
        {
            return IteratorType(mFirst, mLast, mSize);
        }

        IteratorType
        end()
        const
        {
            return IteratorType(mLast, mLast, mSize);
        }

private:
    FwdIt mFirst;
    FwdIt mLast;
    std::size_t mSize;
};

}}

namespace ies
{

//! @brief Create a range of adjacent elements, with runtime size, stride in origin container,
//! each element of range is current window std::vector{const_iterator}.
//! e.g. Container = {0, 1, 2, 3}, first element of AdjacentVectorRange(3), is {it0, it1, it2}, where *it0 = 0, etc.
//!                   ^  ^  ^      first element  {it0, it1, it2}
//!                      ^  ^  ^   second element {it1, it2, it3}
//! @note Use AdjacentArrayRange version if rangeSize is known at compile time.
//! @note Do not use reference of element (Due to impl limit).
//! e.g. do
//!     for (auto itVector : MakeAdjacentVectorRange(container, n))
//!     {
//!         std::cout << "{
//!         for (auto it : itVector)
//!         {
//!             std::cout << " " << *it;
//!         }
//!         std::cout << " }" << std::endl;
//!     }
//! do not write
//!     for (auto &itVector : MakeAdjacentVectorRange(container, n)).
template <typename Container>
Detail::AdjacentVectorRange<typename Container::const_iterator>
MakeAdjacentVectorRange(const Container &container, std::size_t rangeSize)
{
    return Detail::AdjacentVectorRange<typename Container::const_iterator>(std::begin(container), std::end(container), rangeSize);
}

//! @brief [Non-map] Convenient method to convert AdjacentVectorRange's itVector element to *copied* value vector.
//! @note It assume Container has value_type.
//! If Container is std::map, then use mapped_type. (due to map's value_type cannot be copied).
//! To avoid copying value, directly use itVector and dereference.
template <typename Container,
          typename FwdIt,
          typename std::enable_if<!Type::IsMap<Container>::value, int>::type = 0>
std::vector<typename Container::value_type>
ToValueVector(const Container &container,
              const std::vector<FwdIt> &iteratorVector)
{
    (void)container;
    std::vector<typename Container::value_type> vector(iteratorVector.size());
    for (auto &i : IndexRange{0, iteratorVector.size()})
    {
        vector[i] = *iteratorVector[i];
    }
    return vector;
}

//! @brief [Map version] Convenient method to convert AdjacentVectorRange's itVector element to *copied* value vector.
//! If Container is std::map, then use mapped_type. (due to map's value_type cannot be copied).
//! @note To access with key, use itVector instead.
//!     for (auto itVector : MakeAdjacentVectorRange)
//!     {
//!         for (auto it : itVector)
//!         {
//!             it->first ...
//!         }
//!     }
template <typename Container,
          typename FwdIt,
          typename std::enable_if<Type::IsMap<Container>::value, int>::type = 0>
std::vector<typename Container::mapped_type>
ToValueVector(const Container &container,
              const std::vector<FwdIt> &iteratorVector)
{
    (void)container;
    std::vector<typename Container::mapped_type> vector(iteratorVector.size());
    for (auto &i : IndexRange{0, iteratorVector.size()})
    {
        vector[i] = iteratorVector[i]->second;
    }
    return vector;
}

}
