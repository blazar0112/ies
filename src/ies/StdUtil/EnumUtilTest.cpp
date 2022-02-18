#include "ies/StdUtil/EnumUtil.hxx"
#include "ies/StdUtil/RequireCpp17.hpp"

#include "gtest/gtest.h"

#include <type_traits>

namespace ies
{

enum class MyEnum : unsigned char { D=2 };

TEST(EnumUtil, ToUnderlying)
{
    //'' note: can only write as this in C++17 using direct list initialization.
    MyEnum e{2u};
    auto u = ToUnderlying(e);
    EXPECT_EQ(2u, u);
    auto isSame = std::is_same_v<decltype(u), unsigned char>;
    ASSERT_TRUE(isSame);
}

}
