#pragma once

#include "ies/StdUtil/RequireCpp17.hpp"

#include "ies/ies_export.h"

#include <string_view>
#include <vector>

namespace ies
{

//! @brief Split string [input] to tokens separated by any char of [separators], empty token will be ignored.
//! @example
//!     std::string input = "a,,b,c,";
//!     SplitStringView(",", input) => std::vector{"a"_sv, "b"_sv, "c"_sv}.
//! @note Use [reserveHint] to reduce allocation if can estimate size of tokens.
//! @note Using string_view requires string_view outlive source std::string.
IES_EXPORT
std::vector<std::string_view>
SplitStringView(std::string_view separators,
                std::string_view input,
                std::size_t reserveHint=4);

//! @brief Split string [input] to tokens separated by any char of [separators], empty token is preserved.
//! @example
//!     std::string input = "a,,b,c,"
//!     SplitStringPreserve(",", input) => std::vector{"a"_sv, ""_sv, "b"_sv, "c"_sv, ""_sv}.
//! @note Use reserveHint to reduce allocation if can estimate size of tokens.
//! @note Using string_view requires string_view outlive source std::string.
IES_EXPORT
std::vector<std::string_view>
SplitStringViewPreserve(std::string_view separators,
                        std::string_view input,
                        size_t reserveHint=4);


}
