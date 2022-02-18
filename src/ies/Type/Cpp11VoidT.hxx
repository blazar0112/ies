#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include <type_traits>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type { namespace Cpp11 {

template <typename...>
struct make_void { using type = void; };

//! @brief C++11 implementation of C++17's void_t.
//! @see https://en.cppreference.com/w/cpp/types/void_t
template <typename... Ts>
using void_t = typename make_void<Ts...>::type;

}}}
