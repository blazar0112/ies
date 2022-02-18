#pragma once

//'' workaround for warning C4251: class T needs to have dll-interface to be used by clients of class T.
//'' just disable warning, not fix it properly currently
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "ies/ies_export.h"

#include <ctime>
#include <chrono>
#include <map>
#include <string>

namespace ies
{

enum class TimeType
{
    Cpu,
    Real
};

struct TimeInterval
{
    std::string Name;
    int Id{-1};
    clock_t StartClock = clock();
    clock_t EndClock = clock();
    std::chrono::time_point<std::chrono::system_clock> StartReal;
    std::chrono::time_point<std::chrono::system_clock> EndReal;
    double CpuTimeInSec{0};
    double RealTimeInSec{0};
    bool IsEnded{false};
};

enum class TimerPrint
{
    None,
    Cout
};

enum class TimerSpecialName
{
    Anonymous,
    LastStarted
};

enum class TimerStringDetail
{
    Default,
    AddStartEnd
};

//! @brief Timer that help manage multiple named measured TimeIntervals.
//! @note It has some overhead (about 30us?), not good for sub-microsecond measuring.
//! For more light-weighted time measuring, use TimeUtil functions to measure range of 1~50k nanoseconds instead.
//!
//! @example Use case of Timer:
//! Timer timer;
//! timer.Start("a", TimerPrint::Cout);
//! a();
//! timer.End(TimerSpecialName::Last, TimerPrint::Cout); // use last name = "a".
//!
//! auto startMessage = timer.Start(); // use default: anonymous time
//! logger.Log(startMessage);
//! f();
//! auto endMessage = timer.End(); // use End(1)
//!
//! // in this case (sequential usage), above is equivalent to:
//! // auto endMessage = timer.End(""); // use End(1) with name = "" (anonymous time) explicitly.
//! // auto endMessage = timer.End(TimerSpecialName::Anonymous);
//! // auto endMessage = timer.End(TimerSpecialName::LastStarted); // because last started name is anonymous.
//!
//! Lazy usage for simply using Timer for a Interval after constructor:
//! Timer timer; // Start LifeTime
//! timer.GetLifeTime(); //also End LifeTime
//! use Timer::GetLifeTimeName() to use other interface.
class IES_EXPORT Timer
{
public:
        Timer();
        ~Timer() = default;

    //! @brief Start a time, default is anonymous time name = "".
    //! @return formated string indicate time started, same as line printed in TimerPrint::Cout.
    //! @note start time again is error, unless it's anonymous time.
        std::string
        Start(const std::string &name="", TimerPrint print=TimerPrint::None);

    //! @brief End(1) version: end time by name. default is anoymous time name = "".
    //! @note can overwrite previous end position so change the named time interval.
        std::string
        End(const std::string &name="", TimerPrint print=TimerPrint::None);

    //! @brief End(2) version: end time convenient version, end by special name.
    //! @note can overwrite previous end position so change the named time interval.
        std::string
        End(TimerSpecialName specialName, TimerPrint print=TimerPrint::None);

        std::string
        GetLastStartedName()
        const;

    //! @brief Let user get all named timer and its interval (at later time), if wish to inspect details.
    //! example:
    //! // dont want to print when execute a, b, c
    //! timer.Start("a"); a();
    //! timer.Start("b"); b();
    //! timer.Start("c"); c();
    //! // summary later:
    //! for (auto &nameInterval : timer.GetTimeIntervals())
    //! { std::cout << timer.ToString(nameInterval.first) << std::endl; }
    //! Use GetTime if no such need.
        const std::map<std::string, TimeInterval> &
        GetTimeIntervals()
        const;

        bool
        HasTime(const std::string &name)
        const;

    //! @brief Get stored time interval named [name] by TimeType in second [double].
    //! @note The time must be ready (ended before).
        double
        GetTime(const std::string &name="", TimeType type=TimeType::Cpu)
        const;

    //! @brief Get stored time interval named [name] REAL in chrono type.
        std::chrono::duration<double>
        GetRealTime(const std::string &name="")
        const;

    //! @brief Get name used for Timer's LifeTime.
        static
        std::string
        GetLifeTimeName();

    //! @brief Convenient method that End LifeTime and Get LifeTime.
        double
        GetLifeTime(TimeType type=TimeType::Cpu);

        void
        RemoveTime(const std::string &name="");

        void
        Clear();

        void
        EndAll();

        std::string
        ToString(const std::string &name, TimerStringDetail detail = TimerStringDetail::Default)
        const;

        static
        double
        GetCpuPrecisionInSec();

private:
    int mCount{0};
    std::string mLastStartedName;
    std::map<std::string, TimeInterval> mNameToTimeIntervalMap;

        void
        StartTime(const std::string &name);

    //! @brief Set end position of time interval named [name].
    //! @note can overwrite previous end position so change the said time interval.
        void
        EndTime(const std::string &name);
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
