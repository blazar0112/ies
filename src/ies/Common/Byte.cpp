#include "ies/Common/Byte.hpp"

#include <iostream>

#include "fmt/format.h"

#include "ies/Common/IntegralRange.hxx"

namespace ies
{

std::string
ToString(const ByteArray &byteArray, const std::string &name)
{
    return ToString(byteArray, 0, byteArray.size(), name);
}

std::string
ToString(const ByteArray &byteArray, std::size_t begin, std::size_t end, const std::string &name)
{
    if (end>byteArray.size())
    {
        end = byteArray.size();
    }
    return ToString(byteArray.data(), begin, end, name);
}

std::string
ToString(const Byte* data, std::size_t begin, std::size_t end, const std::string &name)
{
    if (begin>end)
    {
        begin = end;
    }
    auto s = "ByteArray {"+name+"}, range ["+std::to_string(begin)+", "+std::to_string(end)+"), size ["+std::to_string(end-begin)+"]:\n"
             "|00 01 02 03 04 05 06 07|08 09 0A 0B 0C 0D 0E 0F| Line Offset\n"
             "+-----------------------+-----------------------+\n";

    if (begin==end)
    {
        return s;
    }

    auto lineCount = begin/16;
    if (begin%16!=0)
    {
        for (auto i : IndexRange(0, begin%16))
        {
            char separate = '|';
            if (i%16!=0 && i%16!=8)
            {
                separate = ' ';
            }
            s += separate+std::string(2, ' ');
        }
    }

    for (auto i : IndexRange(begin, end, EmptyPolicy::Allow))
    {
        char separate = '|';
        if (i%16!=0 && i%16!=8)
        {
            separate = ' ';
        }
        s += separate+fmt::format("{:02X}", data[i]);

        if ((i+1)%16==0)
        {
            s += "| "+fmt::format("{:X}", lineCount)+"0\n";
            ++lineCount;
        }
    }

    if (end%16!=0)
    {
        for (auto i : IndexRange(end%16, 16))
        {
            char separate = '|';
            if (i%16!=0 && i%16!=8)
            {
                separate = ' ';
            }
            s += separate+std::string(2, ' ');
        }

        s += "| "+fmt::format("{:X}", lineCount)+"0\n";
    }
    return s;
}

}
