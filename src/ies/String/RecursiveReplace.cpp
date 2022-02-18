#include "ies/String/RecursiveReplace.hpp"

namespace ies
{

void
RecursiveReplace(std::string &s, const std::string &from, const std::string &to)
{
    if (!from.empty())
    {
        size_t pos = 0;
        while (pos = s.find(from, pos), pos!=std::string::npos)
        {
            s.replace(pos, from.size(), to);
        }
    }
}

}
