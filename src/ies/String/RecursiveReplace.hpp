#pragma once

#include "ies/ies_export.h"

#include <string>

namespace ies
{

//! @brief Recursively replace all occurence of [from] in string [s] to [to] until no occurence.
//! @example
//!     std::string s = "a///////b//c/d//e////";
//!     RecursiveReplace(s, "//", "/");
//!     ASSERT_EQ("a/b/c/d/e/", s);
IES_EXPORT
void
RecursiveReplace(std::string &s, const std::string &from, const std::string &to);

}
