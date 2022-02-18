#include "ies/Time/TimeUtil.hxx"

#include "gtest/gtest.h"

#include "ies/Time/TimeUtilFormat.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Time {

TEST(TimeUtil, CountZeroTime)
{
    auto begin = Now();
    auto ns = CountNs(begin, begin);
    ASSERT_EQ(static_cast<NsCountType>(0), ns);
}

TEST(TimeUtil, FormatUs)
{
    auto begin = Now();
    auto format = Format<std::chrono::microseconds>(CountNs(begin, begin));

    ASSERT_EQ("0.000 us", format);
}

TEST(TimeUtil, Format)
{
    NsCountType nsCount = 1234567;

    EXPECT_EQ("1234567 ns", Format<std::chrono::nanoseconds>(nsCount));
    auto longNsFormat = Format<std::chrono::nanoseconds, UnitNameType::Literal>(nsCount);
    EXPECT_EQ("1234567 nanoseconds", longNsFormat);

    EXPECT_EQ("1234.567 us", Format<std::chrono::microseconds>(nsCount));
    auto longUsFormat = Format<std::chrono::microseconds, UnitNameType::Literal>(nsCount);
    EXPECT_EQ("1234.567 microseconds", longUsFormat);

    EXPECT_EQ("1.235 ms", Format<std::chrono::milliseconds>(nsCount));
    auto longMsFormat = Format<std::chrono::milliseconds, UnitNameType::Literal>(nsCount);
    EXPECT_EQ("1.235 milliseconds", longMsFormat);

    EXPECT_EQ("0.001 s", Format<std::chrono::seconds>(nsCount));
    auto longSecFormat = Format<std::chrono::seconds, UnitNameType::Literal>(nsCount);
    ASSERT_EQ("0.001 seconds", longSecFormat);
}

}}
