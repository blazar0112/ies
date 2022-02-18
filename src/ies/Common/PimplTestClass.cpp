#include "ies/Common/PimplTestClass.hpp"

#include "ies/Common/PimplTestClassImpl.hpp"

namespace ies::PimplTest
{

PimplTestClass::
PimplTestClass(const std::string &name)
:   mPimpl(name)
{
}

PimplTestClass::
~PimplTestClass() = default;

void
PimplTestClass::
SetName(const std::string &name)
{
    mPimpl->SetName(name);
}

std::string
PimplTestClass::
GetName()
const
{
    return mPimpl->GetName();
}

}
