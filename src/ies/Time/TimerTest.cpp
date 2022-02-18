#include "ies/Time/Timer.hpp"

#include "gtest/gtest.h"

#include <iostream>

namespace ies
{

TEST(Timer, Start)
{
    Timer timer;
    auto message = timer.Start("");

    EXPECT_EQ("Timer: started Time [(anonymous)].", message);

    ASSERT_ANY_THROW(
        timer.Start("(anonymous)");
    );

    auto messageA = timer.Start("a");

    ASSERT_EQ("Timer: started Time [a].", messageA);
}

TEST(Timer, LifeTime)
{
    Timer timer;
    auto time = timer.GetLifeTime();
    std::cout << "LifeTime = " << timer.ToString(Timer::GetLifeTimeName()) << '\n';
    ASSERT_GE(0, time);
}

}
