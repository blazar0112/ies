#pragma once

#include <string>

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Terminal { namespace Color {

static const std::string Reset        = "\033[0m";
static const std::string DarkBlack    = "\033[30m";
static const std::string DarkRed      = "\033[31m";
static const std::string DarkGreen    = "\033[32m";
static const std::string DarkYellow   = "\033[33m";
static const std::string DarkBlue     = "\033[34m";
static const std::string DarkMagenta  = "\033[35m";
static const std::string DarkCyan     = "\033[36m";
static const std::string DarkWhite    = "\033[37m";
static const std::string LightBlack   = "\033[1;30m";
static const std::string LightRed     = "\033[1;31m";
static const std::string LightGreen   = "\033[1;32m";
static const std::string LightYellow  = "\033[1;33m";
static const std::string LightBlue    = "\033[1;34m";
static const std::string LightMagenta = "\033[1;35m";
static const std::string LightCyan    = "\033[1;36m";
static const std::string LightWhite   = "\033[1;37m";

}}}
