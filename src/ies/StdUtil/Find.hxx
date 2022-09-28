#pragma once

#include "ies/StdUtil/RequireCpp17.hpp"

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "ies/Type/IsAssociativeContainerV.hxx"
#include "ies/Type/IsJsonV.hxx"

namespace ies
{
//'' Requires C++17 since using std::optional.

//'' Usage note and design rationale:
//'' [C++98/11] directly use STL interface, much boilerplate
//''    auto find = map.find(key);
//''    if (find!=map.end()) { //found and can use find iterator } else { //not found }
//'' [C++11] Use StdUtil::IsIn to express key existence check. But IsIn is a predicate, to use iterator actually need find twice.
//'' also does not preserve constness since it only return bool, does not care. And interface order not same as std::find
//''    if (StdUtil::IsIn(key, map)) { // guranteed has key, just use .at(): auto &value = map.at(key) }
//'' note: order of argument is changed in ies version.
//''    if (ies::IsIn(map, key)) { // guranteed has key, just use .at(): auto &value = map.at(key) }
//'' [C++17] Use ies::Find(), look Test for more usages. iterator/const_iterator to use iterator and preserve constness.
//''    if (auto opt = ies::Find(map, key)) { // can choose to use opt.value(), type depend on what container is }
//'' note: C++17 can declare variable in if statementenables you to write
//''    if (auto it=map.find(key); it!=map.end())
//'' but that's still boilerplate, and ies::Find unify usage between std::string and containers.
//'' [C++20] has std::set::contains(key)->bool. but still not solve may or may not use result iterator of find problem.
//'' contains totally replace ies::IsIn usage.

//! @brief [Generic non-const Container] Simplify usage of associative_container::find(value) [O(lgn)] / std::find(container) [0(n)]
//! return std::optional of iterator/const_iterator.
//! @note For associative_container(i.e. std::set, std::map), value_type = key_type, and key in associative_container cannot be changed.
//! even if returned iterator is non-const, it's same restriction as std::set::find().
//! @note Can use for nlohmann::json.
//! However, if json is array, then need convert to standard container for non-string value.
//! 1. if json is object, then key is type of string by definition, just Find(json, "key").
//! 2. if json is array, and element is string type, then Find(json, "value") is ok (but use O(n) find).
//! 3. if json is array and element is non-string type, then need use get to convert container type.
//! //'' example json = { 1, 0, 2 }
//!     auto array = json.get<std::vector<int>>();
//!     Find(array, 1);
template <typename Container,
          typename Value,
          std::enable_if_t<!std::is_same_v<std::remove_cv_t<Container>, std::string>
                           &&!std::is_same_v<std::remove_cv_t<Container>, std::string_view>, int> = 0>
auto
Find(Container &container, const Value &value)
-> std::optional<decltype(std::begin(container))>
{
    if constexpr (Type::IsJsonV<Container>)
    {
        //'' json object
        if (!container.is_array())
        {
            if (auto it = container.find(value); it!=container.end())
            {
                return {it};
            }
        }
        //'' json array
        else
        {
            if (auto it = std::find(std::begin(container), std::end(container), value); it!=std::end(container))
            {
                return {it};
            }
        }
    }

    // NOLINTNEXTLINE(readability-misleading-indentation)
    if constexpr (Type::IsAssociativeContainerV<Container>)
    {
        //'' associative container, use container.find() [O(lgn)]
        if (auto it = container.find(value); it!=container.end())
        {
            return {it};
        }
    }
    else
    {
        //'' non-associative container, use generic algorithm std::find() [O(n)]
        // NOLINTNEXTLINE(llvm-qualified-auto,readability-qualified-auto)
        if (auto it = std::find(std::begin(container), std::end(container), value); it!=std::end(container))
        {
            return {it};
        }
    }

    return std::nullopt;
}

//! @brief [Generic const Container] Simplify usage of associative_container::find(value) [O(lgn)] / std::find(container) [0(n)]
//! return std::optional of iterator/const_iterator.
template <typename Container,
          typename Value,
          std::enable_if_t<!std::is_same_v<std::remove_cv_t<Container>, std::string>
                           &&!std::is_same_v<std::remove_cv_t<Container>, std::string_view>, int> = 0>
auto
Find(const Container &container, const Value &value)
-> std::optional<decltype(std::begin(container))>
{
    if constexpr (Type::IsJsonV<Container>)
    {
        //'' json object
        if (!container.is_array())
        {
            if (auto it = container.find(value); it!=container.end())
            {
                return {it};
            }
        }
        //'' json array
        else
        {
            if (auto it = std::find(std::begin(container), std::end(container), value); it!=std::end(container))
            {
                return {it};
            }
        }
    }

    // NOLINTNEXTLINE(readability-misleading-indentation)
    if constexpr (Type::IsAssociativeContainerV<Container>)
    {
        //'' associative container, use container.find() [O(lgn)]
        if (auto it = container.find(value); it!=container.end())
        {
            return {it};
        }
    }
    else
    {
        //'' non-associative container, use generic algorithm std::find() [O(n)]
        if (auto it = std::find(std::begin(container), std::end(container), value); it!=std::end(container))
        {
            return {it};
        }
    }

    return std::nullopt;
}

//! @brief [String] Simplify usage of std::string::find(substr) [O(n)], return std::optional of std:size_t (pos).
//! @note If substr is empty, always return pos=0.
inline
std::optional<std::size_t>
Find(const std::string &s, const std::string &substr)
{
    if (auto pos = s.find(substr); pos!=std::string::npos)
    {
        return {pos};
    }
    return std::nullopt;
}

//! @brief [String] Simplify usage of std::string::find(char) [O(n)], return std::optional of std:size_t (pos).
//! @note Need this specialization to support search '\0' without much trouble.
inline
std::optional<std::size_t>
Find(const std::string &s, char c)
{
    if (auto pos = s.find(c); pos!=std::string::npos)
    {
        return {pos};
    }
    return std::nullopt;
}

//! @brief [StringView] Simplify usage of std::string_view::find(substr) [O(n)], return std::optional of std:size_t (pos).
//! @note If substr is empty, always return pos=0.
inline
std::optional<std::size_t>
Find(std::string_view s, std::string_view substr)
{
    if (auto pos = s.find(substr); pos!=std::string_view::npos)
    {
        return {pos};
    }
    return std::nullopt;
}

//! @brief [StringView] Simplify usage of std::string_view::find(char) [O(n)], return std::optional of std:size_t (pos).
//! @note Need this specialization to support search '\0' without much trouble.
inline
std::optional<std::size_t>
Find(std::string_view s, char c)
{
    if (auto pos = s.find(c); pos!=std::string_view::npos)
    {
        return {pos};
    }
    return std::nullopt;
}

}
