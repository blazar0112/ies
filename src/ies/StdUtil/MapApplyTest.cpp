#include "ies/StdUtil/MapApply.hxx"

#include "gtest/gtest.h"

#include <map>

#include "ies/Common/IntegralRange.hxx"
#include "ies/Common/NamedObject.hpp"

namespace ies
{

TEST(MapApply, MapApplyWithInit)
{
    std::map<int, int> map;
    map[2] = 3;
    auto Increment = [](int &value){ ++value; };
    MapApplyWithInit(map, 2, 1, Increment);
    MapApplyWithInit(map, 1, 0, Increment);

    EXPECT_EQ(4, map.at(2));
    ASSERT_EQ(1, map.at(1));
}

TEST(MapApply, DoubleWithInit)
{
    std::map<int, int> map;
    map[0] = 0;
    map[1] = 1;
    map[2] = 3;
    map[3] = 5;
    map[4] = 7;
    auto Double = [](int &value){ value *= 2; };
    MapApplyWithInit(map, 1, 0, Double);
    MapApplyWithInit(map, 3, 0, Double);
    MapApplyWithInit(map, 5, 0, Double);

    EXPECT_EQ(2, map.at(1));
    EXPECT_EQ(10, map.at(3));
    ASSERT_EQ(static_cast<int>(0), map.at(5));
}

TEST(MapApply, DoubleApply)
{
    std::map<int, int> map;
    map[0] = 0;
    map[1] = 1;
    map[2] = 3;
    map[3] = 5;
    map[4] = 7;
    auto Double = [](int &value){ value *= 2; };
    MapApply(map, 1, Double);
    MapApply(map, 3, Double);
    MapApply(map, 5, Double);

    EXPECT_EQ(2, map.at(1));
    EXPECT_EQ(10, map.at(3));
    ASSERT_EQ(static_cast<int>(0), map.at(5));
}

TEST(MapApply, MapIncrementWithInit)
{
    std::map<int, int> map;
    map[2] = 3;
    MapIncrementWithInit(map, 2, 1);
    MapIncrementWithInit(map, 1, 0);

    EXPECT_EQ(4, map.at(2));
    ASSERT_EQ(1, map.at(1));
}

TEST(MapApply, MoreTypes)
{
    std::map<std::string, double> map;
    map["x_coeff"] = 0.2;
    map["y_coeff"] = 0.7;
    auto Operation = [](double &coeff)
    {
        if (coeff<0.5) { coeff += 0.4; }
        else { coeff -= 0.3; }
    };
    MapApplyWithInit(map, "x_coeff", 0.1, Operation);
    MapApplyWithInit(map, "y_coeff", 0.1, Operation);
    MapApplyWithInit(map, "z_coeff", 0.1, Operation);

    EXPECT_DOUBLE_EQ(0.6, map.at("x_coeff"));
    EXPECT_DOUBLE_EQ(0.4, map.at("y_coeff"));
    ASSERT_DOUBLE_EQ(0.5, map.at("z_coeff"));
}

TEST(MapApply, Add)
{
    std::map<int, int> map;
    map[0] = 0;
    map[1] = 1;
    map[2] = 3;
    map[3] = 5;
    map[4] = 7;
    MapAddCount(map, 1, 0);
    MapAddCount(map, 2, -2);
    MapAddCount(map, 3, 3);
    MapAddCount(map, 5, 4);

    EXPECT_EQ(1, map.at(1));
    EXPECT_EQ(1, map.at(2));
    EXPECT_EQ(8, map.at(3));
    ASSERT_EQ(4, map.at(5));
}

TEST(MapApply, NonArithmeticApply)
{
    std::map<int, NamedObject> map;
    NamedObject defaultObject{"default"};
    for (auto i : IntRange(0, 4))
    {
        MapApplyWithInit(map, i, defaultObject, [i](NamedObject &o){ o.SetName(o.GetName()+"_"+std::to_string(i)); });
    }

    EXPECT_EQ("default_0", map.at(0).GetName());
    EXPECT_EQ("default_1", map.at(1).GetName());
    EXPECT_EQ("default_2", map.at(2).GetName());
    ASSERT_EQ("default_3", map.at(3).GetName());
}

}
