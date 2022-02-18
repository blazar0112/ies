#include "ies/Common/Endian.hxx"

#include "gtest/gtest.h"

#include <iostream>

#include "ies/Common/IntegralRangeUsing.hpp"

namespace ies
{

TEST(Endian, ToBigEndianU8)
{
    ByteArray buffer{0xFF};
    auto bigEndian = ToBigEndian<unsigned char>(buffer.data());
    ASSERT_EQ(0xFF, bigEndian);
}

TEST(Endian, ToBigEndianS8)
{
    ByteArray buffer{0xFF};
    auto bigEndian = ToBigEndian<signed char>(buffer.data());
    ASSERT_EQ(-1, bigEndian);
}

TEST(Endian, ToBigEndianU16)
{
    ByteArray buffer{0xAB, 0xCD};
    auto bigEndian = ToBigEndian<uint16_t>(buffer.data());
    ASSERT_EQ(0xABCD, bigEndian);
}

TEST(Endian, ToBigEndianS16)
{
    ByteArray buffer{0xAB, 0xCD};
    auto bigEndian = ToBigEndian<int16_t>(buffer.data());
    ASSERT_EQ(-21555, bigEndian);
}

TEST(Endian, ToBigEndianU32)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04};
    auto littleEndian = *reinterpret_cast<const uint32_t*>(buffer.data());
    EXPECT_EQ(0x04030201u, littleEndian);

    auto bigEndian = ToBigEndian<uint32_t>(buffer.data());
    ASSERT_EQ(0x01020304u, bigEndian);
}

TEST(Endian, ToBigEndianS32)
{
    ByteArray buffer{0xAB, 0xCD, 0xEF, 0x04};
    auto bigEndian = ToBigEndian<int32_t>(buffer.data());
    ASSERT_EQ(-1412567292, bigEndian);
}

TEST(Endian, ToBigEndianU64)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    auto bigEndian = ToBigEndian<uint64_t>(buffer.data());
    ASSERT_EQ(0x0102030405060708ull, bigEndian);
}

TEST(Endian, ToBigEndianS64)
{
    ByteArray buffer{0xFE, 0xDC, 0xBA, 0x90, 0x09, 0xAB, 0xCD, 0xEF};
    auto bigEndian = ToBigEndian<int64_t>(buffer.data());
    ASSERT_EQ(-81985565399200273, bigEndian);
}

TEST(Endian, ToBigEndianFloat)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04};
    auto littleEndian = *reinterpret_cast<const float*>(buffer.data());
    EXPECT_FLOAT_EQ(1.53999e-36F, littleEndian);
    auto bigEndian = ToBigEndian<float>(buffer.data());
    ASSERT_FLOAT_EQ(2.38794e-38F, bigEndian);
}

TEST(Endian, ToBigEndianDouble)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    auto littleEndian = *reinterpret_cast<const double*>(buffer.data());
    EXPECT_DOUBLE_EQ(5.447603722011605e-270, littleEndian);
    auto bigEndian = ToBigEndian<double>(buffer.data());
    ASSERT_DOUBLE_EQ(8.20788039913184e-304, bigEndian);
}

TEST(Endian, ToLittleEndianU32)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04};
    auto littleEndian = ToLittleEndian<uint32_t>(buffer.data());
    ASSERT_EQ(0x04030201u, littleEndian);
}

TEST(Endian, ToLittleEndianDouble)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    auto littleEndian = ToLittleEndian<double>(buffer.data());
    ASSERT_DOUBLE_EQ(5.447603722011605e-270, littleEndian);
}

TEST(Endian, ToBigEndianWithOffset)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    std::size_t offset = 0;
    auto valueCount = buffer.size()/sizeof(uint16_t);
    std::vector<uint16_t> values(valueCount);
    for (auto i : IndexRange(0, valueCount))
    {
        values[i] = ToBigEndian<uint16_t>(buffer.data(), offset);
    }

    EXPECT_EQ(0x0102u, values[0]);
    EXPECT_EQ(0x0304u, values[1]);
    EXPECT_EQ(0x0506u, values[2]);
    EXPECT_EQ(0x0708u, values[3]);
    ASSERT_EQ(8u, offset);
}

TEST(Endian, ToLittleEndianWithOffset)
{
    ByteArray buffer{0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    std::size_t offset = 0;
    auto valueCount = buffer.size()/sizeof(uint16_t);
    std::vector<uint16_t> values(valueCount);
    for (auto i : IndexRange(0, valueCount))
    {
        values[i] = ToLittleEndian<uint16_t>(buffer.data(), offset);
    }

    EXPECT_EQ(0x0201u, values[0]);
    EXPECT_EQ(0x0403u, values[1]);
    EXPECT_EQ(0x0605u, values[2]);
    EXPECT_EQ(0x0807u, values[3]);
    ASSERT_EQ(8u, offset);
}

}
