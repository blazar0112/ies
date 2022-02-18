#pragma once

#include "ies/StdUtil/RequireCpp11Only.hpp"

#include <memory>
#include <utility>

//'' workaround for lacking std::make_unique in C++11, which is fixed in C++14.
#if __cplusplus < 201402L
// NOLINTNEXTLINE(cert-dcl58-cpp)
namespace std
{

template <typename T, typename... Args>
std::unique_ptr<T>
make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
#endif
