#include "ies/Common/PimplTestClass.hpp"

#include "gtest/gtest.h"

namespace ies::PimplTest
{

TEST(PimplTestClass, Test)
{
    PimplTestClass testClass("hi");
    const PimplTestClass constTestClass("const");
    testClass.SetName("non-const");
    //constTestClass.SetName("try modify const"); //propagate_const: compile should error.

    ASSERT_EQ("non-const", testClass.GetName());
}

}
