#include "ies/String/Levenshtein.hpp"

#include "gtest/gtest.h"

#include <string>

namespace ies
{

TEST(Levenshtein, EmptyStrings)
{
    std::string str1;
    std::string str2;
    auto [distance, normalize] = DamerauLevenshtein(str1, str2);
    EXPECT_EQ(static_cast<std::size_t>(0), distance);
    ASSERT_DOUBLE_EQ(1.0, normalize);
}

TEST(Levenshtein, SameStrings)
{
    std::string str1 = "abcde";
    std::string str2 = "abcde";
    auto [distance, normalize] = DamerauLevenshtein(str1, str2);
    EXPECT_EQ(static_cast<std::size_t>(0), distance);
    ASSERT_DOUBLE_EQ(1.0, normalize);
}

TEST(Levenshtein, SimilarStrings)
{
    std::string str1 = "abcde";
    std::string str2 = "abcd";
    auto [distance, normalize] = DamerauLevenshtein(str1, str2);
    EXPECT_EQ(1u, distance);
    ASSERT_DOUBLE_EQ(0.8, normalize);
}

}
