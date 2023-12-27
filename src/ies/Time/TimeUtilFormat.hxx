#pragma once

#include "ies/StdUtil/RequireCpp17.hpp" // IWYU pragma: keep

#include <iostream>

#include "fmt/format.h"

#include "ies/Time/TimeUtil.hxx"

//! @brief TimeUtil: convenient functions for common chrono usage.
//! @code Example usage:
//!     auto begin = s2Time::Now();
//!     DoSomething();
//!     s2Time::PrintNs(begin);
//!     s2Time::PrintNs(begin, s2Time::Now(), "DoSomething");
namespace ies::Time {

enum class UnitNameType
{
    Symbol,
    Literal
};

//! @brief [Primary Template] Helper to convert std::chrono::duration to unit string.
template <typename Duration, UnitNameType UnitName=UnitNameType::Symbol>
struct ToUnit;

template <UnitNameType UnitName> struct ToUnit<std::chrono::nanoseconds, UnitName> { static const char Unit[]; };
template <> inline const char ToUnit<std::chrono::nanoseconds, UnitNameType::Symbol>::Unit[] = "ns";
template <> inline const char ToUnit<std::chrono::nanoseconds, UnitNameType::Literal>::Unit[] = "nanoseconds";

template <UnitNameType UnitName> struct ToUnit<std::chrono::microseconds, UnitName> { static const char Unit[]; };
template <> inline const char ToUnit<std::chrono::microseconds, UnitNameType::Symbol>::Unit[] = "us";
template <> inline const char ToUnit<std::chrono::microseconds, UnitNameType::Literal>::Unit[] = "microseconds";

template <UnitNameType UnitName> struct ToUnit<std::chrono::milliseconds, UnitName> { static const char Unit[]; };
template <> inline const char ToUnit<std::chrono::milliseconds, UnitNameType::Symbol>::Unit[] = "ms";
template <> inline const char ToUnit<std::chrono::milliseconds, UnitNameType::Literal>::Unit[] = "milliseconds";

template <UnitNameType UnitName> struct ToUnit<std::chrono::seconds, UnitName> { static const char Unit[]; };
template <> inline const char ToUnit<std::chrono::seconds, UnitNameType::Symbol>::Unit[] = "s";
template <> inline const char ToUnit<std::chrono::seconds, UnitNameType::Literal>::Unit[] = "seconds";

//! @brief Format to "{:.3f} <UnitName>" in floating point duration if Duration is not nanoseconds.
//! Format to "<nsCount> <UnitName>" directly if Duration is nanoseconds.
//! @note UnitNameType::Symbol: use symbol unit name 'ns' for Duration std::chrono::nanoseconds.
//!       UnitNameType::Literal: use literal unit name 'nanoseconds' for Duration std::chrono::nanoseconds.
//! @example
//!     NsCountType myNsCount = 1234; // have integral type count 1234 nanosecond.
//!     Format<std::chrono::nanoseconds>(myNsCount) => "1234 ns"
//!     Format<std::chrono::microseconds>(myNsCount) => "1.234 us"
//!     Format<std::chrono::milliseconds>(myNsCount) => "0.001 ms"
//!     Format<std::chrono::nanoseconds, UnitNameType::Literal>(myNsCount) => "1234 nanoseconds"
//! @example More practical usage: only want to measure and accumulate part of execution:
//!     NsCountType nsCount = 0;
//!     for (auto i : IntRange(0, 1000))
//!     {
//!         auto begin = Now();
//!         DoSomeThing();
//!         nsCount += CountNs(begin);
//!         DoSomeOtherThing();
//!     }
//!
//!     // expect duration range is milliseconds
//!     std::cout << "DoSomeThing() total time: " << Format<std::chrono::milliseconds>(nsCount) << std::endl;
template <typename Duration, UnitNameType UnitName=UnitNameType::Symbol>
std::string
Format(NsCountType nsCount)
{
    if constexpr (!std::is_same_v<Duration, std::chrono::nanoseconds>)
    {
        using FloatingDuration = std::chrono::duration<float, typename Duration::period>;
        const std::chrono::nanoseconds ns{nsCount};
        return fmt::format("{:.3f} ", FloatingDuration(ns).count())+ToUnit<Duration, UnitName>::Unit;
    }
    else
    {
        return std::to_string(nsCount)+" "+ToUnit<Duration, UnitName>::Unit;
    }
}

//! @brief Print line: [durationName] Time: Format<Duration, UnitNameType>(nsCount).
template <typename Duration, UnitNameType UnitName=UnitNameType::Symbol>
void
Print(NsCountType nsCount, const std::string &durationName="")
{
    std::string prefix;
    if (!durationName.empty())
    {
        prefix = "["+durationName+"] ";
    }
    std::cout << prefix << "Time: " << Format<Duration, UnitName>(nsCount) << "\n";
}

}
