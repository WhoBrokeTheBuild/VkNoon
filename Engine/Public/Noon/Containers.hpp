#ifndef NOON_CONTAINERS_HPP
#define NOON_CONTAINERS_HPP

#include <Noon/Config.hpp>

#include <array>
#include <deque>
#include <set>
#include <unordered_map>
#include <vector>

namespace noon {

template <class T, size_t N>
using Array = std::array<T, N>;

template <class T>
using Queue = std::pmr::deque<T>;

template <class T>
using Set = std::pmr::set<T>;

template <class K, class V>
using Map = std::pmr::unordered_map<K, V>;

template <class T>
using List = std::pmr::vector<T>;

} // namespace noon

#endif // NOON_CONTAINERS_HPP