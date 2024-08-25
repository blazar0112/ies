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

IES_SMART_ENUM(Enum256,
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9,
    e10, e11, e12, e13, e14, e15, e16, e17, e18, e19,
    e20, e21, e22, e23, e24, e25, e26, e27, e28, e29,
    e30, e31, e32, e33, e34, e35, e36, e37, e38, e39,
    e40, e41, e42, e43, e44, e45, e46, e47, e48, e49,
    e50, e51, e52, e53, e54, e55, e56, e57, e58, e59,
    e60, e61, e62, e63, e64, e65, e66, e67, e68, e69,
    e70, e71, e72, e73, e74, e75, e76, e77, e78, e79,
    e80, e81, e82, e83, e84, e85, e86, e87, e88, e89,
    e90, e91, e92, e93, e94, e95, e96, e97, e98, e99,
    e100, e101, e102, e103, e104, e105, e106, e107, e108, e109,
    e110, e111, e112, e113, e114, e115, e116, e117, e118, e119,
    e120, e121, e122, e123, e124, e125, e126, e127, e128, e129,
    e130, e131, e132, e133, e134, e135, e136, e137, e138, e139,
    e140, e141, e142, e143, e144, e145, e146, e147, e148, e149,
    e150, e151, e152, e153, e154, e155, e156, e157, e158, e159,
    e160, e161, e162, e163, e164, e165, e166, e167, e168, e169,
    e170, e171, e172, e173, e174, e175, e176, e177, e178, e179,
    e180, e181, e182, e183, e184, e185, e186, e187, e188, e189,
    e190, e191, e192, e193, e194, e195, e196, e197, e198, e199,
    e200, e201, e202, e203, e204, e205, e206, e207, e208, e209,
    e210, e211, e212, e213, e214, e215, e216, e217, e218, e219,
    e220, e221, e222, e223, e224, e225, e226, e227, e228, e229,
    e230, e231, e232, e233, e234, e235, e236, e237, e238, e239,
    e240, e241, e242, e243, e244, e245, e246, e247, e248, e249,
    e250, e251, e252, e253, e254, e255
);

TEST(SmartEnum, Enum256)
{
    EXPECT_EQ("e255", ToString(Enum256SmartEnum::MaxEnum()));
    ASSERT_EQ(256U, Enum256SmartEnum::Size());
}

IES_SMART_ENUM(TrailingComma, Red, Green, Blue,);

TEST(SmartEnum, TrailingComma)
{
    EXPECT_EQ(3U, TrailingCommaSmartEnum::ToStringVector().size());
    EXPECT_EQ(4U, TrailingCommaSmartEnum::Size());

    //'' current implementation requires storing string in vector and access by index.
    //'' when there's trailing comma it's possible to access out of bound in vector.
    EXPECT_THROW(
        {
            auto& s = ToString(TrailingComma::Blue);
            (void)s;
        },
        std::runtime_error
    );

    ASSERT_THROW(
        {
            auto& r = TrailingCommaSmartEnum::ToRange();
            (void)r;
        },
        std::runtime_error
    );
}

} // namespace MyTest
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
// NOLINTEND(cert-err58-cpp,cppcoreguidelines-owning-memory)
