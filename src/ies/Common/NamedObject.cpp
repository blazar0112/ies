#include "ies/Common/NamedObject.hpp"

namespace ies
{

NamedObject::
NamedObject(std::string name)
:   mName(std::move(name))
{
}

void
NamedObject::
SetName(const std::string &name)
{
    mName = name;
}

const std::string &
NamedObject::
GetName()
const
{
    return mName;
}

std::string
ToString(const NamedObject &object)
{
    return "NamedObject["+object.GetName()+"]";
}

}
