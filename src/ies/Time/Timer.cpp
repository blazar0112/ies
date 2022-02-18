#include "ies/Time/Timer.hpp"

#include <iostream>

#include "ies/StdUtil/Find.hxx"

namespace
{
    const std::string AnonymousPrintTime = "(anonymous)";
}

namespace ies
{

Timer::
Timer()
{
    StartTime(GetLifeTimeName());
}

std::string
Timer::
Start(const std::string &name, TimerPrint print)
{
    if (name==AnonymousPrintTime)
    {
        throw std::runtime_error("cannot start time with name ["+name+"] because it conflicted with internal AnonymousPrintTime.");
    }
    StartTime(name);
    auto printName = name;
    if (name.empty())
    {
        printName = AnonymousPrintTime;
    }
    auto message = "Timer: started Time ["+printName+"].";
    if (print==TimerPrint::Cout)
    {
        std::cout << message << std::endl;
    }
    return message;
}

std::string
Timer::
End(const std::string &name, TimerPrint print)
{
    if (name==AnonymousPrintTime)
    {
        throw std::runtime_error("cannot end time with name ["+name+"] because it conflicted with internal AnonymousPrintTime.");
    }
    EndTime(name);
    auto message = "Timer: ended "+ToString(name)+".";
    if (print==TimerPrint::Cout)
    {
        std::cout << message << std::endl;
    }
    return message;
}

std::string
Timer::
End(TimerSpecialName specialName, TimerPrint print)
{
    std::string name;
    if (specialName==TimerSpecialName::LastStarted)
    {
        name = mLastStartedName;
    }
    return End(name, print);
}

std::string
Timer::
GetLastStartedName()
const
{
    if (!Find(mNameToTimeIntervalMap, mLastStartedName))
    {
        throw std::runtime_error("No time started before.");
    }
    return mLastStartedName;
}

const std::map<std::string, TimeInterval> &
Timer::
GetTimeIntervals()
const
{
    return mNameToTimeIntervalMap;
}

bool
Timer::
HasTime(const std::string &name)
const
{
    return Find(mNameToTimeIntervalMap, name).has_value();
}

void
Timer::
StartTime(const std::string &name)
{
    if (!name.empty()&&HasTime(name))
    {
        throw std::runtime_error("Cannot begin time, already has time named ["+name+"].");
    }

    RemoveTime(name);

    auto &interval = mNameToTimeIntervalMap[name];
    interval.Name = name;
    interval.Id = mCount;
    interval.StartClock = clock();
    interval.StartReal = std::chrono::system_clock::now();

    ++mCount;

    mLastStartedName = name;
}

void
Timer::
EndTime(const std::string &name)
{
    auto findInterval = Find(mNameToTimeIntervalMap, name);
    if (!findInterval)
    {
        throw std::runtime_error("Cannot end time, no time named ["+name+"] started before.");
    }

    auto &interval = findInterval.value()->second;

    interval.EndClock = clock();
    interval.EndReal = std::chrono::system_clock::now();
    interval.IsEnded = true;
    interval.CpuTimeInSec = static_cast<double>(interval.EndClock-interval.StartClock)/CLOCKS_PER_SEC;
    interval.RealTimeInSec = std::chrono::duration<double>(interval.EndReal-interval.StartReal).count();
}

double
Timer::
GetTime(const std::string &name, TimeType type)
const
{
    auto findInterval = Find(mNameToTimeIntervalMap, name);
    if (!findInterval)
    {
        throw std::runtime_error("Cannot get time, no time named ["+name+"] started before.");
    }

    auto &interval = findInterval.value()->second;
    if (!interval.IsEnded)
    {
        throw std::runtime_error("Cannot get time, no time named ["+name+"] ended before.");
    }

    if (type==TimeType::Real)
    {
        return interval.RealTimeInSec;
    }

    return interval.CpuTimeInSec;
}

std::chrono::duration<double>
Timer::
GetRealTime(const std::string &name)
const
{
    auto findInterval = Find(mNameToTimeIntervalMap, name);
    if (!findInterval)
    {
        throw std::runtime_error("Cannot get time, no time named ["+name+"] started before.");
    }

    auto &interval = findInterval.value()->second;
    if (!interval.IsEnded)
    {
        throw std::runtime_error("Cannot get time, no time named ["+name+"] ended before.");
    }

    return interval.EndReal-interval.StartReal;
}

std::string
Timer::
GetLifeTimeName()
{
    return "LifeTime";
}

double
Timer::
GetLifeTime(TimeType type)
{
    End(GetLifeTimeName());
    return GetTime(GetLifeTimeName(), type);
}

void
Timer::
RemoveTime(const std::string &name)
{
    mNameToTimeIntervalMap.erase(name);
}

void
Timer::
Clear()
{
    mNameToTimeIntervalMap.clear();
}

void
Timer::
EndAll()
{
    for (auto &nameIntervalPair : mNameToTimeIntervalMap)
    {
        auto &interval = nameIntervalPair.second;
        if (!interval.IsEnded)
        {
            EndTime(nameIntervalPair.first);
        }
    }
}

std::string
Timer::
ToString(const std::string &name, TimerStringDetail detail)
const
{
    auto addStartEnd = (detail==TimerStringDetail::AddStartEnd);
    if (!HasTime(name))
    {
        throw std::runtime_error("Cannot convert time to string, no time named ["+name+"] started before.");
    }

    auto printName = name;
    if (name.empty())
    {
        printName = AnonymousPrintTime;
    }

    auto &interval = mNameToTimeIntervalMap.at(name);

    std::string text{"Time ["};
    text += printName+"]: CPU {";
    if (addStartEnd)
    {
        text += "Start="+std::to_string(interval.StartClock)+", ";
    }
    if (interval.IsEnded)
    {
        if (addStartEnd)
        {
            text += "End="+std::to_string(interval.EndClock)+", ";
        }
        text += "Time="+std::to_string(interval.CpuTimeInSec)+"s";
    }
    else
    {
        text += "(not ended)";
    }
    text += "}, REAL {";
    if (addStartEnd)
    {
        text += "Start="+std::to_string(std::chrono::system_clock::to_time_t(interval.StartReal))+", ";
    }
    if (interval.IsEnded)
    {
        if (addStartEnd)
        {
            text += "End="+std::to_string(std::chrono::system_clock::to_time_t(interval.EndReal))+", ";
        }
        text += "Time="+std::to_string(interval.RealTimeInSec)+"s";
    }
    else
    {
        text += "(not ended)";
    }
    text += "}";

    return text;
}

double
Timer::
GetCpuPrecisionInSec()
{
    auto startClock = clock();
    auto finishClock = clock();
    while (finishClock == startClock)
    {
        finishClock = clock();
    }
    auto cpuPrecision = static_cast<double>(finishClock - startClock)/(CLOCKS_PER_SEC);
    return cpuPrecision;
}

}
