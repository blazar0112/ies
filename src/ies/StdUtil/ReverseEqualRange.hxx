#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include <algorithm>
#include <functional>
#include <type_traits>

#include "ies/Type/IsMap.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Detail {

template<typename Container,
         typename std::enable_if<!Type::IsMap<Container>::value, int>::type = 0>
typename Container::const_reverse_iterator
ReverseLowerBoundImpl(Container const &c, typename Container::value_type value)
{
    return std::lower_bound(c.crbegin(), c.crend(),
                            value,
                            std::greater<typename Container::value_type>());
}

template<typename Container,
         typename std::enable_if<Type::IsMap<Container>::value, int>::type = 0>
typename Container::const_reverse_iterator
ReverseLowerBoundImpl(Container const &c, typename Container::key_type value)
{
    return std::lower_bound(c.crbegin(), c.crend(),
                            value,
                            [](typename Container::value_type const &pair,
                               typename Container::key_type const key)
                            -> bool
                            {
                                return std::greater<typename Container::key_type>()(pair.first, key);
                            });
}

template<typename Container,
         typename std::enable_if<!Type::IsMap<Container>::value, int>::type = 0>
typename Container::const_reverse_iterator
ReverseUpperBoundImpl(Container const &c, typename Container::value_type value)
{
    return std::upper_bound(c.crbegin(), c.crend(),
                            value,
                            std::greater<typename Container::value_type>());
}

template<typename Container,
         typename std::enable_if<Type::IsMap<Container>::value, int>::type = 0>
typename Container::const_reverse_iterator
ReverseUpperBoundImpl(Container const &c, typename Container::key_type value)
{
    return std::upper_bound(c.crbegin(), c.crend(),
                            value,
                            [](typename Container::key_type const key,
                               typename Container::value_type const &pair)
                            -> bool
                            {
                                return std::greater<typename Container::key_type>()(key, pair.first);
                            });
}

}}

namespace ies
{

//! @brief return first element not great than value (i.e. <=value) from reverse traversing.
//! @note lower_bound in standard return first element not less than value (i.e. >=value).
//! example: v[0, 1, 1, 2, 3, 5]
//!          ---->^                 lower_bound(1) = v[1]'s 1
//!                  ^<---------    reverse_lower_bound(1) = v[2]'s 1
template<typename Container, typename Value>
typename Container::const_reverse_iterator
ReverseLowerBound(Container const &c, Value value)
{
    return Detail::ReverseLowerBoundImpl(c, value);
}

template<typename Container, typename Value>
typename Container::const_reverse_iterator
ReverseUpperBound(Container const &c, Value value)
{
    return Detail::ReverseUpperBoundImpl(c, value);
}

template<typename Container, typename Value>
std::pair<typename Container::const_reverse_iterator,
          typename Container::const_reverse_iterator>
ReverseEqualRange(Container const &c, Value value)
{
    return std::make_pair(ReverseLowerBound(c, value), ReverseUpperBound(c, value));
}

}
