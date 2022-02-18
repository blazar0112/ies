#include "gtest/gtest.h"

#include "ies/StdUtil/RequireCpp11Only.hpp"

#include "ies/StdUtil/Cpp11MakeUnique.hxx"

#include "ies/Common/Pimpl.hxx"

#include "ies/StdUtil/ClassMacro.hpp"
#include "ies/StdUtil/EnumUtil.hxx"

#include "ies/Time/TimeUtil.hxx"

#include "ies/Type/Cpp11VoidT.hxx"
#include "ies/Type/HasStdToString.hxx"
#include "ies/Type/IsAssociativeContainer.hxx"
#include "ies/Type/IsJson.hxx"
#include "ies/Type/IsMap.hxx"

int
main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
