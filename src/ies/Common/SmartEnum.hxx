#pragma once

#include "ies/StdUtil/RequireCpp17.hpp" // IWYU pragma: keep

#include <cstddef>

#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "ies/String/SplitString.hpp"

// NOLINTBEGIN(cppcoreguidelines-macro-usage)
//! @brief These macros are copied from <boost/preprocessor/variadic/size.hpp>
//! Don't want to depend on boost in this header, to reduce overhead for user.
#define IES_VARIADIC_SIZE(...) IES_VARIADIC_SIZE_I(__VA_ARGS__, \
    64, 63, 62, 61, 60, 59, 58, 57, \
    56, 55, 54, 53, 52, 51, 50, 49, \
    48, 47, 46, 45, 44, 43, 42, 41, \
    40, 39, 38, 37, 36, 35, 34, 33, \
    32, 31, 30, 29, 28, 27, 26, 25, \
    24, 23, 22, 21, 20, 19, 18, 17, \
    16, 15, 14, 13, 12, 11, 10, 9, \
    8, 7, 6, 5, 4, 3, 2, 1,)
#define IES_VARIADIC_SIZE_I( \
    e00, e01, e02, e03, e04, e05, e06, e07, \
    e08, e09, e10, e11, e12, e13, e14, e15, \
    e16, e17, e18, e19, e20, e21, e22, e23, \
    e24, e25, e26, e27, e28, e29, e30, e31, \
    e32, e33, e34, e35, e36, e37, e38, e39, \
    e40, e41, e42, e43, e44, e45, e46, e47, \
    e48, e49, e50, e51, e52, e53, e54, e55, \
    e56, e57, e58, e59, e60, e61, e62, e63, \
    size, ...) size

namespace ies::Enum::Detail
{

//! @brief Split <enumDefString> in enum class EnumName {<enumDefString>} to vector of enumeratorStr.
//! @note Same definition for all EnumName declared by IES_SMART_ENUM so as inline function.
inline
std::vector<std::string>
ToVector(const std::string& enumDefString)
{
    if (auto it = enumDefString.find('='); it!=std::string::npos)
    {
        throw std::runtime_error("SmartEnum not support assigned enumerator value.");
    }
    return SplitString(", ", enumDefString);
}

//! @brief Base class of SmartEnum, actual implementation of SmartEnum.
template <typename SmartEnumT, typename EnumT, std::size_t SizeC>
class EnumImpl
{
public:
    using UnderlyingType = std::underlying_type_t<EnumT>;
    // NOLINTNEXTLINE(*-magic-numbers)
    static_assert(SizeC>0&&SizeC<=64);
    static_assert(std::is_same_v<int, UnderlyingType>);

    //! @brief Get min valid value in underlying type int.
    static constexpr int Min() { return 0; }
    //! @brief Get max valid value in underlying type int.
    static constexpr int Max() { return SizeC-1; }
    //! @brief Get min valid enumerator.
    static constexpr EnumT MinEnum() { return static_cast<EnumT>(Min()); }
    //! @brief Get max valid enumerator.
    static constexpr EnumT MaxEnum() { return static_cast<EnumT>(Max()); }
    //! @brief Get compile-time constant size.
    static constexpr std::size_t Size() { return SizeC; }

    //! @brief Return all enumerators in vector of enumerator to use in range-based for.
    static
    const std::vector<EnumT>&
    ToRange()
    {
        static std::vector<EnumT> range;
        if (range.empty())
        {
            range.reserve(Size());
            for (auto i=Min(); i<=Max(); ++i)
            {
                range.emplace_back(static_cast<EnumT>(i));
            }
        }
        return range;
    }

    //! @brief Check if enum has string <enumeratorStr> before using ToEnumName(), which throws if not exist.
    static
    bool
    Has(const std::string& enumeratorStr)
    {
        return SmartEnumT::GetEnumDefString().find(enumeratorStr)!=std::string::npos;
    }

    //! @note Use free function ToString(EnumName), don't use this directly, it's for SmartEnum implementation.
    static
    const std::string&
    ToString(EnumT enumerator)
    {
        auto index = ToIndex(enumerator);
        if (index>=Size())
        {
            static const std::string OutOfBoundStr;
            return OutOfBoundStr;
        }
        return ToStringVector()[index];
    }

    //! @brief Get vector of enumeratorStr.
    static
    const std::vector<std::string>&
    ToStringVector()
    {
        static const std::vector<std::string> stringVector{ToVector(SmartEnumT::GetEnumDefString())};
        return stringVector;
    }
};

} // namespace ies::Enum::Detail

