#pragma once

#include "ies/StdUtil/RequireCpp17.hpp" // IWYU pragma: keep

#include <memory>
#include <string>

#include "ies/StdUtil/ClassMacro.hpp"

namespace ies::PimplTest::Detail
{

struct NameHolder
{
    std::string Name;
    NameHolder(std::string name) : Name(std::move(name)) {}
};

//! @brief Example ImplClass to demo Pimpl class template usage.
//! Implementation (function and member) is located in this class.
class PimplTestClassImpl
{
public:
        explicit PimplTestClassImpl(const std::string &name);
        ~PimplTestClassImpl();
        IES_DELETE_COPY_AND_ASSIGN(PimplTestClassImpl);

        void
        SetName(const std::string &name);

        std::string
        GetName()
        const;

private:
    std::unique_ptr<NameHolder> mNameHolder;
};

}
