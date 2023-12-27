#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <type_traits>

namespace ies
{

//! @brief Get underlying type of enum.
//! @example
//! enum class MyEnum : unsigned char { D=2 };
//! auto u = ToUnderlying(MyEnum::D);
//! assert(std::is_same_v<decltype(u), unsigned char>);
template <typename EnumClassType>
constexpr typename std::underlying_type<EnumClassType>::type
ToUnderlying(EnumClassType enumerator)
{
    return static_cast<typename std::underlying_type<EnumClassType>::type>(enumerator);
}

}
