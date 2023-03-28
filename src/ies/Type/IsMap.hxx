#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <type_traits>
#include <utility>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

template <typename T, typename = void>
struct IsMap : std::false_type {};

template <typename T>
struct IsMap<
    T,
    typename std::enable_if<
        std::is_same<typename T::value_type, std::pair<const typename T::key_type, typename T::mapped_type>>::value,
        void>::type
>
: std::true_type
{};

}}
