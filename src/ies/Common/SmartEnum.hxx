#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp" // IWYU pragma: keep

#include <stdexcept>
#include <string>
#include <vector>

#include "ies/StdUtil/IsIn.hxx"
#include "ies/String/SplitString.hpp"

//! @brief SmartEnum: a reflective and iterative enum helper.
//! Use macro IES_SMART_ENUM(SomeEnumName, Enermerator...) to declare
//! 1. A genuine enum class SomeEnumName with Enumerator.
//! 2. A helper class "SomeEnumNameSmartEnum" with extra utility to introspect enum:
//!     GetName(): to get SomeEnumName as std::string
//!     Min(), Max(), Size(): Compile-Time constexpr.
//!     ToRange(): to use in range-based for iteration.
//!     Has(): check if a string is a enumerator of this enum.
//!     GetEnumString to get all enumerator as string vector
//! 3. Helper function to inspect reflection on enumerator
//!     std::string ToString(SomeEnumName enumerator) to convert enumerator std::string
//!     SomeEnumName ToeSomeEnumName(std::string enumeratorString) to convert std::string to it's matching enumerator
//! Limitatons:
//! 1. cannot assign enumerator value, all enumerator must be value of previous +1. (No enum {Green = 2}) and start from 0.
//! @note: only when using ToString() it's a problem, and only check at that time. so if you never use ToString
//! it's ok to have assigned values.
//! 2. cannot change underlying type
//! 3. Cannot use inside function. (not just enum class declaration but with other definition)

//! @brief it's copied from <boost/preprocessor/variadic/size.hpp>
//! but 1. don't want to depend on boost, to reduce overhead for user.
//! 2. if define same name and user use boost also it's a problem
//! so change name here. should have coutesy for boost.
#define IES_VARIADIC_SIZE(...) IES_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)
#define IES_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Enum { namespace Detail {

inline
std::vector<std::string>
ToVector(const std::string &enumString)
{
    if (IsIn(enumString, "="))
    {
        throw std::runtime_error("SmartEnum not support assigned enumerator value.");
    }
    return SplitString(", ", enumString);
}

template <typename SmartEnumT, typename EnumT, std::size_t SizeC>
class EnumImpl
{
public:
    //'' public functions for all SmartEnums:
    static constexpr int Min() { return 0; }
    static constexpr int Max() { return static_cast<int>(SizeC-1); }
    static constexpr EnumT MinEnum() { return static_cast<EnumT>(Min()); }
    static constexpr EnumT MaxEnum() { return static_cast<EnumT>(Max()); }
    static constexpr std::size_t Size() { return SizeC; }

    //! @brief Return a range of all enumerators.
    static
    const std::vector<EnumT> &
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

    //! @brief Check if enum has string-enumerator before using ToEnumName(). (which throws if not exist)
    static
    bool
    Has(const std::string &enumerator)
    {
        return IsIn(SmartEnumT::GetEnumString(), enumerator);
    }

    //! @brief Use free function ToString(), don't use this directly, it's for implement use.
    static
    std::string
    ToString(EnumT enumerator)
    {
        return ToStringVector().at(static_cast<std::size_t>(enumerator));
    }

private:
    static
    const std::vector<std::string> &
    ToStringVector()
    {
        static const std::vector<std::string> stringVector{ToVector(SmartEnumT::GetEnumString())};
        return stringVector;
    }
};

}}}

//! @brief Declare an enum class EnumName and its auxiliary EnumNameSmartEnum class
//! and helper free functions inside namespace using this macro.
//! Enumerators start from value 0 and incrementally. Cannot assign other values.
//! @note For using a SmartEnum declared in namespace "a" at other namespace
//! Use a::EnumNameSmartEnum alias (which actualy type is a::SmartEnum<a::EnumName>).
#define IES_SMART_ENUM(EnumName, ...) \
    enum class EnumName \
    { \
        __VA_ARGS__ \
    }; \
    template<typename T> class SmartEnum; \
    template<> \
    class SmartEnum<EnumName> \
    : public ::ies::Enum::Detail::EnumImpl<SmartEnum<EnumName>, EnumName, IES_VARIADIC_SIZE(__VA_ARGS__)> \
    { \
    public: \
        using Type = EnumName; \
        static const std::string & GetName() { static const std::string name{#EnumName}; return name; } \
        static \
        const std::string & \
        GetEnumString() \
        { \
            static const std::string enumStr{#__VA_ARGS__}; \
            return enumStr; \
        } \
    }; \
    inline std::size_t ToIndex(EnumName enumerator) { return static_cast<std::size_t>(enumerator); } \
    inline std::string ToString(EnumName enumerator) { return SmartEnum<EnumName>::ToString(enumerator); } \
    inline EnumName To##EnumName(const std::string &enumerator) \
    { \
        for (auto e : SmartEnum<EnumName>::ToRange()) \
        { \
            if (enumerator==ToString(e)) return e; \
        } \
        throw std::invalid_argument("To"#EnumName"(): no enumerator named ["+enumerator+"]."); \
    } \
    using EnumName##SmartEnum = SmartEnum<EnumName>
