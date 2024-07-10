#pragma once

#include "ies/StdUtil/RequireCpp20.hpp" // IWYU pragma: keep

#include <cstddef>
#include <cstdint>

#include <array>
#include <bit>
#include <utility>

namespace ies
{

//! @brief Calculate N^Exp in compile time.
template <uint64_t N, uint64_t Exp>
[[nodiscard]]
consteval uint64_t
CalculateIntegerPower()
{
    constexpr int MaxBitCount = 64;
    constexpr int n2Bits = std::bit_width(N*N);
    if constexpr (Exp%2==1)
    {
        constexpr int nBits = std::bit_width(N);
        constexpr int totalBits = nBits+n2Bits*(Exp-1)/2;
        static_assert(totalBits<=MaxBitCount);
        return N*CalculateIntegerPower<N*N, (Exp-1)/2>();
    }
    else if constexpr (Exp!=0&&Exp%2==0)
    {
        constexpr int totalBits = n2Bits*Exp/2;
        static_assert(totalBits<=MaxBitCount);
        return CalculateIntegerPower<N*N, Exp/2>();
    }
    else
    {
        return 1;
    }
};

} // namespace ies

namespace ies::Detail
{

//! @brief Construct compile time table of [N^0, N^1, ..., N^(MaxExp-1)].
template <uint64_t N, uint64_t MaxExp, uint64_t... I>
[[nodiscard]]
constexpr std::array<uint64_t, MaxExp>
GenerateExpValueList(std::index_sequence<I...> indexes)
{
    (void)indexes;
    return {{ CalculateIntegerPower<N, I>()... }};
}

} // namespace ies::Detail

namespace ies
{

//! @brief Calculate N^exp, construct compile time table of [N^0, N^1, ..., N^(MaxExp-1)] at first call and use cached result.
//! @return N^exp, 0 if exp is out of [0, MaxExp).
//! @note if N^MaxExp is out of uint64_t then static assert fail.
template <uint64_t N, uint64_t MaxExp>
[[nodiscard]]
uint64_t
FastIntegerPow(std::size_t exp)
noexcept
{
    static auto ValueList = Detail::GenerateExpValueList<N, MaxExp>(std::make_index_sequence<MaxExp>{});
    if (exp>=MaxExp) { return 0; }
    return ValueList[exp];
}

} // namespace ies
