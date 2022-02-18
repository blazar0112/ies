#pragma once
//! [C++11 Compatible]
#include "ies/StdUtil/RequireCpp11.hpp"

#include <memory>
#include <utility>

#include "ies/StdUtil/ClassMacro.hpp"

namespace ies
{

//! @class Pimpl(pointer to implementation) with propagate const-correctness capability.
//! @note standard may have propagate_const in future. this class should equivalent to that.
//! that is: std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
//!
//! @example
//! Client: just #include VisibleClass.hpp
//!
//! in VisibleClass.hpp:
//! //can be forward declaration or private nested class, here uses forward declaration as example.
//! //forward declaration: ImplClass can has its own hpp/cpp
//! //private nested class: need put definition inside VisibleClass' implemetation.
//!
//! class ImplClass;
//! class VisibleClass
//! {
//! public:
//!     VisibleClass();  //ctor: no special requirement
//!     ~VisibleClass(); //dtor: VisibleClass must have out of line definition.
//!                      //because unique_ptr dtor need to know size
//!
//!     // example member function
//!     void MemberFunction(int value);
//!     ... (other interface that delegate to ImplClass with constness propagated).
//! private:
//!     //class Impl; //if using private nested class form.
//!     Pimpl<ImplClass> mPimpl;
//! };
//!
//! member function of VisibleClass delegate to Pimpl.
//! void VisibleClass::MemberFunction(int value)
//! {
//!     mPimpl->MemberFunction(value);
//! }
//!
//! in ImplClass.hpp do normal class design.

template <typename T>
class Pimpl {
public:
    using pointer           = T*;
    using const_pointer     = const T*;
    using reference         = T&;
    using const_reference   = const T&;

        template<typename... Args>
        Pimpl(Args&&... args)
        : mP(std::make_unique<T>(std::forward<Args>(args)...))
        {}

        ~Pimpl() = default;
        IES_DELETE_COPY_AND_ASSIGN(Pimpl);

        pointer get() { return mP.get(); }
        const_pointer get() const { return mP.get(); }

        reference operator*() { return *mP.get(); }
        const_reference operator*() const { return *mP.get(); }

        pointer operator->() { return mP.get(); }
        const_pointer operator->() const { return mP.get(); }

private:
    std::unique_ptr<T> mP;
};

//'' Not to be used with pointers or references
template <class T> class Pimpl<T*>;
template <class T> class Pimpl<T&>;

}
