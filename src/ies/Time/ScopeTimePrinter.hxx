#pragma once

#include "ies/StdUtil/RequireCpp17.hpp"

#include "ies/Time/TimeUtilFormat.hxx"
#include "ies/Type/IsDurationV.hxx"

namespace ies::Time
{

//! @brief Profile time in scope and print at end of scope.
template <typename T>
class ScopeTimePrinter
{
public:
        ScopeTimePrinter(std::string timeName)
        :   mTimeName(std::move(timeName))
        {
            static_assert(Type::IsDurationV<T>, "type can only be duration");
            mBegin = Now();
        }

        ~ScopeTimePrinter()
        {
            Print<T>(CountNs(mBegin), mTimeName);
        }

private:
    std::string mTimeName;
    TimePointType mBegin;
};

}
