#include "ies/StdUtil/AdditionOrderedPtrHashSet.hxx"

#include "gtest/gtest.h"

#include <string>
#include <vector>

#include "ies/StdUtil/Find.hxx"

namespace ies
{

// This results in compile errors.
/*
TEST(AdditionOrderedPtrHashSet, IncorrectType)
{
    AdditionOrderedPtrHashSet<int*> s1;
    AdditionOrderedPtrHashSet<int&> s2;
}
*/

TEST(AdditionOrderedPtrHashSet, AdditionOrdered)
{
    const std::string s1 = "a";
    const std::string s2 = "b";
    const std::string s3 = "c";

    AdditionOrderedPtrHashSet<const char> ptrSet;
    auto& v = ptrSet.GetAdditionOrderedVector();
    auto& us = ptrSet.GetUnorderedSet();

    EXPECT_EQ(0U, v.size());

    auto emplace3 = ptrSet.Emplace(s3.c_str());

    EXPECT_TRUE(emplace3);
    EXPECT_EQ(1U, v.size());
    EXPECT_EQ(1U, us.size());
    EXPECT_FALSE(Find(us, s1.c_str()));
    EXPECT_FALSE(Find(us, s2.c_str()));
    EXPECT_TRUE(Find(us, s3.c_str()));

    auto emplace2 = ptrSet.Emplace(s2.c_str());
    EXPECT_TRUE(emplace2);
    EXPECT_FALSE(Find(us, s1.c_str()));
    EXPECT_TRUE(Find(us, s2.c_str()));
    EXPECT_TRUE(Find(us, s3.c_str()));

    auto emplace1 = ptrSet.Emplace(s1.c_str());
    EXPECT_TRUE(emplace1);

    auto emplace2Again = ptrSet.Emplace(s2.c_str());
    EXPECT_FALSE(emplace2Again);

    EXPECT_TRUE(Find(us, s1.c_str()));
    EXPECT_TRUE(Find(us, s2.c_str()));
    EXPECT_TRUE(Find(us, s3.c_str()));

    EXPECT_EQ(3U, v.size());
    EXPECT_EQ(3U, us.size());

    const std::vector<const char*> expectOrder{s3.c_str(), s2.c_str(), s1.c_str()};
    ASSERT_EQ(expectOrder, v);
}

TEST(AdditionOrderedPtrHashSet, Clear)
{
    const std::string s1 = "a";
    const std::string s2 = "b";
    const std::string s3 = "c";

    AdditionOrderedPtrHashSet<const char> ptrSet;
    auto& v = ptrSet.GetAdditionOrderedVector();
    auto& us = ptrSet.GetUnorderedSet();

    ptrSet.Emplace(s3.c_str());
    ptrSet.Emplace(s2.c_str());
    ptrSet.Emplace(s1.c_str());
    ptrSet.Emplace(s2.c_str());

    EXPECT_EQ(3U, v.size());
    EXPECT_EQ(3U, us.size());

    ptrSet.Clear();

    EXPECT_EQ(0U, v.size());
    ASSERT_EQ(0U, us.size());
}

} // namespace ies
