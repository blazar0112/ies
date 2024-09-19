#include "ies/StdUtil/AdditionOrderedPtrHashMap.hxx"

#include "gtest/gtest.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "ies/StdUtil/Find.hxx"

namespace ies
{

// This results in compile errors.
/*
TEST(AdditionOrderedPtrHashMap, IncorrectType)
{
    AdditionOrderedPtrHashMap<int*, std::string> s1;
    AdditionOrderedPtrHashMap<int&, std::string> s2;
}
*/

TEST(AdditionOrderedPtrHashMap, AdditionOrdered)
{
    const std::string s1 = "a";
    const std::string s2 = "b";
    const std::string s3 = "c";

    AdditionOrderedPtrHashMap<const char, std::string> ptrMap;
    auto& v = ptrMap.GetAdditionOrderedVector();
    auto& um = ptrMap.GetUnorderedMap();

    EXPECT_EQ(0U, v.size());

    auto [it3, emplace3] = ptrMap.TryEmplace(s3.c_str(), "cc");

    EXPECT_TRUE(emplace3);
    EXPECT_EQ(1U, v.size());
    EXPECT_EQ(1U, um.size());
    EXPECT_FALSE(Find(um, s1.c_str()));
    EXPECT_FALSE(Find(um, s2.c_str()));
    EXPECT_TRUE(Find(um, s3.c_str()));

    auto [it2, emplace2] = ptrMap.TryEmplace(s2.c_str(), "bb");
    EXPECT_TRUE(emplace2);
    EXPECT_FALSE(Find(um, s1.c_str()));
    EXPECT_TRUE(Find(um, s2.c_str()));
    EXPECT_TRUE(Find(um, s3.c_str()));

    auto [it1, emplace1] = ptrMap.TryEmplace(s1.c_str(), "aa");
    EXPECT_TRUE(emplace1);

    auto [it2Again, emplace2Again] = ptrMap.TryEmplace(s2.c_str(), "dd");
    EXPECT_FALSE(emplace2Again);
    EXPECT_EQ(it2->second, it2Again->second);

    EXPECT_TRUE(Find(um, s1.c_str()));
    EXPECT_TRUE(Find(um, s2.c_str()));
    EXPECT_TRUE(Find(um, s3.c_str()));

    EXPECT_EQ(3U, v.size());
    EXPECT_EQ(3U, um.size());

    const std::vector<const char*> expectKeyOrder{s3.c_str(), s2.c_str(), s1.c_str()};
    const std::vector<std::string> expectValueOrder{"cc", "bb", "aa"};
    std::vector<const char*> actualKeyOrder;
    std::vector<std::string> actualValueOrder;
    actualKeyOrder.reserve(v.size());
    actualValueOrder.reserve(v.size());
    for (auto [keyPtr, valuePtr] : v)
    {
        actualKeyOrder.emplace_back(keyPtr);
        ASSERT_NE(nullptr, valuePtr);
        actualValueOrder.emplace_back(*valuePtr);
    }
    EXPECT_EQ(expectKeyOrder, actualKeyOrder);
    ASSERT_EQ(expectValueOrder, actualValueOrder);
}

TEST(AdditionOrderedPtrHashMap, Clear)
{
    const std::string s1 = "a";
    const std::string s2 = "b";
    const std::string s3 = "c";

    AdditionOrderedPtrHashMap<const char, std::unique_ptr<std::vector<int>>> ptrMap;
    auto& v = ptrMap.GetAdditionOrderedVector();
    auto& um = ptrMap.GetUnorderedMap();

    auto u1 = std::make_unique<std::vector<int>>(1);
    auto u2 = std::make_unique<std::vector<int>>(2);
    auto u3 = std::make_unique<std::vector<int>>(3);
    auto u4 = std::make_unique<std::vector<int>>(4);

    EXPECT_EQ(1U, u1->size());
    EXPECT_EQ(2U, u2->size());
    EXPECT_EQ(3U, u3->size());
    EXPECT_EQ(4U, u4->size());

    ptrMap.TryEmplace(s3.c_str(), std::move(u1));
    ptrMap.TryEmplace(s2.c_str(), std::move(u2));
    ptrMap.TryEmplace(s1.c_str(), std::move(u3));
    ptrMap.TryEmplace(s2.c_str(), std::move(u4));

    EXPECT_EQ(3U, v.size());
    EXPECT_EQ(3U, um.size());

    ptrMap.Clear();

    EXPECT_EQ(0U, v.size());
    ASSERT_EQ(0U, um.size());
}

} // namespace ies
