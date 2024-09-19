#pragma once

#include "ies/StdUtil/RequireCpp17.hpp" // IWYU pragma: keep

#include <type_traits>
#include <unordered_map>
#include <vector>

namespace ies
{

//! @brief Provide an unordered_map with key type `Key*` for hash table lookup and uniqueness
//! and can iterate deterministically from vector ordered by addition of key.
//! This helps to workaround with Pointer Non-determinism checks from
//! static code analyzer by using more memory (using extra vector container).
//! @note Pointer Non-determinism happens when using `[unordered_]set|map` with pointer keys
//! and then trying to iterate based on pointer address that can be non-deterministic
//! among runs due to Address space layout randomization (ASLR).
//! https://en.wikipedia.org/wiki/Address_space_layout_randomization
template <typename Key, typename T>
class AdditionOrderedPtrHashMap
{
public:
    explicit AdditionOrderedPtrHashMap(std::size_t reserveHint=4)
    {
        static_assert(!std::is_pointer_v<Key>, "[Key] cannot be a pointer type.");
        static_assert(!std::is_reference_v<Key>, "[Key] cannot be a reference type.");
        mUnorderedMap.reserve(reserveHint);
        mAdditionOrderedVector.reserve(reserveHint);
    }

    //! @brief For accessing non-iteration unordered_map interfaces.
    //! @note Use GetAdditionOrderedVector to iterate deterministically.
    const std::unordered_map<Key*, T> &
    GetUnorderedMap()
    const
    {
        return mUnorderedMap;
    }

    const std::vector<std::pair<Key*, T*>> &
    GetAdditionOrderedVector()
    const
    {
        return mAdditionOrderedVector;
    }

    //! @brief Emplace [keyPtr] if it's not in map, if added also add to back of vector.
    //! @return Same as unordered_map::try_emplace.
    template <typename... Args>
    std::pair<typename std::unordered_map<Key*, T>::iterator, bool>
    TryEmplace(Key* keyPtr, Args&&... args)
    {
        auto [it, isEmplaced] = mUnorderedMap.try_emplace(keyPtr, std::forward<Args>(args)...);
        if (isEmplaced)
        {
            mAdditionOrderedVector.emplace_back(std::make_pair(keyPtr, &(it->second)));
        }
        return std::make_pair(it, isEmplaced);
    }

    void
    Clear()
    {
        mUnorderedMap.clear();
        mAdditionOrderedVector.clear();
    }

private:
    std::unordered_map<Key*, T> mUnorderedMap;
    std::vector<std::pair<Key*, T*>> mAdditionOrderedVector;
};

} // namespace ies
