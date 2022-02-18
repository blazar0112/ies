#include "ies/StdUtil/FormatString.hxx"

#include "gtest/gtest.h"

#include "ies/Common/SmartEnum.hxx"

namespace ies
{

TEST(FormatString, Array)
{
    ASSERT_EQ("std::array<3>{3, 4, 5}", FormatString(std::array<int, 3>{3, 4, 5}));
}

TEST(FormatString, Deque)
{
    ASSERT_EQ("std::deque{7, 5, 16, 8}", FormatString(std::deque<int>{7, 5, 16, 8}));
}

TEST(FormatString, ForwardList)
{
    ASSERT_EQ(R"(std::forward_list{"a", "b", "a"})", FormatString(std::forward_list<std::string>{"a", "b", "a"}));
}

TEST(FormatString, List)
{
    ASSERT_EQ(R"(std::list{"a", "b", "a"})", FormatString(std::list<std::string>{"a", "b", "a"}));
}

TEST(FormatString, Map)
{
    std::map<std::string, int> map{{"a", 3}, {"b", 4}, {"c", 5}};
    auto formatted = FormatString(map,
        [](const decltype(map)::value_type &pair)
        {
            return "{\""+pair.first+"\", "+std::to_string(pair.second)+"}";
        });
    ASSERT_EQ(R"(std::map{{"a", 3}, {"b", 4}, {"c", 5}})", formatted);
}

TEST(FormatString, Set)
{
    ASSERT_EQ("std::set{3, 4, 5}", FormatString(std::set<int>{3, 4, 5}));
}

TEST(FormatString, Vector)
{
    EXPECT_EQ("std::vector{}", FormatString(std::vector<double>{}));
    EXPECT_EQ("std::vector{0.500000, 4.500000}", FormatString(std::vector<double>{0.5, 4.5}));
    EXPECT_EQ(R"(std::vector{"a"})", FormatString(std::vector<std::string>{"a"}));
    EXPECT_EQ("std::vector{1, 2, 3, 4}", FormatString(std::vector<int>{1, 2, 3, 4}));
    ASSERT_EQ("std::vector{std::vector{}, std::vector{}}",
              FormatString(std::vector<std::vector<int>>(2),
                           [](const std::vector<int> &element){ return FormatString(element); }));
}

IES_SMART_ENUM(Color, Green, Red, Blue);

TEST(FormatString, SmartEnum)
{
    ASSERT_EQ("std::vector{Color.Green, Color.Red, Color.Blue}",
              FormatString(ColorSmartEnum::ToRange(), [](Color color){ return ColorSmartEnum::GetName()+"."+ToString(color); }));
}

}
