#pragma once

#include "ies/StdUtil/RequireCpp20.hpp" // IWYU pragma: keep

#include <cstddef>
#include <cstdint>

#include <array>
#include <bit>
#include <utility>

namespace ies
{

//! @brief Calculate N^Exp in compile time. Check if N^Exp is possible to overflow uint64_t by bits.
template <uint64_t N, uint64_t Exp>
[[nodiscard]]
consteval uint64_t
CalculateIntegerPow()
{
    constexpr int MaxBitCount = 8*sizeof(uint64_t);

    //! @brief Check total bits when n2Bits > CheckN2BitCount.
    //! @note If check N^2 bits too early, it can be false positive, like 10^19.
    //! N=10, Exp=19, nBits = 4, n2Bits = 7, totalBits = 4+7*9.
    //! Workaround: when n2Bits is small enough, go in one level and don't check at this level.
    constexpr int CheckN2BitCount = 8;

    constexpr int nBits = std::bit_width(N-1);
    constexpr int n2Bits = std::bit_width(N*N-1);

    if constexpr (N==0&&Exp==0)
    {
        return 1ULL;
    }
    else if constexpr (N==0)
    {
        return 0ULL;
    }
    else if constexpr (N==1||Exp==0)
    {
        return 1ULL;
    }
    else if constexpr (Exp==1)
    {
        return N;
    }
    else if constexpr (N==2)
    {
        static_assert(Exp<MaxBitCount, "N^MaxExp bit check: > size of uint64.");
        return 1ULL<<Exp;
    }
    else if constexpr (Exp%2==1)
    {
        constexpr int totalBits = nBits+n2Bits*(Exp-1)/2;
        if constexpr (n2Bits>CheckN2BitCount)
        {
            static_assert(nBits<(MaxBitCount/2), "N^MaxExp bit estimate check: may > size of uint64.");
            static_assert(totalBits<MaxBitCount, "N^MaxExp bit estimate check: may > size of uint64.");
        }
        return N*CalculateIntegerPow<N*N, (Exp-1)/2>();
    }
    else
    {
        constexpr int totalBits = n2Bits*Exp/2;
        if constexpr (n2Bits>CheckN2BitCount)
        {
            static_assert(nBits<(MaxBitCount/2), "N^MaxExp bit estimate check: may > size of uint64.");
            static_assert(totalBits<MaxBitCount, "N^MaxExp bit estimate check: may > size of uint64.");
        }
        return CalculateIntegerPow<N*N, Exp/2>();
    }
};

//! @brief Construct compile time table of [N^0, N^1, ..., N^(MaxExp-1)].
template <uint64_t N, uint64_t MaxExp, uint64_t... I>
[[nodiscard]]
constexpr std::array<uint64_t, MaxExp>
GenerateIntegerPowTable(std::index_sequence<I...> indexes)
{
    (void)indexes;
    return {{ CalculateIntegerPow<N, I>()... }};
}

//! @brief Lookup N^exp from compile time constructed table of [N^0, N^1, ..., N^(MaxExp-1)].
//! @return N^exp, 0 if exp is out of [0, MaxExp).
//! @note if N^(MaxExp-1) is out of uint64_t then static assert fail.
//! @note Use CalculateIntegerPow<N, Exp> if only want to calculate N^Exp at compile-time.
//! This is for frequently lookup in [N^0, N^1, ..., N^(MaxExp-1)].
template <uint64_t N, uint64_t MaxExp>
[[nodiscard]]
uint64_t
LookupIntegerPow(std::size_t exp)
noexcept
{
    static auto Table = GenerateIntegerPowTable<N, MaxExp>(std::make_index_sequence<MaxExp>{});
    if (exp>=MaxExp) { return 0ULL; }
    return Table[exp];
}

} // namespace ies
