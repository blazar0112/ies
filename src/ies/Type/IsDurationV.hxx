#pragma once

#include "ies/StdUtil/RequireCpp14.hpp"

#include "ies/Type/IsDuration.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

//! @note Variable template is C++14.
template <typename T>
inline constexpr bool IsDurationV = IsDuration<T>::value;

}}
