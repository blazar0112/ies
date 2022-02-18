#include "ies/StdUtil/RequireCpp17.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <vector>

#include "benchmark/benchmark.h"

#include "ies/Common/AdjacentArrayRange.hxx"
#include "ies/Common/AdjacentVectorRange.hxx"
#include "ies/Common/Endian.hxx"
#include "ies/Common/IntegralRangeUsing.hpp"
#include "ies/Common/StringTree.hpp"

#include "ies/StdUtil/Find.hxx"
#include "ies/StdUtil/FormatString.hxx"
#include "ies/StdUtil/IsIn.hxx"
#include "ies/StdUtil/MapApply.hxx"

#include "ies/String/Levenshtein.hpp"
#include "ies/String/RecursiveReplace.hpp"
#include "ies/String/SplitString.hpp"
#include "ies/String/SplitStringView.hpp"

#include "ies/Time/Timer.hpp"
#include "ies/Time/TimeUtilFormat.hxx"

namespace s2Time = ies::Time;

//'' size = 5405 chars.
const std::string VeryLongString =
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    " ! ( 9, j,a s,ijd, (,)(*^*&sdf98sd9fjs8,99ki20qv3q'09923,842djfojn23j4),,,"
    "joi w j o wef0   mjas,89*(vo1j2io (A, y,)(123j , oasidasd )(aisjdjiqwe), )(Q(W)E0qi0kafkomv091123"
;

IES_SMART_ENUM(AlphabetSE, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);

enum class Alphabet
{
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
};

class Fixture : public benchmark::Fixture
{
public:
    std::vector<int> v;
    std::map<std::string, AlphabetSE> alphabetMap;
    std::vector<int> rawAlphabetEnumRange;
    std::string str = "abcdef";
    std::string treeSpec =
        "{abcdef, a, b, c, d, e, f}"
        "{a, ab, ac, ad, ae, af} {ab, abc, abd, abe, abf} {ac, acd, ace, acf} {ad, ade, adf} {ae, aef}"
        "{b, bc, bd, be, bf} {bc, bcd, bce, bcf} {bd, bde, bdf} {be, bef}"
        "{c, cd, ce, cf} {cd, cde, cdf} {ce, cef}"
        "{d, de, df} {de, def}"
        "{e, ef}"
        "{abc, abcd, abce, abcf}";
    ies::StringTree stringTree{"abcdef", treeSpec};
    ies::Timer timer;
    ies::ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    IntRange intRange{0, 1000};
    IntRange intRange2{250, 750};

        Fixture()
        {
            v = std::vector<int>(1000);
            std::iota(v.begin(), v.end(), 0);
            std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

            for (auto alphabet : AlphabetSESmartEnum::ToRange())
            {
                alphabetMap[ToString(alphabet)] = alphabet;
            }

            rawAlphabetEnumRange = std::vector<int>(26);
            std::iota(rawAlphabetEnumRange.begin(), rawAlphabetEnumRange.end(), 0);
        }
};

void
BM_ExampleStringCreation(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        std::string empty;
        benchmark::DoNotOptimize(empty);
    }
}
BENCHMARK(BM_ExampleStringCreation);

void
BM_ExampleStringCopy(benchmark::State &state)
{
    std::string x = "hello";
    for (auto _ : state)
    {
        (void)_;
        // NOLINTNEXTLINE(performance-unnecessary-copy-initialization)
        std::string copy(x);
    }
}
BENCHMARK(BM_ExampleStringCopy);

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, MakeAdjacentArrayRange)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (auto itArray : ies::MakeAdjacentArrayRange<2>(v))
        {
            sum += *itArray[1];
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, MakeAdjacentArrayRange_ToValueArray)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (auto itArray : ies::MakeAdjacentArrayRange<2>(v))
        {
            auto array = ies::ToValueArray(v, itArray);
            sum += array[1];
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, MakeAdjacentVectorRange)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (auto itVector : ies::MakeAdjacentVectorRange(v, 2))
        {
            sum += *itVector[1];
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, MakeAdjacentVectorRange_ToValueVector)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (auto itVector : ies::MakeAdjacentVectorRange(v, 2))
        {
            auto vector = ies::ToValueVector(v, itVector);
            sum += vector[1];
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, RawEnumIterate)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        int vowelCount = 0;
        for (auto i : rawAlphabetEnumRange)
        {
            auto alphabet = static_cast<Alphabet>(i);
            if (alphabet==Alphabet::A||alphabet==Alphabet::E||alphabet==Alphabet::I||alphabet==Alphabet::O||alphabet==Alphabet::U)
            {
                ++vowelCount;
            }
        }
        benchmark::DoNotOptimize(vowelCount);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, SmartEnumIterate)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        int vowelCount = 0;
        for (auto alphabet : AlphabetSESmartEnum::ToRange())
        {
            if (alphabet==AlphabetSE::A||alphabet==AlphabetSE::E||alphabet==AlphabetSE::I||alphabet==AlphabetSE::O||alphabet==AlphabetSE::U)
            {
                ++vowelCount;
            }
        }
        benchmark::DoNotOptimize(vowelCount);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IsIn)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        int testPassCount = 0;
        if (ies::IsIn(str, "g"))
        {
            ++testPassCount;
        }
        if (ies::IsIn(alphabetMap, "X"))
        {
            ++testPassCount;
        }
        //'' IsIn cannot find vector.
//        std::set<int> set{v.begin(), v.end()};
//        if (StdUtil::IsIn(987, set))
//        {
//            ++testPassCount;
//        }
        benchmark::DoNotOptimize(testPassCount);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, Find)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        int testPassCount = 0;
        if (ies::Find(str, "g"))
        {
            ++testPassCount;
        }
        if (ies::Find(alphabetMap, "X"))
        {
            ++testPassCount;
        }
