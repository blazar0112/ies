#pragma once

#include "ies/StdUtil/RequireCpp17.hpp"

#include <string>

#include "ies/Common/Pimpl.hxx"

namespace ies::PimplTest::Detail
{

class PimplTestClassImpl;

}

namespace ies::PimplTest
{

//! @brief Example VisibleClass to demo Pimpl class template usage.
class PimplTestClass
{
public:
        explicit PimplTestClass(const std::string &name);
        ~PimplTestClass();

        void
        SetName(const std::string &name);

        std::string
        GetName()
        const;

private:
    Pimpl<Detail::PimplTestClassImpl> mPimpl;
};

}
