#include "ies/Common/SmartEnum.hxx"

#include "gtest/gtest.h"

#include <array>
#include <vector>

// NOLINTBEGIN(cert-err58-cpp,cppcoreguidelines-owning-memory)
// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
namespace MyTest
{

IES_SMART_ENUM(MyEnum, A, B, C);
IES_SMART_ENUM(AssignValueEnum, A, B=2, C);
IES_SMART_ENUM(Name, NameA, NameB, NameC);

namespace a
{
    IES_SMART_ENUM(InsideNamespaceEnum, A, B, C, D, E);
}

TEST(SmartEnum, Usage)
{
    EXPECT_EQ(0, MyEnumSmartEnum::Min());
    EXPECT_EQ(2, MyEnumSmartEnum::Max());
    EXPECT_EQ(MyEnum::A, MyEnumSmartEnum::MinEnum());
    EXPECT_EQ(MyEnum::C, MyEnumSmartEnum::MaxEnum());
    EXPECT_EQ(3U, MyEnumSmartEnum::Size());
    const std::vector<MyEnum> range{MyEnum::A, MyEnum::B, MyEnum::C};
    EXPECT_EQ(range, MyEnumSmartEnum::ToRange());
    EXPECT_EQ("A", ToString(MyEnum::A));
    EXPECT_EQ(MyEnum::A, ToMyEnum("A"));
    ASSERT_EQ("A, B, C", MyEnumSmartEnum::GetEnumDefString());
}

TEST(SmartEnum, AssignValue)
{
    //'' since it's paste into enum class definition, so when not using ToString it behavior correctly.
    EXPECT_EQ(3, static_cast<AssignValueEnumSmartEnum::UnderlyingType>(AssignValueEnum::C));
    //'' current implementation requires storing string in vector and access by index, which is not actual enum value
    //'' if enum is not begin with 0 and increase by 1 for each enumerator.
    ASSERT_THROW(
        {
            auto& s = AssignValueEnumSmartEnum::ToString(AssignValueEnum::B);
            (void)s;
        },
        std::runtime_error
    );
}

TEST(SmartEnum, InsideNamespaceEnum)
{
    //'' ADL
    EXPECT_EQ("B", ToString(a::InsideNamespaceEnum::B));
    ASSERT_EQ(5U, a::InsideNamespaceEnumSmartEnum::Size());
}

TEST(SmartEnum, CompileTimeConst)
{
    // NOLINTNEXTLINE(*avoid-c-arrays)
    int cArray[MyEnumSmartEnum::Size()];
    const std::array<int, MyEnumSmartEnum::Size()> array{};

    EXPECT_EQ(3U, array.size());
    ASSERT_EQ(3U, sizeof(cArray)/sizeof(cArray[0]));
}

TEST(SmartEnum, Has)
{
    EXPECT_TRUE(NameSmartEnum::Has("NameC"));
    EXPECT_FALSE(NameSmartEnum::Has("Name"));
    EXPECT_FALSE(NameSmartEnum::Has(", "));
    ASSERT_FALSE(NameSmartEnum::Has("NameD"));
}

TEST(SmartEnum, ToIndex)
{
    ASSERT_EQ(1U, ToIndex(MyEnum::B));
}

TEST(SmartEnum, OutOfBound)
{
    ASSERT_EQ("", ToString(static_cast<MyEnum>(4)));
}

} // namespace MyTest
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cert-err58-cpp,cppcoreguidelines-owning-memory)