//! @brief Macro to declare an enum class EnumName and its auxiliary EnumNameSmartEnum class,
//! which is a reflective and iterative enum helper class, and helper free functions inside same namespace.
//! Macro IES_SMART_ENUM(EnumName, Enumerator...) declares following:
//! 1. A genuine enum class EnumName with incremental enumerators.
//!     No skipping value, start from 0, can use as index in array/vector. Default underlying type int.
//! 2. A helper class "EnumNameSmartEnum" (aliased from SmartEnum<EnumName>) with extra utility to introspect enum:
//!     GetName(): to get EnumName as string.
//!     Size(): Compile-Time constexpr to get size that can be used in constexpr context like array size.
//!     Min(), Max(), MinEnum(), MaxEnum(): convenient function to get min/max value/enumerator.
//!     ToRange(): to use in range-based for iteration.
//!     Has(): check if a string is a enumerator of this enum.
//!     ToStringVector(): to get all enumerator as vector of string.
//! 3. Helper free functions to get reflection on enumerator:
//!     std::size_t ToIndex(EnumName enumerator): convert enumerator to std::size to use as index.
//!     std::string ToString(EnumName enumerator): convert enumerator to std::string.
//!     EnumName ToEnumName(std::string enumeratorString): convert std::string to its matching enumerator.
//!
//! @note Limitations:
//! 1. Cannot assign enumerator value or skipping values, all enumerator must be value of previous +1.
//!     No enum {Green = 2} and start from 0.
//! 2. Only support max 64 enumerators.
//! 3. Use default underlying type int (specified by standard), cannot change underlying type.
//! 4. Cannot declare inside function, since not just declare enum class but with other function definitions.
//! @note For using a SmartEnum declared in namespace "a" at other namespace
//! Can use a::EnumNameSmartEnum alias, which actual type is a::SmartEnum<a::EnumName>.
//! SmartEnum<EnumName> form can be used in template writing.
//! @note End this macro with semicolon.
//! @example
//!     #include "SmartEnum.hxx"
//!     IES_SMART_ENUM(Color, Red, Green, Blue);
#define IES_SMART_ENUM(EnumName, ...) \
    enum class EnumName \
    { \
        __VA_ARGS__ \
    }; \
    /* @brief Primary class template for SmartEnum. */ \
    template <typename T> class SmartEnum; \
    /* @brief SmartEnum<EnumName>: auxiliary SmartEnum for enum class EnumName. */ \
    /* SmartEnum::Type: type alias to EnumName. */ \
    template <> \
    class SmartEnum<EnumName> \
    : public ::ies::Enum::Detail::EnumImpl<SmartEnum<EnumName>, EnumName, IES_VARIADIC_SIZE(__VA_ARGS__)> \
    { \
    public: \
        using Type = EnumName; \
        /* @brief Get enum class "EnumName" as string. */ \
        static const std::string& GetName() { static const std::string name{#EnumName}; return name; } \
        /* @brief Get enum class EnumName{"EnumDef"} as string. */ \
        static const std::string& GetEnumDefString() { static const std::string enumDefString{#__VA_ARGS__}; return enumDefString; } \
    }; \
    /* @brief Convert enumerator to std::size_t, usually use as index for array/vector. */ \
    [[maybe_unused]] inline constexpr std::size_t ToIndex(EnumName enumerator) { return static_cast<std::size_t>(enumerator); } \
    /* @brief Convert enumerator to std::string. Empty if enumerator is out of bound. */ \
    [[maybe_unused]] inline const std::string& ToString(EnumName enumerator) { return SmartEnum<EnumName>::ToString(enumerator); } \
    /* @brief Convert std::string to enumerator of EnumName, throws if <enumeratorStr> not match any enumerator. */ \
    /* @note Use SmartEnum::Has(enumeratorStr) to check before use if don't want to throw exception. */ \
    [[maybe_unused]] inline EnumName To##EnumName(const std::string& enumeratorStr) \
    { \
        for (auto e : SmartEnum<EnumName>::ToRange()) \
        { \
            if (enumeratorStr==ToString(e)) { return e; } \
        } \
        throw std::runtime_error("To"#EnumName"(): no enumerator named ["+enumeratorStr+"]."); \
    } \
    /* @brief Alias type SmartEnum<EnumName> to EnumNameSmartEnum. */ \
    using EnumName##SmartEnum = SmartEnum<EnumName>

// NOLINTEND(cppcoreguidelines-macro-usage)
