#pragma once

#include "ies/StdUtil/RequireCpp14.hpp"

#include "ies/Type/IsJson.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

//! @brief Helper template to get value member of IsJson.
//! @note Variable template is C++14.
template <typename T>
inline constexpr bool IsJsonV = IsJson<T>::value;

}}
