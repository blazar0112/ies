#pragma once

#include "ies/ies_export.h"

#include <string>
#include <utility>

namespace ies
{

//! @brief Compare two string and return their edit distance (Damerau-Levenshtein distance).
//! Use to check if two strings are very similar and difference is caused by typo.
//! @return Pair of {Levenshtein distance, Normalized Factor [0.0~1.0]}.
//!     Normalized Factor is the similarness, where 1.0 is 100% similar.
//! @see https://en.wikipedia.org/wiki/Damerau%E2%80%93Levenshtein_distance
IES_EXPORT
std::pair<std::size_t, double>
DamerauLevenshtein(const std::string &str1, const std::string &str2);

}
