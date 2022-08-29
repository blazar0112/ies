#include "ies/Common/SmartEnum.hxx"

#include "gtest/gtest.h"

namespace MyTest
{

IES_SMART_ENUM(MyEnum, A, B, C);
IES_SMART_ENUM(AssignValueEnum, A, B=2, C);

namespace a
{
    IES_SMART_ENUM(InsideNamespaceEnum, A, B, C, D, E);
}

TEST(SmartEnum, Usage)
{
    EXPECT_EQ(static_cast<int>(0), MyEnumSmartEnum::Min());
    EXPECT_EQ(2, MyEnumSmartEnum::Max());
    EXPECT_EQ(MyEnum::A, MyEnumSmartEnum::MinEnum());
    EXPECT_EQ(MyEnum::C, MyEnumSmartEnum::MaxEnum());
    EXPECT_EQ(3u, MyEnumSmartEnum::Size());
    std::vector<MyEnum> range{MyEnum::A, MyEnum::B, MyEnum::C};
    EXPECT_EQ(range, MyEnumSmartEnum::ToRange());
    EXPECT_EQ("A", ToString(MyEnum::A));
    EXPECT_EQ(MyEnum::A, ToMyEnum("A"));
    ASSERT_EQ("A, B, C", MyEnumSmartEnum::GetEnumString());
}

TEST(SmartEnum, AssignValue)
{
    //'' since it's paste into enum class definition, so when not using ToString it behavior correctly.
    EXPECT_EQ(3, static_cast<int>(AssignValueEnum::C));
    //'' current implementation requires storing string in vector and access by index, which is not actual enum value
    //'' if enum is not begin with 0 and increase by 1 for each enumerator.
    ASSERT_THROW(
        {
            AssignValueEnumSmartEnum::ToString(AssignValueEnum::B);
        },
        std::runtime_error
    );
}

TEST(SmartEnum, InsideNamespaceEnum)
{
    //'' ADL
    EXPECT_EQ("B", ToString(a::InsideNamespaceEnum::B));
    ASSERT_EQ(5u, a::InsideNamespaceEnumSmartEnum::Size());
}

TEST(SmartEnum, CompileTimeConst)
{
    int array[MyEnumSmartEnum::Size()];

    ASSERT_EQ(3u, sizeof(array)/sizeof(array[0]));
}

TEST(SmartEnum, Has)
{
    EXPECT_TRUE(MyEnumSmartEnum::Has("C"));
    ASSERT_FALSE(MyEnumSmartEnum::Has("D"));
}

TEST(SmartEnum, ToIndex)
{
    ASSERT_EQ(1u, ToIndex(MyEnum::B));
}

}
