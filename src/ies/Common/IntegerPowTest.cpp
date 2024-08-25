#include "ies/Common/IntegerPow.hxx"

#include "gtest/gtest.h"

#include <limits>

namespace ies
{

TEST(IntegerPow, CalculateIntegerPowAsConstExpr)
{
    std::array<int, CalculateIntegerPow<2, 10>()> array;
    ASSERT_EQ(1024ULL, array.size());
}

TEST(IntegerPow, CalculateIntegerPowBoundaryCase)
{
    {
        auto result0p0 = CalculateIntegerPow<0, 0>();
        EXPECT_EQ(1ULL, result0p0);
        auto result0p1 = CalculateIntegerPow<0, 1>();
        EXPECT_EQ(0ULL, result0p1);
        auto result0p2 = CalculateIntegerPow<0, 2>();
        EXPECT_EQ(0ULL, result0p2);
        auto result0p100 = CalculateIntegerPow<0, 100>();
        EXPECT_EQ(0ULL, result0p100);
    }

    {
        auto result1p0 = CalculateIntegerPow<1, 0>();
        EXPECT_EQ(1ULL, result1p0);
        auto result1p1 = CalculateIntegerPow<1, 1>();
        EXPECT_EQ(1ULL, result1p1);
        auto result1p2 = CalculateIntegerPow<1, 2>();
        EXPECT_EQ(1ULL, result1p2);
        auto result1p100 = CalculateIntegerPow<1, 100>();
        EXPECT_EQ(1ULL, result1p100);
    }

    {
        auto result2p0 = CalculateIntegerPow<2, 0>();
        EXPECT_EQ(1ULL, result2p0);
        auto result2p1 = CalculateIntegerPow<2, 1>();
        EXPECT_EQ(2ULL, result2p1);
        auto result2p2 = CalculateIntegerPow<2, 2>();
        EXPECT_EQ(4ULL, result2p2);
        auto result2p8 = CalculateIntegerPow<2, 8>();
        EXPECT_EQ(256ULL, result2p8);
        auto result2p16 = CalculateIntegerPow<2, 16>();
        EXPECT_EQ(std::numeric_limits<uint16_t>::max(), result2p16-1);
        auto result2p32 = CalculateIntegerPow<2, 32>();
        EXPECT_EQ(std::numeric_limits<uint32_t>::max(), result2p32-1);
        //auto result2p64 = CalculateIntegerPow<2, 64>(); // error
        auto result2p63 = CalculateIntegerPow<2, 63>();
        EXPECT_EQ(std::numeric_limits<uint64_t>::max(), (result2p63-1)*2+1);
        EXPECT_EQ(9'223'372'036'854'775'808ULL, result2p63);
    }

    {
        auto result4p0 = CalculateIntegerPow<4, 0>();
        EXPECT_EQ(1ULL, result4p0);
        auto result4p1 = CalculateIntegerPow<4, 1>();
        EXPECT_EQ(4ULL, result4p1);
        auto result4p2 = CalculateIntegerPow<4, 2>();
        EXPECT_EQ(16ULL, result4p2);
        auto result4p4 = CalculateIntegerPow<4, 4>();
        EXPECT_EQ(256ULL, result4p4);
        auto result4p8 = CalculateIntegerPow<4, 8>();
        EXPECT_EQ(std::numeric_limits<uint16_t>::max(), result4p8-1);
        auto result4p16 = CalculateIntegerPow<4, 16>();
        EXPECT_EQ(std::numeric_limits<uint32_t>::max(), result4p16-1);
        //auto result4p32 = CalculateIntegerPow<4, 32>(); // error
        auto result4p31 = CalculateIntegerPow<4, 31>();
        auto result2p63 = result4p31*2;
        EXPECT_EQ(std::numeric_limits<uint64_t>::max(), (result2p63-1)*2+1);
        EXPECT_EQ(std::numeric_limits<uint64_t>::max(), (result2p63-1)*2+1);
        EXPECT_EQ(9'223'372'036'854'775'808ULL, result2p63);
    }

    auto result100p0 = CalculateIntegerPow<100, 0>();
    EXPECT_EQ(1ULL, result100p0);
    auto result100p1 = CalculateIntegerPow<100, 1>();
    EXPECT_EQ(100ULL, result100p1);
    auto result100p2 = CalculateIntegerPow<100, 2>();
    EXPECT_EQ(10000ULL, result100p2);
}

/*
TEST(IntegerPow, LargeN)
{
    constexpr auto result4p16add1 = CalculateIntegerPow<4, 16>()+1;
    auto result4p16add1p2 = LookupIntegerPow<result4p16add1, 3>(2); // In 5.1.0 this does not error.
    EXPECT_EQ(0ULL, result4p16add1p2);
    auto result4p16add1p1 = LookupIntegerPow<result4p16add1, 3>(1);
    ASSERT_EQ(std::numeric_limits<uint32_t>::max(), result4p16add1p1-2);
}
*/

TEST(IntegerPow, LookupIntegerPow)
{
    auto result3p4 = LookupIntegerPow<3, 5>(4);
    EXPECT_EQ(81ULL, result3p4);
    auto result7p5 = LookupIntegerPow<7, 6>(5);
    EXPECT_EQ(16807ULL, result7p5);
    auto result10p12 = LookupIntegerPow<10, 15>(12);
    ASSERT_EQ(1'000'000'000'000ULL, result10p12);
}

TEST(IntegerPow, IntegerPow10)
{
    // note: 2^64 = 1.8*10^19.
    auto GetPowOf10 = [](std::size_t exp) -> auto
    {
        //auto result10p21 = CalculateIntegerPow<10, 21>(); // error
        return LookupIntegerPow<10, 20>(exp);
    };
    EXPECT_EQ(1ULL, GetPowOf10(0));
    EXPECT_EQ(10ULL, GetPowOf10(1));
    EXPECT_EQ(100ULL, GetPowOf10(2));
    EXPECT_EQ(1'000ULL, GetPowOf10(3));
    EXPECT_EQ(10'000ULL, GetPowOf10(4));
    EXPECT_EQ(100'000ULL, GetPowOf10(5));
    EXPECT_EQ(1'000'000ULL, GetPowOf10(6));
    EXPECT_EQ(10'000'000ULL, GetPowOf10(7));
    EXPECT_EQ(100'000'000ULL, GetPowOf10(8));
    EXPECT_EQ(1'000'000'000ULL, GetPowOf10(9));
    EXPECT_EQ(10'000'000'000ULL, GetPowOf10(10));
    EXPECT_EQ(100'000'000'000ULL, GetPowOf10(11));
    EXPECT_EQ(1'000'000'000'000ULL, GetPowOf10(12));
    EXPECT_EQ(10'000'000'000'000ULL, GetPowOf10(13));
    EXPECT_EQ(100'000'000'000'000ULL, GetPowOf10(14));
    EXPECT_EQ(1'000'000'000'000'000ULL, GetPowOf10(15));
    EXPECT_EQ(10'000'000'000'000'000ULL, GetPowOf10(16));
    EXPECT_EQ(100'000'000'000'000'000ULL, GetPowOf10(17));
    EXPECT_EQ(1'000'000'000'000'000'000ULL, GetPowOf10(18));
    EXPECT_EQ(10'000'000'000'000'000'000ULL, GetPowOf10(19));
    ASSERT_EQ(0ULL, GetPowOf10(20));
}

TEST(IntegerPow, LookupIntegerPowOutOfTableRange)
{
    auto result = LookupIntegerPow<10, 10>(10);
    ASSERT_EQ(0ULL, result);
}

} // namespace ies
