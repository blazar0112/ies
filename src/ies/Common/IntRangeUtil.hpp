#pragma once

#include "ies/ies_export.h"

#include "ies/Common/IntegralRange.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace IntRangeUtil {

//! @brief Find out overlapping range between range1 and range2, returns empty IntRange if not overlapping.
//! @note If any one of range1 or range2 is empty range, always return empty range.
//! @note Since range1 and range2 can be empty range (or view as a point), and impl. using range1 as subject:
//! Here's exact table: but user should not depend on it because it's impl. defined, only need to know it's empty range (size=0).
//!
//!        range1: | empty | non-empty   1. both empty, return empty range same as range1
//! return range _\+-------+----------   2. range2 empty, return empty range, location depend on range1 and range2 relation
//! range2: empty  | 1:e   |  2:e           same as range2 if range2 point is inside("overlaps") or be side of range1, else is same as 4.
//!                +-------+----------   3. range1 empty, return empty range same as range1 regardless range1 and range2 relation
//!     non-empty  | 3:e   |  4:ne/e     4. both non-empty, return valid range if overlapping, empty range if not.
//!                                         The empty range is side point of range1 of the side closer to range2.
IES_EXPORT
ies::IntRange
FindOverlappingRange(const ies::IntRange &range1, const ies::IntRange &range2);

//! @brief Find how much target is overlapping with subject range by portion of subject size [0, 1] (i.e. 0% to 100%).
//!             30% of subject   100% of subject
//! target     |***|            |**********|
//! subject   |**********|       |***|
IES_EXPORT
double
FindOverlappingPortion(const ies::IntRange &subject, const ies::IntRange &target);

}}
