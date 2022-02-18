#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include <cstring>

#include "ies/Common/Byte.hpp"

//'' This header assumes system using little endian.
//'' i.e. behavior of reinterpret_cast result same as little endian.
namespace ies
{

//! @brief Read buffer[0] by n byte as big endian value T, n is size of T.
//! Can use arithmetic types like integral type or floating type.
//! @example Read 4 bytes in buffer as big endian uint32:
//!     ByteArray buffer{0x01, 0x02, 0x03, 0x04};
//!     auto bigEndian = ToBigEndian<uint32_t>(buffer);
//!     //bigEndian = 0x01020304
//!     auto littleEndian = *reinterpret_cast<const uint32_t*>(buffer.data());
//!     //littleEndian = 0x04030201
template <typename T>
T
ToBigEndian(const Byte* buffer)
{
    constexpr auto size = sizeof(T);
    T value = 0;
    auto* p = reinterpret_cast<char*>(&value);
    std::memcpy(p, buffer, size);
    for (std::size_t i = 0; i<size/2; ++i)
    {
        std::swap(p[i], p[size-i-1]);
    }
    return value;
}

//! @brief Read buffer[offset] by n byte as big endian value T, n is size of T. And then increase offset by n.
template <typename T>
T
ToBigEndian(const Byte* buffer, std::size_t &offset)
{
    auto value = ToBigEndian<T>(&buffer[offset]);
    offset += sizeof(T);
    return value;
}

//! @brief Read buffer[0] by n byte as little endian value T, n is size of T.
//! Just a convenient wrapper to save boilerplate reinterpret when reading little endian in buffer.
template <typename T>
T
ToLittleEndian(const Byte* buffer)
{
    return *reinterpret_cast<const T*>(buffer);
}

//! @brief Read buffer[offset] by n byte as little endian value T, n is size of T. And then increase offset by n.
//! Just a convenient wrapper to save boilerplate reinterpret when reading little endian in buffer.
template <typename T>
T
ToLittleEndian(const Byte* buffer, std::size_t &offset)
{
    auto value = ToLittleEndian<T>(&buffer[offset]);
    offset += sizeof(T);
    return value;
}

}
