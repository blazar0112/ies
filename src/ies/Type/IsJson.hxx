#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include "ies/Type/Cpp11VoidT.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

//! @note Cannot use IsAssociativeContainer because nlohmann::json does not define json::key_type, but in json::object_t::key_type.
template <typename T, typename U = void>
struct IsJson : std::false_type {};

template <typename T>
struct IsJson<T, Cpp11::void_t<typename T::object_t::key_type>> : std::true_type {};

}}
