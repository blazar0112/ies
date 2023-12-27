#include "ies/StdUtil/Find.hxx"

#include <string>
#include <string_view>

namespace FindBuildTime
{

bool Test()
{
    const std::string str{"abcdef"};
    return ies::Find(str, std::string_view("g")).has_value();
}

} // namespace FindBuildTime
