#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

//! @brief Macro to reduce boilerplate in class definition.
//! Usage:
//! class MyClass
//! {
//! public:
//!     IES_DEFAULT_COPY_AND_ASSIGN(MyClass);
//! };
//! @note Put ';' after macro.

#define IES_DEFAULT_COPY_AND_ASSIGN(Class) \
    Class(const Class &) = default; \
    Class & operator=(const Class &) = default

//! @brief Macro to reduce boilerplate in class definition.
//! @see IES_DEFAULT_COPY_AND_ASSIGN for usage.
//! @note Put ';' after macro.
#define IES_DEFAULT_MOVE_AND_MOVE_ASSIGN(Class) \
    Class(Class &&) = default; \
    Class & operator=(Class &&) = default

//! @brief Macro to reduce boilerplate in class definition.
//! @see IES_DEFAULT_COPY_AND_ASSIGN for usage.
//! @note Put ';' after macro.
#define IES_DELETE_COPY_AND_ASSIGN(Class) \
    Class(const Class &) = delete; \
    Class & operator=(const Class &) = delete
