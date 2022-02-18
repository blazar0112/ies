#pragma once

#include "ies/ies_export.h"

#include <string>
#include <vector>

namespace ies
{

using Byte = unsigned char;
using ByteArray = std::vector<Byte>;

    //! @brief Pretty format content of ByteArray. (16 bytes per line)
    //! @example String format of ToString(myArray, "myArray"):
    //! "ByteArray {myArray}, range [0, 21), size [21]:\n"
    //! "|00 01 02 03 04 05 06 07|08 09 0A 0B 0C 0D 0E 0F| Line Offset\n"
    //! "+-----------------------+-----------------------+\n"
    //! "|1B 01 31 01 17 04 18 85|87 14 30 35 39 38 36 35| 00\n"
    //! "|13 02 32 62 88         |                       | 10\n";
    IES_EXPORT
    std::string
    ToString(const ByteArray &byteArray, const std::string &name="unnamed");

    //! @brief Pretty format content of ByteArray of range. (16 bytes per line)
    //! @example String format of ToString(myArray, 0x07, 0x1A):
    //! "ByteArray {unnamed}, range [7, 26), size [19]:\n"
    //! "|00 01 02 03 04 05 06 07|08 09 0A 0B 0C 0D 0E 0F| Line Offset\n"
    //! "+-----------------------+-----------------------+\n"
    //! "|                     85|87 14 30 35 39 38 36 35| 00\n"
    //! "|1B 01 31 01 17 04 18 85|87 14                  | 10\n";
    IES_EXPORT
    std::string
    ToString(const ByteArray &byteArray, std::size_t begin, std::size_t end, const std::string &name="unnamed");

    //! @brief Pretty format content of Byte pointer (of data in ByteArray or c array) between [begin, end).
    //! @note ToString(ByteArray) equivalent to ToString(ByteArray.data(), 0, ByteArray.size()).
    IES_EXPORT
    std::string
    ToString(const Byte* data, std::size_t begin, std::size_t end, const std::string &name="unnamed");
}
