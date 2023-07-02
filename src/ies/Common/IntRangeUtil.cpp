#include "ies/Common/IntRangeUtil.hpp"

// NOLINTNEXTLINE(modernize-concat-nested-namespaces)
namespace ies { namespace IntRangeUtil {

ies::IntRange
FindOverlappingRange(const ies::IntRange &range1, const ies::IntRange &range2)
{
    return {range1.FindClosest(range2.GetBegin()),
            range1.FindClosest(range2.GetEnd()),
            ies::EmptyPolicy::Allow};
}

double
FindOverlappingPortion(const ies::IntRange &subject, const ies::IntRange &target)
{
    if (subject.empty())
    {
        throw std::runtime_error("subject cannot be empty.");
    }
    return static_cast<double>(FindOverlappingRange(subject, target).size())/static_cast<double>(subject.size());
}

}}
