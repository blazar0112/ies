#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include "ies/Type/Cpp11VoidT.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

template <typename T, typename U = void>
struct IsAssociativeContainer : std::false_type {};

template <typename T>
struct IsAssociativeContainer<T, Cpp11::void_t<typename T::key_type>> : std::true_type {};

}}
