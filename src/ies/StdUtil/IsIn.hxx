#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include <string>

#include "ies/Type/IsAssociativeContainer.hxx"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace Detail {

template <typename Container, typename Key>
struct IsInImpl
{
    static
    bool
    IsIn(const Container &container, const Key &key)
    {
        static_assert(Type::IsAssociativeContainer<Container>::value,
                      "container is not associative (key_type not defined).");

        return (container.find(key)!=container.end());
    }
};

template <typename Key>
struct IsInImpl<std::string, Key>
{
    static
    bool
    IsIn(const std::string &container, const Key &key)
    {
        return (container.find(key)!=std::string::npos);
    }
};

}}

namespace ies
{

//! @brief Check if key in container. Deprecated in C++17, use ies::Find instead.
template <typename Container, typename Key>
bool
IsIn(const Container &container, const Key &key)
{
    return Detail::IsInImpl<Container, Key>::IsIn(container, key);
}

}
