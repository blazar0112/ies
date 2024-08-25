#include "ies/Common/IntegerPow.hxx"

namespace IntegerBuildTime
{

uint64_t Test()
{
    return ies::LookupIntegerPow<10, 20>(15);
}

} // namespace IntegerBuildTime
