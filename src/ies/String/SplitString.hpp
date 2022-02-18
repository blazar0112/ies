#pragma once

#include "ies/ies_export.h"

#include <string>
#include <vector>

namespace ies
{

//! @brief Split string [input] to tokens separated by any char of [separators], empty token will be ignored.
//! @example SplitString(",", "a,,b,c,") => std::vector{"a", "b", "c"}.
//! @note Use [reserveHint] to reduce allocation if can estimate size of tokens.
IES_EXPORT
std::vector<std::string>
SplitString(const std::string &separators,
            const std::string &input,
            size_t reserveHint=4);

//! @brief Split string [input] to tokens separated by any char of [separators], empty token is preserved.
//! @example SplitStringPreserve(",", "a,,b,c,") => std::vector{"a", "", "b", "c", ""}.
//! @note Use reserveHint to reduce allocation if can estimate size of tokens.
IES_EXPORT
std::vector<std::string>
SplitStringPreserve(const std::string &separators,
                    const std::string &input,
                    size_t reserveHint=4);

}
