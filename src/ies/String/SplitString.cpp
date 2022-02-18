#include "ies/String/SplitString.hpp"

namespace ies
{

std::vector<std::string>
SplitString(const std::string &separators,
            const std::string &input,
            size_t reserveHint)
{
    std::vector<std::string> tokens;
    tokens.reserve(reserveHint);

    //'' separators = !@#$
    //'' input = !@#$abcd!@#$
    //''         ----^ first not of separators
    //''              ---^ first of separators

    std::size_t start = 0;
    std::size_t end = 0;
    while ((start = input.find_first_not_of(separators, end))!=std::string::npos)
    {
        end = input.find_first_of(separators, start+1);
        if (end==std::string::npos)
        {
            end = input.length();
        }
        tokens.emplace_back(input.substr(start, end-start));
    }
    return tokens;
}

std::vector<std::string>
SplitStringPreserve(const std::string &separators,
                    const std::string &input,
                    size_t reserveHint)
{
    std::vector<std::string> tokens;
    tokens.reserve(reserveHint);

    //'' separators = !@#$
    //'' input = !@#$abcd!@#$
    //''         ^ prevSep
    //''          ^ nextSep

    auto nextSep = input.find_first_of(separators, 0);
    if (nextSep==std::string::npos)
    {
        tokens.emplace_back(input);
        return tokens;
    }
    tokens.emplace_back(input.substr(0, nextSep-0));

    while (nextSep!=std::string::npos)
    {
        auto prevSep = nextSep;
        nextSep = input.find_first_of(separators, prevSep+1);
        tokens.emplace_back(input.substr(prevSep+1, nextSep-(prevSep+1)));
    }

    return tokens;
}

}
