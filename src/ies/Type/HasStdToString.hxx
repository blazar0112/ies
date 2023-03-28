#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <string>

#include "ies/Type/Cpp11VoidT.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

template <typename T, typename = void>
struct HasStdToString : std::false_type {};

template <typename T>
struct HasStdToString<
    T,
    Cpp11::void_t<decltype( std::to_string(T{}) )>
>
: std::true_type
{};

}}
