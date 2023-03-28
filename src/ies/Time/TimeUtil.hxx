#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <chrono>
#include <string>

//! @brief TimeUtil: convenient functions for common chrono usage.
//! @note Format and Print move to TimeUtilFormat to let user without fmt can use TimeUtil.
//! @code Example usage:
//!     auto begin = s2Time::Now();
//!     DoSomething();
//!     s2Time::PrintNs(begin);
//!     s2Time::PrintNs(begin, s2Time::Now(), "DoSomething");
// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Time {

#ifdef _WIN32
    using Clock = std::chrono::steady_clock;
#else
    using Clock = std::chrono::high_resolution_clock;
#endif

//! @brief TimePointType: std::chrono::time_point of clock. clock is steady_clock on windows, high_resolution_clock on linux.
using TimePointType = std::chrono::time_point<Clock>;
//! @brief NsCountType: nanosecond count type.
using NsCountType = long long;

inline
TimePointType
Now()
noexcept
{
    return Clock::now();
}

//! @brief Get count of nanoseconds from begin to now.
template <typename TimePoint>
NsCountType
CountNs(const TimePoint &begin, const TimePoint &end)
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
}

//! @brief Convenient method to get count of nanoseconds from begin to Now().
template <typename TimePoint>
NsCountType
CountNs(const TimePoint &begin)
{
    return CountNs(begin, Now());
}

}}
