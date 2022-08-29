#include "ies/StdUtil/RequireCpp17.hpp"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <set>
#include <vector>

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

#include "ies/Terminal/Color.hpp"
#include "ies/Time/ScopeTimePrinter.hxx"
#include "ies/Time/Timer.hpp"
#include "ies/Time/TimeUtilFormat.hxx"

#include "ies/version.hpp"

namespace Color = ies::Terminal::Color;
namespace s2Time = ies::Time;

IES_SMART_ENUM(AlphabetSE, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z);

int
main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    try
    {
        ies::Time::ScopeTimePrinter<std::chrono::milliseconds> performanceTime("performance");

        std::cout << Color::LightGreen
                  << "IES v"
                  << IES_VERSION_MAJOR
                  << "."
                  << IES_VERSION_MINOR
                  << "." << IES_VERSION_PATCH
                  << Color::Reset
                  << "\n";

        std::vector<int> v(1000);
        std::iota(v.begin(), v.end(), 0);
        std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

        std::map<std::string, AlphabetSE> alphabetMap;
        for (auto alphabet : AlphabetSESmartEnum::ToRange())
        {
            alphabetMap[ToString(alphabet)] = alphabet;
        }

        {
            auto begin = s2Time::Now();
            long long sum = 0;
            for (auto itArray : ies::MakeAdjacentArrayRange<2>(v))
            {
                sum += *itArray[1];
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "AdjacentArrayRange Iterator");
            std::cout << "sum = " << sum << "\n";
        }

        {
            auto begin = s2Time::Now();
            long long sum = 0;
            for (auto itArray : ies::MakeAdjacentArrayRange<2>(v))
            {
                auto array = ies::ToValueArray(v, itArray);
                sum += array[1];
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "AdjacentArrayRange ToValueArray");
            std::cout << "sum = " << sum << "\n";
        }

        {
            auto begin = s2Time::Now();
            long long sum = 0;
            for (auto itVector : ies::MakeAdjacentVectorRange(v, 2))
            {
                sum += *itVector[1];
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "AdjacentVectorRange Iterator");
            std::cout << "sum = " << sum << "\n";
        }

        {
            auto begin = s2Time::Now();
            long long sum = 0;
            for (auto itVector : ies::MakeAdjacentVectorRange(v, 2))
            {
                auto vector = ies::ToValueVector(v, itVector);
                sum += vector[1];
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "AdjacentVectorRange ToValueVector");
            std::cout << "sum = " << sum << "\n";
        }

        {
            enum class Alphabet
            {
                A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z
            };

            std::vector<int> range(26);
            std::iota(range.begin(), range.end(), 0);

            auto begin = s2Time::Now();
            int vowelCount = 0;
            for (auto i : range)
            {
                auto alphabet = static_cast<Alphabet>(i);
                if (alphabet==Alphabet::A||alphabet==Alphabet::E||alphabet==Alphabet::I||alphabet==Alphabet::O||alphabet==Alphabet::U)
                {
                    ++vowelCount;
                }
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "Raw Enum Class");
            std::cout << "vowelCount = " << vowelCount << "\n";
        }

        //'' First time use ToRange will construct vector.
        {
            auto begin = s2Time::Now();
            int vowelCount = 0;
            for (auto alphabet : AlphabetSESmartEnum::ToRange())
            {
                if (alphabet==AlphabetSE::A||alphabet==AlphabetSE::E||alphabet==AlphabetSE::I||alphabet==AlphabetSE::O||alphabet==AlphabetSE::U)
                {
                    ++vowelCount;
                }
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "Smart Enum 1st");
            std::cout << "vowelCount = " << vowelCount << "\n";
        }

        {
            auto begin = s2Time::Now();
            int vowelCount = 0;
            for (auto alphabet : AlphabetSESmartEnum::ToRange())
            {
                if (alphabet==AlphabetSE::A||alphabet==AlphabetSE::E||alphabet==AlphabetSE::I||alphabet==AlphabetSE::O||alphabet==AlphabetSE::U)
                {
                    ++vowelCount;
                }
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "Smart Enum 2nd");
            std::cout << "vowelCount = " << vowelCount << "\n";
        }

        //'' IsIn:
        {
            std::string str = "abcdef";
            auto begin = s2Time::Now();
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
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "IsIn");
            std::cout << "testPassCount = " << testPassCount << "\n";
        }

        //'' Find:
        {
            std::string str = "abcdef";
            auto begin = s2Time::Now();
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
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "Find");
            std::cout << "testPassCount = " << testPassCount << "\n";
        }

        //'' StringTree:
        {
            std::string treeSpec =
                "{abcdef, a, b, c, d, e, f}"
                "{a, ab, ac, ad, ae, af} {ab, abc, abd, abe, abf} {ac, acd, ace, acf} {ad, ade, adf} {ae, aef}"
                "{b, bc, bd, be, bf} {bc, bcd, bce, bcf} {bd, bde, bdf} {be, bef}"
                "{c, cd, ce, cf} {cd, cde, cdf} {ce, cef}"
                "{d, de, df} {de, def}"
                "{e, ef}"
                "{abc, abcd, abce, abcf}";
            auto ctorBegin = s2Time::Now();
            ies::StringTree tree{"abcdef", treeSpec};
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(ctorBegin), "Construct StringTree");

            auto hasBegin = s2Time::Now();
            auto has = tree.HasNode("acdef");
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(hasBegin), "HasNode");
            std::cout << "Has [acdef] ? " << std::boolalpha << has << "\n";
            //std::cout << "Tree = " << ToString(tree) << "\n";

            auto renameBegin = s2Time::Now();
            tree.RenameNode("de", "ed");
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(renameBegin), "Rename");
            std::cout << "Rename [de] -> [ed]\n";
            //std::cout << "Tree = " << ToString(tree) << "\n";

            auto removeBegin = s2Time::Now();
            tree.RemoveNode("b");
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(removeBegin), "Remove");
            std::cout << "Remove [b]\n";
            //std::cout << "Tree = " << ToString(tree) << "\n";

            auto removeLevelBegin = s2Time::Now();
            tree.RemoveLevel(1);
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(removeLevelBegin), "RemoveLevel");
            std::cout << "Remove level 0\n";
            //std::cout << "Tree = " << ToString(tree) << "\n";
        }

        //'' Timer:
        {
            std::string treeSpec =
                "{abcdef, a, b, c, d, e, f}"
                "{a, ab, ac, ad, ae, af} {ab, abc, abd, abe, abf} {ac, acd, ace, acf} {ad, ade, adf} {ae, aef}"
                "{b, bc, bd, be, bf} {bc, bcd, bce, bcf} {bd, bde, bdf} {be, bef}"
                "{c, cd, ce, cf} {cd, cde, cdf} {ce, cef}"
                "{d, de, df} {de, def}"
                "{e, ef}"
                "{abc, abcd, abce, abcf}";

            auto ctorBegin = s2Time::Now();
            ies::Timer timer;
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(ctorBegin), "Construct Timer");

            ies::StringTree tree{"abcdef", treeSpec};

            auto timeUtilBegin = s2Time::Now();
            auto hasBegin = s2Time::Now();
            auto timeUtilNs = s2Time::CountNs(timeUtilBegin);
            auto has = tree.HasNode("acdeg");
            timeUtilBegin = s2Time::Now();
            auto hasNodeNs = s2Time::CountNs(hasBegin);
            timeUtilNs += s2Time::CountNs(timeUtilBegin);
            std::cout << "Has [acdeg] ? " << std::boolalpha << has << "\n"
                      << "Time of HasNode: " << hasNodeNs << "ns\n";
            s2Time::Print<std::chrono::nanoseconds>(timeUtilNs, "TimeUtil");

            auto timerBegin = s2Time::Now();
            timer.Start("HasNode");
            auto timerNs = s2Time::CountNs(timerBegin);
            auto has2 = tree.HasNode("acdeh");
            timerBegin = s2Time::Now();
            timer.End("HasNode");
            timerNs += s2Time::CountNs(timerBegin);
            std::cout << "Has [acdeh] ? " << std::boolalpha << has2 << "\n"
                      << timer.ToString("HasNode") << "\n";
            s2Time::Print<std::chrono::nanoseconds>(timerNs, "Timer");
        }

        //'' BigEndian:
        {
            ies::ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
            auto begin = s2Time::Now();
            auto bigEndian = ies::ToBigEndian<uint64_t>(buffer.data());
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "BigEndian");
            std::cout << "bigEndian = " << fmt::format("0x{:016X}", bigEndian) << "\n";
        }

        //'' LittleEndian:
        {
            ies::ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
            auto begin = s2Time::Now();
            auto littleEndian = ies::ToLittleEndian<uint64_t>(buffer.data());
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "LittleEndian");
            std::cout << "littleEndian = " << fmt::format("0x{:016X}", littleEndian) << "\n";
        }

        //'' MapApply
        {
            std::map<AlphabetSE, int> alphabetCounts;
            auto beginCount = s2Time::Now();
            for (auto alphabet : AlphabetSESmartEnum::ToRange())
            {
                ies::MapIncrementCount(alphabetCounts, alphabet);
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(beginCount), "MapIncrementCount 26 alphabets");

            std::map<AlphabetSE, std::string> alphabetNames;
            auto beginName = s2Time::Now();
            for (auto alphabet : AlphabetSESmartEnum::ToRange())
            {
                ies::MapApplyWithInit(alphabetNames, alphabet, "alphabet_", [alphabet](std::string &name){ name+=ToString(alphabet); });
            }
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(beginName), "MapApply change names of 26 alphabets");
            std::cout << "name of AlphabetSE::A = " << alphabetNames.at(AlphabetSE::A) << "\n";
        }

        std::string veryLongString =
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

        //'' SplitString:
        {
            auto begin = s2Time::Now();
            auto tokens = ies::SplitString(" ,()", veryLongString);
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "SplitString");
            std::cout << "SplitString tokens size = " << tokens.size() << ".\n";
        }

        //'' SplitStringView:
        {
            auto begin = s2Time::Now();
            auto tokens = ies::SplitStringView(" ,()", veryLongString);
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "SplitStringView");
            std::cout << "SplitStringView tokens size = " << tokens.size() << ".\n";
        }

        //'' RecursiveReplace:
        {
            std::string s = "a///////b//c/d//e////";
            auto begin = s2Time::Now();
            ies::RecursiveReplace(s, "//", "/");
            s2Time::Print<std::chrono::nanoseconds>(s2Time::CountNs(begin), "RecursiveReplace");
            std::cout << "After RecursiveReplace s = [" << s << "].\n";
        }

        //'' DamerauLevenshtein
        {
            auto begin = s2Time::Now();
            auto otherVeryLongString = "my_"+veryLongString+"_test";
            auto [distance, factor] = ies::DamerauLevenshtein(veryLongString, otherVeryLongString);
            s2Time::Print<std::chrono::seconds>(s2Time::CountNs(begin), "DamerauLevenshtein");
            std::cout << "Levenshtein distance [" << distance << "], factor = [" << fmt::format("{:.3f}", factor) << "].\n";
        }

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
