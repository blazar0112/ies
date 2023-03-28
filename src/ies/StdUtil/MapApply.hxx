#pragma once

#include "ies/StdUtil/RequireCpp17.hpp" // IWYU pragma: keep

namespace ies
{

//! @brief Apply UnaryOperation on [key]'s value, if map has no key then emplace with [initialValue] first before apply.
template<typename Map, typename UnaryOperation>
typename Map::mapped_type &
MapApplyWithInit(Map &map,
                 typename Map::key_type const &key,
                 typename Map::mapped_type const &initialValue,
                 UnaryOperation const &operation)
{
    auto [it, flag] = map.try_emplace(key, initialValue);
    (void)flag;
    auto &value = it->second;
    operation(value);
    return value;
}

//! @brief Apply UnaryOperation on [key]'s arithmetic value (which use initialValue=0).
template<typename Map, typename UnaryOperation>
typename Map::mapped_type &
MapApply(Map &map,
         typename Map::key_type const &key,
         UnaryOperation const &operation)
{
    return MapApplyWithInit(map, key, 0, operation);
}

//! @brief Convenient function for increment [key]'s value by 1.
//! @note If map has no [key], emplace with initialValue first then increment.
//! Use this for non-zero initial value, otherwise just use MapIncrementCount.
template<typename Map>
typename Map::mapped_type &
MapIncrementWithInit(Map &map,
                     typename Map::key_type const &key,
                     typename Map::mapped_type const &initialValue)
{
    return MapApplyWithInit(map, key, initialValue, [](typename Map::mapped_type &value){ ++value; });
}

//! @brief Convenient function for increment [key]'s count by 1. (count is zero-initialized value).
//! @note This is to use map to count each keys, without trouble to code like:
//!     if (map not has key) map[key] = 0;
//!     ++map[key];
template<typename Map>
typename Map::mapped_type &
MapIncrementCount(Map &map,
                  typename Map::key_type const &key)
{
    return MapIncrementWithInit(map, key, 0);
}

//! @brief Convenient function for increment [key]'s count by [addingValue]. (count is zero-initialized value).
//! @note This is to use map to count each keys, without trouble to code like:
//!     if (map not has key) map[key] = 0;
//!     map[key] += addingValue;
template<typename Map>
typename Map::mapped_type &
MapAddCount(Map &map,
            typename Map::key_type const &key,
            typename Map::mapped_type const &addingValue)
{
    return MapApplyWithInit(map, key, 0, [&addingValue](typename Map::mapped_type &value){ value += addingValue; });
}

}
