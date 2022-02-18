#include "ies/Common/PimplTestClassImpl.hpp"

//#include <iostream>

namespace ies::PimplTest::Detail
{

PimplTestClassImpl::
PimplTestClassImpl(const std::string &name)
:   mNameHolder(std::make_unique<NameHolder>(name))
{
    //std::cout << "PimplTestClassImpl ctor.\n";
}

PimplTestClassImpl::
~PimplTestClassImpl()
{
    //std::cout << "PimplTestClassImpl dtor.\n";
}

void
PimplTestClassImpl::
SetName(const std::string &name)
{
    mNameHolder->Name = name;
}

std::string
PimplTestClassImpl::
GetName()
const
{
    return mNameHolder->Name;
}

}
