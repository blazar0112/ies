#pragma once

//'' workaround for warning C4251: class T needs to have dll-interface to be used by clients of class T.
//'' just disable warning, not fix it properly currently
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "ies/ies_export.h"

#include <string>

namespace ies
{

//! @class NamedObject
//! @brief An object with name. Name can be empty.
class IES_EXPORT NamedObject
{
public:
        explicit NamedObject(std::string name);
        ~NamedObject() = default;

        void
        SetName(const std::string &name);

        const std::string &
        GetName()
        const;

private:
    std::string mName;
};

IES_EXPORT
//! @brief Convert NamedObject to string formatted as 'NamedObject[name]'.
std::string
ToString(const NamedObject &object);

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
