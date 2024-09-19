#pragma once

#include "ies/StdUtil/RequireCpp17.hpp" // IWYU pragma: keep

#include <type_traits>
#include <unordered_set>
#include <vector>

namespace ies
{

//! @brief Provide an unordered_set with key type `Key*` for hash table lookup and uniqueness
//! and can iterate deterministically from vector ordered by addition of key.
//! This helps to workaround with Pointer Non-determinism checks from
//! static code analyzer by using more memory (using extra vector container).
//! @note Pointer Non-determinism happens when using `[unordered_]set|map` with pointer keys
//! and then trying to iterate based on pointer address that can be non-deterministic
//! among runs due to Address space layout randomization (ASLR).
//! https://en.wikipedia.org/wiki/Address_space_layout_randomization
template <typename Key>
class AdditionOrderedPtrHashSet
{
public:
    explicit AdditionOrderedPtrHashSet(std::size_t reserveHint=4)
    {
        static_assert(!std::is_pointer_v<Key>, "[Key] cannot be a pointer type.");
        static_assert(!std::is_reference_v<Key>, "[Key] cannot be a reference type.");
        mUnorderedSet.reserve(reserveHint);
        mAdditionOrderedVector.reserve(reserveHint);
    }

    //! @brief For accessing non-iteration unordered_set interfaces.
    //! @note Use GetAdditionOrderedVector to iterate deterministically.
    const std::unordered_set<Key*> &
    GetUnorderedSet()
    const
    {
        return mUnorderedSet;
    }

    const std::vector<Key*> &
    GetAdditionOrderedVector()
    const
    {
        return mAdditionOrderedVector;
    }

    //! @brief Emplace [keyPtr] if it's not in set, if added also add to back of vector.
    //! @return True if insertion in set and vector took place.
    bool
    Emplace(Key* keyPtr)
    {
        auto [it, isEmplaced] = mUnorderedSet.emplace(keyPtr);
        if (isEmplaced)
        {
            mAdditionOrderedVector.emplace_back(keyPtr);
        }
        return isEmplaced;
    }

    void
    Clear()
    {
        mUnorderedSet.clear();
        mAdditionOrderedVector.clear();
    }

private:
    std::unordered_set<Key*> mUnorderedSet;
    std::vector<Key*> mAdditionOrderedVector;
};

} // namespace ies
