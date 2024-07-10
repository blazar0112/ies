#include "ies/Common/FastIntegerPow.hxx"

#include "gtest/gtest.h"

namespace ies
{

TEST(FastIntegerPow, CalculateIntegerPowerIsConstEval)
{
    std::array<int, CalculateIntegerPower<2, 10>()> array;
    ASSERT_EQ(1024ULL, array.size());
}

TEST(FastIntegerPow, FastIntegerPow)
{
    auto result3p4 = FastIntegerPow<3, 5>(4);
    EXPECT_EQ(81ULL, result3p4);
    auto result10p12 = FastIntegerPow<10, 15>(12);
    ASSERT_EQ(1'0000'0000'0000ULL, result10p12);
}

TEST(FastIntegerPow, FastIntegerPow10)
{
    auto GetPowOf10 = [](std::size_t exp) -> auto
    {
        // FastIntegerPow<10, 20> calculate 10^19 which is out of uint64_t and static_assert will fail.
        return FastIntegerPow<10, 19>(exp);
    };
    EXPECT_EQ(1ULL, GetPowOf10(0));
    EXPECT_EQ(10ULL, GetPowOf10(1));
    EXPECT_EQ(100ULL, GetPowOf10(2));
    EXPECT_EQ(1000ULL, GetPowOf10(3));
    EXPECT_EQ(1'0000ULL, GetPowOf10(4));
    EXPECT_EQ(10'0000ULL, GetPowOf10(5));
    EXPECT_EQ(100'0000ULL, GetPowOf10(6));
    EXPECT_EQ(1000'0000ULL, GetPowOf10(7));
    EXPECT_EQ(1'0000'0000ULL, GetPowOf10(8));
    EXPECT_EQ(10'0000'0000ULL, GetPowOf10(9));
    EXPECT_EQ(100'0000'0000ULL, GetPowOf10(10));
    EXPECT_EQ(1000'0000'0000ULL, GetPowOf10(11));
    EXPECT_EQ(1'0000'0000'0000ULL, GetPowOf10(12));
    EXPECT_EQ(10'0000'0000'0000ULL, GetPowOf10(13));
    EXPECT_EQ(100'0000'0000'0000ULL, GetPowOf10(14));
    EXPECT_EQ(1000'0000'0000'0000ULL, GetPowOf10(15));
    EXPECT_EQ(1'0000'0000'0000'0000ULL, GetPowOf10(16));
    EXPECT_EQ(10'0000'0000'0000'0000ULL, GetPowOf10(17));
    EXPECT_EQ(100'0000'0000'0000'0000ULL, GetPowOf10(18));
    ASSERT_EQ(0ULL, GetPowOf10(19));
}

TEST(FastIntegerPow, FastIntegerPowOutOfRange)
{
    auto result = FastIntegerPow<10, 10>(10);
    ASSERT_EQ(0ULL, result);
}

} // namespace ies
