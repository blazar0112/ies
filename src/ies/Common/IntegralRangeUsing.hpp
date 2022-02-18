#pragma once

//! @brief This header promote common using IntegralRanges, use if you don't have name collision issue.
//! If you have name collision issue, simply change back to using IntegralRange.hxx header and qualifiy with 'ies::'.

#include "ies/Common/IntegralRange.hxx"

//! @brief Promote to global space for convenience.
template <typename T, ies::RangeDirection Direction=ies::RangeDirection::Forward>
using IntegralRange = ies::IntegralRange<T, Direction>;

using IntRange = ies::IntRange;
using ReverseIntRange = ies::ReverseIntRange;
using UIntRange = ies::UIntRange;
using ReverseUIntRange = ies::ReverseUIntRange;
using IndexRange = ies::IndexRange;
using ReverseIndexRange = ies::ReverseIndexRange;