//        if (ies::Find(v, 987))
//        {
//            ++testPassCount;
//        }
        benchmark::DoNotOptimize(testPassCount);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, StringTree_Construct)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        ies::StringTree tree{"abcdef", treeSpec};
        benchmark::DoNotOptimize(tree);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, StringTree_HasNode)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        stringTree.HasNode("acdef");
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, StringTree_ConstructAndRenameNode)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        ies::StringTree tree{"abcdef", treeSpec};
        tree.RenameNode("de", "ed");
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, StringTree_ConstructAndRemoveNode)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        ies::StringTree tree{"abcdef", treeSpec};
        tree.RemoveNode("b");
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, StringTree_ConstructAndRemoveLevel)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        ies::StringTree tree{"abcdef", treeSpec};
        tree.RemoveLevel(1);
    }
}

void
BM_Timer_Construct(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        ies::Timer timer;
    }
}
BENCHMARK(BM_Timer_Construct);

void
BM_Timer_ConstructStartAndEnd(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        ies::Timer timer;
        timer.Start("HasNode");
        timer.End("HasNode");
    }
}
BENCHMARK(BM_Timer_ConstructStartAndEnd);

void
BM_TimeUtil_CountNs(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto begin = s2Time::Now();
        auto nsCount = s2Time::CountNs(begin);
        benchmark::DoNotOptimize(nsCount);
    }
}
BENCHMARK(BM_TimeUtil_CountNs);

void
BM_TimeUtil_CountNsAndFormat(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto begin = s2Time::Now();
        auto nsCount = s2Time::CountNs(begin);
        auto format = s2Time::Format<std::chrono::milliseconds>(nsCount);
        benchmark::DoNotOptimize(format);
    }
}
BENCHMARK(BM_TimeUtil_CountNsAndFormat);

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, ToBigEndian)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto bigEndian = ies::ToBigEndian<uint64_t>(buffer.data());
        benchmark::DoNotOptimize(bigEndian);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, ToLittleEndian)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto littleEndian = ies::ToLittleEndian<uint64_t>(buffer.data());
        benchmark::DoNotOptimize(littleEndian);
    }
}

void
BM_MapIncrementCount(benchmark::State &state)
{
    std::map<AlphabetSE, long long> alphabetCounts;
    for (auto _ : state)
    {
        (void)_;
        for (auto alphabet : AlphabetSESmartEnum::ToRange())
        {
            ies::MapIncrementCount(alphabetCounts, alphabet);
        }
    }
}
BENCHMARK(BM_MapIncrementCount);

void
BM_SplitString(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto tokens = ies::SplitString(" ,()", VeryLongString);
    }
}
BENCHMARK(BM_SplitString);

void
BM_SplitStringView(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto tokens = ies::SplitStringView(" ,()", VeryLongString);
    }
}
BENCHMARK(BM_SplitStringView);

void
BM_RecursiveReplace(benchmark::State &state)
{
    std::string s = "a///////b//c/d//e////";
    for (auto _ : state)
    {
        (void)_;
        ies::RecursiveReplace(s, "//", "/");
    }
}
BENCHMARK(BM_RecursiveReplace);

void
BM_DamerauLevenshtein(benchmark::State &state)
{
    auto otherVeryLongString = "my_"+VeryLongString+"_test";
    for (auto _ : state)
    {
        (void)_;
        ies::DamerauLevenshtein(VeryLongString, otherVeryLongString);
    }
}
BENCHMARK(BM_DamerauLevenshtein);

void
BM_IntRange_Construct(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        IntRange irange{0, 1000};
        benchmark::DoNotOptimize(irange);
    }
}
BENCHMARK(BM_IntRange_Construct);

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_RawLoop_Const)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (int i=0; i<1000; ++i)
        {
            sum += i;
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_DEFINE_F(Fixture, IntRange_RawLoop_Iterate)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (int i=0; i<state.range(0); ++i)
        {
            sum += i;
        }
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK_REGISTER_F(Fixture, IntRange_RawLoop_Iterate)
->Arg(10)
->Arg(100)
->Arg(1000)
;

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_Iterate)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (auto i : intRange)
        {
            sum += i;
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_IterateAndConstruct)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        long long sum = 0;
        for (auto i : IntRange{0, 1000})
        {
            sum += i;
        }
        benchmark::DoNotOptimize(sum);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_IsInRange)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto isInRange = intRange.IsInRange(1750);
        benchmark::DoNotOptimize(isInRange);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_FindClosest)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto closest = intRange.FindClosest(1750);
        benchmark::DoNotOptimize(closest);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_FindDistance)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto distance = intRange.FindDistance(1750);
        benchmark::DoNotOptimize(distance);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_Contains)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto contains = intRange.Contains(intRange2);
        benchmark::DoNotOptimize(contains);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_Overlaps)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto overlaps = intRange.Overlaps(intRange2);
        benchmark::DoNotOptimize(overlaps);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, IntRange_Equal)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto equals = (intRange==intRange2);
        benchmark::DoNotOptimize(equals);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, Shift)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto shifted = Shift(intRange, 50);
        benchmark::DoNotOptimize(shifted);
    }
}

// NOLINTNEXTLINE(readability-redundant-member-init)
BENCHMARK_F(Fixture, ToString)(benchmark::State &state)
{
    for (auto _ : state)
    {
        (void)_;
        auto format = ToString(intRange);
        benchmark::DoNotOptimize(format);
    }
}

BENCHMARK_MAIN();
