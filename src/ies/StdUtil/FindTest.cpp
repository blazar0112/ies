#include "ies/StdUtil/Find.hxx"

#include "gtest/gtest.h"

#include <array>
#include <list>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "nlohmann/json.hpp"

using Json = nlohmann::json;

namespace ies
{

TEST(Find, Set)
{
    std::set<int> numbers{2, 5};

    if (auto opt2 = Find(std::as_const(numbers), 2))
    {
        EXPECT_EQ(2, *opt2.value());
        EXPECT_TRUE(std::is_const_v<std::remove_reference_t<decltype(*opt2.value())>>);
    }

    if (auto opt5 = Find(numbers, 5))
    {
        //'' note: std::set not alllowed to modify keys, even though return iterator.
        EXPECT_EQ(5, *opt5.value());
        auto opt5ContainsIterator = std::is_same_v<std::remove_reference_t<decltype(opt5.value())>, std::set<int>::iterator>;
        EXPECT_TRUE(opt5ContainsIterator);
        EXPECT_TRUE(!std::is_const<decltype(opt5.value())>::value);
        EXPECT_EQ(2, *std::prev(opt5.value()));
    }

    ASSERT_FALSE(Find(numbers, 3));
}

TEST(Find, Map)
{
    std::map<int, std::string> alphabets{{1, "a"}, {2, "b"}, {4, "d"}};

    EXPECT_FALSE(Find(alphabets, 3));

    if (auto opt2 = Find(std::as_const(alphabets), 2))
    {
        EXPECT_EQ(2, opt2.value()->first);
        EXPECT_EQ("b", opt2.value()->second);
        auto opt2ContainsConstIterator = std::is_same_v<std::remove_reference_t<decltype(opt2.value())>, std::map<int, std::string>::const_iterator>;
        EXPECT_TRUE(opt2ContainsConstIterator);
    }

    if (auto opt4 = Find(alphabets, 4))
    {
        EXPECT_EQ(4, opt4.value()->first);
        EXPECT_EQ("d", opt4.value()->second);
        auto opt4ContainsIterator = std::is_same_v<std::remove_reference_t<decltype(opt4.value())>, std::map<int, std::string>::iterator>;
        EXPECT_TRUE(opt4ContainsIterator);
        EXPECT_TRUE(!std::is_const_v<decltype(opt4.value()->second)>);
        opt4.value()->second = "e";
    }

    std::map<int, std::string> expectAlphabets{{1, "a"}, {2, "b"}, {4, "e"}};
    ASSERT_EQ(expectAlphabets, alphabets);
}

TEST(Find, String)
{
    //''                     0123456
    std::string alphabets = "abcdefghijklmno";

    EXPECT_FALSE(Find(alphabets, "z"));

    if (auto opt = Find(alphabets, "ghi"))
    {
        EXPECT_EQ(6u, opt.value());
        EXPECT_EQ("ghijklmno", alphabets.substr(opt.value()));
    }

    ASSERT_FALSE(Find(alphabets, "cba"));
}

TEST(Find, ConstString)
{
    const std::string alphabets = "abcdefghijklmno";

    EXPECT_FALSE(Find(alphabets, "z"));

    ASSERT_FALSE(Find(alphabets, "cba"));
}

TEST(Find, CharAndNullTerminatedString)
{
    //'' this constrctor will keep embeded null char.
    const std::string alphabets{"abc\0\0\0def", 5};

    EXPECT_EQ(5u, alphabets.size());

    //'' this demos why std::string is not working to search null char, because it construct an empty string.
    // NOLINTNEXTLINE(bugprone-string-literal-with-embedded-nul)
    EXPECT_EQ(std::size_t{0}, Find(alphabets, "\0").value());
    EXPECT_EQ(std::size_t{3}, Find(alphabets, {"\0", 1}).value());

    ASSERT_EQ(std::size_t{3}, Find(alphabets, '\0').value());
}


TEST(Find, List)
{
    std::list<std::string> list{"abc", "b", "d", "c", "b", "c", "e"};

    if (auto opt = Find(list, "c"))
    {
        EXPECT_EQ("c", *opt.value());
        EXPECT_EQ(3, std::distance(list.begin(), opt.value()));
    }

    ASSERT_FALSE(Find(list, "z"));
}

TEST(Find, Vector)
{
    std::vector<int> vec{1, 2, 3, 4, 3, 2, 1};

    if (auto opt = Find(vec, 3))
    {
        EXPECT_EQ(3, *opt.value());
        EXPECT_EQ(2, std::distance(vec.begin(), opt.value()));
    }

    ASSERT_FALSE(Find(vec, 5));
}

TEST(Find, Array)
{
    std::array<int, 5> array{4, 3, 2, 1, 0};

    if (auto opt = Find(array, 3))
    {
        EXPECT_EQ(3, *opt.value());
        EXPECT_EQ(1, std::distance(array.begin(), opt.value()));
    }

    ASSERT_FALSE(Find(array, 5));
}

TEST(Find, CArray)
{
    int intArray[5] = {4, 3, 2, 1, 0};

    if (auto opt = Find(intArray, 3))
    {
        EXPECT_EQ(3, *opt.value());
        EXPECT_EQ(1, std::distance(std::begin(intArray), opt.value()));
    }

    ASSERT_FALSE(Find(intArray, 5));
}

TEST(Find, Json)
{
    Json json = {
        {"a", "hi"},
        {"b", 3}
    };

    EXPECT_FALSE(json.is_array());
    EXPECT_TRUE(Find(json, "a"));
    ASSERT_FALSE(Find(json, "c"));
}

TEST(Find, JsonArray)
{
    Json json;
    json = { "a", "b", "a" };
    auto findA = Find(json, "a");
    auto index = std::distance(json.begin(), findA.value());
    int expectAIndex = 0;

    EXPECT_TRUE(json.is_array());
    EXPECT_EQ(expectAIndex, index);
    EXPECT_TRUE(Find(json, "a"));
    ASSERT_FALSE(Find(json, "c"));
}

TEST(Find, JsonConstArray)
{
    Json json;
    json = { "a", "b", "a" };
    const auto &constJson = json;

    EXPECT_TRUE(constJson.is_array());
    EXPECT_TRUE(Find(constJson, "a"));
    ASSERT_FALSE(Find(constJson, "c"));
}

TEST(Find, JsonArrayOfNumber)
{
    Json json;
    json = { 1, 0, 2 };
    //'' cannot directly use find for non-string keys.
    auto array = json.get<std::vector<int>>();

    EXPECT_TRUE(json.is_array());
    EXPECT_TRUE(Find(array, 1));
    ASSERT_FALSE(Find(array, 3));
}

TEST(Find, Temporary)
{
    auto findC = Find(std::list<std::string>{"abc", "b", "d", "c", "b", "c", "e"}, "c");

    ASSERT_TRUE(findC);
}

TEST(Find, StringView)
{
    std::string text = "abcdefg";
    std::string_view textView{text};

    EXPECT_FALSE(Find(textView, "h").has_value());
    EXPECT_EQ(0u, Find(textView, "a"));
    ASSERT_EQ(1u, Find(textView, 'b'));
}

}
