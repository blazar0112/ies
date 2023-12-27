#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <chrono>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Type {

template <typename T>
struct IsDuration : std::false_type {};

template <typename Rep, typename Period>
struct IsDuration<std::chrono::duration<Rep, Period>> : std::true_type {};

}}
