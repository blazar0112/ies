#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

//! @note Do not just include this header to include STL containers.

#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <vector>

#include "ies/Type/HasStdToString.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Detail {

template <typename Container> std::string DetectContainerName(const Container &/*unused*/) { return "Unknown Container"; }
template <typename T, std::size_t N> std::string DetectContainerName(const std::array<T, N> &/*unused*/) { return "std::array<"+std::to_string(N)+">"; }
template <typename T> std::string DetectContainerName(const std::deque<T> &/*unused*/) { return "std::deque"; }
template <typename T> std::string DetectContainerName(const std::forward_list<T> &/*unused*/) { return "std::forward_list"; }
template <typename T> std::string DetectContainerName(const std::list<T> &/*unused*/) { return "std::list"; }
template <typename T> std::string DetectContainerName(const std::set<T> &/*unused*/) { return "std::set"; }
template <typename Key, typename Value> std::string DetectContainerName(const std::map<Key, Value> &/*unused*/) { return "std::map"; }
template <typename T> std::string DetectContainerName(const std::vector<T> &/*unused*/) { return "std::vector"; }

}}

namespace ies
{

//! @brief [String Version] Pretty format STL-container (compatible) of element string.
//! e.g. FormatString(std::vector<std::string>{"a", "b"}) = R"(std::vector{"a", "b"})".
//! @note Consider fmt::format and fmt::join.
template <typename T,
          typename std::enable_if<std::is_same<std::string, typename T::value_type>::value, int>::type = 0>
std::string
FormatString(const T &container)
{
    std::string s;
    s += Detail::DetectContainerName(container)+"{";
    auto isFirst = true;
    for (auto &e : container)
    {
        if (!isFirst) { s+= ", "; }
        s += "\""+e+"\"";
        isFirst = false;
    }
    s += "}";
    return s;
}

//! @brief [Simple Value Version] Pretty format STL-container (compatible) of element has std::to_string overload.
//! e.g. FormatString(std::vector<int>{1, 2}) = "std::vector{1, 2}".
//! @note Consider fmt::format and fmt::print.
template <typename T,
          typename std::enable_if<Type::HasStdToString<typename T::value_type>::value, int>::type = 0>
std::string
FormatString(const T &container)
{
    std::string s;
    s += Detail::DetectContainerName(container)+"{";
    auto isFirst = true;
    for (auto &e : container)
    {
        if (!isFirst) { s+= ", "; }
        s += std::to_string(e);
        isFirst = false;
    }
    s += "}";
    return s;
}

//! @brief [Non-simple Value Version] Pretty format STL-container (compatible) of element has no std::to_string overload and not string itself.
//! e.g.
//! std::map<std::string, int> map{{"a", 3}, {"b", 4}, {"c", 5}};
//! auto formatted = FormatString(map,
//! [](const decltype(map)::value_type &pair)
//! {
//!     return "{\""+pair.first+"\", "+std::to_string(pair.second)+"}";
//! });
//! formatted = R"(std::map{{"a", 3}, {"b", 4}, {"c", 5}})"
//! @note Consider fmt::format and fmt::print.
template <typename T, typename ToStringMethod,
          typename std::enable_if<!std::is_same<std::string, typename T::value_type>::value, int>::type = 0,
          typename std::enable_if<!Type::HasStdToString<typename T::value_type>::value, int>::type = 0>
std::string
FormatString(const T &container, ToStringMethod f)
{
    std::string s;
    s += Detail::DetectContainerName(container)+"{";
    auto isFirst = true;
    for (auto &e : container)
    {
        if (!isFirst) { s+= ", "; }
        s += f(e);
        isFirst = false;
    }
    s += "}";
    return s;
}

}
