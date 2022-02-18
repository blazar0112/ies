#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include "ies/Type/IsAssociativeContainer.hxx"

namespace ies
{

//! @brief Erase all element from associative container if satisfy [predicate].
//! @note C++20 has std::erase_if(c, predicate) interface for STL containers. Use that if have C++20.
//! @see This is to save boilerplate occur in example in
//!      https://en.cppreference.com/w/cpp/container/set/erase
//!      https://en.cppreference.com/w/cpp/container/map/erase
//! @example
//! std::set<int> set{1, 2, 3}
//! EraseIf(set, [](const int &key){ return key%2==1; });
//! std::map<std::string, int> map{{"a", 1}, {"b", 2}, {"ab", 3}, {"abc", 4}};
//! EraseIf(map, [](const decltype(map)::value_type &pair){ return pair.first[0]=='a' && pair.second%3==1; });
template <typename Container, typename Predicate>
void
EraseIf(Container &container, const Predicate &predicate)
{
    static_assert(Type::IsAssociativeContainer<Container>::value, "only support associative container.");
    for (auto it=container.begin(); it!=container.end(); )
    {
        if (predicate(*it)) { it = container.erase(it); }
        else { ++it; }
    }
}

}
